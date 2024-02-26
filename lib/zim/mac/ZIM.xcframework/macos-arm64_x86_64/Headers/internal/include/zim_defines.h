#ifndef __ZIM_DEFINES_H__
#define __ZIM_DEFINES_H__

//
//  zim_defines.h
//  ZIM
//
//  Copyright © 2021 Zego. All rights reserved.
//

/* Macros which declare an exportable function */
#define ZIM_API

/* Macros which declare an exportable variable */
#define ZIM_VAR extern

/* Macros which declare the called convention for exported functions */
#define ZIM_CALL

#if defined(_WIN32) /* For MSVC */
#undef ZIM_API
#undef ZIM_VAR
#undef ZIM_CALL
#if defined(ZIM_EXPORTS)
#define ZIM_API __declspec(dllexport)
#define ZIM_VAR __declspec(dllexport)
#elif defined(ZIM_STATIC)
#define ZIM_API // Nothing
#define ZIM_VAR // Nothing
#else
#define ZIM_API __declspec(dllimport)
#define ZIM_VAR __declspec(dllimport) extern
#endif
#define ZIM_CALL __cdecl
#define ZIM_DISABLE_DEPRECATION_WARNINGS __pragma(warning(push)) __pragma(warning(disable : 4996))
#define ZIM_ENABLE_DEPRECATION_WARNINGS __pragma(warning(pop))
#pragma warning(disable : 4068) /* Ignore 'unknown pragma mark' warning */
#else                           /* For GCC or clang */
#undef ZIM_API
#define ZIM_API __attribute__((visibility("default")))
#define ZIM_DISABLE_DEPRECATION_WARNINGS                                                           \
    _Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"")
#define ZIM_ENABLE_DEPRECATION_WARNINGS _Pragma("GCC diagnostic pop")
#endif

#ifdef __cplusplus
#define ZIM_EXTERN_BEGIN extern "C" {
#define ZIM_EXTERN_END }
#else
#define ZIM_EXTERN_BEGIN
#define ZIM_EXTERN_END
#endif

#ifdef _MSC_VER
#pragma warning(disable : 26812)
#endif

/* Compatibility for C */
#ifndef __cplusplus
#include <stdbool.h>
#endif

#if defined(__APPLE_OS__) || defined(__APPLE__)
#include "TargetConditionals.h"
#endif

/// Definition of the error code
#include "zim_error_code.h"

/// Define the zim handle
typedef void *zim_handle;

/// Define the API called result
typedef int zim_result;

/// Define the sequence
typedef int zim_sequence;

#define ZIM_MAX_COMMON_LEN (512)
#define ZIM_MAX_USERID_LEN (32)
#define ZIM_MAX_USERNAME_LEN (64)
#define ZIM_MAX_ROOMID_LEN (32)
#define ZIM_MAX_ROOMNAME_LEN (64)
#define ZIM_MAX_ROOMATTRIBUTES_NUMBER (10)

// MARK: - Enum

// MARK: - MainEnum

enum zim_geofencing_type {

    zim_geofencing_type_none = 0,

    zim_geofencing_type_include = 1,

    zim_geofencing_type_exclude = 2
};

enum zim_geofencing_area {
    // 中国大陆（不包含港澳台）
    zim_geofencing_area_cn = 2,
    // 北美
    zim_geofencing_area_na = 3,
    // 欧洲，包括英国
    zim_geofencing_area_eu = 4,
    // 亚洲，不包括中国大陆、印度
    zim_geofencing_area_as = 5,
    // 印度
    zim_geofencing_area_in = 6,
};

/// Connection state.
///
/// Description: The state machine that identifies the current connection state.
///
/// Use cases: It can be used to determine whether the login/logout is successful,
/// and to handle abnormal situations such as network disconnection.
///
/// Caution: Please use it with the connection event parameter.
///
enum zim_connection_state {

    /// Unconnected state, enter this state before logging in and after logging out. If there is a steady state abnormality in the process of logging in, such as AppID or Token are incorrect, or if the same user name is logged in elsewhere and the local end is kicked out, it will enter this state.
    zim_connection_state_disconnected = 0,

    /// The state that the connection is being requested. It will enter this state after successful execution login function. The display of the UI is usually performed using this state. If the connection is interrupted due to poor network quality, the SDK will perform an internal retry and will return to this state.
    zim_connection_state_connecting = 1,

    /// The state that is successfully connected. Entering this state indicates that login successfully and the user can use the SDK functions normally.
    zim_connection_state_connected = 2,

    /// The state that the reconnecting is being requested. It will enter this state after successful execution login function. The display of the UI is usually performed using this state. If the connection is interrupted due to poor network quality, the SDK will perform an internal retry and will return to this state.
    zim_connection_state_reconnecting = 3

};

/// The event that caused the connection status to change.
///
/// Description: The reason for the change of the connection state.
///
/// Use cases: It can be used to determine whether the login/logout is successful,
/// and to handle abnormal situations such as network disconnection.
///
/// Caution: Please use it with the connection state parameter.
///
enum zim_connection_event {

    /// Success.
    zim_connection_event_success = 0,

    /// The user actively logs in.
    zim_connection_event_active_login = 1,

    /// Connection timed out.
    zim_connection_event_login_timeout = 2,

    /// The network connection is temporarily interrupted.
    zim_connection_event_login_interrupted = 3,

    /// Being kicked out.
    zim_connection_event_kicked_out = 4,

    /// Being token expired.
    zim_connection_event_token_expired = 5,

    // Being unregistered
    zim_connection_event_unregistered = 6
};

// MARK: - MessageEnum

/// The priority of the message.
///
/// Description: Identifies the priority of a message.
///
/// Use cases: It can be used to set the priority when a message is sent.
/// The higher the priority, the higher the reliability.
/// Low priority may be discarded due to weak network.
///
/// Caution: The higher the priority, the higher the cost.
///
enum zim_message_priority {

    /// Low priority, generally used to send unimportant messages such as barrage message in a room.
    zim_message_priority_low = 1,

    /// Medium priority.
    zim_message_priority_medium = 2,

    /// High priority, generally used to send regular chat messages
    zim_message_priority_high = 3

};

/// The type of the message.
///
/// Description: Identifies the type of current message.
///
/// Use cases: It can be used to determine what type of message this message is.
///
enum zim_message_type {
    /// Only used for C
    zim_message_type_empty = -1,
    zim_message_type_unknown = 0,
    /// Normal text message.
    zim_message_type_text = 1,

    zim_message_type_command = 2,

    zim_message_type_image = 11,

    zim_message_type_file = 12,

    zim_message_type_audio = 13,

    zim_message_type_video = 14,

    zim_message_type_barrage = 20,

    zim_message_type_system = 30,

    zim_message_type_revoke = 31,

    zim_message_type_combine = 100,
    /// max custom value < 400
    zim_message_type_custom = 200,
};

enum zim_media_file_type {
    zim_media_file_type_original_file = 1,
    zim_media_file_type_large_image = 2,
    zim_media_file_type_thumbnail = 3,
    zim_media_file_type_video_first_frame = 4
};

enum zim_message_order {
    descending = 0,
    ascending = 1,
};

enum zim_message_direction { zim_message_direction_send = 0, zim_message_direction_receive = 1 };

enum zim_message_sent_status {
    zim_message_sent_status_sending = 0,
    zim_message_sent_status_send_success = 1,
    zim_message_sent_status_send_failed = 2
};

enum zim_revoke_type {
    // 未定义撤回状态
    zim_revoke_type_unknown = -1,
    zim_revoke_type_two_way = 0,
    zim_revoke_type_one_way = 1,
};

enum zim_message_receipt_status {
    zim_message_receipt_status_unknown = -1,
    // 没有回执，默认状态
    zim_message_receipt_status_none = 0,
    // 生效中，如单聊对方未读，群聊还有人未读
    zim_message_receipt_status_processing = 1, // sdk自己定义的
    // 回执完成，如单聊对方已读，群聊没有人未读
    zim_message_receipt_status_done = 2,
    // 回执过期
    zim_message_receipt_status_expired = 3,
    // 回执失败
    zim_message_receipt_status_failed = 4,
};

enum zim_message_revoke_status {
    zim_message_revoke_status_unknown = -1,
    zim_message_revoke_status_self_revoke = 0,        // 主动撤回
    zim_message_revoke_status_system_revoke = 1,      // 服务器撤回
    zim_message_revoke_status_server_api_revoke = 2,  // 后台API撤回
    zim_message_revoke_status_group_admin_revoke = 3, // 管理员撤回
    zim_message_revoke_status_group_owner_revoke = 4, // 群主撤回
};

enum zim_message_mentioned_type {
    zim_message_mentioned_type_mention_me = 1,
    zim_message_mentioned_type_mention_all = 2,
    zim_message_mentioned_type_mention_all_and_me = 3,
};
enum zim_message_delete_type {
    zim_message_delete_type_message_list_deleted = 0,
    zim_message_delete_type_conversation_all_messages_deleted = 1,
    zim_message_delete_type_all_conversation_messages_deleted = 2,
};

// MARK: - Offline Push
enum zim_cx_handle_type {
    zim_cx_handle_type_generic = 1,
    zim_cx_handle_type_phone_number = 2,
    zim_cx_handle_type_email_address = 3,
};

struct zim_voip_config {
    char *ios_voip_handle_value;
    enum zim_cx_handle_type ios_voip_handle_type;
    bool ios_voip_has_video;
};

struct zim_push_config {
    char *title;
    char *content;
    char *payload;
    char *resources_id;
    bool enable_voip_config;
    bool enable_badge;
    int badge_increment;
    struct zim_voip_config voip_config;
};
// MARK: - ConversationEnum

enum zim_conversation_type {
    // 非会话类型消息
    zim_conversation_type_unknown = -1,
    // 单聊类型
    zim_conversation_type_peer = 0,
    // 房间类型
    zim_conversation_type_room = 1,
    // 群聊类型
    zim_conversation_type_group = 2,
};

enum zim_conversation_notification_status {
    zim_conversation_notification_status_notify = 1,
    zim_conversation_notification_status_do_not_disturb = 2,
};

enum zim_conversation_event {
    // 会话新增事件
    zim_conversation_event_added = 0,
    // 会话更新事件
    zim_conversation_event_updated = 1,
    // 会话不可用事件
    zim_conversation_event_disabled = 2,
    // 会话被删除事件
    zim_conversation_event_deleted = 3
};

// MARK: - RoomEnum

/// Connection state.
///
/// Description: The state machine that identifies the current connection state.
///
/// Use cases: It can be used to judge whether the user enters/exit the room successfully, and handles abnormal situations such as network disconnection.
///
/// Caution: Please use it with the connection event parameter.
///
enum zim_room_state {

    /// Disconnected state, enter this state before entering the room and after exiting the room.
    zim_room_state_disconnected = 0,

    /// The connection state is being requested, and it will enter this state after the action of entering the room is executed successfully. The application interface is usually displayed through this state.
    zim_room_state_connecting = 1,

    /// The connection is successful. Entering this state means that the room has been successfully entered, and the user can use the room's functions normally.
    zim_room_state_connected = 2
};

/// The event that caused the room connection status to change.
///
/// Description: The reason for the change of the connection state.
///
/// Use cases: It can be used to determine whether the login/logout is successful,
/// and to handle abnormal situations such as network disconnection.
///
/// Caution: Please use it with the connection state parameter.
///
enum zim_room_event {

    /// Success.
    zim_room_event_success = 0,

    /// The network in the room is temporarily interrupted.
    zim_room_event_network_interrupted = 1,

    /// The network in the room is disconnected.
    zim_room_event_network_disconnected = 2,

    /// The room not exist
    zim_room_event_room_not_exist = 3,

    /// The user actively creates a room
    zim_room_event_active_create = 4,

    /// Failed to create room.
    zim_room_event_create_failed = 5,

    /// The user starts to enter the room
    zim_room_event_active_enter = 6,

    /// user failed to enter the room.
    zim_room_event_enter_failed = 7,

    /// user was kicked out of the room.
    zim_room_event_kicked_out = 8,

    zim_room_event_connect_timeout = 9,

    zim_room_event_kicked_out_by_other_device = 10

};

/// Room attribute change event enum.
///
/// Description: Identify the cause of the room attributes updated event.
///
enum zim_room_attributes_update_action {
    zim_room_attributes_action_set = 0,
    zim_room_attributes_action_delete = 1
};

// MARK: - GroupEnum

enum zim_group_event {
    //zim_group_event_none = 0,
    zim_group_event_created = 1,
    zim_group_event_dismissed = 2,
    zim_group_event_joined = 3,
    zim_group_event_invited = 4,
    zim_group_event_left = 5,
    zim_group_event_kicked_out = 6
};

enum zim_group_state { zim_group_state_quit = 0, zim_group_state_enter = 1 };

enum zim_group_member_event {
    //zim_group_member_event_none = 0,
    zim_group_member_event_joined = 1,
    zim_group_member_event_left = 2,
    //zim_group_member_event_modify = 3,
    zim_group_member_event_kicked_out = 4,
    zim_group_member_event_invited = 5
};

enum zim_friend_list_change_action {
    zim_friend_list_change_action_added = 0,
    zim_friend_list_change_action_deleted = 1
};

enum zim_group_member_state { zim_group_member_state_quit = 0, zim_group_member_state_enter = 1 };

enum zim_blacklist_change_action {
    zim_blacklist_change_action_added = 0,
    zim_blacklist_change_action_removed = 1
};

enum zim_friend_application_change_action {
    zim_friend_application_change_action_added = 0,
    zim_friend_application_change_action_deleted = 1
};

enum zim_group_message_notification_status {
    zim_group_message_notification_status_notify = 1,
    zim_group_message_notification_status_do_not_disturb = 2
};

enum zim_group_attributes_update_action {
    zim_group_attributes_update_action_set = 0,
    zim_group_attributes_update_action_delete = 1
};

enum zim_group_mute_mode {
    zim_group_mute_mode_none = 0,
    zim_group_mute_mode_normal = 1,
    zim_group_mute_mode_all = 2,
    zim_group_mute_mode_custom = 3
};

// MARK: - CallEnum

enum zim_call_user_state {
    zim_call_user_state_unknown = -1,
    zim_call_user_state_inviting = 0,
    zim_call_user_state_accepted = 1,
    zim_call_user_state_rejected = 2,
    zim_call_user_state_cancelled = 3,
    zim_call_user_state_offline = 4,
    zim_call_user_state_received = 5,
    zim_call_user_state_timeout = 6,
    zim_call_user_state_quit = 7,
    zim_call_user_state_end = 8
};

enum zim_call_state {
    zim_call_state_unknown = -1,
    zim_call_state_started = 1,
    zim_call_state_ended = 2
};

enum zim_call_invitation_mode {
    zim_invitation_mode_unknown = -1,
    zim_invitation_mode_general = 0,
    zim_invitation_mode_advanced = 1,
};

enum zim_friend_application_type {
    zim_friend_application_type_unknown = -1,
    zim_friend_application_type_none = 0,
    zim_friend_application_type_received = 1, // 收到的申请
    zim_friend_application_type_send = 2,     // 自己发送的申请
    zim_friend_application_type_both = 3,     // 双方同时发起的申请
};

enum zim_friend_relation_check_type {
    zim_friend_relation_check_type_unknown = -1,
    zim_friend_relation_check_type_both = 0,
    zim_friend_relation_check_type_single = 1,
};

enum zim_friend_application_state {
    zim_friend_application_state_unknown = -1,
    zim_friend_application_state_waiting = 1,  // 待响应
    zim_friend_application_state_accepted = 2, // 已同意
    zim_friend_application_state_rejected = 3, // 已拒绝
    zim_friend_application_state_expired = 4,  // 已过期
    zim_friend_application_state_disabled = 5, // 已失效
};

enum zim_user_relation_type {
    zim_user_relation_type_unknown = 0,

    zim_user_relation_type_single_no = 1,
    /** 单向校验 - A 的好友表中有 B，但无法确定 B 的好友表中是否有 A  */
    zim_user_relation_type_single_have = 2,
    /** 双向校验 - A 的好友表中没有 B，B 的好友表中也没有 A */
    zim_user_relation_type_both_all_no = 3,
    /** 双向校验 - A 的好友表中有 B，但 B 的好友表中没有 A */
    zim_user_relation_type_both_self_have = 4,
    /** 双向校验 - A 的好友表中没有 B，但 B 的好友表中有 A */
    zim_user_relation_type_both_other_have = 5,
    /** 双向校验 - A 的好友表中有 B，B 的好友表中也有 A */
    zim_user_relation_type_both_all_have = 6,

};

// MARK: - MainStruct

/// Error structure
///
/// Description: A structure composed of error code enumeration values and detailed description of the corresponding error information.
///
struct zim_error {
    enum zim_error_code code;
    const char *message;
};

/// Log configuration
///
/// Description: Configure the storage path of log files and the maximum log file size.
///
struct zim_log_config {

    /// The storage path of the log files. Refer to the official website document for the default path.
    char *log_path;

    /// The maximum log file size (Bytes). The default maximum size is 5MB (5 * 1024 * 1024 Bytes)
    unsigned long long log_size;
};

/// Cache configuration
///
/// Description: Configure the storage path of cache files (such as chat records).
///
struct zim_cache_config {

    /// The storage path of the cache files. Refer to the official website document for the default path.
    char *cache_path;
};

struct zim_login_config {
    char *user_name;

    char *token;

    bool is_offline_login;
};

/// User information object.
///
/// Description: Identifies a unique user.
///
/// Caution: Note that the userID must be unique under the same appID, otherwise mutual kicks out will occur.
/// It is strongly recommended that userID corresponds to the user ID of the business APP,
/// that is, a userID and a real user are fixed and unique, and should not be passed to the SDK in a random userID.
/// Because the unique and fixed userID allows ZEGO technicians to quickly locate online problems.
///
struct zim_user_info {

    /// User ID, a string with a maximum length of 32 bytes or less. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'.
    char *user_id;

    /// User name, a string with a maximum length of 64 bytes or less
    char *user_name;

    char *user_avatar_url;
};

struct zim_user_full_info {
    /// User ID, a string with a maximum length of 32 bytes or less. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'.
    struct zim_user_info base_info;

    char *user_avatar_url;

    /// User extra, a string with a maximum length of 2K bytes.
    char *user_extended_data;
};

struct zim_room_member_info {
    struct zim_user_info user_info;
};

struct zim_error_user_info {
    const char *user_id;
    unsigned int reason;
};

struct zim_friend_relation_info {
    enum zim_user_relation_type type;
    const char *user_id;
};

struct zim_friend_info {
    const char *friend_alias;
    long long create_time;
    long long update_time;
    const char *wording;
    unsigned int friend_attributes_length;
    const struct zim_friend_attribute *friend_attributes;
    struct zim_user_info user_info;
};

struct zim_users_info_query_config {
    bool is_query_from_server;
};

struct zim_message_reaction_user_info {
    const char *user_id;
};

struct zim_message_reaction {
    const char *conversation_id;
    enum zim_conversation_type conversation_type;
    long long message_id;

    struct zim_message_reaction_user_info *user_list;
    unsigned int user_info_length;

    unsigned int total_count;
    const char *reaction_type;
    bool self_included;
};

struct zim_message_reaction_users_query_config {
    unsigned long long next_flag;
    const char *reaction_type;
    unsigned int count;
};

struct zim_message_mentioned_info {
    long long message_id;
    const char *from_user_id;
    enum zim_message_mentioned_type type;
};

// MARK: - MessageStruct

/// Base class of message object
///
/// Description: Identifies the basic parameters of a message.
///
/// Caution: Some of the parameters, such as Message ID, only have values ​​during the callback.
/// Developers do not need to assign values ​​to these parameters when they actively create this object for sending messages.
///
struct zim_message {

    // BaseMessage

    /// Identifies the type of this message.
    enum zim_message_type type;

    /// Description: The unique ID that identifies this message.
    ///
    /// Use cases: Can be used to index other messages.
    ///
    /// Caution: When the developer actively creates a message, there is no need to modify this parameter.
    /// This parameter only has a value during callback.
    long long message_id;

    long long local_message_id;
    /// Description: Identifies which User sent this message.
    ///
    /// Use cases: Used to show who sent a message.
    ///
    /// Caution: When the developer actively creates a message, there is no need to modify this parameter.
    /// This parameter only has a value during callback.
    char *sender_user_id;

    char *conversation_id;

    enum zim_message_direction direction;

    enum zim_message_sent_status sent_status;

    enum zim_conversation_type conversation_type;

    enum zim_message_receipt_status receipt_status;

    long long conversation_seq;

    /// Description: Identifies the sending time of a message
    ///
    /// Use cases: Used to present the sending time of a message, and can be used for message sorting.
    ///
    /// Caution: This is a standard UNIX timestamp, in milliseconds.
    unsigned long long timestamp;

    /// Developers can use this field to sort the index
    long long order_key;

    bool is_user_inserted;

    char *extended_data;

    char *local_extended_data;

    bool is_broadcast_message;

    struct zim_message_reaction *reactions;
    unsigned int reaction_length;

    char **mentioned_user_ids;
    unsigned int mentioned_user_ids_length;
    bool is_mention_all;

    // TextMessage/BarrageMessage/SystemMessage Only

    /// The content of the message supports arbitrary data. If the message type is text, it should be a standard UTF-8 string.
    char *message;

    // CommandMessage Only

    unsigned char *command_message;

    /// The length of the message. If the message type is text, it should be the length of the string.
    unsigned int command_message_length;

    // MediaMessage/FileMessage Only
    // 文件本地地址，该地址为发送富媒体消息时，用户唯一需要填入的字段
    char *file_local_path;

    // 文件下载地址
    char *file_download_url;

    char *file_uid;

    // 文件名，仅在接收文件消息时使用，供 SDK 返回给上层以展示文件名，其余时候为 nullptr
    char *file_name;

    // 文件大小，接收图片消息时为原图大小，接收文件信息时为文件大小，接收音视频消息时为音视频大小，
    // 由发送端 SDK 获取文件后，自动填入，并在发送消息时写入消息体内，接收端自行解析，以便接收端展示文件大小
    unsigned long long file_size;

    // ImageMessage Only

    // 大图下载地址，仅在接收图片消息时使用，供 SDK 返回给上层以给到上层使用文件下载（大图下载），其余时候为 nullptr
    char *large_image_download_url;

    // 缩略图下载地址，仅在接收图片/视频消息时使用，供 SDK 返回给上层以给到上层使用文件下载（缩略图/视频首帧缩略图下载），其余时候为 nullptr
    char *thumbnail_download_url;

    char *large_image_local_path;

    char *thumbnail_local_path;

    unsigned int original_image_width;
    unsigned int original_image_height;

    unsigned int large_image_width;
    unsigned int large_image_height;

    unsigned int thumbnail_width;
    unsigned int thumbnail_height;

    // AudioMessage/VideoMessage

    // 音视频时长，由发送端 SDK 获取文件后，自动填入，并在发送消息时写入消息体内，接收端自行解析，以便接收端展示时长
    unsigned long long media_duration;

    // VideoMessage Only
    char *video_first_frame_local_path;
    char *video_first_frame_download_url;
    unsigned int video_first_frame_width;
    unsigned int video_first_frame_height;

    // RevokeMessage Only
    enum zim_message_revoke_status revoke_status;
    enum zim_revoke_type revoke_type;
    unsigned long long revoke_timestamp;
    char *operated_userid;
    enum zim_message_type original_message_type;
    char *original_text_message;
    char *revoke_extended_data;

    // CustomMessage Only
    unsigned int sub_type;
    char *searched_content;

    // CombineMessage Only
    struct zim_message *combine_message_list;
    unsigned int combine_message_list_length;
    char *combine_title;
    char *combine_summary;
    char *combine_id;
    bool is_server_message;
};

struct zim_message_send_config {
    enum zim_message_priority priority;
    bool has_receipt;
    bool is_notify_mentioned_users;
    bool enable_offline_push;
    struct zim_push_config push_config;
};

/// Configuration for querying message.
///
/// Description: When querying message, you need to configure this object.
///
struct zim_message_query_config {

    /// Description: The unique ID that identifies a message.
    struct zim_message *next_message;

    /// Description: How many messages are retrieved in one query.
    ///
    /// Caution: To obtain messages in pages to reduce overhead, it is recommended to obtain within 100 messages at a time.
    unsigned int count;

    /// Description: Whether to query message from old to new forwardly or from new to old reversely.
    bool reverse;
};

struct zim_message_delete_config {
    bool is_also_delete_server_message;
};

struct zim_message_sent_status_change_info {
    struct zim_message message;
    enum zim_message_sent_status status;
    char *reason;
};

struct zim_message_receipt_info {
    enum zim_message_receipt_status status;
    long long message_id;
    char *conversation_id;
    enum zim_conversation_type conversation_type;
    unsigned int read_member_count;
    unsigned int unread_member_count;
    bool is_self_operated;
};

struct zim_group_message_receipt_member_query_config {
    unsigned int next_flag;
    unsigned int count;
};

struct zim_message_search_config {
    struct zim_message *next_message;
    unsigned int count;
    enum zim_message_order order;
    char **keywords;
    unsigned int keywords_length;
    enum zim_message_type *message_types;
    unsigned int message_types_length;
    int *sub_message_types;
    unsigned int sub_message_types_length;
    char **sender_user_ids;
    unsigned int sender_user_ids_length;
    long long start_time;
    long long end_time;
};

struct zim_conversation_search_config {
    unsigned int next_flag;
    unsigned int total_conversation_count;
    unsigned int conversation_message_count;
    char **keywords;
    unsigned int keywords_length;
    enum zim_message_type *message_types;
    unsigned int message_types_length;
    int *sub_message_types;
    unsigned int sub_message_types_length;
    char **sender_user_ids;
    unsigned int sender_user_ids_length;
    long long start_time;
    long long end_time;
};

struct zim_conversation_search_info {
    char *conversation_id;
    enum zim_conversation_type conversation_type;
    unsigned int total_message_count;
    struct zim_message *message_list;
    unsigned int message_list_length;
};

struct zim_conversations_all_deleted_info {
    unsigned int count;
};

struct zim_message_revoke_config {
    bool enable_offline_push;
    struct zim_push_config push_config;
    char *revoke_extended_data;
};

// MARK: - ConversationStruct

struct zim_conversation_attribute {
    char *key;
    char *value;
};

struct zim_conversation {
    // 会话ID
    char *conversation_id;
    // 会话名
    char *conversation_name;
    // 会话别名
    char *conversation_alias;
    // 会话头像
    char *conversation_avatar_url;
    // 会话类型
    enum zim_conversation_type type;
    // 未读消息数
    unsigned int unread_message_count;
    // 最后一条消息对象
    struct zim_message last_message;
    // 用于排序的索引
    long long order_key;
    // 会话的免打扰状态
    enum zim_conversation_notification_status notification_status;
    // 会话的置顶状态
    bool is_pinned;
    // 会话草稿
    char *draft;

    long user_self_muted_expiry_time;
    long group_muted_expiry_time;
    bool group_is_disabled;

    const struct zim_message_mentioned_info *mentioned_info_list;
    unsigned int mentioned_info_list_length;
};

struct zim_conversation_change_info {
    enum zim_conversation_event event;
    struct zim_conversation conversation;
};

struct zim_conversation_query_config {
    //struct zim_conversation *next_conversation;
    long long next_order_key;
    // 一次分页拉取的会话数量
    int count;
};

struct zim_conversation_delete_config {
    // 是否同时删除服务端会话
    bool is_also_delete_server_conversation;
};

// MARK: - GroupStruct

struct zim_group_attribute {
    char *key;
    char *value;
};

struct zim_group_mute_info {
    enum zim_group_mute_mode mute_mode;
    long long mute_expired_time;
    int *mute_role_list;
    unsigned int mute_role_list_length;
};

struct zim_group_member_info {
    char *user_id;
    char *user_name;
    char *member_nick_name;
    int member_role;
    char *member_avatar_url;
    long long mute_expired_time;
};

struct zim_group_operated_info {
    char *user_id;
    char *user_name;
    char *member_nick_name;
    int member_role;
};

struct zim_group_info {
    char *group_id;
    char *group_name;
    char *group_avatar_url;
};

struct zim_group_advanced_config {
    char *group_notice;
    struct zim_group_attribute *group_attributes;
    unsigned int group_attributes_length;
};

struct zim_group_full_info {
    struct zim_group_info base_info;
    char *group_notice;
    struct zim_group_attribute *group_attributes;
    unsigned int group_attributes_length;
    enum zim_group_message_notification_status notification_status;
    struct zim_group_mute_info muted_info;
};

struct zim_group {
    struct zim_group_info base_info;
    enum zim_group_message_notification_status notification_status;
};

struct zim_group_member_query_config {
    unsigned int next_flag;
    unsigned int count;
};

struct zim_group_attributes_update_info {
    enum zim_group_attributes_update_action action;

    struct zim_group_attribute *group_attributes;

    unsigned int group_attributes_length;
};

struct zim_group_mute_config {
    enum zim_group_mute_mode mode;
    int duration;
    int *mute_role_list;
    unsigned int mute_role_list_length;
};

struct zim_group_member_list_mute_config {
    int duration;
};

struct zim_group_muted_members_query_config {
    unsigned long long next_flag;
    unsigned int count;
};

struct zim_group_search_config {
    unsigned int next_flag;
    unsigned int count;
    char **keywords;
    unsigned int keywords_length;
    bool is_also_match_group_member_user_name;
    bool is_also_match_group_member_nick_name;
};

struct zim_group_search_info {
    struct zim_group_info group_info;
    struct zim_group_member_info *user_list;
    unsigned int user_list_length;
};

struct zim_group_member_search_config {
    unsigned int next_flag;
    unsigned int count;
    char **keywords;
    unsigned int keywords_length;
    bool is_also_match_group_member_nick_name;
};

// MARK: - RoomStruct

/// Room information object.
///
/// Description: Identifies a unique room.
///
struct zim_room_info {

    /// Room ID, a string with a maximum length of 32 bytes or less. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'.
    char *room_id;

    /// Room name, a string with a maximum length of 64 bytes or less
    char *room_name;
};

/// Room attribute object.
///
/// Description: Room attribute structure.
///
struct zim_room_attribute {

    /// The sequence of the room attribute
    //unsigned int sequence;

    /// Attribute key, visible characters, no more than 16
    char *key;

    /// The attribute value, which can be any character, does not exceed 1k in a single entry, and does not exceed 5k in total
    char *value;
};

struct zim_room_advanced_config {

    struct zim_room_attribute *room_attributes;

    unsigned int room_attributes_length;

    unsigned int room_destroy_delay_time;
};

/// Room attribute updated information (usually obtained by server push)
///
/// Description: Room attributes updated information structure.
///

struct zim_room_attributes_update_info {

    /// Room attribute change event
    enum zim_room_attributes_update_action room_attributes_action;

    /// 房间属性信息
    struct zim_room_attribute *room_attributes;

    unsigned int room_attributes_length;
};

struct zim_room_attributes_set_config {
    bool is_force;
    bool is_delete_after_owner_left;
    bool is_update_owner;
};

struct zim_room_attributes_delete_config {
    bool is_force;
};

struct zim_room_attributes_batch_operation_config {
    bool is_force;
    bool is_delete_after_owner_left;
    bool is_update_owner;
};

struct zim_room_member_attribute {
    char *key;
    char *value;
};

struct zim_room_member_attributes_info {
    char *user_id;
    struct zim_room_member_attribute *attributes;
    unsigned int attributes_length;
};

struct zim_room_member_attributes_update_info {
    struct zim_room_member_attributes_info attributes_info;
};

struct zim_room_member_attributes_operated_info {
    struct zim_room_member_attributes_info attributes_info;
    char **error_keys;
    unsigned int error_keys_length;
};

struct zim_room_member_attributes_set_config {
    bool is_delete_after_owner_left;
};

struct zim_room_member_attributes_query_config {
    char *next_flag;
    unsigned int count;
};

/// The room details object.
///
/// Description: Identifies the basic parameters of a message.
///
/// Caution: Identifies the detailed information of a room.
///
struct zim_room_full_info {

    /// The base object of the room.
    struct zim_room_info base_info;
};

/// Configuration for querying member.
///
/// Description: When querying member, you need to configure this object.
///
struct zim_room_member_query_config {

    /// Description: The flag of the paging query. For the first query, set this field to an empty string. If the "nextFlag" field of the callback is not an empty string, it needs to be set here to continue the query on the next page.
    char *next_flag;

    /// Description: How many messages are retrieved in one query.
    ///
    /// Caution: To obtain messages in pages to reduce overhead, it is recommended to obtain within 100 messages at a time.
    unsigned int count;
};

struct zim_room_operated_info {
    char *user_id;
};

// MARK: - CallStruct

struct zim_call_user_info {
    const char *user_id;
    enum zim_call_user_state user_state;
    const char *extended_data;
};

struct zim_call_invite_config {
    int timeout;
    char *extended_data;
    bool enable_offline_push;
    enum zim_call_invitation_mode mode;
    struct zim_push_config push_config;
};

struct zim_calling_invite_config {
    bool enable_offline_push;
    struct zim_push_config push_config;
};

struct zim_call_join_config {
    char *extended_data;
};

struct zim_call_cancel_config {
    char *extended_data;
    bool enable_offline_push;
    struct zim_push_config push_config;
};

struct zim_call_accept_config {
    char *extended_data;
};

struct zim_call_reject_config {
    char *extended_data;
};

struct zim_call_quit_config {
    char *extended_data;
    bool enable_offline_push;
    struct zim_push_config push_config;
};

struct zim_query_call_list_config {
    unsigned int count;

    long long next_flag;
};

struct zim_call_end_config {
    char *extended_data;
    bool enable_offline_push;
    struct zim_push_config push_config;
};

struct zim_call_invitation_sent_info {
    int timeout;
    struct zim_call_user_info *old_error_invitees;
    struct zim_error_user_info *error_invitees;
    unsigned int error_invitees_length;
    unsigned int old_error_invitees_length;
};

struct zim_calling_invitation_sent_info {
    struct zim_error_user_info *error_invitees;
    unsigned int error_invitees_length;
};

struct zim_call_join_sent_info {
    const char *extended_data;
    long long create_time;
    long long join_time;
    const struct zim_call_user_info *call_user_list;
    unsigned int call_user_list_length;
};

struct zim_call_quit_sent_info {
    long long create_time;
    long long accept_time;
    long long quit_time;
};

struct zim_call_end_sent_info {
    long long create_time;
    long long accept_time;
    long long end_time;
};

struct zim_call_invitation_received_info {
    enum zim_call_invitation_mode mode;
    long long create_time;
    int timeout;
    const char *caller;
    const char *inviter;
    const struct zim_call_user_info *call_user_list;
    unsigned int call_user_list_length;
    const char *extended_data;
};

struct zim_call_invitation_created_info {
    enum zim_call_invitation_mode mode;
    const char *caller;
    const char *extended_data;
    unsigned int timeout;
    long long create_time;
    unsigned int call_user_list_length;
    const struct zim_call_user_info *call_user_list;
};

struct zim_call_invitation_cancelled_info {
    enum zim_call_invitation_mode mode;
    const char *inviter;
    char *extended_data;
};

struct zim_call_invitation_accepted_info {
    const char *invitee;
    char *extended_data;
};

struct zim_call_invitation_rejected_info {
    const char *invitee;
    char *extended_data;
};

struct zim_call_invitation_timeout_info {
    enum zim_call_invitation_mode mode;
    bool also_invoke_discard_method;
};

struct zim_call_invitation_ended_info {
    enum zim_call_invitation_mode mode;
    const char *caller;
    const char *operated_user_id;
    const char *extended_data;
    long long end_time;
};

struct zim_event_call_user_state_changed_info {
    const char *call_id;
    const struct zim_call_user_info *call_user_list;
    unsigned int call_user_list_length;
};

struct zim_call_info {
    const char *call_id;
    const char *caller;
    const char *inviter;
    long long create_time;
    long long end_time;
    enum zim_call_state state;
    enum zim_call_invitation_mode mode;
    const char *extended_data;
    const struct zim_call_user_info *call_user_list;
    unsigned int call_user_list_length;
    // int timeout;
    //    long long call_duration;
    //    long long user_duration;
};

struct zim_event_call_state_changed_info {
    struct zim_call_info call_info;
};

struct zim_friend_attribute {
    const char *key;
    const char *value;
};

struct zim_friend_add_config {
    const char *wording;
    const char *friend_alias;
    struct zim_friend_attribute *friend_attributes;
    unsigned int friend_attributes_length;
};

enum zim_friend_delete_type {
    zim_friend_delete_type_both = 0,
    zim_friend_delete_type_single = 1,
};

struct zim_friend_delete_config {
    enum zim_friend_delete_type type;
};

struct zim_friend_application_send_config {
    const char *wording;
    const char *friend_alias;
    struct zim_friend_attribute *friend_attributes;
    unsigned int friend_attributes_length;
    bool enable_offline_push;
    struct zim_push_config push_config;
};

struct zim_friend_application_info {
    struct zim_user_info apply_user;
    const char *wording;
    long long create_time;
    long long update_time;
    enum zim_friend_application_type type;
    enum zim_friend_application_state state;
};

struct zim_friend_application_accept_config {
    const char *friend_alias;
    struct zim_friend_attribute *friend_attributes;
    unsigned int friend_attributes_length;
    bool enable_offline_push;
    struct zim_push_config push_config;
};

struct zim_friend_application_reject_config {
    struct zim_push_config push_config;
    bool enable_offline_push;
};

struct zim_friend_list_query_config {
    unsigned int count;
    unsigned int next_flag;
};

struct zim_blacklist_query_config {
    unsigned int count;
    unsigned int next_flag;
};

struct zim_friend_search_config {
    unsigned int count;
    unsigned int next_flag;
    char **keywords;
    unsigned int keywords_length;
    bool is_also_match_friend_alias;
};

struct zim_friend_application_list_query_config {
    unsigned int count;
    unsigned int next_flag;
};

struct zim_friend_relation_check_config {
    enum zim_friend_relation_check_type type;
};

#endif //__ZIM_DEFINES_H__
