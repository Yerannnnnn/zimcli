#ifndef __ZIM_ERROR_CODE_H__
#define __ZIM_ERROR_CODE_H__

//
//  zim_error_code.h
//  ZIM
//
//  Copyright © 2021 Zego. All rights reserved.
//

/**
 * The define of error code.
 *
 * Description: Developers can find the details of the error code in the developer documentation website according to the error code.
 *
 * Use cases: It can be used to collect and record errors in the process of using the SDK.
 *
 */
enum zim_error_code {

    //     /**
    //      * Description: Success without exception.
    //      *
    //      * Use cases: Used to indicate that the operation is executed correctly.
    //      */
    //     zim_error_code_success = 0,
    //
    //     /**
    //      * Description: Failed, the guarantee is wrong.
    //      *
    //      * Use cases: Used in the operation guarantee strategy.
    //      */
    //     zim_error_code_failed = 1,
    //
    //     /**
    //      * Description: The incoming parameter is invalid.
    //      *
    //      * Use case: Used to protect the bottom strategy when the interface call fails due to incorrect parameters.
    //      */
    //     zim_error_code_param_invalid = 6000001,
    //
    //     /**
    //      * Description: The SDK is not initialized.
    //      *
    //      * Use case: Used for error return when the ZIM SDK is not initialized successfully.
    //      */
    //     zim_error_code_no_init = 6000002,
    //
    //     /**
    //      * Description: Trigger the background business frequency limit.
    //      *
    //      * Use cases: Used to trigger the error return when the background frequency limit is triggered.
    //      */
    //     zim_error_code_touch_limit = 6000003,
    //
    //     /**
    //      * Description: Invalid AppID.
    //      *
    //      * Use case: Used for error return of failure due to invalid AppID.
    //      */
    //     zim_error_code_invalid_appid = 6000004,
    //
    //     /**
    //      * Description: Failed to upload logs.
    //      *
    //      * Use cases: Used to return the error when the user fails to upload the log.
    //      */
    //     zim_error_code_upload_log_failed = 6000010,
    //
    //     /**
    //      * Description: Login failed.
    //      *
    //      * Use cases: Used to protect the bottom line when the login fails.
    //      */
    //     zim_error_code_login_failed = 6000101,
    //
    //     /**
    //      * Description: The login failed due to background reasons.
    //      *
    //      * Use cases: Used for general error reporting of background errors that do not need to be external. The background error code will be marked with code.
    //      */
    //     zim_error_code_login_server_error = 6000102,
    //
    //     /**
    //      * Description: The token is invalid.
    //      *
    //      * Use cases: Used for the bottom-keeping strategy when the token verification fails.
    //      */
    //     zim_error_code_token_invalid = 6000103,
    //
    //     /**
    //      * Description: Network Error.
    //      *
    //      * Use case: Used for error return when the connection status changes due to network reasons, or the execution operation fails due to network errors.
    //      */
    //     zim_error_code_network_error = 6000104,
    //
    //     zim_error_code_server_disconnected = 6000105,
    //
    //     /**
    //      * Description: The token has expired.
    //      *
    //      * Use cases: Used to return an error that the target token has expired when the token verification fails.
    //      */
    //     zim_error_code_token_expired = 6000106,
    //
    //     /**
    //      * Description: The token version number is wrong.
    //      *
    //      * Use case: Used to return an error that the target token version does not match when the token verification fails.
    //      */
    //     zim_error_code_token_version_error = 6000107,
    //
    //     /**
    //      * Description: There is already a user logged in.
    //      *
    //      * Use case: Used in the case of already logged in, the error returned by calling login again.
    //      */
    //     zim_error_code_already_login = 6000111,
    //
    //     /**
    //      * Description: Not logged in.
    //      *
    //      * Use case: Used to call an error return that requires logging in to perform operations on the premise of unsuccessful login.
    //      */
    //     zim_error_code_no_login = 6000121,
    //
    //     /**
    //      * Description: Connection service error
    //      *
    //      * Use case: Used for unified access link error return.
    //      */
    //     zim_error_code_connection_service_error = 6000150,
    //
    //     /**
    //      * Description: Failed to send message.
    //      *
    //      * Use case: Used to protect the bottom policy when sending a message fails.
    //      */
    //     zim_error_code_send_message_failed = 6000201,
    //
    //     /**
    //      * Description: The message sending failed due to background reasons.
    //      *
    //      * Use case: Used for general error reporting of background errors that do not need to be external. The background error code will be marked with code.
    //      */
    //     zim_error_code_message_server_error = 6000202,
    //
    //     /**
    //      * Description: The message failed to be sent due to network problems.
    //      *
    //      * Use case: Used for error return when sending a message failed due to network problems.
    //      */
    //     zim_error_code_message_network_error = 6000204,
    //
    //     /**
    //      * Description: The user is not in the correct room.
    //      *
    //      * Use case: Used to return the error that the user is not in the target room when sending a room message.
    //      */
    //     zim_error_code_send_room_message_failed_for_not_in_the_room = 6000205,
    //
    //     /**
    //      * Use case: This error code is returned when a message is sent to a user id that does not exist
    //      */
    //     zim_error_code_message_user_not_exist = 6000206,
    //
    //     /**
    //      * Description: The room does not exist.
    //      *
    //      * Use case: Used for the bottom-keeping strategy when an error is caused by performing room operations when not entering the room.
    //      */
    //     zim_error_code_room_not_exist = 6000301,
    //
    //     /**
    //      * Description: The room operation failed due to background reasons.
    //      *
    //      * Use case: Used for general error reporting of background errors that do not need to be external. The background error code will be marked with code.
    //      */
    //     zim_error_code_room_server_error = 6000302,
    //
    //     /**
    //      * Description: Failed to create room.
    //      *
    //      * Use case: Used to ensure the bottom-line strategy when the room fails to be created.
    //      */
    //     zim_error_code_create_room_failed = 6000303,
    //
    //     /**
    //      * Description: Failed to join the room.
    //      *
    //      * Use case: Used to ensure the bottom-line strategy when joining the room fails.
    //      */
    //     zim_error_code_join_room_failed = 6000304,
    //
    //     /**
    //      * Description: Failed to leave the room.
    //      *
    //      * Use case: Used to guarantee the bottom-line strategy when leaving the room failed.
    //      */
    //     zim_error_code_leave_room_failed = 6000305,
    //
    //     /**
    //      * Description: Want to create an existing room.
    //      *
    //      * Use case: It is used to return an error that fails to create a room using an existing RoomID.
    //      */
    //     zim_error_code_create_exist_room = 6000311,
    //
    //     /**
    //      * Description: The number of rooms is limited.
    //      *
    //      * Use case: Used to create a room, the maximum number of rooms has been reached and it fails to return an error.
    //      */
    //     zim_error_code_create_room_count_limit = 6000312,
    //
    //     /**
    //      * Description: The number of users entering the room reaches the upper limit.
    //      *
    //      * Use case: Used to return the error when the number of users entering the room reaches the upper limit.
    //      */
    //     zim_error_code_join_room_limit = 6000321,
    //     zim_error_code_conversation_not_exist = 6000601,
    //
    //     zim_error_code_db_util_open_failure = 6000701,
    //     zim_error_code_db_util_write_failure = 6000702,
    //     zim_error_code_db_util_delete_failure = 6000703,
    //     zim_error_code_db_util_query_failure = 6000704,
    zim_error_code_success = 0,
    zim_error_code_failed = 1,

    zim_error_code_common_module_params_invalid = 6000001,
    zim_error_code_common_module_engine_not_init = 6000002,
    zim_error_code_common_module_invalid_appid = 6000003,
    zim_error_code_common_module_trigger_sdk_frequency_limit = 6000004,
    zim_error_code_common_module_trigger_server_frequency_limit = 6000005,
    zim_error_code_common_module_switch_server_error = 6000006,
    zim_error_code_common_module_im_server_error = 6000007,
    zim_error_code_common_module_im_database_error = 6000008,
    zim_error_code_common_module_im_server_disconnect = 6000009,
    zim_error_code_common_module_upload_log_error = 6000010,
    zim_error_code_common_module_user_is_not_exist = 6000011,
    zim_error_code_common_module_user_is_operation_limit = 6000012,
    zim_error_code_common_module_unsupported_request_of_current_menu = 6000013,
    zim_error_code_common_module_exceed_dau_limit = 6000015,
    zim_error_code_common_module_exceed_mau_limit = 6000016,

    zim_error_code_network_module_common_error = 6000101,
    zim_error_code_network_module_server_error = 6000102,
    zim_error_code_network_module_token_invalid = 6000103,
    zim_error_code_network_module_network_error = 6000104,
    zim_error_code_network_module_request_timeout = 6000105,
    zim_error_code_network_module_token_expired = 6000106,
    zim_error_code_network_module_token_version_error = 6000107,
    zim_error_code_network_module_token_time_is_too_short = 6000108,
    zim_error_code_network_module_rsp_proto_parse_error = 6000109,
    zim_error_code_network_module_user_has_already_logged = 6000111,
    zim_error_code_network_module_user_is_not_logged = 6000121,
    zim_error_code_network_module_user_has_already_logged_out = 6000122,
    zim_error_code_network_module_user_offline = 6000123,
    zim_error_code_network_module_user_id_error = 6000124,

    zim_error_code_message_module_common_error = 6000201,
    zim_error_code_message_module_server_error = 6000202,
    zim_error_code_message_module_send_message_failed = 6000203,
    zim_error_code_message_module_target_does_not_exist = 6000204,

    zim_error_code_message_module_file_error = 6000210,
    zim_error_code_message_module_file_not_exist = 6000211,
    zim_error_code_message_module_file_server_error = 6000212,
    zim_error_code_message_module_file_type_unsupported = 6000213,
    zim_error_code_message_module_file_size_invalid = 6000214,
    zim_error_code_message_module_file_duration_invalid = 6000215,
    zim_error_code_message_module_file_permission_denied = 6000216,
    zim_error_code_message_module_file_download_failed = 6000217,
    zim_error_code_message_module_file_download_limit = 6000218,
    zim_error_code_message_module_file_download_url_not_found = 6000219,
    zim_error_code_message_module_file_download_http_request_server_error = 6000220,

    zim_error_code_message_module_audit_rejected = 6000221,
    zim_error_code_message_module_audit_failed = 6000222,
    zim_error_code_message_module_audit_custom_sent_rejected = 6000230,
    zim_error_code_message_module_muted = 6000231,

    zim_error_code_message_module_call_error = 6000270,
    zim_error_code_message_module_cancel_call_error = 6000271,
    zim_error_code_message_module_call_server_error = 6000272,
    zim_error_code_message_module_is_not_invitor = 6000273,
    zim_error_code_message_module_is_not_invitee = 6000274,
    zim_error_code_message_module_call_already_exists = 6000275,
    zim_error_code_message_module_call_does_not_exist = 6000276,
    zim_error_code_message_module_receipt_read_error = 6000277,
    zim_error_code_message_module_message_exceeds_revoke_time = 6000278,
    zim_error_code_message_module_message_has_been_revoked = 6000279,
    zim_error_code_message_module_message_reaction_type_existed = 6000280,
    zim_error_code_message_module_call_invite_user_does_not_exist = 6000281,
    zim_error_code_message_module_message_receipt_limit = 6000282,
    zim_error_code_message_module_user_already_in_the_call = 6000283,
    zim_error_code_message_module_sender_in_blacklist = 6000284,

    zim_error_code_room_module_common_error = 6000301,
    zim_error_code_room_module_server_error = 6000302,
    zim_error_code_room_module_create_room_error = 6000303,
    zim_error_code_room_module_join_room_error = 6000304,
    zim_error_code_room_module_leave_room_error = 6000306,
    zim_error_code_room_module_room_member_query_failed = 6000310,
    zim_error_code_room_module_room_member_query_failed_completely = 6000311,
    zim_error_code_room_module_user_is_already_in_the_room = 6000320,
    zim_error_code_room_module_user_is_not_in_the_room = 6000321,
    zim_error_code_room_module_the_room_does_not_exist = 6000322,
    zim_error_code_room_module_the_room_already_exists = 6000323,
    zim_error_code_room_module_the_number_of_existing_rooms_has_reached_limit = 6000324,
    zim_error_code_room_module_the_number_of_joined_rooms_has_reached_limit = 6000325,
    zim_error_code_room_module_the_room_is_connecting = 6000326,
    zim_error_code_room_module_room_attributes_common_error = 6000330,
    zim_error_code_room_module_room_attributes_operation_failed_completely = 6000331,
    zim_error_code_room_module_room_attributes_operation_failed_partly = 6000332,
    zim_error_code_room_module_room_attributes_query_failed = 6000333,
    zim_error_code_room_module_the_number_of_room_attributes_exceeds_limit = 6000334,
    zim_error_code_room_module_the_length_of_room_attribute_key_exceeds_limit = 6000335,
    zim_error_code_room_module_the_length_of_room_attribute_value_exceeds_limit = 6000336,
    zim_error_code_room_module_the_total_length_of_room_attributes_value_exceeds_limit = 6000337,
    zim_error_code_room_module_room_member_attributes_common_error = 6000350,
    zim_error_code_room_module_room_member_attributes_kv_exceeds_limit = 6000351,
    zim_error_code_room_module_room_member_attributes_key_exceeds_limit = 6000352,
    zim_error_code_room_module_room_member_attributes_value_exceeds_limit = 6000353,
    zim_error_code_room_module_room_member_attributes_set_room_user_attributes_count_exceed_limit =
        6000357,

    zim_error_code_zpns_module_push_id_invalid = 6000401,

    zim_error_code_group_module_common_error = 6000501,
    zim_error_code_group_module_server_error = 6000502,
    zim_error_code_group_module_create_group_error = 6000503,
    zim_error_code_group_module_dismiss_group_error = 6000504,
    zim_error_code_group_module_join_group_error = 6000505,
    zim_error_code_group_module_leave_group_error = 6000506,
    zim_error_code_group_module_kick_out_group_member_error = 6000507,
    zim_error_code_group_module_invite_user_into_group_error = 6000508,
    zim_error_code_group_module_transfer_owner_error = 6000509,
    zim_error_code_group_module_update_group_info_error = 6000510,
    zim_error_code_group_module_query_group_info_error = 6000511,
    zim_error_code_group_module_group_attributes_operation_failed = 6000512,
    zim_error_code_group_module_group_attributes_query_failed = 6000513,
    zim_error_code_group_module_update_group_member_info_error = 6000514,
    zim_error_code_group_module_query_group_member_info_error = 6000515,
    zim_error_code_group_module_query_group_list_error = 6000516,
    zim_error_code_group_module_query_group_member_list_error = 6000517,
    zim_error_code_group_module_user_is_not_in_the_group = 6000521,
    zim_error_code_group_module_member_is_already_in_the_group = 6000522,
    zim_error_code_group_module_group_does_not_exist = 6000523,
    zim_error_code_group_module_group_already_exists = 6000524,
    zim_error_code_group_module_group_member_has_reached_limit = 6000525,
    zim_error_code_group_module_group_attribute_does_not_exist = 6000526,
    zim_error_code_group_module_group_with_dismissed = 6000527,
    zim_error_code_group_module_the_number_of_group_attributes_exceeds_limit = 6000531,
    zim_error_code_group_module_the_length_of_group_attribute_key_exceeds_limit = 6000532,
    zim_error_code_group_module_the_length_of_group_attribute_value_exceeds_limit = 6000533,
    zim_error_code_group_module_the_total_length_of_group_attribute_value_exceeds_limit = 6000534,
    zim_error_code_group_module_no_corresponding_operation_authority = 6000541,
    zim_error_code_group_module_group_database_error = 6000542,

    zim_error_code_conversation_module_common_error = 6000601,
    zim_error_code_conversation_module_server_error = 6000602,
    zim_error_code_conversation_module_conversation_does_not_exist = 6000603,
    zim_error_code_conversation_module_conversation_pinned_list_reached_limit = 6000604,

    zim_error_code_database_module_open_database_error = 6000701,
    zim_error_code_database_module_modify_database_error = 6000702,
    zim_error_code_database_module_delete_database_error = 6000703,
    zim_error_code_database_module_selete_database_error = 6000704,
    zim_error_code_database_module_db_is_not_opened = 6000710,

    // 好友关系链模块错误码
    zim_error_code_friend_module_friends_num_limit = 6000801,
    // 同意或者拒绝好友申请状态错误
    zim_error_code_friend_module_friend_application_status_error = 6000802,
    // 对方已经是好友
    zim_error_code_friend_module_is_already_your_friend = 6000803,
    // 已经在黑名单内
    zim_error_code_friend_module_already_add_to_blacklist = 6000804,
    // 不能添加自己到黑名单
    zim_error_code_friend_module_cannot_add_self_to_blacklist = 6000805,
    // 已经从黑名单中删除
    zim_error_code_friend_module_already_delete_from_blacklist = 6000806,
    // 该用户不在黑名单里
    zim_error_code_friend_module_user_not_in_blacklist = 6000807,
    // 黑名单限制
    zim_error_code_friend_module_blacklist_list_quantity_limit = 6000808,
    // 好友列表超出限制
    zim_error_code_friend_module_friend_operation_limit_exceeded = 6000809,
    // 不允许添加自己
    zim_error_code_friend_module_cannot_add_self_to_friend_list = 6000810,
    // 该好友没注册
    zim_error_code_friend_module_friend_are_unregistered = 6000811,
    // 不是你的好友
    zim_error_code_friend_module_not_your_friend = 6000812,
    // 不允许删除自己
    zim_error_code_friend_module_cannot_delete_self = 6000813,
    // 申请已经过期
    zim_error_code_friend_module_friend_application_expired = 6000814,
    zim_error_code_friend_module_add_blacklist_fail = 6000815,
    zim_error_code_friend_module_del_blacklist_fail = 6000816,
};

#endif //__ZIM_ERROR_CODE_H__
