#ifndef __ZIM_H__
#define __ZIM_H__

//
//  zim.h
//  ZIM
//
//  Copyright © 2021 Zego. All rights reserved.
//

#include "zim_defines.h"

ZIM_EXTERN_BEGIN

// MARK: - Main

/// Gets the SDK's version number.
///
/// When the SDK is running, the developer finds that it does not match the expected situation and submits the problem and related logs to the ZEGO technical staff for locating. The ZEGO technical staff may need the information of the engine version to assist in locating the problem.
/// Developers can also collect this information as the version information of the engine used by the app, so that the SDK corresponding to each version of the app on the line.
///
/// @return SDK version
ZIM_API const char *ZIM_CALL zim_get_version(void);

//ZIM_API const char *ZIM_CALL zim_get_pushid();
ZIM_API void ZIM_CALL zim_set_pushid(const char *pushid);

ZIM_API void ZIM_CALL zim_set_badge(int badge, const char *pushID);

ZIM_API void ZIM_CALL zim_set_android_env(void *jvm, void *context);

ZIM_API void ZIM_CALL zim_write_custom_log(const char *custom_log, const char *module_name);

/// Create a ZIM instance.
///
/// You need to create and initialize an ZIM instance before calling any other function.
/// The SDK supports the creation of multiple ZIM instances.
///
/// @param handle [in/out] The handle of the instance.
/// @param app_id Application ID issued by ZEGO for developers, please apply from the ZEGO Admin Console https://console.zego.im
ZIM_API void ZIM_CALL zim_create(zim_handle *handle, unsigned int app_id, const char *app_sign);

ZIM_API void ZIM_CALL zim_get_instance(zim_handle *handle);
/// Destroy the ZIM instance.
///
/// Used to release resources used by ZIM.
///
/// @param handle [in/out] The handle of the instance.
ZIM_API void ZIM_CALL zim_destroy(zim_handle *handle);

/// Set log related configuration.
///
/// When you need to customize the log file size and path, you need to call this function to set the log configuration.
/// It must be set before calling [create] to take effect. If it is set after [create], it will take effect at the next [create].
///
/// @param config Log configuration object
ZIM_API void ZIM_CALL zim_set_log_config(struct zim_log_config config);

ZIM_API const char *ZIM_CALL zim_get_log_path(void);

ZIM_API unsigned long long ZIM_CALL zim_get_log_size(void);

typedef void (*zim_on_log_uploaded_callback)(zim_handle handle, struct zim_error error_info,
                                             zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_log_uploaded_callback(
    zim_handle handle, zim_on_log_uploaded_callback callback_function);

/// Upload log and call after setting up log path
///
/// Description: After calling this function, you will receive the [onLogUploaded] callback, which can be used to determine whether the upload is successful.
ZIM_API void ZIM_CALL zim_upload_log(zim_handle handle, zim_sequence *sequence);

/// Set cache related configuration.
///
/// When you need to customize the path of cache files (such as chat records), you need to call this function to set the configuration.
/// It must be set before calling [create] to take effect. If it is set after [create], it will take effect at the next [create].
///
/// @param config Cache configuration object
ZIM_API void ZIM_CALL zim_set_cache_config(struct zim_cache_config config);

ZIM_API const char *ZIM_CALL zim_get_cache_path(void);

/// Set advanced config.
///
/// When you want to set some advanced config, you can call this api with string that would be used to set advanced config.
///
/// @param handle The handle of the instance.
/// @param command The command string wanted to be set.
ZIM_API void ZIM_CALL zim_set_advanced_config(const char *key, const char *value);

ZIM_API bool ZIM_CALL zim_set_geofencing_config(const int *area_list, unsigned int area_list_length,
                                                enum zim_geofencing_type type);

// MARK: - User

/// Callback when other members join the room.
///
/// After joining a room, this callback will be triggered when other members also join this room.
///
/// @param handle ZIM instance.
/// @param member_list List of members who joined the room.
/// @param member_list_length The length of the member list.
/// @param room_id The ID of the room where this event occurred.
typedef void (*zim_on_logged_in_callback)(zim_handle handle, struct zim_error error_info,
                                          zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_logged_in_callback(zim_handle handle,
                                                      zim_on_logged_in_callback callback_function);

/// Login, you must log in before using all functions.
///
/// Description: After invoking this function, you will receive the [onConnectionStateChanged] callback,
/// which can be used to determine whether the login is successful.
///
/// @param handle The handle of the instance.
/// @param user_info Unique ID used to identify the user. Note that the userID must be unique under the same appID, otherwise mutual kicks out will occur.
/// @param token The token issued by the developer's business server, used to ensure security. The generation rules are detailed in ZEGO document website.
ZIM_API void ZIM_CALL zim_login(zim_handle handle, const char *user_id,
                                struct zim_login_config config, zim_sequence *sequence);

/// Log out, you cannot receive messages after logging out.
///
/// Description: After invoking this function, you will receive the [onConnectionStateChanged] callback.
///
/// @param handle The handle of the instance.
ZIM_API void ZIM_CALL zim_logout(zim_handle handle);

/// Callback of the result of renewing the token.
///
/// @param handle The handle of the instance.
/// @param token The renewed token.
/// @param error_code Error code.
/// @param sequence Context that identifies which invocation triggered this callback.
typedef void (*zim_on_token_renewed_callback)(zim_handle handle, const char *token,
                                              struct zim_error error_info, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_token_renewed_callback(
    zim_handle handle, zim_on_token_renewed_callback callback_function);

/// Update the authentication token.
///
/// Description: After receiving the [onTokenWillExpire] callback, you needs to request your authentication server to regenerate a token in time.
///
/// @param handle The handle of the instance.
/// @param user_id The token issued by the developer's business server, used to ensure security. The generation rules are detailed in ZEGO document website.
ZIM_API void ZIM_CALL zim_renew_token(zim_handle handle, const char *token, zim_sequence *sequence);

typedef void (*zim_on_users_info_queried_callback)(
    zim_handle handle, const struct zim_user_full_info *user_list, unsigned int user_list_length,
    const struct zim_error_user_info *error_user_list, unsigned int error_user_list_length,
    struct zim_error error_info, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_users_info_queried_callback(
    zim_handle handle, zim_on_users_info_queried_callback callback_function);

ZIM_API void ZIM_CALL zim_query_users_info(zim_handle handle, const char **user_id_list,
                                           unsigned int user_id_list_length,
                                           struct zim_users_info_query_config config,
                                           zim_sequence *sequence);

typedef void (*zim_on_user_name_updated_callback)(zim_handle handle, const char *user_name,
                                                  struct zim_error error_info,
                                                  zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_user_name_updated_callback(
    zim_handle handle, zim_on_user_name_updated_callback callback_function);

ZIM_API void ZIM_CALL zim_update_user_name(zim_handle handle, const char *user_name,
                                           zim_sequence *sequence);

typedef void (*zim_on_user_avatar_url_updated_callback)(zim_handle handle,
                                                        const char *user_avatar_url,
                                                        struct zim_error error_info,
                                                        zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_user_avatar_url_updated_callback(
    zim_handle handle, zim_on_user_avatar_url_updated_callback callback_function);

ZIM_API void ZIM_CALL zim_update_user_avatar_url(zim_handle handle, const char *user_avatar_url,
                                                 zim_sequence *sequence);

typedef void (*zim_on_user_extended_data_updated_callback)(zim_handle handle,
                                                           const char *user_extended_data,
                                                           struct zim_error error_info,
                                                           zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_user_extended_data_updated_callback(
    zim_handle handle, zim_on_user_extended_data_updated_callback callback_function);

ZIM_API void ZIM_CALL zim_update_user_extended_data(zim_handle handle,
                                                    const char *user_extended_data,
                                                    zim_sequence *sequence);

typedef void (*zim_on_message_local_extended_data_updated_callback)(zim_handle handle,
                                                                    struct zim_message message,
                                                                    struct zim_error error_info,
                                                                    zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_message_local_extended_data_updated_callback(
    zim_handle handle, zim_on_message_local_extended_data_updated_callback callback_function);

/// Update the local expandable field of the message.
///
/// Description: After the user logs in, calling this interface allows updating the local expandable field of the message.
///
/// @param handle The handle of the instance.
/// @param extended_data The expandable message field visible only on this end can store additional information locally and currently has a length limit of 128K. If you have special requirements, please contact ZEGO technical support for configuration.
/// @param message Message body to be updated.
/// @param sequence Context that identifies which invocation triggered this callback.
ZIM_API void ZIM_CALL zim_update_message_local_extended_data(zim_handle handle,
                                                             const char *local_extended_data,
                                                             struct zim_message message,
                                                             zim_sequence *sequence);

// MARK: - Conversation

typedef void (*zim_on_conversation_queried_callback)(zim_handle handle,
                                                     struct zim_conversation conversation,
                                                     struct zim_error error_info,
                                                     zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_conversation_queried_callback(
    zim_handle handle, zim_on_conversation_queried_callback callback_function);

ZIM_API void ZIM_CALL zim_query_conversation(zim_handle handle, const char *conversation_id,
                                             enum zim_conversation_type conversation_type,
                                             zim_sequence *sequence);

typedef void (*zim_on_conversation_list_queried_callback)(
    zim_handle handle, const struct zim_conversation *conversation_list,
    unsigned int conversation_list_length, struct zim_error error_info, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_conversation_list_queried_callback(
    zim_handle handle, zim_on_conversation_list_queried_callback callback_function);

ZIM_API void ZIM_CALL zim_query_conversation_list(zim_handle handle,
                                                  struct zim_conversation_query_config config,
                                                  zim_sequence *sequence);

// query all pinned conversation list
typedef void (*zim_on_conversation_pinned_list_queried_callback)(
    zim_handle handle, const struct zim_conversation *conversation_list,
    unsigned int conversation_list_length, struct zim_error error_info, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_conversation_pinned_list_queried_callback(
    zim_handle handle, zim_on_conversation_pinned_list_queried_callback callback_function);

ZIM_API void ZIM_CALL zim_query_conversation_pinned_list(
    zim_handle handle, struct zim_conversation_query_config config, zim_sequence *sequence);

// update conversation's pinned state callback
typedef void (*zim_on_conversation_pinned_state_updated_callback)(
    zim_handle handle, const char *conversation_id, enum zim_conversation_type conversation_type,
    struct zim_error error_info, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_conversation_pinned_state_updated_callback(
    zim_handle handle, zim_on_conversation_pinned_state_updated_callback callback_function);

ZIM_API void ZIM_CALL zim_update_conversation_pinned_state(
    zim_handle handle, bool is_pinned, const char *conversation_id,
    enum zim_conversation_type conversation_type, zim_sequence *sequence);

typedef void (*zim_on_conversation_deleted_callback)(zim_handle handle, const char *conversation_id,
                                                     enum zim_conversation_type conversation_type,
                                                     struct zim_error error_info,
                                                     zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_conversation_deleted_callback(
    zim_handle handle, zim_on_conversation_deleted_callback callback_function);

ZIM_API void ZIM_CALL zim_delete_conversation(zim_handle handle, const char *conversation_id,
                                              enum zim_conversation_type conversation_type,
                                              struct zim_conversation_delete_config config,
                                              zim_sequence *sequence);

typedef void (*zim_on_conversations_all_deleted_callback)(zim_handle handle,
                                                          struct zim_error error_info,
                                                          zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_conversations_all_deleted_callback(
    zim_handle handle, zim_on_conversations_all_deleted_callback callback_function);

ZIM_API void ZIM_CALL zim_delete_all_conversations(zim_handle handle,
                                                   struct zim_conversation_delete_config config,
                                                   zim_sequence *sequence);

typedef void (*zim_on_conversation_unread_message_count_cleared_callback)(
    zim_handle handle, const char *conversation_id, enum zim_conversation_type conversation_type,
    struct zim_error error_info, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_conversation_unread_message_count_cleared_callback(
    zim_handle handle, zim_on_conversation_unread_message_count_cleared_callback callback_function);

ZIM_API void ZIM_CALL zim_clear_conversation_unread_message_count(
    zim_handle handle, const char *conversation_id, enum zim_conversation_type conversation_type,
    zim_sequence *sequence);

typedef void (*zim_on_conversation_total_unread_message_count_cleared_callback)(
    zim_handle handle, struct zim_error error_info, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_conversation_total_unread_message_count_cleared_callback(
    zim_handle handle,
    zim_on_conversation_total_unread_message_count_cleared_callback callback_function);

ZIM_API void ZIM_CALL zim_clear_conversation_total_unread_message_count(zim_handle handle,
                                                                        zim_sequence *sequence);

typedef void (*zim_on_conversation_notification_status_set_callback)(
    zim_handle handle, const char *conversation_id, enum zim_conversation_type conversation_type,
    struct zim_error error_info, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_conversation_notification_status_set_callback(
    zim_handle handle, zim_on_conversation_notification_status_set_callback callback_function);

ZIM_API void ZIM_CALL zim_set_conversation_notification_status(
    zim_handle handle, enum zim_conversation_notification_status status,
    const char *conversation_id, enum zim_conversation_type conversation_type,
    zim_sequence *sequence);

typedef void (*zim_on_conversation_message_receipt_read_sent_callback)(
    zim_handle handle, const char *conversation_id, enum zim_conversation_type conversation_type,
    struct zim_error error_info, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_conversation_message_receipt_read_sent_callback(
    zim_handle handle, zim_on_conversation_message_receipt_read_sent_callback callback_function);

ZIM_API void ZIM_CALL zim_send_conversation_message_receipt_read(
    zim_handle handle, const char *conversation_id, enum zim_conversation_type conversation_type,
    zim_sequence *sequence);

typedef void (*zim_on_combine_message_detail_queried_callback)(zim_handle handle,
                                                               const struct zim_message message,
                                                               struct zim_error error_info,
                                                               zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_message_query_combine_message_detail_callback(
    zim_handle handle, zim_on_combine_message_detail_queried_callback callback_function);

ZIM_API void ZIM_CALL zim_query_combine_message_detail(zim_handle handle,
                                                       struct zim_message message,
                                                       zim_sequence *sequence);
typedef void (*zim_on_conversation_draft_set_callback)(zim_handle handle,
                                                       const char *conversation_id,
                                                       enum zim_conversation_type conversation_type,
                                                       struct zim_error error_info,
                                                       zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_conversation_draft_set_callback(
    zim_handle handle, zim_on_conversation_draft_set_callback callback_function);

ZIM_API void ZIM_CALL zim_set_conversation_draft(zim_handle handle, const char *draft,
                                                 const char *conversation_id,
                                                 enum zim_conversation_type conversation_type,
                                                 zim_sequence *sequence);

// MARK: - Message

/// Callback of the result of sending the message.
///
/// @param handle The handle of the instance.
/// @param message The sent message object, from which parameters such as messageID can be obtained. If the sending fails, the messageID parameter in the message will be an empty string.
/// @param error_code Error code.
/// @param sequence Context that identifies which invocation triggered this callback.
typedef void (*zim_on_message_sent_callback)(zim_handle handle, struct zim_message message,
                                             struct zim_error error_info, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_message_sent_callback(
    zim_handle handle, zim_on_message_sent_callback callback_function);

typedef void (*zim_on_message_attached_callback)(zim_handle handle, struct zim_message message,
                                                 zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_message_attached_callback(
    zim_handle handle, zim_on_message_attached_callback callback_function);

/// Send peer-to-peer messages.
///
/// Description: After invoking this function, you will receive the [MessageSent] callback,
/// which can be used to determine whether the message is sent successfully.
///
/// @param handle The handle of the instance.
/// @param message The message to be sent.
/// @param to_user_id The ID of the user who will receive the message.
/// @param sequence [in/out] Context that identifies this invocation when callback.
ZIM_API void ZIM_CALL zim_send_peer_message(zim_handle handle, struct zim_message message,
                                            const char *to_user_id,
                                            struct zim_message_send_config *config,
                                            zim_sequence *sequence);

/// Send room messages.
///
/// Description: After invoking this function, you will receive the [MessageSent] callback,
/// which can be used to determine whether the message is sent successfully.
///
/// @param handle The handle of the instance.
/// @param message The message to be sent.
/// @param to_room_id The ID of the room which will receive the message.
/// @param sequence [in/out] Context that identifies this invocation when callback.
ZIM_API void ZIM_CALL zim_send_room_message(zim_handle handle, struct zim_message message,
                                            const char *to_room_id,
                                            struct zim_message_send_config *config,
                                            zim_sequence *sequence);

ZIM_API void ZIM_CALL zim_send_group_message(zim_handle handle, struct zim_message message,
                                             const char *to_group_id,
                                             struct zim_message_send_config *config,
                                             zim_sequence *sequence);

ZIM_API void ZIM_CALL zim_send_message(zim_handle handle, struct zim_message message,
                                       const char *to_conversation_id,
                                       enum zim_conversation_type conversation_type,
                                       struct zim_message_send_config config,
                                       zim_sequence *sequence);

typedef void (*zim_on_media_uploading_progress_callback)(zim_handle handle,
                                                         const struct zim_message message,
                                                         long long current_file_size,
                                                         long long total_file_size,
                                                         zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_media_uploading_progress_callback(
    zim_handle handle, zim_on_media_uploading_progress_callback callback_function);

ZIM_API void ZIM_CALL zim_send_media_message(zim_handle handle, struct zim_message message,
                                             const char *to_conversation_id,
                                             enum zim_conversation_type conversation_type,
                                             struct zim_message_send_config config,
                                             zim_sequence *sequence);

typedef void (*zim_on_media_downloading_progress_callback)(zim_handle handle,
                                                           const struct zim_message message,
                                                           long long current_file_size,
                                                           long long total_file_size,
                                                           zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_media_downloading_progress_callback(
    zim_handle handle, zim_on_media_downloading_progress_callback callback_function);

typedef void (*zim_on_media_downloaded_callback)(zim_handle handle,
                                                 const struct zim_message message,
                                                 struct zim_error error_info,
                                                 zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_media_downloaded_callback(
    zim_handle handle, zim_on_media_downloaded_callback callback_function);

ZIM_API void ZIM_CALL zim_download_media_file(zim_handle handle, struct zim_message message,
                                              enum zim_media_file_type file_type,
                                              zim_sequence *sequence);

/// Callback of the result of querying messages.
///
/// @param handle The handle of the instance.
/// @param message_list The message list of the query result.
/// @param message_list_length The length of the message list.
/// @param error_code Error code.
/// @param sequence Context that identifies which invocation triggered this callback.
typedef void (*zim_on_message_queried_callback)(zim_handle handle, const char *conversation_id,
                                                enum zim_conversation_type conversation_type,
                                                const struct zim_message *message_list,
                                                unsigned int message_list_length,
                                                struct zim_error error_info, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_message_queried_callback(
    zim_handle handle, zim_on_message_queried_callback callback_function);

/// Paging query point-to-point historical message.
///
/// Description: After invoking this function, you will receive the [MessageQueried] callback,
/// which contains the results of this query.
/// You can continue the next query based on the ID of the last message in the callback result's message list.
///
/// @param handle The handle of the instance.
/// @param user_id The user ID of the historical message to be queried.
/// @param config Configuration of query message operation.
/// @param sequence [in/out] Context that identifies this invocation when callback.
ZIM_API void ZIM_CALL zim_query_history_message(zim_handle handle, const char *conversation_id,
                                                enum zim_conversation_type conversation_type,
                                                struct zim_message_query_config config,
                                                zim_sequence *sequence);

typedef void (*zim_on_message_deleted_callback)(zim_handle handle, const char *conversation_id,
                                                enum zim_conversation_type conversation_type,
                                                struct zim_error error_info, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_message_deleted_callback(
    zim_handle handle, zim_on_message_deleted_callback callback_function);

ZIM_API void ZIM_CALL zim_delete_messages(zim_handle handle, struct zim_message *message_list,
                                          unsigned int member_list_length,
                                          const char *conversation_id,
                                          enum zim_conversation_type conversation_type,
                                          struct zim_message_delete_config config,
                                          zim_sequence *sequence);

ZIM_API void ZIM_CALL zim_delete_all_message(zim_handle handle, const char *conversation_id,
                                             enum zim_conversation_type conversation_type,
                                             struct zim_message_delete_config config,
                                             zim_sequence *sequence);

typedef void (*zim_on_conversation_messages_all_deleted_callback)(zim_handle handle,
                                                                  struct zim_error error_info,
                                                                  zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_conversation_messages_all_deleted_callback(
    zim_handle handle, zim_on_conversation_messages_all_deleted_callback callback_function);

ZIM_API void ZIM_CALL zim_delete_all_conversation_messages(zim_handle handle,
                                                           struct zim_message_delete_config config,
                                                           zim_sequence *sequence);

typedef void (*zim_on_message_inserted_callback)(zim_handle handle, struct zim_message message,
                                                 struct zim_error error_info,
                                                 zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_message_inserted_callback(
    zim_handle handle, zim_on_message_inserted_callback callback_function);

ZIM_API void ZIM_CALL zim_insert_message_to_local_db(zim_handle handle, struct zim_message message,
                                                     const char *conversation_id,
                                                     enum zim_conversation_type conversation_type,
                                                     const char *sender_user_id,
                                                     zim_sequence *sequence);

typedef void (*zim_on_message_receipts_read_sent_callback)(
    zim_handle handle, const char *conversation_id, enum zim_conversation_type conversation_type,
    const signed long long *error_message_id_list, unsigned int error_message_id_list_length,
    struct zim_error error_info, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_message_receipts_read_sent_callback(
    zim_handle handle, zim_on_message_receipts_read_sent_callback callback_function);

ZIM_API void ZIM_CALL zim_send_message_receipts_read(zim_handle handle,
                                                     struct zim_message *message_list,
                                                     unsigned int message_list_length,
                                                     const char *conversation_id,
                                                     enum zim_conversation_type conversation_type,
                                                     zim_sequence *sequence);

typedef void (*zim_on_message_receipts_info_queried_callback)(
    zim_handle handle, const struct zim_message_receipt_info *receipt_info_list,
    unsigned int receipt_info_list_length, const long long *error_message_id_list,
    unsigned int error_message_id_list_length, struct zim_error error_info, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_message_receipts_info_queried_callback(
    zim_handle handle, zim_on_message_receipts_info_queried_callback callback_function);

ZIM_API void ZIM_CALL zim_query_message_receipts_info(zim_handle handle,
                                                      struct zim_message *message_list,
                                                      unsigned int message_list_length,
                                                      const char *conversation_id,
                                                      enum zim_conversation_type conversation_type,
                                                      zim_sequence *sequence);

typedef void (*zim_on_message_revoked_callback)(zim_handle handle, struct zim_message message,
                                                struct zim_error error_info, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_message_revoked_callback(
    zim_handle handle, zim_on_message_revoked_callback callback_function);

ZIM_API void ZIM_CALL zim_revoke_message(zim_handle handle, struct zim_message message,
                                         struct zim_message_revoke_config config,
                                         zim_sequence *sequence);

typedef void (*zim_on_messages_searched_callback)(
    zim_handle handle, const char *conversation_id, enum zim_conversation_type conversation_type,
    const struct zim_message *message_list, unsigned int message_list_length,
    const struct zim_message *next_message, struct zim_error error_info, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_messages_searched_callback(
    zim_handle handle, zim_on_messages_searched_callback callback_function);

/// Search local message list.
///
/// Business scenario: When it is necessary to search for past local messages based on keywords or other conditions, this interface can be called to perform pagination search for local messages.
/// Restrictions: Effective after login, invalid after logout. Searching is not supported in the room scene (conversationType=1).
///
/// @param handle The handle of the instance.
/// @param conversation_id The conversation ID of the local message to be searchd.
/// @param conversation_type conversation type.
/// @param config Search the configuration of local messages.
/// @param sequence [in/out] Context that identifies this invocation when callback.
ZIM_API void ZIM_CALL zim_search_local_messages(zim_handle handle, const char *conversation_id,
                                                enum zim_conversation_type conversation_type,
                                                struct zim_message_search_config config,
                                                zim_sequence *sequence);

typedef void (*zim_on_messages_global_searched_callback)(
    zim_handle handle, const struct zim_message *message_list, unsigned int message_list_length,
    const struct zim_message *next_message, struct zim_error error_info, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_messages_global_searched_callback(
    zim_handle handle, zim_on_messages_global_searched_callback callback_function);

/// Search global local message list.
///
/// Business scenario: When there is a need to perform a global search of previous local messages based on keywords or other conditions, you can invoke this interface to search local messages by grouping them according to conversations.
/// Restrictions: Effective after login, invalid after logout. Searching global is not supported in the room scene (conversationType=1).
///
/// @param handle The handle of the instance.
/// @param config Search global the configuration of local messages.
/// @param sequence [in/out] Context that identifies this invocation when callback.
ZIM_API void ZIM_CALL zim_search_global_local_messages(
    zim_handle handle, const struct zim_message_search_config config, zim_sequence *sequence);

typedef void (*zim_on_conversations_searched_callback)(
    zim_handle handle, const struct zim_conversation_search_info *conversation_search_info_list,
    unsigned int conversation_search_info_list_length, unsigned int next_flag,
    struct zim_error error_info, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_conversations_searched_callback(
    zim_handle handle, zim_on_conversations_searched_callback callback_function);

/// Search local conversations on local messages.
///
/// Business scenario: When there is a need to search conversations using keywords or other conditions, this API can be called for searching.
/// Restrictions: effective after logging in and becomes invalid after logging out. Searching is not supported in room scenarios (conversationType=1).
///
/// @param handle The handle of the instance.
/// @param config Global search conversation config.
/// @param sequence [in/out] Context that identifies this invocation when callback.
ZIM_API void ZIM_CALL zim_search_local_conversations(zim_handle handle,
                                                     struct zim_conversation_search_config config,
                                                     zim_sequence *sequence);
typedef void (*on_message_reaction_added_callback)(zim_handle handle,
                                                   struct zim_message_reaction reaction,
                                                   struct zim_error error_info,
                                                   zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_message_added_reaction_callback(
    zim_handle handle, on_message_reaction_added_callback callback_function);

ZIM_API void ZIM_CALL zim_add_message_reaction(zim_handle handle, const char *reaction_type,
                                               struct zim_message message, zim_sequence *sequence);

typedef void (*on_message_reaction_deleted_callback)(zim_handle handle,
                                                     struct zim_message_reaction reaction,
                                                     struct zim_error error_info,
                                                     zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_message_deleted_reaction_callback(
    zim_handle handle, on_message_reaction_deleted_callback callback_function);

ZIM_API void ZIM_CALL zim_delete_message_reaction(zim_handle handle, const char *reaction_type,
                                                  struct zim_message message,
                                                  zim_sequence *sequence);

typedef void (*on_message_reaction_users_queried_callback)(
    zim_handle handle, const struct zim_message message, const char *reaction_type,
    const struct zim_message_reaction_user_info *user_infos, unsigned int user_info_length,
    long long next_flag, unsigned int total_count, struct zim_error error_info,
    zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_message_queried_reaction_user_list_callback(
    zim_handle handle, on_message_reaction_users_queried_callback callback_function);

ZIM_API void ZIM_CALL zim_query_message_reaction_user_list(
    zim_handle handle, struct zim_message message,
    struct zim_message_reaction_users_query_config config, zim_sequence *sequence);

typedef void (*on_message_reactions_changed_event)(zim_handle handle,
                                                   const struct zim_message_reaction *reactions,
                                                   unsigned int reaction_length);

ZIM_API void ZIM_CALL zim_register_message_reactions_changed_event(
    zim_handle handle, on_message_reactions_changed_event event_function);

// MARK: - Room

/// Callback of the result of creating the room.
///
/// @param handle The handle of the instance.
/// @param room_info Details of the room created. If the creation fails, the roomID parameter in roomInfo will be an empty string.
/// @param error_code Error code.
/// @param sequence Context that identifies which invocation triggered this callback.
typedef void (*zim_on_room_created_callback)(zim_handle handle, struct zim_room_full_info room_info,
                                             struct zim_error error_info, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_room_created_callback(
    zim_handle handle, zim_on_room_created_callback callback_function);
/// Create a room.
///
/// Description: When a room is created, other users can join this room through [joinRoom] function.
/// When everyone leaves the room, the room will be automatically destroyed.
///
/// @param handle The handle of the instance.
/// @param room_info The configuration information of the room to be created.
/// @param sequence [in/out] Context that identifies this invocation when callback.
ZIM_API void ZIM_CALL zim_create_room(zim_handle handle, struct zim_room_info room_info,
                                      struct zim_room_advanced_config *config,
                                      zim_sequence *sequence);

/// Callback of the result of joining the room.
///
/// @param handle The handle of the instance.
/// @param room_info Details of the room joined. If the join fails, the roomID parameter in roomInfo will be an empty string.
/// @param error_code Error code.
/// @param sequence Context that identifies which invocation triggered this callback.
typedef void (*zim_on_room_joined_callback)(zim_handle handle, struct zim_room_full_info room_info,
                                            struct zim_error error_info, zim_sequence sequence);

ZIM_API void ZIM_CALL
zim_register_room_joined_callback(zim_handle handle, zim_on_room_joined_callback callback_function);

/// Join a room.
///
/// Description: If the room does not exist, the join fails and you need to call [createRoom] to create the room first.
/// When everyone leaves the room, the room will be automatically destroyed.
///
/// @param handle The handle of the instance.
/// @param room_id ID of the room to join.
/// @param sequence [in/out] Context that identifies this invocation when callback.
ZIM_API void ZIM_CALL zim_join_room(zim_handle handle, const char *room_id, zim_sequence *sequence);

/// Callback of the result of entering the room.
///
/// @param handle The handle of the instance.
/// @param room_info Details of the room joined. If the join fails, the roomID parameter in roomInfo will be an empty string.
/// @param error_code Error code.
/// @param sequence Context that identifies which invocation triggered this callback.
typedef void (*zim_on_room_entered_callback)(zim_handle handle, struct zim_room_full_info room_info,
                                             struct zim_error error_info, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_room_entered_callback(
    zim_handle handle, zim_on_room_entered_callback callback_function);

ZIM_API void ZIM_CALL zim_enter_room(zim_handle handle, struct zim_room_info room_info,
                                     struct zim_room_advanced_config *config,
                                     zim_sequence *sequence);

/// Callback of the result of leaving the room.
///
/// @param handle The handle of the instance.
/// @param error_code Error code.
/// @param sequence Context that identifies which invocation triggered this callback.
typedef void (*zim_on_room_left_callback)(zim_handle handle, const char *room_id,
                                          struct zim_error error_info, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_room_left_callback(zim_handle handle,
                                                      zim_on_room_left_callback callback_function);

/// Leave a room.
///
/// Description: If the current user is not in this room, the exit fails.
/// When everyone leaves the room, the room will be automatically destroyed.
///
/// @param handle The handle of the instance.
/// @param room_id ID of the room to leave.
/// @param sequence [in/out] Context that identifies this invocation when callback.
ZIM_API void ZIM_CALL zim_leave_room(zim_handle handle, const char *room_id,
                                     zim_sequence *sequence);

/// Callback of the result of querying the room members list.
///
/// @param handle The handle of the instance.
/// @param member_list List of members in the room.
/// @param member_list_length The length of the member list.
/// @param next_flag The flag of the paging query. If this field is an empty string, the query has been completed. Otherwise, you need to set this value to the "nextFlag" field of ZIMQueryMemberConfig for the next page query.
/// @param error_code Error code.
/// @param sequence Context that identifies which invocation triggered this callback.
typedef void (*zim_on_room_member_queried_callback)(zim_handle handle, const char *room_id,
                                                    const struct zim_user_info *user_list,
                                                    unsigned int user_list_length,
                                                    const char *next_flag,
                                                    struct zim_error error_info,
                                                    zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_room_member_queried_callback(
    zim_handle handle, zim_on_room_member_queried_callback callback_function);

/// Query the list of members in the room.
///
/// Description: After joining a room, you can use this function to get the list of members in the room.
///
/// @param handle The handle of the instance.
/// @param room_id ID of the room to query.
/// @param config Configuration of query room member operation.
/// @param sequence [in/out] Context that identifies this invocation when callback.
ZIM_API void ZIM_CALL zim_query_room_member_list(zim_handle handle, const char *room_id,
                                                 struct zim_room_member_query_config config,
                                                 zim_sequence *sequence);

typedef void (*zim_on_room_members_queried)(zim_handle handle, const char *room_id,
                                            const struct zim_room_member_info *member_list,
                                            unsigned int member_list_length,
                                            const struct zim_error_user_info *error_user_list,
                                            unsigned int error_user_list_length,
                                            struct zim_error error_info, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_room_members_queried_callback(
    zim_handle handle, zim_on_room_members_queried callback_function);

ZIM_API void ZIM_CALL zim_query_room_members(zim_handle handle, const char **user_ids,
                                             unsigned int user_ids_length, const char *room_id,
                                             zim_sequence *sequence);

/// Callback of the result of querying the online members count in the room.
///
/// @param handle The handle of the instance.
/// @param count The number of online members of the room.
/// @param error_code Error code.
/// @param sequence Context that identifies which invocation triggered this callback.
typedef void (*zim_on_room_online_member_count_queried_callback)(zim_handle handle,
                                                                 const char *room_id,
                                                                 unsigned int count,
                                                                 struct zim_error error_info,
                                                                 zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_room_online_member_count_queried_callback(
    zim_handle handle, zim_on_room_online_member_count_queried_callback callback_function);

/// Query the number of online members in the room.
///
/// Description: After joining a room, you can use this function to get the number of online members in the room.
///
/// @param handle The handle of the instance.
/// @param room_id ID of the room to query.
/// @param sequence [in/out] Context that identifies this invocation when callback.
ZIM_API void ZIM_CALL zim_query_room_online_member_count(zim_handle handle, const char *room_id,
                                                         zim_sequence *sequence);

/// Callback of the result of setting the room attributes in the room.
///
///
typedef void (*zim_on_room_attributes_operated_callback)(zim_handle handle, const char *room_id,
                                                         const char **error_key_list,
                                                         unsigned int error_key_list_length,
                                                         struct zim_error error_info,
                                                         zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_room_attributes_operated_callback(
    zim_handle handle, zim_on_room_attributes_operated_callback callback_function);

/// Set the attributes of a room.
///
/// Description: You can add room attributes as long as the total number is less than 10, if [is_auto_delete] is set to true,
/// the new room attributes added in this operation will be automatically cleared when you leave the room,
/// you can only update your own room attributes unless you set the [is_force_set] parameter to true
///
ZIM_API void ZIM_CALL zim_set_room_attributes(zim_handle handle,
                                              struct zim_room_attribute *room_attributes,
                                              unsigned int room_attributes_length,
                                              const char *room_id,
                                              struct zim_room_attributes_set_config config,
                                              zim_sequence *sequence);

/// Delete the attributes of a room
///
/// Description: You can delete the corresponding room attributes created by yourself by passing in one or more [room_attributes_keys].
/// For room attributes created by others, you can delete them only when your [is_force_delete] parameter is set to true.
///
ZIM_API void ZIM_CALL zim_delete_room_attributes(zim_handle handle, const char **keys,
                                                 unsigned int keys_length, const char *room_id,
                                                 struct zim_room_attributes_delete_config config,
                                                 zim_sequence *sequence);

typedef void (*zim_on_room_attributes_batch_operated_callback)(zim_handle handle,
                                                               const char *room_id,
                                                               struct zim_error error_info,
                                                               zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_room_attributes_batch_operated_callback(
    zim_handle handle, zim_on_room_attributes_batch_operated_callback callback_function);

ZIM_API void ZIM_CALL zim_begin_room_attributes_batch_operation(
    zim_handle handle, const char *room_id,
    struct zim_room_attributes_batch_operation_config config, zim_sequence *sequence);

ZIM_API void ZIM_CALL zim_end_room_attributes_batch_operation(zim_handle handle,
                                                              const char *room_id,
                                                              zim_sequence *sequence);

/// Callback of the result of querying the room attributes in the room.
///
///
typedef void (*zim_on_room_attributes_queried_callback)(
    zim_handle handle, const char *room_id, const struct zim_room_attribute *room_attributes,
    unsigned int room_attributes_length, struct zim_error error_info, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_room_attributes_queried_callback(
    zim_handle handle, zim_on_room_attributes_queried_callback callback_function);

/// Query the attributes of a room
///
/// Description: You can pass in [room_id] to query all room attributes in the specified room.
///
ZIM_API void ZIM_CALL zim_query_room_all_attributes(zim_handle handle, const char *room_id,
                                                    zim_sequence *sequence);

typedef void (*zim_on_room_members_attributes_operated_callback)(
    zim_handle handle, const char *room_id,
    const struct zim_room_member_attributes_operated_info *infos, unsigned int infos_length,
    char **error_user_list, unsigned int error_user_list_length, struct zim_error error_info,
    zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_room_members_attributes_operated_callback(
    zim_handle handle, zim_on_room_members_attributes_operated_callback callback_function);

ZIM_API void ZIM_CALL zim_set_room_members_attributes(
    zim_handle handle, struct zim_room_member_attribute *attributes, unsigned int attributes_length,
    const char **user_ids, unsigned int user_ids_length, const char *room_id,
    struct zim_room_member_attributes_set_config config, zim_sequence *sequence);

typedef void (*zim_on_room_members_attributes_queried_callback)(
    zim_handle handle, const char *room_id, const struct zim_room_member_attributes_info *infos,
    unsigned int infos_length, struct zim_error error_info, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_room_members_attributes_queried_callback(
    zim_handle handle, zim_on_room_members_attributes_queried_callback callback_function);

ZIM_API void ZIM_CALL zim_query_room_members_attributes(zim_handle handle, const char **user_ids,
                                                        unsigned int user_ids_length,
                                                        const char *room_id,
                                                        zim_sequence *sequence);

typedef void (*zim_on_room_member_attributes_list_queried_callback)(
    zim_handle handle, const char *room_id, const struct zim_room_member_attributes_info *infos,
    unsigned int infos_length, const char *next_flag, struct zim_error error_info,
    zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_room_member_attributes_list_queried_callback(
    zim_handle handle, zim_on_room_member_attributes_list_queried_callback callback_function);

ZIM_API void ZIM_CALL zim_query_room_member_attributes_list(
    zim_handle handle, const char *room_id, struct zim_room_member_attributes_query_config config,
    zim_sequence *sequence);

// MARK: - Group

typedef void (*zim_on_group_created_callback)(zim_handle handle,
                                              struct zim_group_full_info group_info,
                                              const struct zim_group_member_info *user_list,
                                              unsigned int user_list_length,
                                              const struct zim_error_user_info *error_user_list,
                                              unsigned int error_user_list_length,
                                              struct zim_error error_info, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_group_created_callback(
    zim_handle handle, zim_on_group_created_callback callback_function);

ZIM_API void ZIM_CALL zim_create_group(zim_handle handle, struct zim_group_info group_info,
                                       const char **user_ids, unsigned int user_ids_length,
                                       struct zim_group_advanced_config *config,
                                       zim_sequence *sequence);

typedef void (*zim_on_group_dismissed_callback)(zim_handle handle, const char *group_id,
                                                struct zim_error error_info, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_group_dismissed_callback(
    zim_handle handle, zim_on_group_dismissed_callback callback_function);

ZIM_API void ZIM_CALL zim_dismiss_group(zim_handle handle, const char *group_id,
                                        zim_sequence *sequence);

typedef void (*zim_on_group_joined_callback)(zim_handle handle,
                                             struct zim_group_full_info room_info,
                                             struct zim_error error_info, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_group_joined_callback(
    zim_handle handle, zim_on_group_joined_callback callback_function);

ZIM_API void ZIM_CALL zim_join_group(zim_handle handle, const char *group_id,
                                     zim_sequence *sequence);

typedef void (*zim_on_group_left_callback)(zim_handle handle, const char *group_id,
                                           struct zim_error error_info, zim_sequence sequence);

ZIM_API void ZIM_CALL
zim_register_group_left_callback(zim_handle handle, zim_on_group_left_callback callback_function);

ZIM_API void ZIM_CALL zim_leave_group(zim_handle handle, const char *group_id,
                                      zim_sequence *sequence);

typedef void (*zim_on_group_users_invited_callback)(
    zim_handle handle, const char *group_id, const struct zim_group_member_info *user_list,
    unsigned int user_list_length, const struct zim_error_user_info *error_user_list,
    unsigned int error_user_list_length, struct zim_error error_info, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_group_users_invited_callback(
    zim_handle handle, zim_on_group_users_invited_callback callback_function);

ZIM_API void ZIM_CALL zim_invite_users_into_group(zim_handle handle, const char **user_ids,
                                                  unsigned int user_ids_length,
                                                  const char *group_id, zim_sequence *sequence);

typedef void (*zim_on_group_member_kicked_callback)(
    zim_handle handle, const char *group_id, const char **kicked_user_id_list,
    unsigned int kicked_user_id_list_length, const struct zim_error_user_info *error_user_list,
    unsigned int error_user_list_length, struct zim_error error_info, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_group_member_kicked_callback(
    zim_handle handle, zim_on_group_member_kicked_callback callback_function);

ZIM_API void ZIM_CALL zim_kick_group_members(zim_handle handle, const char **user_ids,
                                             unsigned int user_ids_length, const char *group_id,
                                             zim_sequence *sequence);

typedef void (*zim_on_group_owner_transferred_callback)(zim_handle handle, const char *group_id,
                                                        const char *to_user_id,
                                                        struct zim_error error_info,
                                                        zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_group_owner_transferred_callback(
    zim_handle handle, zim_on_group_owner_transferred_callback callback_function);

ZIM_API void ZIM_CALL zim_transfer_group_owner(zim_handle handle, const char *to_user_id,
                                               const char *group_id, zim_sequence *sequence);

typedef void (*zim_on_group_name_updated_callback)(zim_handle handle, const char *group_id,
                                                   const char *group_name,
                                                   struct zim_error error_info,
                                                   zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_group_name_updated_callback(
    zim_handle handle, zim_on_group_name_updated_callback callback_function);

ZIM_API void ZIM_CALL zim_update_group_name(zim_handle handle, const char *group_name,
                                            const char *group_id, zim_sequence *sequence);

typedef void (*zim_on_group_avatar_url_updated_callback)(zim_handle handle, const char *group_id,
                                                         const char *group_avatar_url,
                                                         struct zim_error error_info,
                                                         zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_group_avatar_url_updated_callback(
    zim_handle handle, zim_on_group_avatar_url_updated_callback callback_function);

ZIM_API void ZIM_CALL zim_update_group_avatar_url(zim_handle handle, const char *group_avatar_url,
                                                  const char *group_id, zim_sequence *sequence);

typedef void (*zim_on_group_notice_updated_callback)(zim_handle handle, const char *group_id,
                                                     const char *group_notice,
                                                     struct zim_error error_info,
                                                     zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_group_notice_updated_callback(
    zim_handle handle, zim_on_group_notice_updated_callback callback_function);

ZIM_API void ZIM_CALL zim_update_group_notice(zim_handle handle, const char *group_notice,
                                              const char *group_id, zim_sequence *sequence);

typedef void (*zim_on_group_info_queried_callback)(zim_handle handle,
                                                   struct zim_group_full_info group_info,
                                                   struct zim_error error_info,
                                                   zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_group_info_queried_callback(
    zim_handle handle, zim_on_group_info_queried_callback callback_function);

ZIM_API void ZIM_CALL zim_query_group_info(zim_handle handle, const char *group_id,
                                           zim_sequence *sequence);

typedef void (*zim_on_group_attributes_operated_callback)(zim_handle handle, const char *group_id,
                                                          const char **error_keys,
                                                          unsigned int error_keys_length,
                                                          struct zim_error error_info,
                                                          zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_group_attributes_operated_callback(
    zim_handle handle, zim_on_group_attributes_operated_callback callback_function);

ZIM_API void ZIM_CALL zim_set_group_attributes(zim_handle handle,
                                               struct zim_group_attribute *group_attributes,
                                               unsigned int group_attributes_length,
                                               const char *group_id, zim_sequence *sequence);

ZIM_API void ZIM_CALL zim_delete_group_attributes(zim_handle handle, const char **keys,
                                                  unsigned int keys_length, const char *group_id,
                                                  zim_sequence *sequence);

typedef void (*zim_on_group_attributes_queried_callback)(
    zim_handle handle, const char *group_id, struct zim_group_attribute *group_attributes,
    unsigned int group_attributes_length, struct zim_error error_info, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_group_attributes_queried_callback(
    zim_handle handle, zim_on_group_attributes_queried_callback callback_function);

ZIM_API void ZIM_CALL zim_query_group_attributes(zim_handle handle, const char **keys,
                                                 unsigned int keys_length, const char *group_id,
                                                 zim_sequence *sequence);

ZIM_API void ZIM_CALL zim_query_group_all_attributes(zim_handle handle, const char *group_id,
                                                     zim_sequence *sequence);

typedef void (*zim_on_group_member_nickname_updated_callback)(
    zim_handle handle, const char *group_id, const char *for_user_id, const char *nickname,
    struct zim_error error_info, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_group_member_nickname_updated_callback(
    zim_handle handle, zim_on_group_member_nickname_updated_callback callback_function);

ZIM_API void ZIM_CALL zim_set_group_member_nickname(zim_handle handle,
                                                    const char *group_member_nickname,
                                                    const char *for_user_id, const char *group_id,
                                                    zim_sequence *sequence);

typedef void (*zim_on_group_member_role_updated_callback)(zim_handle handle, const char *group_id,
                                                          const char *for_user_id, int role,
                                                          struct zim_error error_info,
                                                          zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_group_member_role_updated_callback(
    zim_handle handle, zim_on_group_member_role_updated_callback callback_function);

ZIM_API void ZIM_CALL zim_set_group_member_role(zim_handle handle, int role,
                                                const char *for_user_id, const char *group_id,
                                                zim_sequence *sequence);

typedef void (*zim_on_group_muted_callback)(zim_handle handle, const char *group_id, bool is_mute,
                                            enum zim_group_mute_mode mode,
                                            long long expired_timestamp, int *role_list,
                                            unsigned int role_list_length,
                                            struct zim_error error_info, zim_sequence sequence);

ZIM_API void ZIM_CALL
zim_register_group_muted_callback(zim_handle handle, zim_on_group_muted_callback callback_function);

ZIM_API void ZIM_CALL zim_mute_group(zim_handle handle, bool is_mute, const char *group_id,
                                     struct zim_group_mute_config config, zim_sequence *sequence);
//
typedef void (*zim_on_group_member_list_muted_callback)(
    zim_handle handle, const char *group_id, bool is_mute, int duration, const char **user_id_list,
    unsigned int user_id_list_length, struct zim_error_user_info *error_user_list,
    unsigned int error_user_list_length, struct zim_error error_info, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_group_member_list_muted_callback(
    zim_handle handle, zim_on_group_member_list_muted_callback callback_function);

ZIM_API void ZIM_CALL zim_mute_group_members(zim_handle handle, bool is_mute, const char *group_id,
                                             const char **user_id_list,
                                             unsigned int user_id_list_length,
                                             struct zim_group_member_list_mute_config config,
                                             zim_sequence *sequence);
//
typedef void (*zim_on_group_muted_member_list_queried_callback)(
    zim_handle handle, const char *group_id, unsigned long long next_flag,
    struct zim_group_member_info *info_list, unsigned int info_list_length,
    struct zim_error error_info, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_group_muted_member_list_queried_callback(
    zim_handle handle, zim_on_group_muted_member_list_queried_callback callback_function);

ZIM_API void ZIM_CALL zim_query_group_muted_member_list(
    zim_handle handle, const char *group_id, struct zim_group_muted_members_query_config config,
    zim_sequence *sequence);

typedef void (*zim_on_group_member_info_queried_callback)(zim_handle handle, const char *group_id,
                                                          struct zim_group_member_info user_info,
                                                          struct zim_error error_info,
                                                          zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_group_member_info_queried_callback(
    zim_handle handle, zim_on_group_member_info_queried_callback callback_function);

ZIM_API void ZIM_CALL zim_query_group_member_info(zim_handle handle, const char *user_id,
                                                  const char *group_id, zim_sequence *sequence);

typedef void (*zim_on_group_list_queried_callback)(zim_handle handle,
                                                   const struct zim_group *group_list,
                                                   unsigned int group_list_length,
                                                   struct zim_error error_info,
                                                   zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_group_list_queried_callback(
    zim_handle handle, zim_on_group_list_queried_callback callback_function);

ZIM_API void ZIM_CALL zim_query_group_list(zim_handle handle, zim_sequence *sequence);

typedef void (*zim_on_group_member_list_queried_callback)(
    zim_handle handle, const char *group_id, const struct zim_group_member_info *user_list,
    unsigned int user_list_length, unsigned int next_flag, struct zim_error error_info,
    zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_group_member_list_queried_callback(
    zim_handle handle, zim_on_group_member_list_queried_callback callback_function);

ZIM_API void ZIM_CALL zim_query_group_member_list(zim_handle handle, const char *group_id,
                                                  struct zim_group_member_query_config config,
                                                  zim_sequence *sequence);

typedef void (*zim_on_group_member_count_queried_callback)(zim_handle handle, const char *group_id,
                                                           unsigned int count,
                                                           struct zim_error error_info,
                                                           zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_group_member_count_queried_callback(
    zim_handle handle, zim_on_group_member_count_queried_callback callback_function);

ZIM_API void ZIM_CALL zim_query_group_member_count(zim_handle handle, const char *group_id,
                                                   zim_sequence *sequence);

typedef void (*zim_on_group_message_receipt_member_list_queried_callback)(
    zim_handle handle, const char *group_id, const struct zim_group_member_info *user_list,
    unsigned int user_list_length, unsigned int next_flag, struct zim_error error_info,
    zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_group_message_receipt_member_list_queried_callback(
    zim_handle handle, zim_on_group_message_receipt_member_list_queried_callback callback_function);

ZIM_API void ZIM_CALL zim_query_group_message_receipt_read_member_list(
    zim_handle handle, struct zim_message message, const char *group_id,
    struct zim_group_message_receipt_member_query_config config, zim_sequence *sequence);

ZIM_API void ZIM_CALL zim_query_group_message_receipt_unread_member_list(
    zim_handle handle, struct zim_message message, const char *group_id,
    struct zim_group_message_receipt_member_query_config config, zim_sequence *sequence);

typedef void (*zim_on_groups_searched_callback)(
    zim_handle handle, const struct zim_group_search_info *group_search_info_list,
    unsigned int group_search_info_list_length, unsigned int next_flag, struct zim_error error_info,
    zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_groups_searched_callback(
    zim_handle handle, zim_on_groups_searched_callback callback_function);

/// Search local groups.
///
/// Business scenario: When you need to search for local groups by keywords or other criteria, you can call this interface to search.
///
/// @param handle The handle of the instance.
/// @param config Configuration for searching groups.
/// @param sequence Context that identifies which invocation triggered this callback.
ZIM_API void ZIM_CALL zim_search_local_groups(zim_handle handle,
                                              struct zim_group_search_config config,
                                              zim_sequence *sequence);

typedef void (*zim_on_group_members_searched_callback)(
    zim_handle handle, const char *group_id, const struct zim_group_member_info *user_list,
    unsigned int user_list_length, unsigned int next_flag, struct zim_error error_info,
    zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_group_members_searched_callback(
    zim_handle handle, zim_on_group_members_searched_callback callback_function);

/// Search local group members.
///
/// Business scenario: When you need to search for local group members by keywords or other criteria, you can call this interface to search.
///
/// @param handle The handle of the instance.
/// @param group_id Group ID of the joined group.
/// @param config The configuration for searching group members.
/// @param sequence Context that identifies which invocation triggered this callback.
ZIM_API void ZIM_CALL zim_search_local_group_members(zim_handle handle, const char *group_id,
                                                     struct zim_group_member_search_config config,
                                                     zim_sequence *sequence);

// MARK: - Callinvite

typedef void (*zim_on_call_invitation_sent_callback)(zim_handle handle, const char *call_id,
                                                     struct zim_call_invitation_sent_info info,
                                                     struct zim_error error_info,
                                                     zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_call_invitation_sent_callback(
    zim_handle handle, zim_on_call_invitation_sent_callback callback_function);

///
/// @param handle The handle of the instance.
/// @param invitees The user ID of the historical message to be queried.
/// @param invitees_count Configuration of query message operation.
/// @param config Configuration of call invite operation..
/// @param sequence [in/out] Context that identifies this invocation when callback.
ZIM_API void ZIM_CALL zim_call_invite(zim_handle handle, const char **invitees,
                                      unsigned int invitees_length,
                                      struct zim_call_invite_config config, zim_sequence *sequence);

typedef void (*zim_on_calling_invitation_sent_callback)(
    zim_handle handle, const char *call_id, struct zim_calling_invitation_sent_info info,
    struct zim_error error_info, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_calling_invitation_sent_callback(
    zim_handle handle, zim_on_calling_invitation_sent_callback callback_function);

ZIM_API void ZIM_CALL zim_calling_invite(zim_handle handle, const char *call_id,
                                         const char **invitees, unsigned int invitees_count,
                                         struct zim_calling_invite_config config,
                                         zim_sequence *sequence);

typedef void (*zim_on_call_join_sent_callback)(zim_handle handle, const char *call_id,
                                               struct zim_call_join_sent_info info,
                                               struct zim_error error_info, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_call_join_sent_callback(
    zim_handle handle, zim_on_call_join_sent_callback callback_function);

ZIM_API void ZIM_CALL zim_call_join(zim_handle handle, const char *call_id,
                                    struct zim_call_join_config config, zim_sequence *sequence);

typedef void (*zim_on_call_cancel_sent_callback)(zim_handle handle, const char *call_id,
                                                 const char **error_invitees,
                                                 unsigned int error_invitees_length,
                                                 struct zim_error error_info,
                                                 zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_call_cancel_sent_callback(
    zim_handle handle, zim_on_call_cancel_sent_callback callback_function);

///
/// @param handle The handle of the instance.
/// @param call_id The handle of the instance.
/// @param invitees The user ID of the historical message to be queried.
/// @param invitees_count Configuration of query message operation.
/// @param config Configuration of call invite operation..
/// @param sequence [in/out] Context that identifies this invocation when callback.
ZIM_API void ZIM_CALL zim_call_cancel(zim_handle handle, const char *call_id, const char **invitees,
                                      unsigned int invitees_length,
                                      struct zim_call_cancel_config config, zim_sequence *sequence);

typedef void (*zim_on_call_acceptance_sent_callback)(zim_handle handle, const char *call_id,
                                                     struct zim_error error_info,
                                                     zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_call_acceptance_sent_callback(
    zim_handle handle, zim_on_call_acceptance_sent_callback callback_function);

///
/// @param handle The handle of the instance.
/// @param call_id The handle of the instance.
/// @param config Configuration of call invite operation..
/// @param sequence [in/out] Context that identifies this invocation when callback.
ZIM_API void ZIM_CALL zim_call_accept(zim_handle handle, const char *call_id,
                                      struct zim_call_accept_config config, zim_sequence *sequence);

typedef void (*zim_on_call_rejection_sent_callback)(zim_handle handle, const char *call_id,
                                                    struct zim_error error_info,
                                                    zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_call_rejection_sent_callback(
    zim_handle handle, zim_on_call_rejection_sent_callback callback_function);

///
/// @param handle The handle of the instance.
/// @param call_id The handle of the instance.
/// @param config Configuration of call invite operation..
/// @param sequence [in/out] Context that identifies this invocation when callback.
ZIM_API void ZIM_CALL zim_call_reject(zim_handle handle, const char *call_id,
                                      struct zim_call_reject_config config, zim_sequence *sequence);

typedef void (*zim_on_call_quit_sent_callback)(zim_handle handle, const char *call_id,
                                               struct zim_call_quit_sent_info info,
                                               struct zim_error error_info, zim_sequence sequence);
ZIM_API void ZIM_CALL zim_register_call_quit_sent_callback(
    zim_handle handle, zim_on_call_quit_sent_callback callback_function);
ZIM_API void ZIM_CALL zim_call_quit(zim_handle handle, const char *call_id,
                                    struct zim_call_quit_config config, zim_sequence *sequence);

typedef void (*zim_on_call_end_sent_callback)(zim_handle handle, const char *call_id,
                                              struct zim_call_end_sent_info info,
                                              struct zim_error error_info, zim_sequence sequence);
ZIM_API void ZIM_CALL zim_register_call_end_sent_callback(
    zim_handle handle, zim_on_call_end_sent_callback callback_function);

ZIM_API void ZIM_CALL zim_call_end(zim_handle handle, const char *call_id,
                                   struct zim_call_end_config config, zim_sequence *sequence);

typedef void (*zim_on_call_list_queried_callback)(zim_handle handle,
                                                  struct zim_call_info *call_list,
                                                  unsigned int call_list_length,
                                                  long long next_flag, struct zim_error error_info,
                                                  zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_call_list_queried_callback(
    zim_handle handle, zim_on_call_list_queried_callback callback_function);

ZIM_API void ZIM_CALL zim_query_call_list(zim_handle handle,
                                          struct zim_query_call_list_config config,
                                          zim_sequence *sequence);

// MARK: - Notification Event

// MARK: - Main Event

/// The callback for error information.
///
/// When an exception occurs in the SDK, the callback will prompt detailed information.
///
/// @param handle ZIM instance.
/// @param error_code Error code, please refer to the error codes document.
/// @param desc A detailed description of the error.
typedef void (*zim_on_error_event)(zim_handle handle, struct zim_error error_info);

ZIM_API void ZIM_CALL zim_register_error_event(zim_handle handle,
                                               zim_on_error_event event_function);

/// The event callback when the connection state changes.
///
/// @param handle ZIM instance.
/// @param state The current connection state after changed.
/// @param event The event that caused the connection state to change.
/// @param extended_data Extra information when the event occurs, a standard JSON string.
typedef void (*zim_on_connection_state_changed_event)(zim_handle handle,
                                                      enum zim_connection_state state,
                                                      enum zim_connection_event event,
                                                      const char *extended_data);

ZIM_API void ZIM_CALL zim_register_connection_state_changed_event(
    zim_handle handle, zim_on_connection_state_changed_event event_function);

/// A reminder callback that the token is about to expire.
///
/// When receiving this callback, you should invoke the [renewToken] function to update the token in time.
///
/// @param handle ZIM instance.
/// @param second The remaining second before the token expires.
typedef void (*zim_on_token_will_expire_event)(zim_handle handle, unsigned int second);

ZIM_API void ZIM_CALL zim_register_token_will_expire_event(
    zim_handle handle, zim_on_token_will_expire_event event_function);

typedef void (*zim_on_user_info_updated_event)(zim_handle handle, struct zim_user_full_info info);

ZIM_API void ZIM_CALL zim_register_user_info_updated_event(
    zim_handle handle, zim_on_user_info_updated_event event_function);

// MARK: - Conversation Event

typedef void (*zim_on_conversation_changed_event)(zim_handle handle,
                                                  const struct zim_conversation_change_info *infos,
                                                  unsigned int infos_length);

ZIM_API void ZIM_CALL zim_register_conversation_changed_event(
    zim_handle handle, zim_on_conversation_changed_event event_function);

typedef void (*zim_on_conversation_total_unread_message_count_updated_event)(
    zim_handle handle, unsigned int totol_unread_message_count);

ZIM_API void ZIM_CALL zim_register_conversation_total_unread_message_count_updated_event(
    zim_handle handle,
    zim_on_conversation_total_unread_message_count_updated_event callback_function);

typedef void (*zim_on_conversations_all_deleted_event)(
    zim_handle handle, struct zim_conversations_all_deleted_info info);

ZIM_API void ZIM_CALL zim_register_conversations_all_deleted_event(
    zim_handle handle, zim_on_conversations_all_deleted_event callback_function);

// TODO: zim_on_conversation_message_receipt_changed_event

// MARK: - Message Event

/// The callback for receiving peer-to-peer message.
///
/// When receiving peer-to-peer message from other user, you will receive this callback.
///
/// @param handle ZIM instance.
/// @param message_list List of received messages.
/// @param message_list_length The length of the message list.
/// @param from_user_id The user ID of the message sender.
typedef void (*zim_on_receive_peer_message_event)(zim_handle handle,
                                                  const struct zim_message *message_list,
                                                  unsigned int message_list_length,
                                                  const char *from_user_id);

ZIM_API void ZIM_CALL zim_register_receive_peer_message_event(
    zim_handle handle, zim_on_receive_peer_message_event event_function);

/// The callback for receiving room message.
///
/// This callback will be triggered when new message is received in a room.
///
/// @param handle ZIM instance.
/// @param message_list List of received messages.
/// @param message_list_length The length of the message list.
/// @param from_room_id ID of the room where the message was received
typedef void (*zim_on_receive_room_message_event)(zim_handle handle,
                                                  const struct zim_message *message_list,
                                                  unsigned int message_list_length,
                                                  const char *from_room_id);

ZIM_API void ZIM_CALL zim_register_receive_room_message_event(
    zim_handle handle, zim_on_receive_room_message_event event_function);

typedef void (*zim_on_receive_group_message_event)(zim_handle handle,
                                                   const struct zim_message *message_list,
                                                   unsigned int message_list_length,
                                                   const char *from_group_id);

ZIM_API void ZIM_CALL zim_register_receive_group_message_event(
    zim_handle handle, zim_on_receive_group_message_event event_function);

/// The callback for send status has changed
///
/// @param handle ZIM instance.
/// @param message_list List of messages sent status changed info .
/// @param message_list_length The length of the changed info .
typedef void (*zim_on_message_sent_status_changed_event)(
    zim_handle handle, const struct zim_message_sent_status_change_info *infos,
    unsigned int infos_length);

ZIM_API void ZIM_CALL zim_register_message_sent_status_changed_event(
    zim_handle handle, zim_on_message_sent_status_changed_event event_function);

typedef void (*zim_on_message_receipt_changed_event)(zim_handle handle, bool is_conv_read,
                                                     const struct zim_message_receipt_info *infos,
                                                     unsigned int infos_length);

ZIM_API void ZIM_CALL zim_register_message_receipt_changed_event(
    zim_handle handle, zim_on_message_receipt_changed_event event_function);

typedef void (*zim_on_message_revoke_received_event)(zim_handle handle,
                                                     const struct zim_message *message_list,
                                                     unsigned int message_list_length);

ZIM_API void ZIM_CALL zim_register_message_revoke_received_event(
    zim_handle handle, zim_on_message_revoke_received_event event_function);

typedef void (*zim_on_broadcast_message_received_event)(zim_handle handle,
                                                        struct zim_message message);

ZIM_API void ZIM_CALL zim_register_broadcast_message_received_event(
    zim_handle handle, zim_on_broadcast_message_received_event event_function);

typedef void (*zim_on_message_deleted_event)(zim_handle handle, const char *conversation_id,
                                             enum zim_conversation_type conversation_type,
                                             bool is_all_message,
                                             const struct zim_message *message_list,
                                             unsigned int message_list_length,
                                             enum zim_message_delete_type message_delete_type);

ZIM_API void ZIM_CALL
zim_register_message_deleted_event(zim_handle handle, zim_on_message_deleted_event event_function);

// MARK: - Room Event

typedef void (*zim_on_room_state_changed_event)(zim_handle handle, enum zim_room_state state,
                                                enum zim_room_event event,
                                                const char *extended_data, const char *room_id);

ZIM_API void ZIM_CALL zim_register_room_state_changed_event(
    zim_handle handle, zim_on_room_state_changed_event event_function);

/// Callback when other members join the room.
///
/// After joining a room, this callback will be triggered when other members also join this room.
///
/// @param handle ZIM instance.
/// @param member_list List of members who joined the room.
/// @param member_list_length The length of the member list.
/// @param room_id The ID of the room where this event occurred.
typedef void (*zim_on_room_member_joined_event)(zim_handle handle,
                                                struct zim_user_info *member_list,
                                                unsigned int member_list_length,
                                                const char *room_id);

ZIM_API void ZIM_CALL zim_register_room_member_joined_event(
    zim_handle handle, zim_on_room_member_joined_event event_function);

/// Callback when other members leave the room.
///
/// After joining a room, this callback will be triggered when other members leave this room.
///
/// @param handle ZIM instance.
/// @param member_list List of members who left the room.
/// @param member_list_length The length of the member list.
/// @param room_id The ID of the room where this event occurred.
typedef void (*zim_on_room_member_left_event)(zim_handle handle, struct zim_user_info *member_list,
                                              unsigned int member_list_length, const char *room_id);

ZIM_API void ZIM_CALL zim_register_room_member_left_event(
    zim_handle handle, zim_on_room_member_left_event event_function);

typedef void (*zim_on_room_attributes_updated_event)(zim_handle handle, const char *room_id,
                                                     struct zim_room_attributes_update_info info,
                                                     const char *extended_data);

ZIM_API void ZIM_CALL zim_register_room_attributes_updated_event(
    zim_handle handle, zim_on_room_attributes_updated_event event_function);

typedef void (*zim_on_room_attributes_batch_updated_event)(
    zim_handle handle, const char *room_id, struct zim_room_attributes_update_info *infos,
    unsigned int infos_length, const char *extended_data);

ZIM_API void ZIM_CALL zim_register_room_attributes_batch_updated_event(
    zim_handle handle, zim_on_room_attributes_batch_updated_event event_function);

typedef void (*zim_on_room_member_attributes_updated_event)(
    zim_handle handle, struct zim_room_member_attributes_update_info *infos,
    unsigned int infos_length, struct zim_room_operated_info operated_info, const char *room_id);

ZIM_API void ZIM_CALL zim_register_room_member_attributes_updated_event(
    zim_handle handle, zim_on_room_member_attributes_updated_event event_function);

// MARK: - Friend Manager Event

typedef void (*zim_on_friend_list_changed_event)(zim_handle handle,
                                                 const struct zim_friend_info *friend_info_list,
                                                 unsigned int friend_info_list_length,
                                                 unsigned int action);

ZIM_API void ZIM_CALL zim_register_on_friend_list_changed_event(
    zim_handle handle, zim_on_friend_list_changed_event callback);

typedef void (*zim_on_friend_info_updated_event)(zim_handle handle,
                                                 const struct zim_friend_info *friend_info_list,
                                                 unsigned int friend_info_list_length);
ZIM_API void ZIM_CALL zim_register_on_friend_info_updated_event(
    zim_handle handle, zim_on_friend_info_updated_event callback);

typedef void (*zim_on_friend_application_updated_event)(
    zim_handle handle, const struct zim_friend_application_info *friend_application_info_list,
    unsigned int friend_info_list_length);
ZIM_API void ZIM_CALL zim_register_on_friend_application_updated_event(
    zim_handle handle, zim_on_friend_application_updated_event callback);

typedef void (*zim_on_friend_application_changed_event)(
    zim_handle handle, enum zim_friend_application_change_action action,
    const struct zim_friend_application_info *friend_application_info_list,
    unsigned int friend_info_list_length);
ZIM_API void ZIM_CALL zim_register_on_friend_application_changed_event(
    zim_handle handle, zim_on_friend_application_changed_event callback);

typedef void (*zim_on_blacklist_changed_event)(zim_handle handle,
                                               const struct zim_user_info *user_infos,
                                               enum zim_blacklist_change_action action,
                                               unsigned int user_info_list_length);
ZIM_API void ZIM_CALL
zim_register_on_blacklist_changed_event(zim_handle handle, zim_on_blacklist_changed_event callback);

// MARK: - Group Event

typedef void (*zim_on_group_state_changed_event)(zim_handle handle, enum zim_group_state state,
                                                 enum zim_group_event event,
                                                 struct zim_group_operated_info operated_info,
                                                 struct zim_group_full_info group_info);

ZIM_API void ZIM_CALL zim_register_group_state_changed_event(
    zim_handle handle, zim_on_group_state_changed_event event_function);

typedef void (*zim_on_group_member_state_changed_event)(
    zim_handle handle, enum zim_group_member_state state, enum zim_group_member_event event,
    const struct zim_group_member_info *user_list, unsigned int user_list_length,
    struct zim_group_operated_info operated_info, const char *group_id);

ZIM_API void ZIM_CALL zim_register_group_member_state_changed_event(
    zim_handle handle, zim_on_group_member_state_changed_event event_function);

typedef void (*zim_on_group_name_updated_event)(zim_handle handle, const char *group_name,
                                                struct zim_group_operated_info operated_info,
                                                const char *group_id);

ZIM_API void ZIM_CALL zim_register_group_name_updated_event(
    zim_handle handle, zim_on_group_name_updated_event event_function);

typedef void (*zim_on_group_notice_updated_event)(zim_handle handle, const char *group_notice,
                                                  struct zim_group_operated_info operated_info,
                                                  const char *group_id);

ZIM_API void ZIM_CALL zim_register_group_notice_updated_event(
    zim_handle handle, zim_on_group_notice_updated_event event_function);

typedef void (*zim_on_group_avatar_url_updated_event)(zim_handle handle,
                                                      const char *group_avatar_url,
                                                      struct zim_group_operated_info operated_info,
                                                      const char *group_id);

ZIM_API void ZIM_CALL zim_register_group_avatar_url_updated_event(
    zim_handle handle, zim_on_group_avatar_url_updated_event event_function);

typedef void (*zim_on_group_mute_info_updated_event)(zim_handle handle,
                                                     const struct zim_group_mute_info mute_info,
                                                     struct zim_group_operated_info operated_info,
                                                     const char *group_id);

ZIM_API void ZIM_CALL zim_register_group_mute_info_updated_event(
    zim_handle handle, zim_on_group_mute_info_updated_event event_function);

typedef void (*zim_on_group_attributes_updated_event)(
    zim_handle handle, const struct zim_group_attributes_update_info *infos,
    unsigned int infos_length, struct zim_group_operated_info operated_info, const char *group_id);

ZIM_API void ZIM_CALL zim_register_group_attributes_updated_event(
    zim_handle handle, zim_on_group_attributes_updated_event callback_function);

typedef void (*zim_on_group_member_info_updated_event)(
    zim_handle handle, const struct zim_group_member_info *user_infos,
    unsigned int user_infos_length, struct zim_group_operated_info operated_info,
    const char *group_id);

ZIM_API void ZIM_CALL zim_register_group_member_info_updated_event(
    zim_handle handle, zim_on_group_member_info_updated_event event_function);

// MARK: - Call Event

typedef void (*zim_on_call_invitation_received_event)(zim_handle handle,
                                                      struct zim_call_invitation_received_info info,
                                                      const char *call_id);

ZIM_API void ZIM_CALL zim_register_call_invitation_received_event(
    zim_handle handle, zim_on_call_invitation_received_event event_function);

typedef void (*zim_on_call_invitation_created_event)(zim_handle handle,
                                                     struct zim_call_invitation_created_info info,
                                                     const char *call_id);

ZIM_API void ZIM_CALL zim_register_call_invitation_created_event(
    zim_handle handle, zim_on_call_invitation_created_event event_function);

typedef void (*zim_on_call_invitation_cancelled_event)(
    zim_handle handle, struct zim_call_invitation_cancelled_info info, const char *call_id);

ZIM_API void ZIM_CALL zim_register_call_invitation_cancelled_event(
    zim_handle handle, zim_on_call_invitation_cancelled_event event_function);

typedef void (*zim_on_call_invitation_accepted_event)(zim_handle handle,
                                                      struct zim_call_invitation_accepted_info info,
                                                      const char *call_id);

ZIM_API void ZIM_CALL zim_register_call_invitation_accepted_event(
    zim_handle handle, zim_on_call_invitation_accepted_event callback_function);

typedef void (*zim_on_call_invitation_rejected_event)(zim_handle handle,
                                                      struct zim_call_invitation_rejected_info info,
                                                      const char *call_id);

ZIM_API void ZIM_CALL zim_register_call_invitation_rejected_event(
    zim_handle handle, zim_on_call_invitation_rejected_event event_function);

typedef void (*zim_on_call_invitation_timeout_event)(zim_handle handle,
                                                     struct zim_call_invitation_timeout_info,
                                                     const char *call_id);

ZIM_API void ZIM_CALL zim_register_call_invitation_timeout_event(
    zim_handle handle, zim_on_call_invitation_timeout_event callback_function);

typedef void (*zim_on_call_invitees_answered_timeout_event)(zim_handle handle,
                                                            const char **invitees,
                                                            unsigned int invitees_length,
                                                            const char *call_id);

ZIM_API void ZIM_CALL zim_register_call_invitees_answered_timeout_event(
    zim_handle handle, zim_on_call_invitees_answered_timeout_event event_function);

typedef void (*zim_on_call_invitation_ended_event)(zim_handle handle,
                                                   struct zim_call_invitation_ended_info,
                                                   const char *call_id);

ZIM_API void ZIM_CALL zim_register_call_invitation_ended_event(
    zim_handle handle, zim_on_call_invitation_ended_event callback_function);

typedef void (*zim_on_call_state_changed_event)(zim_handle handle,
                                                struct zim_event_call_state_changed_info);

ZIM_API void ZIM_CALL zim_register_call_state_changed_event(
    zim_handle handle, zim_on_call_state_changed_event event_function);

typedef void (*zim_on_call_user_state_changed_event)(zim_handle handle,
                                                     struct zim_event_call_user_state_changed_info);

ZIM_API void ZIM_CALL zim_register_call_user_state_changed_event(
    zim_handle handle, zim_on_call_user_state_changed_event event_function);

typedef void (*zim_on_friend_added_callback)(zim_handle handle,
                                             const struct zim_friend_info friend_info,
                                             struct zim_error error, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_friend_added_callback(zim_handle handle,
                                                         zim_on_friend_added_callback callback);

ZIM_API void ZIM_CALL zim_add_friend(zim_handle handle, const char *user_ids,
                                     struct zim_friend_add_config config, zim_sequence *sequence);

typedef void (*zim_on_friend_application_sent_callback)(
    zim_handle handle, const struct zim_friend_application_info friend_application_info,
    const struct zim_error error, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_friend_application_sent_callback(
    zim_handle handle, zim_on_friend_application_sent_callback callback);

ZIM_API void ZIM_CALL zim_send_friend_application(zim_handle handle, const char *user_id,
                                                  struct zim_friend_application_send_config config,
                                                  zim_sequence *sequence);

typedef void (*zim_on_delete_friend_callback)(zim_handle handle,
                                              const struct zim_error_user_info *error_user_list,
                                              unsigned int error_user_list_length,
                                              const struct zim_error error, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_delete_friends_callback(zim_handle handle,
                                                           zim_on_delete_friend_callback callback);

ZIM_API void ZIM_CALL zim_delete_friends(zim_handle handle, const char **user_ids,
                                         unsigned int user_ids_length,
                                         struct zim_friend_delete_config config,
                                         zim_sequence *sequence);

typedef void (*zim_on_friends_relation_check_callback)(
    zim_handle handle, const struct zim_friend_relation_info *friend_relation_info,
    unsigned int friend_relation_info_length, const struct zim_error_user_info *error_user_list,
    unsigned int error_user_list_length, const struct zim_error error, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_friends_relation_check_callback(
    zim_handle handle, zim_on_friends_relation_check_callback callback);

ZIM_API void ZIM_CALL zim_check_friends_relation(zim_handle handle, const char **user_ids,
                                                 unsigned int user_ids_length,
                                                 struct zim_friend_relation_check_config config,
                                                 zim_sequence *sequence);

typedef void (*zim_on_update_friend_alias_callback)(zim_handle handle,
                                                    const struct zim_friend_info friend_info,
                                                    const struct zim_error error,
                                                    zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_update_friend_alias_callback(
    zim_handle handle, zim_on_update_friend_alias_callback callback);

ZIM_API void ZIM_CALL zim_update_friend_alias(zim_handle handle, const char *friend_alias,
                                              const char *user_id, zim_sequence *sequence);

typedef void (*zim_on_update_friend_attributes_callback)(zim_handle handle,
                                                         const struct zim_friend_info friend_info,
                                                         const struct zim_error error,
                                                         zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_update_friend_attributes_callback(
    zim_handle handle, zim_on_update_friend_attributes_callback callback);

ZIM_API void ZIM_CALL zim_update_friend_attributes(zim_handle handle,
                                                   struct zim_friend_attribute *friend_attributes,
                                                   unsigned int friend_attributes_length,
                                                   const char *user_id, zim_sequence *sequence);

typedef void (*zim_on_friends_info_queried_callback)(
    zim_handle handle, const struct zim_friend_info *friend_info_list,
    unsigned int friend_info_list_length, const struct zim_error_user_info *error_user_list,
    unsigned int error_user_list_length, const struct zim_error error, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_friends_info_queried_callback(
    zim_handle handle, zim_on_friends_info_queried_callback callback);

typedef void (*zim_on_friends_searched_callback)(zim_handle handle,
                                                 const struct zim_friend_info *friend_infos,
                                                 unsigned int friend_info_list_length,
                                                 unsigned int next_flag,
                                                 const struct zim_error error,
                                                 zim_sequence sequence);

ZIM_API void ZIM_CALL zim_search_local_friends(zim_handle handle,
                                               struct zim_friend_search_config config,
                                               zim_sequence *sequence);

ZIM_API void ZIM_CALL zim_register_friends_searched_callback(
    zim_handle handle, zim_on_friends_searched_callback callback);

ZIM_API void ZIM_CALL zim_query_friends_info(zim_handle handle, const char **user_ids,
                                             unsigned int user_ids_length, zim_sequence *sequence);

typedef void (*zim_on_friend_application_accepted_callback)(
    zim_handle handle, const struct zim_friend_info friend_info, const struct zim_error error,
    zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_friend_application_accepted_callback(
    zim_handle handle, zim_on_friend_application_accepted_callback callback);

ZIM_API void ZIM_CALL zim_accept_friend_application(
    zim_handle handle, const char *user_id, struct zim_friend_application_accept_config config,
    zim_sequence *sequence);

typedef void (*zim_on_friend_application_reject_callback)(zim_handle handle,
                                                          const struct zim_user_info friend_info,
                                                          const struct zim_error error,
                                                          zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_friend_application_reject_callback(
    zim_handle handle, zim_on_friend_application_reject_callback callback);

ZIM_API void ZIM_CALL zim_friend_reject_application(
    zim_handle handle, const char *user_id, struct zim_friend_application_reject_config config,
    zim_sequence *sequence);

typedef void (*zim_on_query_friend_list_callback)(zim_handle handle,
                                                  const struct zim_friend_info *friend_info_list,
                                                  unsigned int friend_info_list_length,
                                                  unsigned int next_flag,
                                                  const struct zim_error error,
                                                  zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_query_friend_list_callback(
    zim_handle handle, zim_on_query_friend_list_callback callback);

ZIM_API void ZIM_CALL zim_query_friend_list(zim_handle handle,
                                            struct zim_friend_list_query_config config,
                                            zim_sequence *sequence);

typedef void (*zim_on_query_friend_application_list_callback)(
    zim_handle handle, const struct zim_friend_application_info *friend_application_info_list,
    unsigned int friend_application_info_list_length, unsigned int next_flag,
    const struct zim_error error, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_query_friend_application_list_callback(
    zim_handle handle, zim_on_query_friend_application_list_callback callback);

ZIM_API void ZIM_CALL zim_query_friend_application_list(
    zim_handle handle, struct zim_friend_application_list_query_config config,
    zim_sequence *sequence);

typedef void (*zim_on_blacklist_users_added_callback)(
    zim_handle handle, const struct zim_error_user_info *error_user_info_list,
    unsigned int error_user_info_list_length, const struct zim_error error, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_blacklist_users_added_callback(
    zim_handle handle, zim_on_blacklist_users_added_callback callback);

ZIM_API void ZIM_CALL zim_add_users_to_blacklist(zim_handle handle, const char **user_ids,
                                                 unsigned int user_ids_length,
                                                 zim_sequence *sequence);

typedef void (*zim_on_blacklist_users_remove_callback)(
    zim_handle handle, const struct zim_error_user_info *error_user_info_list,
    unsigned int error_user_info_list_length, const struct zim_error error, zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_blacklist_users_remove_callback(
    zim_handle handle, zim_on_blacklist_users_remove_callback callback);

ZIM_API void ZIM_CALL zim_remove_users_from_blacklist(zim_handle handle, const char **user_ids,
                                                      unsigned int user_ids_length,
                                                      zim_sequence *sequence);

typedef void (*zim_on_blacklist_queried_callback)(zim_handle handle,
                                                  const struct zim_user_info *blacklist_user_infos,
                                                  unsigned int blacklist_user_infos_length,
                                                  unsigned int next_flag,
                                                  const struct zim_error error,
                                                  zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_blacklist_queried_callback(
    zim_handle handle, zim_on_blacklist_queried_callback callback);

ZIM_API void ZIM_CALL zim_query_blacklist(zim_handle handle,
                                          struct zim_blacklist_query_config config,
                                          zim_sequence *sequence);

typedef void (*zim_on_check_user_is_in_blacklist_callback)(zim_handle handle,
                                                           bool is_user_in_blacklist,
                                                           const struct zim_error error,
                                                           zim_sequence sequence);

ZIM_API void ZIM_CALL zim_register_check_user_is_in_blacklist_callback(
    zim_handle handle, zim_on_check_user_is_in_blacklist_callback callback);

ZIM_API void ZIM_CALL zim_check_user_is_in_blacklist(zim_handle handle, const char *user_id,
                                                     zim_sequence *sequence);

ZIM_EXTERN_END
#endif //__ZIM_H__
