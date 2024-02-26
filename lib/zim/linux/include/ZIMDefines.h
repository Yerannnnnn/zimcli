#pragma once

//
//  ZIMDefines.h
//  ZIM
//
//  Copyright © 2021 Zego. All rights reserved.
//

#include "ZIMErrorCode.h"
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace zim {

class ZIMBridge;
class ZIMConverter;

// MARK: - Enum

// Connection state.
//
// Description: The state machine that identifies the current connection state.
//
// Use cases: It can be used to determine whether the login/logout is successful,
// and to handle abnormal situations such as network disconnection.
//
// Caution: Please use it with the connection event parameter.
//
enum ZIMConnectionState {

    // Unconnected state, enter this state before logging in and after logging out. If there is a steady state abnormality in the process of logging in, such as AppID or Token are incorrect, or if the same user name is logged in elsewhere and the local end is kicked out, it will enter this state.
    ZIM_CONNECTION_STATE_DISCONNECTED = 0,

    // The state that the connection is being requested. It will enter this state after successful execution login function. The display of the UI is usually performed using this state. If the connection is interrupted due to poor network quality, the SDK will perform an internal retry and will return to this state.
    ZIM_CONNECTION_STATE_CONNECTING = 1,

    // The state that is successfully connected. Entering this state indicates that login successfully and the user can use the SDK functions normally.
    ZIM_CONNECTION_STATE_CONNECTED = 2,

    // The state that the reconnecting is being requested. It will enter this state after successful execution login function. The display of the UI is usually performed using this state. If the connection is interrupted due to poor network quality, the SDK will perform an internal retry and will return to this state.
    ZIM_CONNECTION_STATE_RECONNECTING = 3
};

// The event that caused the connection status to change.
//
// Description: The reason for the change of the connection state.
//
// Use cases: It can be used to determine whether the login/logout is successful,
// and to handle abnormal situations such as network disconnection.
//
// Caution: Please use it with the connection state parameter.
//
enum ZIMConnectionEvent {

    // Success.
    ZIM_CONNECTION_EVENT_SUCCESS = 0,

    // The user actively logs in.
    ZIM_CONNECTION_EVENT_ACTIVE_LOGIN = 1,

    // Connection timed out.
    ZIM_CONNECTION_EVENT_LOGIN_TIMEOUT = 2,

    // The network connection is temporarily interrupted.
    ZIM_CONNECTION_EVENT_LOGIN_INTERRUPTED = 3,

    // Being kicked out.
    ZIM_CONNECTION_EVENT_KICKED_OUT = 4,

    // Being token expired.
    ZIM_CONNECTION_EVENT_TOKEN_EXPIRED = 5,

    ZIM_CONNECTION_EVENT_UNREGISTERED = 6
};

enum ZIMGeofencingType {

    ZIM_GEOFENCING_TYPE_NONE = 0,

    ZIM_GEOFENCING_TYPE_INCLUDE = 1,

    ZIM_GEOFENCING_TYPE_EXCLUDE = 2
};

class ZIMGeofencingArea {

    static const int ZIM_GEOFENCING_AREA_CN = 2;

    static const int ZIM_GEOFENCING_AREA_NA = 3;

    static const int ZIM_GEOFENCING_AREA_EU = 4;

    static const int ZIM_GEOFENCING_AREA_AS = 5;

    static const int ZIM_GEOFENCING_AREA_IN = 6;
};

enum ZIMRoomState {

    // Unconnected state, enter this state before logging in and after logging out. If there is a steady state abnormality in the process of logging in, such as AppID or Token are incorrect, or if the same user name is logged in elsewhere and the local end is kicked out, it will enter this state.
    ZIM_ROOM_STATE_DISCONNECTED = 0,

    // The state that the connection is being requested. It will enter this state after successful execution login function. The display of the UI is usually performed using this state. If the connection is interrupted due to poor network quality, the SDK will perform an internal retry and will return to this state.
    ZIM_ROOM_STATE_CONNECTING = 1,

    // The state that is successfully connected. Entering this state indicates that login successfully and the user can use the SDK functions normally.
    ZIM_ROOM_STATE_CONNECTED = 2

};

enum ZIMRoomEvent {

    // Success.
    ZIM_ROOM_EVENT_SUCCESS = 0,
    ZIM_ROOM_EVENT_NETWORK_INTERRUPTED = 1,
    ZIM_ROOM_EVENT_NETWORK_DISCONNECTED = 2,
    ZIM_ROOM_EVENT_ROOM_NOT_EXIST = 3,
    ZIM_ROOM_EVENT_ACTIVE_CREATE = 4,
    ZIM_ROOM_EVENT_CREATE_FAILED = 5,
    ZIM_ROOM_EVENT_ACTIVE_ENTER = 6,
    ZIM_ROOM_EVENT_ENTER_FAILED = 7,
    ZIM_ROOM_EVENT_KICKED_OUT = 8,
    ZIM_ROOM_EVENT_CONNECT_TIMEOUT = 9,
    ZIM_ROOM_EVENT_KICKED_OUT_BY_OTHER_DEVICE = 10
};

enum ZIMRoomAttributesUpdateAction {
    ZIM_ROOM_ATTRIBUTES_UPDATE_ACTION_SET = 0,
    ZIM_ROOM_ATTRIBUTES_UPDATE_ACTION_DELETE = 1
};

enum ZIMBlacklistChangeAction {
    ZIM_BLACKLIST_CHANGE_ACTION_ADDED = 0,
    ZIM_BLACKLIST_CHANGE_ACTION_REMOVED = 1
};

enum ZIMFriendListChangeAction {
    ZIM_FRIEND_LIST_CHANGE_ACTION_ADDED = 0,
    ZIM_FRIEND_LIST_CHANGE_ACTION_DELETED = 1,
};

enum ZIMFriendApplicationListChangeAction {
    ZIM_FRIEND_APPLICATION_LIST_CHANGE_ACTION_ADDED = 0,
    ZIM_FRIEND_APPLICATION_LIST_CHANGE_ACTION_DELETED = 1,
};

enum ZIMGroupAttributesUpdateAction {
    ZIM_GROUP_ATTRIBUTES_UPDATE_ACTION_SET = 0,
    ZIM_GROUP_ATTRIBUTES_UPDATE_ACTION_DELETE = 1
};

enum ZIMUserRelationType {
    ZIM_USER_RELATION_TYPE_UNKNOWN = 0,
    ZIM_USER_RELATION_TYPE_SINGLE_NO = 1,
    ZIM_USER_RELATION_TYPE_SINGLE_HAVE = 2,
    ZIM_USER_RELATION_TYPE_BOTH_ALL_NO = 3,
    ZIM_USER_RELATION_TYPE_BOTH_SELF_HAVE = 4,
    ZIM_USER_RELATION_TYPE_BOTH_OTHER_HAVE = 5,
    ZIM_USER_RELATION_TYPE_BOTH_ALL_HAVE = 6,
};

// The priority of the message.
//
// Description: Identifies the priority of a message.
//
// Use cases: It can be used to set the priority when a message is sent.
// The higher the priority, the higher the reliability.
// Low priority may be discarded due to weak network.
//
// Caution: The higher the priority, the higher the cost.
//
enum ZIMMessagePriority {

    // Description: Low priority.
    //
    // Use cases: Generally used to send unimportant messages such as barrage message in a room.

    ZIM_MESSAGE_PRIORITY_LOW = 1,

    // Description: Medium priority.
    //
    // Use cases: Generally used to send regular chat messages.

    ZIM_MESSAGE_PRIORITY_MEDIUM = 2,

    // Description: High priority.
    //
    // Use cases: Generally used to send important information such as gifts and rewards in a room.

    ZIM_MESSAGE_PRIORITY_HIGH = 3

};

// The type of the message.
//
// Description: Identifies the type of current message.
//
// Use cases: It can be used to determine what type of message this message is.
//
enum ZIMMessageType {

    ZIM_MESSAGE_TYPE_UNKNOWN = 0,

    // Normal text message.
    ZIM_MESSAGE_TYPE_TEXT = 1,

    // Custom binary message.
    ZIM_MESSAGE_TYPE_COMMAND = 2,

    ZIM_MESSAGE_TYPE_IMAGE = 11,

    ZIM_MESSAGE_TYPE_FILE = 12,

    ZIM_MESSAGE_TYPE_AUDIO = 13,

    ZIM_MESSAGE_TYPE_VIDEO = 14,

    ZIM_MESSAGE_TYPE_BARRAGE = 20,

    ZIM_MESSAGE_TYPE_SYSTEM = 30,

    ZIM_MESSAGE_TYPE_REVOKE = 31,

    ZIM_MESSAGE_TYPE_COMBINE = 100,

    ZIM_MESSAGE_TYPE_CUSTOM = 200,
};

enum ZIMMediaFileType {
    ZIM_MEDIA_FILE_TYPE_ORIGINAL_FILE = 1,

    ZIM_MEDIA_FILE_TYPT_LARGE_IMAGE = 2,

    ZIM_MEDIA_FILE_TYPT_THUMBNAIL = 3,

    ZIM_MEDIA_FILE_TYPE_VIDEO_FIRST_FRAME = 4
};

enum ZIMMessageDirection { ZIM_MESSAGE_DIRECTION_SEND = 0, ZIM_MESSAGE_DIRECTION_RECEIVE = 1 };

enum ZIMMessageSentStatus {
    ZIM_MESSAGE_SENT_STATUS_SENDING = 0,
    ZIM_MESSAGE_SENT_STATUS_SEND_SUCCESS = 1,
    ZIM_MESSAGE_SENT_STATUS_SEND_FAILED = 2
};

enum ZIMMessageReceiptStatus {
    ZIM_MESSAGE_RECEIPT_STATUS_UNKNOWN = -1,
    ZIM_MESSAGE_RECEIPT_STATUS_NONE = 0,
    ZIM_MESSAGE_RECEIPT_STATUS_PROCESSING = 1,
    ZIM_MESSAGE_RECEIPT_STATUS_DONE = 2,
    ZIM_MESSAGE_RECEIPT_STATUS_EXPIRED = 3,
    ZIM_MESSAGE_RECEIPT_STATUS_FAILED = 4
};

enum ZIMRevokeType {
    ZIM_REVOKE_TYPE_UNKNOWN = -1,
    ZIM_REVOKE_TYPE_TWO_WAY = 0,
    ZIM_REVOKE_TYPE_ONE_WAY = 1
};

enum ZIMMessageRevokeStatus {
    ZIM_MESSAGE_REVOKE_STATUS_UNKNOWN = -1,
    ZIM_MESSAGE_REVOKE_STATUS_SELF_REVOKE = 0,
    ZIM_MESSAGE_REVOKE_STATUS_SYSTEM_REVOKE = 1,
    ZIM_MESSAGE_REVOKE_STATUS_SERVER_API_REVOKE = 2,
    ZIM_MESSAGE_REVOKE_STATUS_GROUP_ADMIN_REVOKE = 3,
    ZIM_MESSAGE_REVOKE_STATUS_GROUP_OWNER_REVOKE = 4
};

enum ZIMMessageDeleteType {
    ZIM_MESSAGE_DELETE_TYPE_MESSAGE_LIST_DELETED = 0,
    ZIM_MESSAGE_DELETE_TYPE_CONVERSATION_ALL_MESSAGES_DELETED = 1,
    ZIM_MESSAGE_DELETE_TYPE_ALL_CONVERSATION_MESSAGES_DELETED = 2,
};

enum ZIMMessageOrder { ZIM_MESSAGE_ORDER_DESCENDING = 0, ZIM_MESSAGE_ORDER_ASCENDING = 1 };

enum ZIMMessageMentionedType {
    ZIM_MESSAGE_MENTIONED_TYPE_MENTION_ME = 1,
    ZIM_MESSAGE_MENTIONED_TYPE_MENTION_ALL = 2,
    ZIM_MESSAGE_MENTIONED_TYPE_MENTION_ALL_AND_ME = 3
};

enum ZIMConversationType {
    ZIM_CONVERSATION_TYPE_UNKNOWN = -1,
    ZIM_CONVERSATION_TYPE_PEER = 0,
    ZIM_CONVERSATION_TYPE_ROOM = 1,
    ZIM_CONVERSATION_TYPE_GROUP = 2
};

enum ZIMConversationNotificationStatus {
    ZIM_CONVERSATION_NOTIFICATION_STATUS_NOTIFY = 1,
    ZIM_CONVERSATION_NOTIFICATION_STATUS_DO_NOT_DISTURB = 2
};

enum ZIMConversationEvent {
    ZIM_CONVERSATION_EVENT_ADDED = 0,
    ZIM_CONVERSATION_EVENT_UPDATED = 1,
    ZIM_CONVERSATION_EVENT_DISABLED = 2,
    ZIM_CONVERSATION_EVENT_DELETED = 3
};

// 群变更事件
enum ZIMGroupEvent {
    ZIM_GROUP_EVENT_CREATED = 1,
    ZIM_GROUP_EVENT_DISMISSED = 2,
    ZIM_GROUP_EVENT_JOINED = 3,
    ZIM_GROUP_EVENT_INVITED = 4,
    ZIM_GROUP_EVENT_LEFT = 5,
    ZIM_GROUP_EVENT_KICKED_OUT = 6
};

// 群状态
enum ZIMGroupState { ZIM_GROUP_STATE_QUIT = 0, ZIM_GROUP_STATE_ENTER = 1 };

enum ZIMGroupMuteMode {
    ZIM_GROUP_MUTE_MODE_NONE = 0,
    ZIM_GROUP_MUTE_MODE_NORMAL = 1,
    ZIM_GROUP_MUTE_MODE_ALL = 2,
    ZIM_GROUP_MUTE_MODE_CUSTOM = 3,
};

enum ZIMGroupMemberState { ZIM_GROUP_MEMBER_STATE_QUIT = 0, ZIM_GROUP_MEMBER_STATE_ENTER = 1 };

enum ZIMGroupMemberEvent {
    ZIM_GROUP_MEMBER_EVENT_JOINED = 1,
    ZIM_GROUP_MEMBER_EVENT_LEFT = 2,
    ZIM_GROUP_MEMBER_EVENT_KICKED_OUT = 4,
    ZIM_GROUP_MEMBER_EVENT_INVITED = 5
};

enum ZIMCallInvitationMode {
    ZIM_INVITATION_MODE_UNKNOWN = -1,
    ZIM_INVITATION_MODE_GENERAL = 0,
    ZIM_INVITATION_MODE_ADVANCED = 1
};

enum ZIMCallState {
    ZIM_CALL_STATE_UNKNOWN = -1,
    ZIM_CALL_STATE_STARTED = 1,
    ZIM_CALL_STATE_ENDED = 2
};

enum ZIMCallUserState {
    ZIM_CALL_USER_STATE_UNKNOWN = -1,
    ZIM_CALL_USER_STATE_INVITING = 0,
    ZIM_CALL_USER_STATE_ACCEPTED = 1,
    ZIM_CALL_USER_STATE_REJECTED = 2,
    ZIM_CALL_USER_STATE_CANCELLED = 3,
    /// ZIM_CALL_USER_STATE_OFFLINE is deprecated since ZIM 2.9.0
    ZIM_CALL_USER_STATE_OFFLINE = 4,
    ZIM_CALL_USER_STATE_RECEIVED = 5,
    ZIM_CALL_USER_STATE_TIMEOUT = 6,
    ZIM_CALL_USER_STATE_QUIT = 7,
    ZIM_CALL_USER_STATE_ENDED = 8,
};

enum ZIMCXHandleType {
    ZIM_CX_HANDLE_TYPE_GENERIC = 1,
    ZIM_CX_HANDLE_TYPE_PHONE_NUMBER = 2,
    ZIM_CX_HANDLE_TYPE_EMAIL_ADDRESS = 3
};

enum ZIMFriendApplicationType {
    ZIM_FRIEND_APPLICATION_TYPE_UNKNOWN = -1,
    ZIM_FRIEND_APPLICATION_TYPE_NONE = 0,
    ZIM_FRIEND_APPLICATION_TYPE_RECEIVED = 1,
    ZIM_FRIEND_APPLICATION_TYPE_SENT = 2,
    ZIM_FRIEND_APPLICATION_TYPE_BOTH = 3,
};

enum ZIMFriendRelationCheckType {
    ZIM_FRIEND_RELATION_CHECK_TYPE_UNKNOWN = -1,
    ZIM_FRIEND_RELATION_CHECK_TYPE_BOTH = 0,
    ZIM_FRIEND_RELATION_CHECK_TYPE_SINGLE = 1,
};

enum ZIMFriendDeleteType {
    ZIM_FRIEND_DELETE_TYPE_BOTH = 0,
    ZIM_FRIEND_DELETE_TYPE_SINGLE = 1,
};

enum ZIMFriendApplicationState {
    ZIM_FRIEND_APPLICATION_STATE_UNKNOWN = -1,
    ZIM_FRIEND_APPLICATION_STATE_WAITING = 1,
    ZIM_FRIEND_APPLICATION_STATE_ACCEPTED = 2,
    ZIM_FRIEND_APPLICATION_STATE_REJECTED = 3,
    ZIM_FRIEND_APPLICATION_STATE_EXPIRED = 4,
    ZIM_FRIEND_APPLICATION_STATE_DISABLED = 5,
};

// User information object.
//
// Description: Identifies a unique user.
//
// Caution: Note that the userID must be unique under the same appID, otherwise mutual kicks out will occur.
// It is strongly recommended that userID corresponds to the user ID of the business APP,
// that is, a userID and a real user are fixed and unique, and should not be passed to the SDK in a random userID.
// Because the unique and fixed userID allows ZEGO technicians to quickly locate online problems.
//

class ZIMAppConfig {
  public:
    unsigned int appID;
    std::string appSign;

  public:
    ZIMAppConfig() : appID(0) {}
};

class ZIMLoginConfig {
  public:
    ZIMLoginConfig() : isOfflineLogin(false) {}

  public:
    std::string userName;
    std::string token;
    bool isOfflineLogin;
};

class ZIMUserInfo {

  public:
    // User ID, a string with a maximum length of 32 bytes or less. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'.
    std::string userID;

    // User name, a string with a maximum length of 64 bytes or less
    std::string userName;

    std::string userAvatarUrl;

  public:
    ZIMUserInfo() {}
    ZIMUserInfo(const std::string &id, const std::string &name, const std::string &avatar)
        : userID(id), userName(name), userAvatarUrl(avatar) {}
};

class ZIMRoomMemberInfo : public ZIMUserInfo {
  public:
    ZIMRoomMemberInfo() {}
    ZIMRoomMemberInfo(const std::string &id, const std::string &name) : ZIMUserInfo(id, name, "") {}
};

class ZIMUsersInfoQueryConfig {
  public:
    bool isQueryFromServer;

  public:
    ZIMUsersInfoQueryConfig() : isQueryFromServer(false) {}
    ZIMUsersInfoQueryConfig(bool is_query_server) : isQueryFromServer(is_query_server) {}
};

class ZIMUserFullInfo {

  public:
    ZIMUserInfo baseInfo;

    // @deprecated
    std::string userAvatarUrl;

    std::string extendedData;

  public:
    ZIMUserFullInfo() {}
    ZIMUserFullInfo(const std::string &id, const std::string &name, const std::string &avatar_url,
                    const std::string &extended_data)
        : baseInfo(id, name, avatar_url), userAvatarUrl(avatar_url), extendedData(extended_data) {}
};

typedef int ZIMGroupMemberRole;
#define ZIM_GROUP_MEMBER_ROLE_OWNER 1
#define ZIM_GROUP_MEMBER_ROLE_MEMBER 3

enum ZIMGroupMessageNotificationStatus {
    ZIM_GROUP_MESSAGE_NOTIFICATION_STATUS_NOTIFY = 1,
    ZIM_GROUP_MESSAGE_NOTIFICATION_STATUS_DO_NOT_DISTURB = 2
};

class ZIMErrorUserInfo {
  public:
    std::string userID;
    unsigned int reason;

  public:
    ZIMErrorUserInfo() : reason(0) {}
    ZIMErrorUserInfo(const std::string &user_id, unsigned int reason)
        : userID(user_id), reason(reason) {}
};

// Group information.
class ZIMGroupInfo {
  public:
    std::string groupID;
    std::string groupName;
    std::string groupAvatarUrl;

  public:
    ZIMGroupInfo() {}
    ZIMGroupInfo(const std::string &group_id, const std::string &group_name,
                 const std::string &group_avatar_url)
        : groupID(group_id), groupName(group_name), groupAvatarUrl(group_avatar_url) {}
};

class ZIMGroupMuteInfo {
  public:
    ZIMGroupMuteInfo() : mode(ZIMGroupMuteMode::ZIM_GROUP_MUTE_MODE_NONE), expiredTime(0) {}

  public:
    ZIMGroupMuteMode mode;
    long long expiredTime;
    std::vector<ZIMGroupMemberRole> roles;
};

// Group advanced configuration.
class ZIMGroupAdvancedConfig {
  public:
    std::string groupNotice;
    std::unordered_map<std::string, std::string> groupAttributes;

  public:
    ZIMGroupAdvancedConfig() {}
};

class ZIMGroupMuteConfig {
  public:
    ZIMGroupMuteConfig() : mode(ZIMGroupMuteMode::ZIM_GROUP_MUTE_MODE_ALL), duration(-1) {}

  public:
    ZIMGroupMuteMode mode;
    int duration;
    std::vector<ZIMGroupMemberRole> roles;
};

class ZIMGroupMemberMuteConfig {
  public:
    ZIMGroupMemberMuteConfig() : duration(-1) {}

  public:
    int duration;
};

// ZIMGroup
class ZIMGroup {
  public:
    ZIMGroupInfo baseInfo;
    ZIMGroupMessageNotificationStatus notificationStatus;

  public:
    ZIMGroup()
        : notificationStatus(
              ZIMGroupMessageNotificationStatus::ZIM_GROUP_MESSAGE_NOTIFICATION_STATUS_NOTIFY) {}
};

// Complete group information.
class ZIMGroupFullInfo {
  public:
    ZIMGroupInfo baseInfo;
    std::string groupNotice;
    std::unordered_map<std::string, std::string> groupAttributes;
    ZIMGroupMessageNotificationStatus notificationStatus;
    ZIMGroupMuteInfo mutedInfo;

  public:
    ZIMGroupFullInfo()
        : notificationStatus(
              ZIMGroupMessageNotificationStatus::ZIM_GROUP_MESSAGE_NOTIFICATION_STATUS_NOTIFY) {}
};

// Group member information.
class ZIMGroupMemberInfo : public ZIMUserInfo {
  public:
    std::string memberNickname;
    ZIMGroupMemberRole memberRole;
    // @deprecated
    std::string memberAvatarUrl;
    long long muteExpiredTime;

  public:
    ZIMGroupMemberInfo() : memberRole(ZIM_GROUP_MEMBER_ROLE_MEMBER), muteExpiredTime(0) {}
};

// Information that the group has operated on.
class ZIMGroupOperatedInfo {
  public:
    // @deprecated
    ZIMGroupMemberInfo operatedUserInfo;

    std::string userID;
    std::string userName;
    std::string memberNickname;
    ZIMGroupMemberRole memberRole;

  public:
    ZIMGroupOperatedInfo() { this->memberRole = ZIM_GROUP_MEMBER_ROLE_MEMBER; }
};

// Group member query configuration.
class ZIMGroupMemberQueryConfig {
  public:
    unsigned int count;
    unsigned int nextFlag;

  public:
    ZIMGroupMemberQueryConfig() : count(0), nextFlag(0) {}
};

class ZIMGroupMemberMutedListQueryConfig {
  public:
    unsigned int count;
    unsigned long long nextFlag;

  public:
    ZIMGroupMemberMutedListQueryConfig() : count(0), nextFlag(0) {}
};

// MARK: - Model

// Error structure
//
// Description: A structure composed of error code enumeration values and detailed description of the corresponding error information.
//
class ZIMError {

  public:
    // The storage path of the log files. Refer to the official website document for the default path.

    ZIMErrorCode code;

    // Error infomation description.

    std::string message;

  public:
    ZIMError() : code(ZIMErrorCode::ZIM_ERROR_CODE_SUCCESS) {}
};

// Log configuration
//
// Description: Configure the storage path of log files and the maximum log file size.
//
class ZIMLogConfig {

  public:
    // The storage path of the log files. Refer to the official website document for the default path.
    std::string logPath;

    // The maximum log file size (Bytes). The default maximum size is 5MB (5 * 1024 * 1024 Bytes)
    unsigned long long logSize;

  public:
    ZIMLogConfig() { this->logSize = 5 * 1024 * 1024; }
    ZIMLogConfig(const std::string &path, unsigned long long size) {
        this->logPath = path;
        this->logSize = size;
    }
};

// Cache configuration
//
// Description: Configure the storage path of cache files (such as chat records).
//
class ZIMCacheConfig {

  public:
    // The storage path of the cache files. Refer to the official website document for the default path.
    std::string cachePath;

  public:
    ZIMCacheConfig() {}
};
class ZIMMessageReactionUserInfo {
  public:
    std::string userID;

  public:
    ZIMMessageReactionUserInfo(){};
};
class ZIMMessageReaction {
  public:
    std::string reactionType;
    std::string conversationID;
    ZIMConversationType conversationType;
    long long messageID;
    unsigned int totalCount;
    bool isSelfIncluded;
    std::vector<ZIMMessageReactionUserInfo> userList;

  public:
    ZIMMessageReaction() : messageID(0), totalCount(0), isSelfIncluded(false) {}
};
class ZIMMessageReactionUserQueryConfig {
  public:
    unsigned long long nextFlag;
    unsigned int count;
    std::string reactionType;

  public:
    ZIMMessageReactionUserQueryConfig() : nextFlag(0), count(0) {}
};

class ZIMMessageMentionedInfo {
  public:
    long long messageID;
    std::string fromUserID;
    ZIMMessageMentionedType type;

  public:
    ZIMMessageMentionedInfo() : messageID(0), type(ZIM_MESSAGE_MENTIONED_TYPE_MENTION_ME) {}
};

// Base class of message object
//
// Description: Identifies the basic parameters of a message.
//
// Caution: Some of the parameters, such as Message ID, only have values ​​during the callback.
// Developers do not need to assign values ​​to these parameters when they actively create this object for sending messages.
//
class ZIMMessage {

  public:
    ZIMMessage()
        : isMentionAll(false), type(ZIMMessageType::ZIM_MESSAGE_TYPE_UNKNOWN), messageID(0),
          localMessageID(0), conversationType(ZIMConversationType::ZIM_CONVERSATION_TYPE_UNKNOWN),
          conversationSeq(0), direction(ZIMMessageDirection::ZIM_MESSAGE_DIRECTION_SEND),
          sentStatus(ZIMMessageSentStatus::ZIM_MESSAGE_SENT_STATUS_SENDING),
          receiptStatus(ZIMMessageReceiptStatus::ZIM_MESSAGE_RECEIPT_STATUS_NONE), timestamp(0),
          orderKey(0), userInserted(false), broadcastMessage(false), serverMessage(false) {}
    ZIMMessage(ZIMMessageType messageType)
        : isMentionAll(false), type(messageType), messageID(0), localMessageID(0),
          conversationType(ZIMConversationType::ZIM_CONVERSATION_TYPE_UNKNOWN), conversationSeq(0),
          direction(ZIMMessageDirection::ZIM_MESSAGE_DIRECTION_SEND),
          sentStatus(ZIMMessageSentStatus::ZIM_MESSAGE_SENT_STATUS_SENDING),
          receiptStatus(ZIMMessageReceiptStatus::ZIM_MESSAGE_RECEIPT_STATUS_NONE), timestamp(0),
          orderKey(0), userInserted(false), broadcastMessage(false), serverMessage(false) {}
    virtual ~ZIMMessage() {}

  public:
    ZIMMessageType getType() { return type; }

    long long getMessageID() { return messageID; }

    long long getLocalMessageID() { return localMessageID; }

    const std::string &getSenderUserID() { return senderUserID; }

    const std::string &getConversationID() { return conversationID; }

    ZIMConversationType getConversationType() { return conversationType; }

    long long getConversationSeq() { return conversationSeq; }

    ZIMMessageDirection getDirection() { return direction; }

    ZIMMessageSentStatus getSentStatus() { return sentStatus; }

    ZIMMessageReceiptStatus getReceiptStatus() { return receiptStatus; }

    unsigned long long getTimestamp() { return timestamp; }

    long long getOrderKey() { return orderKey; }

    bool isUserInserted() { return userInserted; }

    bool isBroadcastMessage() { return broadcastMessage; }

    bool isServerMessage() { return serverMessage; }

  public:
    std::string extendedData;
    std::string localExtendedData;
    std::vector<ZIMMessageReaction> reactions;
    std::vector<std::string> mentionedUserIDs;
    bool isMentionAll;

  protected:
    // Identifies the type of this message.
    ZIMMessageType type;

    // Description: The unique ID that identifies this message.
    //
    // Use cases: Can be used to index other messages.
    //
    // Caution: When the developer actively creates a message, there is no need to modify this parameter.
    // This parameter only has a value during callback.
    long long messageID;

    long long localMessageID;

    // Description: Identifies which User sent this message.
    //
    // Use cases: Used to show who sent a message.
    //
    // Caution: When the developer actively creates a message, there is no need to modify this parameter.
    // This parameter only has a value during callback.
    std::string senderUserID;

    std::string conversationID;

    ZIMConversationType conversationType;

    long long conversationSeq;

    ZIMMessageDirection direction;

    ZIMMessageSentStatus sentStatus;

    ZIMMessageReceiptStatus receiptStatus;

    // Description: Identifies the sending time of a message
    //
    // Use cases: Used to present the sending time of a message, and can be used for message sorting.
    //
    // Caution: This is a standard UNIX timestamp, in milliseconds.
    unsigned long long timestamp;

    long long orderKey;

    bool userInserted;

    bool broadcastMessage;

    bool serverMessage;

  public:
    friend class ZIMBridge;
    friend class ZIMConverter;
};

class ZIMVoIPConfig {
  public:
    ZIMCXHandleType iOSVoIPHandleType;
    std::string iOSVoIPHandleValue;
    bool iOSVoIPHasVideo;

  public:
    ZIMVoIPConfig() : iOSVoIPHandleType(ZIMCXHandleType::ZIM_CX_HANDLE_TYPE_GENERIC) {}
};

// Description:Offline push configuration.
class ZIMPushConfig {
  public:
    std::string title;
    std::string content;
    std::string payload;
    std::string resourcesID;
    bool enableBadge;
    int badgeIncrement;
    ZIMVoIPConfig *voIPConfig;

  public:
    ZIMPushConfig() : enableBadge(false), badgeIncrement(0), voIPConfig(nullptr) {}
};

// Configure message sending.
class ZIMMessageSendConfig {
  public:
    ZIMPushConfig *pushConfig;
    ZIMMessagePriority priority;
    bool hasReceipt;
    bool isNotifyMentionedUsers;

  public:
    ZIMMessageSendConfig()
        : pushConfig(nullptr), priority(ZIMMessagePriority::ZIM_MESSAGE_PRIORITY_LOW),
          hasReceipt(false), isNotifyMentionedUsers(true) {}
};

// Example Query message configuration.
class ZIMMessageQueryConfig {
  public:
    std::shared_ptr<ZIMMessage> nextMessage;
    unsigned int count;
    bool reverse;

  public:
    ZIMMessageQueryConfig() {
        this->nextMessage = nullptr;
        this->count = 0;
        this->reverse = false;
    }
};

// Delete configurations related to messages.
class ZIMMessageDeleteConfig {
  public:
    bool isAlsoDeleteServerMessage;

  public:
    ZIMMessageDeleteConfig() { this->isAlsoDeleteServerMessage = true; }
};

class ZIMMessageRevokeConfig {
  public:
    ZIMPushConfig *config;
    std::string revokeExtendedData;

  public:
    ZIMMessageRevokeConfig() : config(nullptr), revokeExtendedData("") {}
};

class ZIMMessageDeletedInfo {
  public:
    std::string conversationID;
    ZIMConversationType conversationType;
    ZIMMessageDeleteType messageDeleteType;
    // @deprecated Deprecated since 2.14.0, pleace use  [messageDeleteType] instead.
    bool isDeleteConversationAllMessage;
    std::vector<std::shared_ptr<ZIMMessage>> messageList;

  public:
    ZIMMessageDeletedInfo()
        : conversationType(ZIMConversationType::ZIM_CONVERSATION_TYPE_PEER),
          isDeleteConversationAllMessage(false) {}
};

// Session specific content.
class ZIMConversation {
  public:
    std::string conversationID;
    std::string conversationName;
    std::string conversationAlias;
    std::string conversationAvatarUrl;
    ZIMConversationType type;
    unsigned int unreadMessageCount;
    std::shared_ptr<ZIMMessage> lastMessage;
    long long orderKey;
    ZIMConversationNotificationStatus notificationStatus;
    bool isPinned;
    std::vector<ZIMMessageMentionedInfo> mentionedInfoList;
    std::string draft;

  public:
    ZIMConversation()
        : type(ZIMConversationType::ZIM_CONVERSATION_TYPE_PEER), unreadMessageCount(0),
          lastMessage(nullptr), orderKey(-1),
          notificationStatus(
              ZIMConversationNotificationStatus::ZIM_CONVERSATION_NOTIFICATION_STATUS_NOTIFY),
          isPinned(false) {}
};

class ZIMMessageSentStatusChangeInfo {
  public:
    ZIMMessageSentStatus status;
    std::shared_ptr<ZIMMessage> message;
    std::string reason;

  public:
    ZIMMessageSentStatusChangeInfo()
        : status(ZIMMessageSentStatus::ZIM_MESSAGE_SENT_STATUS_SENDING) {}
};

//  Session change information.
class ZIMConversationChangeInfo {
  public:
    ZIMConversationEvent event;
    std::shared_ptr<ZIMConversation> conversation;

  public:
    ZIMConversationChangeInfo() : event(ZIMConversationEvent::ZIM_CONVERSATION_EVENT_ADDED) {}
};

// Query the relevant configuration of the session.
class ZIMConversationQueryConfig {
  public:
    std::shared_ptr<ZIMConversation> nextConversation;
    unsigned int count;

  public:
    ZIMConversationQueryConfig() {
        this->nextConversation = nullptr;
        this->count = 0;
    }
};

// Delete session configuration.
class ZIMConversationDeleteConfig {
  public:
    bool isAlsoDeleteServerConversation;

  public:
    ZIMConversationDeleteConfig() { this->isAlsoDeleteServerConversation = true; }
};

class ZIMConversationsAllDeletedInfo {
  public:
    unsigned int count;

  public:
    ZIMConversationsAllDeletedInfo() { this->count = 0; }
};

// Normal text message object.
//
// Description: Identifies the basic parameters of a message.
//
// Caution: If the Type parameter of the base class is Text during callback,
// you can force the base class message object to be of this type.
//
class ZIMTextMessage : public ZIMMessage {

  public:
    // The content of the text message.
    std::string message;

  public:
    ZIMTextMessage() : ZIMMessage(ZIM_MESSAGE_TYPE_TEXT) {}
    ZIMTextMessage(const std::string &msg) : ZIMMessage(ZIM_MESSAGE_TYPE_TEXT), message(msg) {}
};

class ZIMMediaMessage : public ZIMMessage {
  public:
    std::string fileLocalPath;
    std::string fileDownloadUrl;
    std::string fileName;

  public:
    const std::string &getFileUID() { return fileUID; }
    long long getFileSize() { return fileSize; }

  protected:
    std::string fileUID;
    long long fileSize = 0;

  public:
    ZIMMediaMessage(ZIMMessageType mediaType) : ZIMMessage(mediaType) {}

    ZIMMediaMessage(ZIMMessageType mediaType, const std::string &localPath,
                    const std::string &fileUrl)
        : ZIMMessage(mediaType), fileLocalPath(localPath), fileDownloadUrl(fileUrl) {}

  public:
    friend class ZIMBridge;
    friend class ZIMConverter;
};

class ZIMImageMessage : public ZIMMediaMessage {

  public:
    std::string thumbnailDownloadUrl;
    std::string largeImageDownloadUrl;

  public:
    const std::string &getThumbnailLocalPath() { return thumbnailLocalPath; }
    const std::string &getLargeImageLocalPath() { return largeImageLocalPath; }
    unsigned int getOriginalImageWidth() { return originalImageWidth; }
    unsigned int getOriginalImageHeight() { return originalImageHeight; }
    unsigned int getLargeImageWidth() { return largeImageWidth; }
    unsigned int getLargeImageHeight() { return largeImageHeight; }
    unsigned int getThumbnailImageWidth() { return thumbnailWidth; }
    unsigned int getThumbnailImageHeight() { return thumbnailHeight; }

  protected:
    std::string thumbnailLocalPath;
    std::string largeImageLocalPath;
    unsigned int originalImageWidth = 0;
    unsigned int originalImageHeight = 0;
    unsigned int largeImageWidth = 0;
    unsigned int largeImageHeight = 0;
    unsigned int thumbnailWidth = 0;
    unsigned int thumbnailHeight = 0;

  public:
    ZIMImageMessage(const std::string &localPath)
        : ZIMMediaMessage(ZIM_MESSAGE_TYPE_IMAGE, localPath, "") {}

    ZIMImageMessage(const std::string &fileUrl, const std::string &largeUrl,
                    const std::string &thumbnailUrl)
        : ZIMMediaMessage(ZIM_MESSAGE_TYPE_IMAGE, "", fileUrl), thumbnailDownloadUrl(thumbnailUrl),
          largeImageDownloadUrl(largeUrl) {}

  public:
    friend class ZIMBridge;
    friend class ZIMConverter;
};

class ZIMFileMessage : public ZIMMediaMessage {

  public:
    ZIMFileMessage(const std::string &localPath)
        : ZIMMediaMessage(ZIM_MESSAGE_TYPE_FILE, localPath, "") {}

  public:
    friend class ZIMBridge;
    friend class ZIMConverter;
};

class ZIMAudioMessage : public ZIMMediaMessage {

  public:
    unsigned int audioDuration;

  public:
    ZIMAudioMessage(const std::string &localPath, unsigned int duration)
        : ZIMMediaMessage(ZIM_MESSAGE_TYPE_AUDIO, localPath, ""), audioDuration(duration) {}

  public:
    friend class ZIMBridge;
    friend class ZIMConverter;
};

class ZIMVideoMessage : public ZIMMediaMessage {

  public:
    unsigned int videoDuration = 0;
    std::string videoFirstFrameDownloadUrl;

  public:
    const std::string &getVideoFirstFrameLocalPath() { return videoFirstFrameLocalPath; }
    unsigned int getVideoFirstFrameWidth() { return videoFirstFrameWidth; }
    unsigned int getVideoFirstFrameHeight() { return videoFirstFrameHeight; }

  protected:
    std::string videoFirstFrameLocalPath;
    unsigned int videoFirstFrameWidth = 0;
    unsigned int videoFirstFrameHeight = 0;

  public:
    ZIMVideoMessage(const std::string &videoPath, unsigned int duration)
        : ZIMMediaMessage(ZIM_MESSAGE_TYPE_VIDEO, videoPath, ""), videoDuration(duration) {}

    ZIMVideoMessage(const std::string &videoUrl, const std::string &videoFirstFrameUrl,
                    unsigned int duration)
        : ZIMMediaMessage(ZIM_MESSAGE_TYPE_VIDEO, "", videoUrl), videoDuration(duration),
          videoFirstFrameDownloadUrl(videoFirstFrameUrl) {}

  public:
    friend class ZIMBridge;
    friend class ZIMConverter;
};

//[[deprecated("Deprecated since ZIM 2.8.0, please use [ZIMCustomMessage] instead.")]];
class ZIMSystemMessage : public ZIMMessage {
  public:
    // The content of the text message.
    std::string message;

  public:
    ZIMSystemMessage() : ZIMMessage(ZIM_MESSAGE_TYPE_SYSTEM) {}
    ZIMSystemMessage(const std::string &msg) : ZIMMessage(ZIM_MESSAGE_TYPE_SYSTEM), message(msg) {}

  public:
    friend class ZIMBridge;
    friend class ZIMConverter;
};

class ZIMCustomMessage : public ZIMMessage {
  public:
    // The content of the text message.
    std::string message;
    unsigned int subType = 0;
    std::string searchedContent;

  public:
    ZIMCustomMessage(const std::string &msg, const unsigned int subType)
        : ZIMMessage(ZIM_MESSAGE_TYPE_CUSTOM), message(msg), subType(subType) {}

  public:
    friend class ZIMBridge;
    friend class ZIMConverter;
};

class ZIMRevokeMessage : public ZIMMessage {
  protected:
    ZIMRevokeType revokeType = ZIMRevokeType::ZIM_REVOKE_TYPE_UNKNOWN;
    unsigned long long revokeTimestamp = 0;
    std::string operatedUserID;
    ZIMMessageType originalMessageType = ZIMMessageType::ZIM_MESSAGE_TYPE_UNKNOWN;
    std::string originalTextMessageContent;
    std::string revokeExtendedData;
    ZIMMessageRevokeStatus revokeStatus = ZIMMessageRevokeStatus::ZIM_MESSAGE_REVOKE_STATUS_UNKNOWN;

  public:
    ZIMRevokeType getRevokeType() { return revokeType; }
    unsigned long long getRevokeTimestamp() { return revokeTimestamp; }
    const std::string &getOperatedUserID() { return operatedUserID; }
    ZIMMessageType getOriginalMessageType() { return originalMessageType; }
    const std::string &getOriginalTextMessageContent() { return originalTextMessageContent; }
    const std::string &getRevokeExtendedData() { return revokeExtendedData; }
    ZIMMessageRevokeStatus getRevokeStatus() { return revokeStatus; }

  public:
    ZIMRevokeMessage() : ZIMMessage(ZIM_MESSAGE_TYPE_REVOKE) {}

  public:
    friend class ZIMBridge;
    friend class ZIMConverter;
};
// Custom binary message object.
//
// Description: Identifies a binary message.
//
// Caution: If the Type parameter of the base class is Custom during callback,
// you can force the base class message object to be of this type.
//
class ZIMCommandMessage : public ZIMMessage {

  public:
    // The content of the custom message.
    std::vector<uint8_t> message;

  public:
    ZIMCommandMessage() : ZIMMessage(ZIMMessageType::ZIM_MESSAGE_TYPE_COMMAND) {}
    ZIMCommandMessage(const std::vector<uint8_t> &msg)
        : ZIMMessage(ZIMMessageType::ZIM_MESSAGE_TYPE_COMMAND), message(msg) {}
};

class ZIMCombineMessage : public ZIMMessage {
  public:
    std::string title;
    std::string summary;
    std::vector<std::shared_ptr<ZIMMessage>> messageList;

  protected:
    std::string combineID;

  public:
    const std::string &getCombineID() { return combineID; }

  public:
    ZIMCombineMessage(const std::string &title, const std::string &summary,
                      std::vector<std::shared_ptr<ZIMMessage>> messageList)
        : ZIMMessage(ZIMMessageType::ZIM_MESSAGE_TYPE_COMBINE), title(title), summary(summary),
          messageList(messageList) {}
    ZIMCombineMessage() : ZIMMessage(ZIMMessageType::ZIM_MESSAGE_TYPE_COMBINE) {}
    ~ZIMCombineMessage() {}

  public:
    friend class ZIMBridge;
    friend class ZIMConverter;
};

// Barrage text message object.
//
// Description: This message type only supports sending in the room, and is used in scenarios where there is a large concurrency that does not require delivery.
//
//
class ZIMBarrageMessage : public ZIMMessage {

  public:
    // The content of the text message.
    std::string message;

  public:
    ZIMBarrageMessage() : ZIMMessage(ZIMMessageType::ZIM_MESSAGE_TYPE_BARRAGE) {}
    ZIMBarrageMessage(const std::string &msg)
        : ZIMMessage(ZIMMessageType::ZIM_MESSAGE_TYPE_BARRAGE), message(msg) {}
};

using ZIMMessageAttachedCallback = std::function<void(const std::shared_ptr<ZIMMessage> &message)>;

using ZIMMediaUploadingProgress =
    std::function<void(const std::shared_ptr<ZIMMediaMessage> &message,
                       unsigned long long currentFileSize, unsigned long long totalFileSize)>;

class ZIMMessageSendNotification {
  public:
    ZIMMessageAttachedCallback onMessageAttached = nullptr;

  public:
    ZIMMessageSendNotification() {}
    ZIMMessageSendNotification(ZIMMessageAttachedCallback cbAttach) : onMessageAttached(cbAttach) {}
};

class ZIMMediaMessageSendNotification {
  public:
    ZIMMessageAttachedCallback onMessageAttached = nullptr;
    ZIMMediaUploadingProgress onMediaUploadingProgress = nullptr;

  public:
    ZIMMediaMessageSendNotification() {}
    ZIMMediaMessageSendNotification(ZIMMessageAttachedCallback cbAttach,
                                    ZIMMediaUploadingProgress cbProgress)
        : onMessageAttached(cbAttach), onMediaUploadingProgress(cbProgress) {}
};

class ZIMMessageReceiptInfo {
  public:
    ZIMMessageReceiptStatus status;
    long long messageID;
    std::string conversationID;
    ZIMConversationType conversationType;
    unsigned int readMemberCount;
    unsigned int unreadMemberCount;
    bool isSelfOperated;

  public:
    ZIMMessageReceiptInfo()
        : status(ZIM_MESSAGE_RECEIPT_STATUS_NONE), messageID(0),
          conversationType(ZIM_CONVERSATION_TYPE_PEER), readMemberCount(0), unreadMemberCount(0),
          isSelfOperated(false) {}
};

class ZIMMessageSearchConfig {
  public:
    std::shared_ptr<ZIMMessage> nextMessage;
    unsigned int count;
    ZIMMessageOrder order;
    std::vector<std::string> keywords;
    std::vector<ZIMMessageType> messageTypes;
    std::vector<unsigned int> subMessageTypes;
    std::vector<std::string> senderUserIDs;
    long long startTime;
    long long endTime;

  public:
    ZIMMessageSearchConfig() : order(ZIM_MESSAGE_ORDER_DESCENDING), startTime(0), endTime(0) {}
};

class ZIMConversationSearchConfig {
  public:
    unsigned int nextFlag;
    unsigned int totalConversationCount;
    unsigned int conversationMessageCount;
    std::vector<std::string> keywords;
    std::vector<ZIMMessageType> messageTypes;
    std::vector<unsigned int> subMessageTypes;
    std::vector<std::string> senderUserIDs;
    long long startTime;
    long long endTime;

  public:
    ZIMConversationSearchConfig() : startTime(0), endTime(0) {}
};

class ZIMConversationSearchInfo {
  public:
    std::string conversationID;
    ZIMConversationType conversationType;
    unsigned int totalMessageCount;
    std::vector<std::shared_ptr<ZIMMessage>> messageList;

  public:
    ZIMConversationSearchInfo() : conversationType(ZIM_CONVERSATION_TYPE_PEER) {}
};

// Room information object.
//
// Description: Identifies a unique room.
//
class ZIMRoomInfo {

  public:
    // Room ID, a string with a maximum length of 32 bytes or less. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'.
    std::string roomID;

    // Room name, a string with a maximum length of 64 bytes or less
    std::string roomName;

  public:
    ZIMRoomInfo() {}
    ZIMRoomInfo(const std::string &id, const std::string &name) {
        this->roomID = id;
        this->roomName = name;
    }
};

// The room details object.
//
// Description: Identifies the basic parameters of a message.
//
// Caution: Identifies the detailed information of a room.
//
class ZIMRoomFullInfo {

  public:
    // The base object of the room.
    ZIMRoomInfo baseInfo;

  public:
    ZIMRoomFullInfo() {}
};

// Configuration for querying member.
//
// Description: When querying member, you need to configure this object.
//
class ZIMRoomMemberQueryConfig {

  public:
    // Description: The flag of the paging query. For the first query, set this field to an empty string. If the "nextFlag" field of the callback is not an empty string, it needs to be set here to continue the query on the next page.
    std::string nextFlag;

    // Description: How many messages are retrieved in one query.
    //
    // Caution: To obtain messages in pages to reduce overhead, it is recommended to obtain within 100 messages at a time.
    unsigned int count;

  public:
    ZIMRoomMemberQueryConfig() { count = 0; }
    ZIMRoomMemberQueryConfig(const std::string &flag, unsigned int cnt) {
        this->nextFlag = flag;
        this->count = cnt;
    }
};

// Configuration of room advanced
//
// Description: When create a room, user could add roomAttributes and destroy delay time by using this config
//
class ZIMRoomAdvancedConfig {
  public:
    // Description: Room attributes of a room.
    std::unordered_map<std::string, std::string> roomAttributes;
    // Description: Room will exist time after all of the users in the room left
    unsigned int roomDestroyDelayTime;

  public:
    ZIMRoomAdvancedConfig() : roomDestroyDelayTime(0) {}
};

// The behavior attribute set by the room attribute.
//
// Description: The behavior attribute set by the room attribute.
//
class ZIMRoomAttributesSetConfig {
  public:
    // Description: Room attributes are automatically deleted after the owner leaves the room.
    bool isDeleteAfterOwnerLeft;
    // Description: Whether the operation is mandatory, that is, the property of the room whose owner is another user can be modified.
    bool isForce;
    // Description: Whether to update the owner of the room attribute involved.
    bool isUpdateOwner;

  public:
    ZIMRoomAttributesSetConfig() {
        this->isDeleteAfterOwnerLeft = false;
        this->isForce = false;
        this->isUpdateOwner = false;
    }
    ZIMRoomAttributesSetConfig(bool left, bool force, bool update) {
        this->isDeleteAfterOwnerLeft = left;
        this->isForce = force;
        this->isUpdateOwner = update;
    }
};

// The behavior attribute set by the room attribute.
//
// Description: The behavior attribute set by the room attribute.
//
class ZIMRoomAttributesDeleteConfig {
  public:
    // Description: Whether the operation is mandatory, that is, the property of the room whose owner is another user can be deleted.
    bool isForce;

  public:
    ZIMRoomAttributesDeleteConfig() { this->isForce = false; }
    ZIMRoomAttributesDeleteConfig(bool force) { this->isForce = force; }
};

// The behavior attribute set by the room attribute.
//
// Description: The behavior attribute set by the room attribute.
//
class ZIMRoomAttributesBatchOperationConfig {
  public:
    // Description: Room attributes are automatically deleted after the owner leaves the room.
    bool isDeleteAfterOwnerLeft;
    // Description: Whether the operation is mandatory, that is, the property of the room whose owner is another user can be modified.
    bool isForce;
    // Description: Whether to update the owner of the room attribute involved.
    bool isUpdateOwner;

  public:
    ZIMRoomAttributesBatchOperationConfig() {
        this->isDeleteAfterOwnerLeft = false;
        this->isForce = false;
        this->isUpdateOwner = false;
    }
    ZIMRoomAttributesBatchOperationConfig(bool left, bool force, bool update) {
        this->isDeleteAfterOwnerLeft = left;
        this->isForce = force;
        this->isUpdateOwner = update;
    }
};

// Notice of Room Attribute Change.
//
// Description: Notice of Room Attribute Change.
//
class ZIMRoomAttributesUpdateInfo {
  public:
    // Description: Behavioral information of room attribute change notification.
    ZIMRoomAttributesUpdateAction action;

    // Description: Room attributes.
    std::unordered_map<std::string, std::string> roomAttributes;

  public:
    ZIMRoomAttributesUpdateInfo()
        : action(ZIMRoomAttributesUpdateAction::ZIM_ROOM_ATTRIBUTES_UPDATE_ACTION_SET) {}
};

// Room user attribute information.
class ZIMRoomMemberAttributesInfo {
  public:
    ZIMRoomMemberAttributesInfo() {}

  public:
    std::string userID;

    //Detailed description: room user attributes.
    std::unordered_map<std::string, std::string> attributes;
};

// Room user attribute operation information.
class ZIMRoomMemberAttributesOperatedInfo {
  public:
    ZIMRoomMemberAttributesOperatedInfo() {}

  public:
    //Detailed description: room user attribute information.
    ZIMRoomMemberAttributesInfo attributesInfo;

    //Detail description: The key of the room user attribute operation failure.
    std::vector<std::string> errorKeys;
};

// Detail description: room user attribute setting configuration, including configurable whether to retain user attributes after the user leaves, etc.

// Business scenarios: When setting room user attributes, developers can choose to use them according to their actual application scenarios.
class ZIMRoomMemberAttributesSetConfig {

  public:
    // Detail description: Configure whether the room user attributes are saved after the user leaves the room.

    // Required: not required.

    // Default: true.
    bool isDeleteAfterOwnerLeft;

  public:
    ZIMRoomMemberAttributesSetConfig() { this->isDeleteAfterOwnerLeft = true; }
};

// Room user attribute update information.
class ZIMRoomMemberAttributesUpdateInfo {
  public:
    ZIMRoomMemberAttributesUpdateInfo() {}
    // Detailed description: room user attributes.
  public:
    ZIMRoomMemberAttributesInfo attributesInfo;
};
// Detailed description: Room operation information.
class ZIMRoomOperatedInfo {

  public:
    //Detail description: UserID of the operator.
    std::string userID;
};

// Room user attribute query configuration.
class ZIMRoomMemberAttributesQueryConfig {
  public:
    // Detail description: Query anchor for room user properties.

    // Required or not: not required, it is empty by default for the first time, which means to start the query from the beginning.
    std::string nextFlag;

    // Detail description: The number of paginated queries.

    // Required: Required.
    unsigned int count;

  public:
    ZIMRoomMemberAttributesQueryConfig() { count = 0; }
    ZIMRoomMemberAttributesQueryConfig(const std::string &flag, unsigned int cnt) {
        this->nextFlag = flag;
        this->count = cnt;
    }
};
// Group attribute update information.
//
// Description: Group attribute update information.
//
class ZIMGroupAttributesUpdateInfo {
  public:
    // Description: Group attribute update action.
    ZIMGroupAttributesUpdateAction action;

    // Description: group attributes.
    std::unordered_map<std::string, std::string> groupAttributes;

  public:
    ZIMGroupAttributesUpdateInfo()
        : action(ZIMGroupAttributesUpdateAction::ZIM_GROUP_ATTRIBUTES_UPDATE_ACTION_SET) {}
};

class ZIMGroupMessageReceiptMemberQueryConfig {
  public:
    unsigned int nextFlag;
    unsigned int count;

  public:
    ZIMGroupMessageReceiptMemberQueryConfig() : nextFlag(0), count(0) {}
};

class ZIMGroupSearchConfig {
  public:
    unsigned int nextFlag;
    unsigned int count;
    std::vector<std::string> keywords;
    bool isAlsoMatchGroupMemberUserName;
    bool isAlsoMatchGroupMemberNickname;

  public:
    ZIMGroupSearchConfig()
        : isAlsoMatchGroupMemberUserName(false), isAlsoMatchGroupMemberNickname(false) {}
};

class ZIMGroupMemberSearchConfig {
  public:
    unsigned int nextFlag;
    unsigned int count;
    std::vector<std::string> keywords;
    bool isAlsoMatchGroupMemberNickname;

  public:
    ZIMGroupMemberSearchConfig() : isAlsoMatchGroupMemberNickname(false) {}
};

class ZIMGroupSearchInfo {
  public:
    ZIMGroupInfo groupInfo;
    std::vector<ZIMGroupMemberInfo> userList;

  public:
    ZIMGroupSearchInfo() {}
};

// MARK: CallInvite

// Call invitation user information.
class ZIMCallUserInfo {
  public:
    // Description: userID.
    std::string userID;
    // Description: user status.
    ZIMCallUserState state;
    std::string extendedData;

  public:
    ZIMCallUserInfo() : state(ZIMCallUserState::ZIM_CALL_USER_STATE_INVITING) {}
};

// The behavior property of the Send Call Invitation setting.
class ZIMCallInviteConfig {
  public:
    // Description: The timeout setting of the call invitation, the unit is seconds.
    int timeout;
    // Description: Extended field, through which the inviter can carry information to the invitee.
    std::string extendedData;

    ZIMCallInvitationMode mode;

    ZIMPushConfig *pushConfig;

  public:
    ZIMCallInviteConfig() : timeout(90), pushConfig(nullptr) {}
};

class ZIMCallingInviteConfig {
  public:
    ZIMPushConfig *pushConfig;

  public:
    ZIMCallingInviteConfig() : pushConfig(nullptr) {}
};

class ZIMCallJoinConfig {
  public:
    ZIMCallJoinConfig() {}

  public:
    std::string extendedData;
};

class ZIMCallQuitConfig {
  public:
    // Description: Extended field, through which the inviter can carry information to the invitee.
    std::string extendedData;

    ZIMPushConfig *pushConfig;

  public:
    ZIMCallQuitConfig() : pushConfig(nullptr) {}
};

class ZIMCallEndConfig {
  public:
    // Description: Extended field, through which the inviter can carry information to the invitee.
    std::string extendedData;

    ZIMPushConfig *pushConfig;

  public:
    ZIMCallEndConfig() : pushConfig(nullptr) {}
};

class ZIMCallInvitationQueryConfig {
  public:
    unsigned int count;

    long long nextFlag;

  public:
    ZIMCallInvitationQueryConfig() : count(0), nextFlag(0) {}
};

class ZIMBlacklistQueryConfig {
  public:
    unsigned int count;

    unsigned int nextFlag;

  public:
    ZIMBlacklistQueryConfig() : count(0), nextFlag(0) {}
};

class ZIMCallInfo {
  public:
    std::string callID;

    std::string caller;

    ZIMCallInvitationMode mode;

    ZIMCallState state;

    // Description: Extended field, through which the inviter can carry information to the invitee.
    std::string extendedData;

    long long createTime;

    long long endTime;

    std::vector<ZIMCallUserInfo> callUserList;
};

// Behavior property that cancels the call invitation setting.
class ZIMCallCancelConfig {
  public:
    // Description: Extended field, through which the inviter can carry information to the invitee.
    std::string extendedData;

    ZIMPushConfig *pushConfig;

  public:
    ZIMCallCancelConfig() : pushConfig(nullptr) {}
};

// Behavior property set by accepting a call invitation.
class ZIMCallAcceptConfig {
  public:
    // Description: Extended field, through which the inviter can carry information to the invitee.
    std::string extendedData;

  public:
    ZIMCallAcceptConfig() {}
};

// The behavior property of the reject call invitation setting.
class ZIMCallRejectConfig {
  public:
    // Description: Extended field, through which the inviter can carry information to the invitee.
    std::string extendedData;

  public:
    ZIMCallRejectConfig() {}
};

// Call invitation sent message.
class ZIMCallInvitationSentInfo {
  public:
    // Description: The timeout setting of the call invitation, the unit is seconds.
    int timeout;
    // Description: User id that has not received a call invitation.
    std::vector<ZIMErrorUserInfo> errorUserList;

    // @deprecated Deprecated since 2.9.0, pleace use  [errorList] instead.
    std::vector<ZIMCallUserInfo> errorInvitees;

  public:
    ZIMCallInvitationSentInfo() : timeout(0) {}
};

class ZIMCallingInvitationSentInfo {
  public:
    // Description: User id that has not received a call invitation.
    std::vector<ZIMErrorUserInfo> errorUserList;

  public:
    ZIMCallingInvitationSentInfo() {}
};

class ZIMCallEndedSentInfo {
  public:
    long long createTime;

    long long acceptTime;

    long long endTime;

  public:
    ZIMCallEndedSentInfo() : createTime(0), acceptTime(0), endTime(0) {}
};

// Information to accept the call invitation.
class ZIMCallInvitationReceivedInfo {
  public:
    // Description: Inviter ID.
    std::string inviter;

    std::string caller;
    // Description: Extended field, through which the inviter can carry information to the invitee.
    std::string extendedData;
    // Description: The timeout setting of the call invitation, the unit is seconds.
    int timeout;
    ZIMCallInvitationMode mode;
    long long createTime;
    std::vector<ZIMCallUserInfo> callUserList;

  public:
    ZIMCallInvitationReceivedInfo()
        : timeout(0), mode(ZIM_INVITATION_MODE_UNKNOWN), createTime(0) {}
};

// Cancel the call invitation message.
class ZIMCallInvitationCancelledInfo {
  public:
    ZIMCallInvitationMode mode;
    // Description: The inviter ID of the call invitation.
    std::string inviter;
    // Description: Extended field, through which the inviter can carry information to the invitee.
    std::string extendedData;

  public:
    ZIMCallInvitationCancelledInfo() {}
};

class ZIMCallJoinSentInfo {
  public:
    ZIMCallJoinSentInfo() : createTime(0), joinTime(0) {}

  public:
    std::string extendedData;
    long long createTime;
    long long joinTime;
    std::vector<ZIMCallUserInfo> callUserList;
};

class ZIMCallQuitSentInfo {
  public:
    long long createTime;
    long long acceptTime;
    long long quitTime;

  public:
    ZIMCallQuitSentInfo() : createTime(0), acceptTime(0), quitTime(0) {}
};

class ZIMCallInvitationCreatedInfo {
  public:
    ZIMCallInvitationMode mode;
    std::string caller;
    std::string extendedData;
    unsigned int timeout;
    long long createTime;
    std::vector<ZIMCallUserInfo> callUserList;

  public:
    ZIMCallInvitationCreatedInfo() : mode(ZIM_INVITATION_MODE_UNKNOWN), timeout(0), createTime(0) {}
};

class ZIMCallInvitationEndedInfo {
  public:
    std::string caller;
    std::string operatedUserID;
    std::string extendedData;
    ZIMCallInvitationMode mode;
    long long endTime;
    ZIMCallInvitationEndedInfo() : mode(ZIM_INVITATION_MODE_UNKNOWN), endTime(0) {}
};

// Accept the call invitation message.
class ZIMCallInvitationAcceptedInfo {
  public:
    // Description: Invitee ID.
    std::string invitee;
    // Description: Extended field, through which the inviter can carry information to the invitee.
    std::string extendedData;

  public:
    ZIMCallInvitationAcceptedInfo() {}
};

// Reject the call invitation message.
class ZIMCallInvitationRejectedInfo {
  public:
    // Description: Invitee ID.
    std::string invitee;
    // Description: Extended field, through which the inviter can carry information to the invitee.
    std::string extendedData;

  public:
    ZIMCallInvitationRejectedInfo() {}
};

class ZIMCallInvitationTimeoutInfo {
  public:
    ZIMCallInvitationMode mode;

  public:
    ZIMCallInvitationTimeoutInfo() : mode(ZIM_INVITATION_MODE_UNKNOWN) {}
};

class ZIMCallUserStateChangeInfo {
  public:
    std::vector<ZIMCallUserInfo> callUserList;
};

class ZIMFriendApplicationListQueryConfig {
  public:
    unsigned int count;

    unsigned int nextFlag;

  public:
    ZIMFriendApplicationListQueryConfig() : count(0), nextFlag(0) {}
};

class ZIMFriendSearchConfig {
  public:
    unsigned int count;
    unsigned int nextFlag;
    std::vector<std::string> keywords;
    bool isAlsoMatchFriendAlias;

  public:
    ZIMFriendSearchConfig() : count(0), nextFlag(0), isAlsoMatchFriendAlias(false) {}
};

class ZIMFriendRelationCheckConfig {
  public:
    ZIMFriendRelationCheckType type;

  public:
    ZIMFriendRelationCheckConfig() : type(ZIM_FRIEND_RELATION_CHECK_TYPE_BOTH) {}
};

class ZIMFriendDeleteConfig {
  public:
    ZIMFriendDeleteType type;

  public:
    ZIMFriendDeleteConfig() : type(ZIM_FRIEND_DELETE_TYPE_BOTH) {}
};

class ZIMFriendListQueryConfig {
  public:
    unsigned int count;

    unsigned int nextFlag;

  public:
    ZIMFriendListQueryConfig() : count(0), nextFlag(0) {}
};

class ZIMFriendApplicationInfo {
  public:
    ZIMUserInfo applyUser;
    std::string wording;
    long long createTime;
    long long updateTime;
    ZIMFriendApplicationType type;
    ZIMFriendApplicationState state;

  public:
    ZIMFriendApplicationInfo()
        : createTime(0), updateTime(0), type(ZIM_FRIEND_APPLICATION_TYPE_UNKNOWN),
          state(ZIM_FRIEND_APPLICATION_STATE_UNKNOWN) {}
};

class ZIMFriendApplicationRejectConfig {
  public:
    ZIMPushConfig *pushConfig;

  public:
    ZIMFriendApplicationRejectConfig() : pushConfig(nullptr) {}
};

class ZIMFriendRelationInfo {
  public:
    ZIMUserRelationType type;
    std::string userID;

  public:
    ZIMFriendRelationInfo() : type(ZIM_USER_RELATION_TYPE_SINGLE_NO) {}
};

class ZIMFriendApplicationSendConfig {
  public:
    std::string wording;
    std::string friendAlias;
    std::unordered_map<std::string, std::string> friendAttributes;
    ZIMPushConfig *pushConfig;

  public:
    ZIMFriendApplicationSendConfig() : pushConfig(nullptr) {}
};

class ZIMFriendApplicationAcceptConfig {
  public:
    std::string friendAlias;
    std::unordered_map<std::string, std::string> friendAttributes;
    ZIMPushConfig *pushConfig;

  public:
    ZIMFriendApplicationAcceptConfig() : pushConfig(nullptr) {}
};

class ZIMFriendInfo : public ZIMUserInfo {
  public:
    std::string friendAlias;
    long long createTime; // 添加时间
    std::string wording;
    std::unordered_map<std::string, std::string> friendAttributes;

  public:
    ZIMFriendInfo() : createTime(0) {}
};

class ZIMFriendAddConfig {
  public:
    std::string friendAlias;
    std::string wording;
    std::unordered_map<std::string, std::string> friendAttributes;

  public:
    ZIMFriendAddConfig() {}
};

// MARK: Main
//
// Callback of the log upload result .
//
// @param errorCode Error code.
using ZIMLogUploadedCallback = std::function<void(const ZIMError &errorInfo)>;

// Callback of the result login.
//
// @param errorCode Error code.
using ZIMLoggedInCallback = std::function<void(const ZIMError &errorInfo)>;

// Callback of the result of renewing the token.
//
// @param token The renewed token.
// @param errorCode Error code.
using ZIMTokenRenewedCallback =
    std::function<void(const std::string &token, const ZIMError &errorInfo)>;

using ZIMUserNameUpdatedCallback =
    std::function<void(const std::string &userName, const ZIMError &errorInfo)>;

using ZIMUserAvatarUrlUpdatedCallback =
    std::function<void(const std::string &userAvatarUrl, const ZIMError &errorInfo)>;

using ZIMUserExtendedDataUpdatedCallback =
    std::function<void(const std::string &extendedData, const ZIMError &errorInfo)>;

using ZIMUsersInfoQueriedCallback = std::function<void(
    const std::vector<ZIMUserFullInfo> &userFullInfos,
    const std::vector<ZIMErrorUserInfo> &errorUserList, const ZIMError &errorInfo)>;

// MARK: Message

// Callback of the result of sending the message.
//
// @param message The sent message object, from which parameters such as messageID can be obtained. If the sending fails, the messageID parameter in the message will be an empty string.
// @param errorCode Error code.

using ZIMMessageReactionAddedCallback =
    std::function<void(const ZIMMessageReaction &reaction, const ZIMError &errorInfo)>;
using ZIMMessageReactionDeletedCallback =
    std::function<void(const ZIMMessageReaction &reaction, const ZIMError &errorInfo)>;
using ZIMMessageReactionUserListQueriedCallback = std::function<void(
    const std::shared_ptr<ZIMMessage> &message,
    const std::vector<ZIMMessageReactionUserInfo> &userList, const std::string &reactionType,
    const long long nextFlag, const unsigned int totalCount, const ZIMError &errorInfo)>;
using ZIMMessageSentCallback =
    std::function<void(const std::shared_ptr<ZIMMessage> &message, const ZIMError &errorInfo)>;

// Callback of the result of querying messages.
//
// @param conversationID ConversationID.
// @param conversationType Conversation Type.
// @param messageList The message list of the query result.
// @param errorCode Error code.
using ZIMMessageQueriedCallback = std::function<void(
    const std::string &conversationID, ZIMConversationType conversationType,
    const std::vector<std::shared_ptr<ZIMMessage>> &messageList, const ZIMError &errorInfo)>;

using ZIMMediaDownloadedCallback =
    std::function<void(const std::shared_ptr<ZIMMediaMessage> &message, const ZIMError &errorInfo)>;

using ZIMMediaDownloadingProgress =
    std::function<void(const std::shared_ptr<ZIMMediaMessage> &message,
                       unsigned long long currentFileSize, unsigned long long totalFileSize)>;

// A callback to delete a message.
//
// @param conversationID ConversationID.
// @param conversationType Conversation Type.
// @param errorInfo Delete the error code of the message. 0 means the deletion of the message succeeded, non-0 means the deletion of the message failed. If the deletion message fails, the developer should check the official website error code document to query the solution.

using ZIMMessageLocalExtendedDataUpdatedCallback =
    std::function<void(const std::shared_ptr<ZIMMessage> &message, const ZIMError &errorInfo)>;

using ZIMMessageDeletedCallback =
    std::function<void(const std::string &conversationID, ZIMConversationType conversationType,
                       const ZIMError &errorInfo)>;
using ZIMConversationMessagesAllDeletedCallback = std::function<void(const ZIMError &errorInfo)>;
using ZIMMessageRevokedCallback =
    std::function<void(const std::shared_ptr<ZIMMessage> &message, const ZIMError &errorInfo)>;

using ZIMMessageReceiptsReadSentCallback =
    std::function<void(const std::string &conversationID, ZIMConversationType conversationType,
                       const std::vector<long long> &errorMessageIDs, const ZIMError &errorInfo)>;

using ZIMMessageReceiptsInfoQueriedCallback =
    std::function<void(const std::vector<ZIMMessageReceiptInfo> &infos, std::vector<long long>,
                       const ZIMError &errorInfo)>;

using ZIMMessagesSearchedCallback =
    std::function<void(const std::string &conversationID, ZIMConversationType conversationType,
                       const std::vector<std::shared_ptr<ZIMMessage>> &messageList,
                       const std::shared_ptr<ZIMMessage> &nextMessage, const ZIMError &errorInfo)>;

using ZIMMessagesGlobalSearchedCallback =
    std::function<void(const std::vector<std::shared_ptr<ZIMMessage>> &messageList,
                       const std::shared_ptr<ZIMMessage> &nextMessage, const ZIMError &errorInfo)>;

using ZIMConversationsSearchedCallback =
    std::function<void(const std::vector<ZIMConversationSearchInfo> &conversationSearchInfoList,
                       unsigned int nextFlag, const ZIMError &errorInfo)>;

// MARK: Conversation

// Available since: 2.0.0 and above.

// Description: After the session list is queried, the callback is used to return the query result.

// Use cases: The logic after the session list query can be done in this callback.

// When to call /Trigger: Description Triggered when the session list is queried.

// Related APIs: [queryConversationList].
// @param conversationID ConversationID.
// @param conversationType Conversation Type.
// @param errorInfo Error code for querying the conversation list. 0 means querying the session list succeeds, non-0 means querying the session list fails. If there is a failure to query the call list, the developer should check the official website error code document to query the solution.sage failed. If the deletion message fails, the developer should check the official website error code document to query the solution.

using ZIMConversationQueriedCallback =
    std::function<void(std::shared_ptr<ZIMConversation> &conversation, const ZIMError &errorInfo)>;

using ZIMConversationListQueriedCallback =
    std::function<void(const std::vector<std::shared_ptr<ZIMConversation>> &conversationList,
                       const ZIMError &errorInfo)>;

using ZIMConversationPinnedListQueriedCallback =
    std::function<void(const std::vector<std::shared_ptr<ZIMConversation>> &conversationList,
                       const ZIMError &errorInfo)>;

// Callback for conversation deletion.
//
// Available since: 2.0.0 and above.
// Description: After a session is deleted, the deletion result is returned using this callback.
// Use cases: You can do the deleted session logic in this callback.
// When to call /Trigger: Description Triggered after the session was deleted.
// Related APIs: [[deleteConversation].
// @param conversationID ConversationID.
// @param conversationType Conversation Type.
// @param errorInfo Delete the error code of the call. 0 means that the session was deleted successfully, non-0 means that the session was deleted failed. If the deletion of the call fails, the developer should check the official website error code document to query the solution.

using ZIMConversationPinnedStateUpdatedCallback =
    std::function<void(const std::string &conversationID, ZIMConversationType conversationType,
                       const ZIMError &errorInfo)>;

using ZIMConversationDeletedCallback =
    std::function<void(const std::string &conversationID, ZIMConversationType conversationType,
                       const ZIMError &errorInfo)>;

using ZIMConversationsAllDeletedCallback = std::function<void(const ZIMError &errorInfo)>;

// Callback for conversation unread clearing.
//
// Available since: 2.0.0 and above.
// Description: This callback returns the result of clearing a session if the session is not read.
// Use cases: You can do clear unread logic in this callback.
// When to call /Trigger: Triggered after clearing session unread.
// Related APIs: [clearConversationUnreadMessageCount].
// @param conversationID ConversationID.
// @param conversationType Conversation Type.
// @param errorInfo Clears the error code for the number of unread calls. 0 means clearing the unread number of the session successfully, non-0 means the clearing the unread number of the session failed. If there is a failure to clear the number of unread calls, the developer should check the official website error code document to query the solution.

using ZIMConversationUnreadMessageCountClearedCallback =
    std::function<void(const std::string &conversationID, ZIMConversationType conversationType,
                       const ZIMError &errorInfo)>;

using ZIMConversationTotalUnreadMessageCountClearedCallback =
    std::function<void(const ZIMError &errorInfo)>;

// Callback for conversation notification state settings.
//
// Available since: 2.0.0 and above.
// Description: This callback returns the result of group notification after the group notification status is set.
// Use cases: You can do the logic after setting the group notification status in this callback.
// When to call /Trigger: Triggered when the group notification status is set.
// Related APIs: [setConversationNotificationStatus].
// @param conversationID ConversationID.
// @param conversationType Conversation Type.
// @param errorInfo Set the error code for the callback notification state. 0 means setting the call back notification state successfully, non-0 means setting the call back notification state fails. If there is a failure to set the callback notification status, the developer should check the official website error code document to query the solution.

using ZIMConversationNotificationStatusSetCallback =
    std::function<void(const std::string &conversationID, ZIMConversationType conversationType,
                       const ZIMError &errorInfo)>;

using ZIMConversationMessageReceiptReadSentCallback =
    std::function<void(const std::string &conversationID, ZIMConversationType conversationType,
                       const ZIMError &errorInfo)>;

using ZIMCombineMessageDetailQueriedCallback =
    std::function<void(const std::shared_ptr<ZIMCombineMessage> &message, ZIMError &errorInfo)>;
using ZIMConversationDraftSetCallback =
    std::function<void(const std::string &conversationID, ZIMConversationType conversationType,
                       const ZIMError &errorInfo)>;

// MARK: Room

// Callback of the result of creating the room.
//
// @param roomInfo Details of the room created. If the creation fails, the roomID parameter in roomInfo will be an empty string.
// @param errorCode Error code.
using ZIMRoomCreatedCallback =
    std::function<void(const ZIMRoomFullInfo &roomInfo, const ZIMError &errorInfo)>;

// Callback of the result of joining the room.
//
// @param roomInfo Details of the room joined. If the join fails, the roomID parameter in roomInfo will be an empty string.
// @param errorCode Error code.
using ZIMRoomJoinedCallback =
    std::function<void(const ZIMRoomFullInfo &roomInfo, const ZIMError &errorInfo)>;

// Callback of the result of entering the room.
//
// @param roomInfo Details of the room entered. If the join fails, the roomID parameter in roomInfo will be an empty string.
// @param errorCode Error code.
using ZIMRoomEnteredCallback =
    std::function<void(const ZIMRoomFullInfo &roomInfo, const ZIMError &errorInfo)>;

// Callback of the result of leaving the room.
//
// @param errorCode Error code.
using ZIMRoomLeftCallback =
    std::function<void(const std::string &roomID, const ZIMError &errorInfo)>;

// Callback of the result of querying the room members list.
//
// @param memberList List of members in the room.
// @param nextFlag The flag of the paging query. If this field is an empty string, the query has been completed. Otherwise, you need to set this value to the "nextFlag" field of ZIMQueryMemberConfig for the next page query.
// @param errorCode Error code.
using ZIMRoomMemberQueriedCallback =
    std::function<void(const std::string &roomID, const std::vector<ZIMUserInfo> &memberList,
                       const std::string &nextFlag, const ZIMError &errorInfo)>;

using ZIMRoomMembersQueriedCallback = std::function<void(
    const std::string &roomID, const std::vector<ZIMRoomMemberInfo> &memberList,
    const std::vector<ZIMErrorUserInfo> &errorUserList, const ZIMError &errorInfo)>;

// Callback of the result of querying the online members count in the room.
//
// @param count The number of online members of the room.
// @param errorCode Error code.
using ZIMRoomOnlineMemberCountQueriedCallback =
    std::function<void(const std::string &roomID, unsigned int count, const ZIMError &errorInfo)>;

// Callback for conversation notification state settings.
//
// The return of the operation result of the room attribute.
// @param errorInfo Error information.
using ZIMRoomAttributesOperatedCallback =
    std::function<void(const std::string &roomID, const std::vector<std::string> &errorKeyList,
                       const ZIMError &errorInfo)>;

// Complete the property operation of the combined room.
// @param errorInfo Error information.
using ZIMRoomAttributesBatchOperatedCallback =
    std::function<void(const std::string &roomID, const ZIMError &errorInfo)>;

// Return of room attribute query results.
// @param roomAttributes Room attributes.
// @param errorInfo Error information.
using ZIMRoomAttributesQueriedCallback = std::function<void(
    const std::string &roomID, const std::unordered_map<std::string, std::string> &roomAttributes,
    const ZIMError &errorInfo)>;

using ZIMRoomMembersAttributesOperatedCallback = std::function<void(
    const std::string &roomID, const std::vector<ZIMRoomMemberAttributesOperatedInfo> &infos,
    const std::vector<std::string> &errorUserList, const ZIMError &errorInfo)>;

using ZIMRoomMembersAttributesQueriedCallback = std::function<void(
    const std::string &roomID, const std::vector<ZIMRoomMemberAttributesInfo> &infos,
    const ZIMError &errorInfo)>;

using ZIMRoomMemberAttributesListQueriedCallback = std::function<void(
    const std::string &roomID, const std::vector<ZIMRoomMemberAttributesInfo> &infos,
    const std::string &nextFlag, const ZIMError &errorInfo)>;
// Returns the result of creating a group.
// @param groupInfo groupInfo.
// @param userList user list.
// @param errorUserList errorUserList.
// @param errorInfo Error code for group creation. 0 means the group creation is successful, non-0 means the group creation fails. If there is a failure to create a group, the developer should check the official website error code document to query the solution.

using ZIMGroupCreatedCallback = std::function<void(
    const ZIMGroupFullInfo &groupInfo, const std::vector<ZIMGroupMemberInfo> &userList,
    const std::vector<ZIMErrorUserInfo> &errorUserList, const ZIMError &errorInfo)>;

// Returns the result of disbanding the group.
// @param groupInfo groupInfo.
// @param errorInfo The error code for disbanding the group. 0 means that the group is successfully disbanded, and non-0 means that the group failed to be disbanded. If there is a failure to dissolve the group, the developer should check the official website error code document to find the solution.

using ZIMGroupDismissedCallback =
    std::function<void(const std::string &groupID, const ZIMError &errorInfo)>;

// Returns the result of joining the group.
// @param groupInfo groupInfo.
// @param errorInfo Error code to join the group. 0 means joining the group successfully, non-0 means joining the group failed. If there is a failure to join the group, the developer should check the official website error code document to query the solution.

using ZIMGroupJoinedCallback =
    std::function<void(const ZIMGroupFullInfo &groupInfo, const ZIMError &errorInfo)>;

// Return of the result of leaving the group.
// @param groupID GroupID.
// @param errorInfo Error code for leaving the group. 0 means leaving the group successfully, non-0 means leaving the group failed. If there is a failure to leave the group, the developer should check the official website error code document to query the solution.
using ZIMGroupLeftCallback =
    std::function<void(const std::string &groupID, const ZIMError &errorInfo)>;

// The result of the user being invited to the group is returned.
// @param groupID GroupID.
// @param userList user list.
// @param errorUserList List of users who failed to invite.
// @param errorInfo Error code for inviting users to join the group. 0 means to invite the user to join the group, non-0 means that the invitation to join the group fails. If there is a failure to invite users to join the group, the developer should check the official website error code document to query the solution.

using ZIMGroupUsersInvitedCallback = std::function<void(
    const std::string &groupID, const std::vector<ZIMGroupMemberInfo> &userList,
    const std::vector<ZIMErrorUserInfo> &errorUserList, const ZIMError &errorInfo)>;

// Returns the result of kicking out a group member.
// @param groupID GroupID.
// @param kickedUserIDList List of kicked members.
// @param errorUserList Bad user list.
// @param errorInfo Error code for kicking out a group member. 0 means that the group member was successfully kicked out, and non-0 means that the group member failed to be kicked out. If a group member is kicked out, the developer should check the official website error code document to query the solution.
using ZIMGroupMemberKickedCallback = std::function<void(
    const std::string &groupID, const std::vector<std::string> &kickedUserIDList,
    const std::vector<ZIMErrorUserInfo> &errorUserList, const ZIMError &errorInfo)>;

// Returns the result of the group owner assignment.
// @param groupID GroupID.
// @param toUserID userID.
// @param errorInfo The error code of the transfer group owner. 0 means the transfer of the group owner is successful, non-0 means the transfer of the group owner failed. If the transfer of the group owner fails, the developer should check the official website error code document to query the solution.

using ZIMGroupOwnerTransferredCallback = std::function<void(
    const std::string &groupID, const std::string &toUserID, const ZIMError &errorInfo)>;

// Returns the result of the group name update.
// @param groupID GroupID.
// @param groupName groupName.
// @param errorInfo Update the error code of the group name. 0 means the update of the group name succeeds, non-0 means the update of the group name fails. If there is a failure to update the group name, the developer should check the official website error code document to query the solution.

using ZIMGroupNameUpdatedCallback = std::function<void(
    const std::string &groupID, const std::string &groupName, const ZIMError &errorInfo)>;

using ZIMGroupAvatarUrlUpdatedCallback = std::function<void(
    const std::string &groupID, const std::string &groupAvatarUrl, const ZIMError &errorInfo)>;

// The return of the results of the group announcement update.
// @param groupID GroupID.
// @param groupNotice Group announcement.
// @param errorInfo Update the error code of the group announcement. 0 means the update group announcement is successful, non-0 means the update group announcement fails. If the update group announcement fails, the developer should check the official website error code document to query the solution.
using ZIMGroupNoticeUpdatedCallback = std::function<void(
    const std::string &groupID, const std::string &groupNotice, const ZIMError &errorInfo)>;

using ZIMGroupMutedCallback =
    std::function<void(const std::string &groupID, bool isMute, const ZIMGroupMuteInfo &mutedInfo,
                       const ZIMError &errorInfo)>;

using ZIMGroupMembersMutedCallback = std::function<void(
    const std::string &groupID, bool isMute, int duration,
    const std::vector<std::string> &mutedUserIDs,
    const std::vector<ZIMErrorUserInfo> &errorUserList, const ZIMError &errorInfo)>;

using ZIMGroupMemberMutedListQueriedCallback =
    std::function<void(const std::string &groupID, unsigned long long nextFlag,
                       const std::vector<ZIMGroupMemberInfo> &userList, const ZIMError &errorInfo)>;

// Returns the result of querying group information.
// @param groupInfo groupInfo.
// @param errorInfo Error code for querying group information. 0 means the query group information is successful, non-0 means the query group information fails. If there is a failure to query group information, the developer should check the official website for the error code document query solution.
using ZIMGroupInfoQueriedCallback =
    std::function<void(const ZIMGroupFullInfo &groupInfo, const ZIMError &errorInfo)>;

// Returns the result of a group attribute operation.
// @param groupID GroupID.
// @param errorKeys errorKeys.
// @param errorInfo Set the error code of the group attribute. 0 means the group attribute setting is successful, non-0 means the group attribute setting fails. If there is a failure to set the group properties, the developer should check the official website error code documentation for solutions.
using ZIMGroupAttributesOperatedCallback =
    std::function<void(const std::string &groupID, const std::vector<std::string> &errorKeys,
                       const ZIMError &errorInfo)>;

// The return of query group attribute results.
// @param groupID GroupID.
// @param groupAttributes groupAttributes.
// @param errorInfo Error code for querying group attributes. 0 means the query group attribute succeeds, non-0 means the query group attribute fails. If there is a failure to query group properties, the developer should check the official website for the error code document query solution.

using ZIMGroupAttributesQueriedCallback = std::function<void(
    const std::string &groupID, const std::unordered_map<std::string, std::string> &groupAttributes,
    const ZIMError &errorInfo)>;

// Returns the result of the member role update.
// @param groupID GroupID.
// @param forUserID userID.
// @param role role.
// @param errorInfo Error code for querying group attributes. 0 means the query group attribute succeeds, non-0 means the query group attribute fails. If there is a failure to query group properties, the developer should check the official website for the error code document query solution.

using ZIMGroupMemberRoleUpdatedCallback =
    std::function<void(const std::string &groupID, const std::string &forUserID,
                       ZIMGroupMemberRole role, const ZIMError &errorInfo)>;

// Returns the result of the group member's nickname update.
// @param groupID GroupID.
// @param forUserID userID.
// @param nickname userNickname.
// @param errorInfo Set the error code of the group member's nickname. 0 means setting the group member's nickname successfully, non-0 means setting the group member's nickname failed. If there is a failure to set the group member's nickname, the developer should check the official website error code document to query the solution.

using ZIMGroupMemberNicknameUpdatedCallback =
    std::function<void(const std::string &groupID, const std::string &forUserID,
                       const std::string &nickname, const ZIMError &errorInfo)>;

// Returns the result of the group member query.
// @param groupID GroupID.
// @param userInfo Group member information.
// @param errorInfo Query the error code of group members. 0 means querying group members successfully, non-0 means querying group members failed. If there is a failure to query group members, the developer should check the official website for the error code document query solution.

using ZIMGroupMemberInfoQueriedCallback = std::function<void(
    const std::string &groupID, const ZIMGroupMemberInfo &userInfo, const ZIMError &errorInfo)>;

// Returns the results of the query group list.
// @param groupList groupList.
// @param errorInfo Query the error code of the group list. 0 means the query group list succeeds, non-0 means the query group list fails. If there is a failure to query the group list, the developer should check the official website for the error code document query solution.
using ZIMGroupListQueriedCallback =
    std::function<void(const std::vector<ZIMGroup> &groupList, const ZIMError &errorInfo)>;

// Returns the result of querying the group member list.
// @param groupID GroupID.
// @param userList user list.
// @param nextFlag nextFlag.
// @param errorInfo Error code for querying the group member list. 0 means that the query of the group member list succeeds, and non-0 means that the query of the group member list fails. If there is a failure to query the group member list, the developer should check the official website for the error code document query solution.

using ZIMGroupMemberListQueriedCallback =
    std::function<void(const std::string &groupID, const std::vector<ZIMGroupMemberInfo> &userList,
                       unsigned int nextFlag, const ZIMError &errorInfo)>;

using ZIMGroupMemberCountQueriedCallback =
    std::function<void(const std::string &groupID, unsigned int count, const ZIMError &errorInfo)>;

using ZIMGroupMessageReceiptMemberListQueriedCallback =
    std::function<void(const std::string &groupID, const std::vector<ZIMGroupMemberInfo> &userList,
                       unsigned int nextFlag, const ZIMError &errorInfo)>;

using ZIMGroupsSearchedCallback =
    std::function<void(const std::vector<ZIMGroupSearchInfo> &groupSearchInfoList,
                       unsigned int nextFlag, const ZIMError &errorInfo)>;

using ZIMGroupMembersSearchedCallback =
    std::function<void(const std::string &groupID, const std::vector<ZIMGroupMemberInfo> &userList,
                       unsigned int nextFlag, const ZIMError &errorInfo)>;

// Action callback for sending a call invitation.
// @param callID The created call invitation ID.
// @param info Send a call invitation message.
// @param errorInfo Error code for sending call invitation. 0 means sending the call invitation successfully, non-0 means sending the call invitation failed. If there is a failure to send a call invitation, the developer should check the official website error code documentation for solutions.

using ZIMCallInvitationSentCallback = std::function<void(
    const std::string &callID, const ZIMCallInvitationSentInfo &info, const ZIMError &errorInfo)>;

/// Supported version: 2.0.0.
///
/// Detail description: Operation callback for sending a call invitation.
///
/// Business scenario: After the operation of sending a call invitation is performed, the success or failure can be known through this callback.
///
/// Notification timing: The result is returned after the operation of sending the call invitation is completed.
///
/// Related interface: [callInvite], send a call invitation.
using ZIMCallingInvitationSentCallback =
    std::function<void(const std::string &callID, const ZIMCallingInvitationSentInfo &info,
                       const ZIMError &errorInfo)>;

using ZIMCallJoinSentCallback = std::function<void(
    const std::string &callID, const ZIMCallJoinSentInfo &info, const ZIMError &errorInfo)>;

/// Quit the callback closure of the call invitation operation.
using ZIMCallQuitSentCallback = std::function<void(
    const std::string &callID, const ZIMCallQuitSentInfo &info, const ZIMError &errorInfo)>;

/// Result callback of ending the call invitation.
using ZIMCallEndSentCallback = std::function<void(
    const std::string &callID, const ZIMCallEndedSentInfo &info, const ZIMError &errorInfo)>;

/// Result Callback of querying the call list.
using ZIMCallInvitationListQueriedCallback = std::function<void(
    const std::vector<ZIMCallInfo> &callList, long long nextFlag, const ZIMError &errorInfo)>;

using ZIMBlacklistUsersAddedCallback = std::function<void(
    const std::vector<ZIMErrorUserInfo> &errorUserList, const ZIMError &errorInfo)>;

using ZIMBlacklistUsersRemovedCallback = std::function<void(
    const std::vector<ZIMErrorUserInfo> &errorUserList, const ZIMError &errorInfo)>;

using ZIMBlacklistQueriedCallback = std::function<void(
    const std::vector<ZIMUserInfo> &blacklist, unsigned int nextFlag, const ZIMError &errorInfo)>;

using ZIMBlacklistCheckedCallback =
    std::function<void(bool isUserInBlacklist, const ZIMError &errorInfo)>;

using ZIMFriendApplicationListQueriedCallback =
    std::function<void(const std::vector<ZIMFriendApplicationInfo> &applicationList,
                       unsigned int nextFlag, const ZIMError &errorInfo)>;

using ZIMFriendListQueriedCallback =
    std::function<void(const std::vector<ZIMFriendInfo> &friendList, unsigned int nextFlag,
                       const ZIMError &errorInfo)>;

using ZIMFriendApplicationRejectedCallback =
    std::function<void(const ZIMUserInfo &userInfo, const ZIMError &errorInfo)>;

using ZIMFriendApplicationAcceptedCallback =
    std::function<void(const ZIMFriendInfo &friendInfo, const ZIMError &errorInfo)>;

using ZIMFriendsInfoQueriedCallback = std::function<void(
    const std::vector<ZIMFriendInfo> &friendInfos,
    const std::vector<ZIMErrorUserInfo> &errorUserList, const ZIMError &errorInfo)>;

using ZIMFriendAttributesUpdatedCallback =
    std::function<void(const ZIMFriendInfo &friendInfo, const ZIMError &errorInfo)>;

using ZIMFriendAliasUpdatedCallback =
    std::function<void(const ZIMFriendInfo &friendInfo, const ZIMError &errorInfo)>;

using ZIMFriendsRelationCheckedCallback = std::function<void(
    const std::vector<ZIMFriendRelationInfo> &relationInfos,
    const std::vector<ZIMErrorUserInfo> &errorUserList, const ZIMError &errorInfo)>;

using ZIMFriendsDeletedCallback = std::function<void(
    const std::vector<ZIMErrorUserInfo> &errorUserList, const ZIMError &errorInfo)>;

using ZIMFriendApplicationSentCallback =
    std::function<void(const ZIMFriendApplicationInfo &applicationInfo, const ZIMError &errorInfo)>;

using ZIMFriendAddedCallback =
    std::function<void(const ZIMFriendInfo &friendInfo, const ZIMError &errorInfo)>;

using ZIMFriendsSearchedCallback =
    std::function<void(const std::vector<ZIMFriendInfo> &friendInfos, unsigned int nextFlag,
                       const ZIMError &errorInfo)>;

// Callback for the action to cancel the call invitation.
// @param callID CallID.
// @param errorInvitees The user who failed to cancel the call invitation.
// @param errorInfo Error code to cancel the call invitation. 0 means canceling the call invitation successfully, non-0 means canceling the call invitation failed. If there is a failure to cancel the call invitation, the developer should check the official website error code document to query the solution.

using ZIMCallCancelSentCallback =
    std::function<void(const std::string &callID, const std::vector<std::string> &errorInvitees,
                       const ZIMError &errorInfo)>;

// Action callback for accepting call invitations.
// @param callID CallID.
// @param errorInfo Error code to accept call invitation. 0 means accepting the call invitation successfully, non-0 means accepting the call invitation failed. If there is a failure to accept the call invitation, the developer should check the official website error code documentation for solutions.
using ZIMCallAcceptanceSentCallback =
    std::function<void(const std::string &callID, const ZIMError &errorInfo)>;

//Action callback for rejecting a call invitation.
// @param callID CallID.
// @param errorInfo Error code to reject call invitation. 0 means rejecting the call invitation successfully, non-0 means failing to reject the call invitation. If there is a failure to reject the call invitation, the developer should check the official website error code documentation for solutions.
using ZIMCallRejectionSentCallback =
    std::function<void(const std::string &callID, const ZIMError &errorInfo)>;

// Supported Versions: 2.4.0 and above.

// Detail description: A callback for the result of inserting a local message.

// Business scenario: The developer can use the [errorCode] in the callback to determine whether the insertion is successful.

// Notification timing: This callback is triggered when the developer calls the [insertMessageToLocalDB] interface.

// Related interfaces: Insert local messages through [insertMessageToLocalDB], and the success or failure will be notified through this callback.
// @param Message object.
// @param errorInfo Insert the error code for the local message. 0 means inserting the message successfully, non-0 means inserting the message failed. If there is a failure to insert the message, the developer should check the official website error code document to query the solution.
using ZIMMessageInsertedCallback =
    std::function<void(const std::shared_ptr<ZIMMessage> &message, const ZIMError &errorInfo)>;

} // namespace zim
