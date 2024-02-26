#pragma once

//
//  ZIMInternal.h
//  ZIM
//
//  Created by Patrick Fu on 2021/7/15.
//  Copyright © 2021 Zego. All rights reserved.
//

namespace zim {

static std::unordered_map<zim_handle, std::shared_ptr<ZIMImpl>> zim_handle_map_;
static std::mutex zim_handle_map_mutex_;

// MARK: - Static main functions implementation

std::string ZIM::getVersion() { return zim_get_version(); }

ZIM *ZIM::getInstance() {
    zim_handle handle = nullptr;
    zim_get_instance(&handle);
    if (handle == nullptr) {
        return nullptr;
    } else {
        auto instance = zim_handle_map_[handle];
        return instance.get();
    }
}

void zim_register_callback(zim_handle &handle) {
    // main callback
    zim_register_log_uploaded_callback(handle, ZIMBridge::on_log_uploaded);
    zim_register_logged_in_callback(handle, ZIMBridge::on_logged_in);
    zim_register_token_renewed_callback(handle, ZIMBridge::zim_on_token_renewed);
    zim_register_user_name_updated_callback(handle, ZIMBridge::on_user_name_updated);
    zim_register_user_avatar_url_updated_callback(handle, ZIMBridge::on_user_avatar_updated);
    zim_register_user_extended_data_updated_callback(handle,
                                                     ZIMBridge::on_user_extended_data_updated);
    zim_register_users_info_queried_callback(handle, ZIMBridge::on_users_info_queried);

    // message callback
    zim_register_message_sent_callback(handle, ZIMBridge::on_message_sent);
    zim_register_message_queried_callback(handle, ZIMBridge::on_message_queried);
    zim_register_message_deleted_callback(handle, ZIMBridge::on_message_deleted);
    zim_register_conversation_messages_all_deleted_callback(
        handle, ZIMBridge::on_conversation_messages_all_deleted);
    zim_register_message_inserted_callback(handle, ZIMBridge::on_message_inserted);
    zim_register_message_revoked_callback(handle, ZIMBridge::on_message_revoked);
    zim_register_message_attached_callback(handle, ZIMBridge::on_message_attached);
    zim_register_media_uploading_progress_callback(handle, ZIMBridge::on_send_media_file_progress);
    zim_register_media_downloaded_callback(handle, ZIMBridge::on_downloaded_file_message);
    zim_register_media_downloading_progress_callback(handle,
                                                     ZIMBridge::on_download_media_file_progress);
    zim_register_message_receipts_read_sent_callback(handle, ZIMBridge::on_message_receipts_acked);
    zim_register_message_receipts_info_queried_callback(handle,
                                                        ZIMBridge::on_message_receipts_queried);
    zim_register_message_local_extended_data_updated_callback(
        handle, ZIMBridge::on_message_local_extended_data_updated);
    zim_register_messages_searched_callback(handle, ZIMBridge::on_messages_searched);
    zim_register_messages_global_searched_callback(handle, ZIMBridge::on_messages_global_searched);
    zim_register_conversations_searched_callback(handle, ZIMBridge::on_conversations_searched);
    zim_register_message_added_reaction_callback(handle, ZIMBridge::on_message_reaction_added);
    zim_register_message_deleted_reaction_callback(handle, ZIMBridge::on_message_reaction_deleted);
    zim_register_message_queried_reaction_user_list_callback(
        handle, ZIMBridge::on_message_reaction_users_queried);
    zim_register_message_query_combine_message_detail_callback(
        handle, ZIMBridge::on_combine_message_detail_queried);

    // conversation callback
    zim_register_conversation_queried_callback(handle, ZIMBridge::on_conversation_queried);
    zim_register_conversation_list_queried_callback(handle,
                                                    ZIMBridge::on_conversation_list_queried);
    zim_register_conversation_pinned_list_queried_callback(
        handle, ZIMBridge::on_conversation_pinned_list_queried);
    zim_register_conversation_pinned_state_updated_callback(
        handle, ZIMBridge::on_conversation_pinned_state_updated);
    zim_register_conversation_deleted_callback(handle, ZIMBridge::on_conversation_deleted);
    zim_register_conversation_unread_message_count_cleared_callback(
        handle, ZIMBridge::on_conversation_unread_message_count_cleared);
    zim_register_conversation_message_receipt_read_sent_callback(
        handle, ZIMBridge::on_conversation_receipt_acked);
    zim_register_conversation_draft_set_callback(handle, ZIMBridge::on_conversation_draft_set);
    zim_register_conversation_notification_status_set_callback(
        handle, ZIMBridge::on_conversation_notification_status_set);
    zim_register_conversations_all_deleted_callback(handle,
                                                    ZIMBridge::on_conversations_all_deleted);
    zim_register_conversation_total_unread_message_count_cleared_callback(
        handle, ZIMBridge::on_conversation_total_unread_message_count_cleared);

    // room callback
    zim_register_room_created_callback(handle, ZIMBridge::on_room_created);
    zim_register_room_joined_callback(handle, ZIMBridge::on_room_joined);
    zim_register_room_entered_callback(handle, ZIMBridge::on_room_entered);
    zim_register_room_left_callback(handle, ZIMBridge::on_room_left);
    zim_register_room_member_queried_callback(handle, ZIMBridge::on_member_queried);
    zim_register_room_online_member_count_queried_callback(
        handle, ZIMBridge::on_room_online_member_count_queried);
    zim_register_room_attributes_operated_callback(handle, ZIMBridge::on_room_attributes_operated);
    zim_register_room_attributes_queried_callback(handle, ZIMBridge::on_room_attributes_queried);
    zim_register_room_attributes_batch_operated_callback(
        handle, ZIMBridge::on_room_attributes_batch_operated);
    zim_register_room_members_attributes_operated_callback(
        handle, ZIMBridge::on_room_members_attributes_operated);
    zim_register_room_members_attributes_queried_callback(
        handle, ZIMBridge::on_room_members_attributes_queried);
    zim_register_room_member_attributes_list_queried_callback(
        handle, ZIMBridge::on_room_members_attributes_paging_queried);
    zim_register_room_members_queried_callback(handle, ZIMBridge::on_room_members_queried);
    // group callback
    zim_register_group_created_callback(handle, ZIMBridge::on_group_created);
    zim_register_group_dismissed_callback(handle, ZIMBridge::on_group_dismissed);
    zim_register_group_joined_callback(handle, ZIMBridge::on_group_joined);
    zim_register_group_left_callback(handle, ZIMBridge::on_group_left);
    zim_register_group_users_invited_callback(handle, ZIMBridge::on_group_invited_users);
    zim_register_group_member_kicked_callback(handle, ZIMBridge::on_group_member_kicked_out);
    zim_register_group_owner_transferred_callback(handle, ZIMBridge::on_group_owner_transferred);
    zim_register_group_name_updated_callback(handle, ZIMBridge::on_group_name_updated);
    zim_register_group_avatar_url_updated_callback(handle, ZIMBridge::on_group_avatar_updated);
    zim_register_group_notice_updated_callback(handle, ZIMBridge::on_group_notice_updated);
    zim_register_group_muted_callback(handle, ZIMBridge::on_group_muted);
    zim_register_group_member_list_muted_callback(handle, ZIMBridge::on_group_member_list_muted);
    zim_register_group_muted_member_list_queried_callback(
        handle, ZIMBridge::on_group_muted_member_list_queried);
    zim_register_group_info_queried_callback(handle, ZIMBridge::on_group_info_queried);
    zim_register_group_attributes_operated_callback(handle,
                                                    ZIMBridge::on_group_attributes_operated);
    zim_register_group_attributes_queried_callback(handle, ZIMBridge::on_group_attributes_queried);
    zim_register_group_member_nickname_updated_callback(
        handle, ZIMBridge::on_group_member_nickname_updated);
    zim_register_group_member_role_updated_callback(handle,
                                                    ZIMBridge::on_group_member_role_updated);
    zim_register_group_member_info_queried_callback(
        handle, ZIMBridge::on_group_member_info_queried_updated);
    zim_register_group_list_queried_callback(handle, ZIMBridge::on_group_list_queried);
    zim_register_group_member_list_queried_callback(handle,
                                                    ZIMBridge::on_group_member_list_queried);
    zim_register_group_member_count_queried_callback(handle,
                                                     ZIMBridge::on_group_member_count_queried);
    zim_register_group_message_receipt_member_list_queried_callback(
        handle, ZIMBridge::on_group_receipt_member_list_queried);
    zim_register_groups_searched_callback(handle, ZIMBridge::on_groups_searched);
    zim_register_group_members_searched_callback(handle, ZIMBridge::on_group_members_searched);

    // call callback
    zim_register_call_invitation_sent_callback(handle, ZIMBridge::on_call_invitation_sent);
    zim_register_call_join_sent_callback(handle, ZIMBridge::on_call_join_sent_callback);
    zim_register_call_cancel_sent_callback(handle, ZIMBridge::on_call_cancel_sent);
    zim_register_call_acceptance_sent_callback(handle, ZIMBridge::on_call_acceptance_sent);
    zim_register_call_rejection_sent_callback(handle, ZIMBridge::on_call_rejection_sent);
    zim_register_calling_invitation_sent_callback(handle,
                                                  ZIMBridge::on_calling_invitation_sent_callback);
    zim_register_call_quit_sent_callback(handle, ZIMBridge::on_call_quit_sent_callback);
    zim_register_call_end_sent_callback(handle, ZIMBridge::on_call_end_sent_callback);
    zim_register_call_list_queried_callback(handle, ZIMBridge::on_call_list_queried_callback);

    // main event
    zim_register_error_event(handle, ZIMBridge::on_error);
    zim_register_connection_state_changed_event(handle, ZIMBridge::on_connection_state_changed);
    zim_register_token_will_expire_event(handle, ZIMBridge::on_token_will_expire);

    // user event
    zim_register_user_info_updated_event(handle, ZIMBridge::on_user_info_updated);

    // message event
    zim_register_receive_peer_message_event(handle, ZIMBridge::on_receive_peer_message);
    zim_register_receive_room_message_event(handle, ZIMBridge::on_receive_room_message);
    zim_register_receive_group_message_event(handle, ZIMBridge::on_receive_group_message);
    zim_register_message_receipt_changed_event(handle, ZIMBridge::on_message_receipt_changed);
    zim_register_message_revoke_received_event(handle, ZIMBridge::on_message_revoke_received);
    zim_register_broadcast_message_received_event(handle, ZIMBridge::on_broadcast_message_received);
    zim_register_message_deleted_event(handle, ZIMBridge::on_message_deleted_event);
    zim_register_message_sent_status_changed_event(handle,
                                                   ZIMBridge::on_message_sent_status_changed);
    zim_register_message_reactions_changed_event(handle, ZIMBridge::on_message_reactions_changed);

    // conversation event
    zim_register_conversation_changed_event(handle, ZIMBridge::on_conversation_changed);
    zim_register_conversation_total_unread_message_count_updated_event(
        handle, ZIMBridge::on_conversation_total_unread_message_count_updated);
    zim_register_conversations_all_deleted_event(handle,
                                                 ZIMBridge::on_conversations_all_deleted_event);

    // room event
    zim_register_room_state_changed_event(handle, ZIMBridge::on_room_state_changed);
    zim_register_room_member_joined_event(handle, ZIMBridge::on_room_member_joined);
    zim_register_room_member_left_event(handle, ZIMBridge::on_room_member_left);
    zim_register_room_attributes_updated_event(handle, ZIMBridge::on_room_attributes_updated);
    zim_register_room_attributes_batch_updated_event(handle,
                                                     ZIMBridge::on_room_attributes_batch_updated);
    zim_register_room_member_attributes_updated_event(
        handle, ZIMBridge::on_room_members_attributes_updated);

    // group event
    zim_register_group_state_changed_event(handle, ZIMBridge::on_group_state_updated);
    zim_register_group_member_state_changed_event(handle, ZIMBridge::on_group_member_state_updated);
    zim_register_group_name_updated_event(handle, ZIMBridge::on_group_name_update_notified);
    zim_register_group_avatar_url_updated_event(handle, ZIMBridge::on_group_avatar_update_notified);
    zim_register_group_notice_updated_event(handle, ZIMBridge::on_group_notice_update_notified);
    zim_register_group_attributes_updated_event(handle, ZIMBridge::on_group_attributes_updated);
    zim_register_group_member_info_updated_event(handle, ZIMBridge::on_group_member_info_updated);
    zim_register_group_mute_info_updated_event(handle,
                                               ZIMBridge::on_group_mute_info_updated_notified);

    // call event
    zim_register_call_invitation_received_event(handle, ZIMBridge::on_call_invitation_received);
    zim_register_call_invitation_cancelled_event(handle, ZIMBridge::on_call_invitation_cancelled);
    zim_register_call_invitation_accepted_event(handle, ZIMBridge::on_call_invitation_accepted);
    zim_register_call_invitation_rejected_event(handle, ZIMBridge::on_call_invitation_rejected);
    zim_register_call_invitation_timeout_event(handle, ZIMBridge::on_call_invitation_timeout);
    zim_register_call_invitees_answered_timeout_event(handle,
                                                      ZIMBridge::on_call_invitees_answered_timeout);
    zim_register_call_user_state_changed_event(handle, ZIMBridge::on_call_user_state_changed);
    zim_register_call_invitation_created_event(handle, ZIMBridge::on_call_invitation_created);
    zim_register_call_invitation_ended_event(handle, ZIMBridge::on_call_invitation_ended);

    zim_register_on_blacklist_changed_event(handle, ZIMBridge::on_blacklist_changed_event);
    zim_register_blacklist_users_added_callback(handle,
                                                ZIMBridge::on_blacklist_users_added_callback);
    zim_register_blacklist_users_remove_callback(handle,
                                                 ZIMBridge::on_blacklist_users_remove_callback);
    zim_register_blacklist_queried_callback(handle, ZIMBridge::on_blacklist_queried_callback);
    zim_register_check_user_is_in_blacklist_callback(
        handle, ZIMBridge::on_check_user_is_in_blacklist_callback);

    zim_register_friends_searched_callback(handle, ZIMBridge::on_friends_searched_callback);
    zim_register_on_friend_info_updated_event(handle, ZIMBridge::on_friend_info_updated_event);
    zim_register_on_friend_list_changed_event(handle, ZIMBridge::on_friend_list_changed_event);
    zim_register_on_friend_application_updated_event(
        handle, ZIMBridge::on_friend_application_updated_event);
    zim_register_on_friend_application_changed_event(
        handle, ZIMBridge::on_friend_application_changed_event);

    zim_register_friend_added_callback(handle, ZIMBridge::on_friend_added_callback);
    zim_register_friend_application_sent_callback(handle,
                                                  ZIMBridge::on_friend_application_sent_callback);
    zim_register_delete_friends_callback(handle, ZIMBridge::on_delete_friend_callback);
    zim_register_friends_relation_check_callback(handle,
                                                 ZIMBridge::on_check_friend_relation_callback);
    zim_register_update_friend_alias_callback(handle, ZIMBridge::on_update_friend_alias_callback);
    zim_register_update_friend_attributes_callback(handle,
                                                   ZIMBridge::on_update_friend_attributes_callback);
    zim_register_friends_info_queried_callback(handle, ZIMBridge::on_friends_info_queried_callback);
    zim_register_friend_application_accepted_callback(
        handle, ZIMBridge::on_friend_application_accepted_callback);
    zim_register_friend_application_reject_callback(
        handle, ZIMBridge::on_friend_application_reject_callback);
    zim_register_query_friend_list_callback(handle, ZIMBridge::on_query_friend_list_callback);
    zim_register_query_friend_application_list_callback(
        handle, ZIMBridge::on_query_friend_application_list_callback);
}

ZIM *ZIM::create(unsigned int appID) {
    {
        std::lock_guard<std::mutex> lock(zim_handle_map_mutex_);
        if (!zim_handle_map_.empty())
            return nullptr;
    }

    zim_handle handle = nullptr;
    std::string appsign = "";
    zim_create(&handle, appID, appsign.c_str());
    if (!handle) {
        return nullptr;
    }
    auto instance = std::make_shared<ZIMImpl>();
    instance->handle_ = handle;

    {
        std::lock_guard<std::mutex> lock(zim_handle_map_mutex_);
        zim_handle_map_[handle] = instance;
    }
    zim_register_callback(handle);
    return instance.get();
}

ZIM *ZIM::create(const ZIMAppConfig &config) {
    {
        std::lock_guard<std::mutex> lock(zim_handle_map_mutex_);
        if (!zim_handle_map_.empty())
            return nullptr;
    }

    zim_handle handle = nullptr;
    zim_create(&handle, config.appID, config.appSign.c_str());
    if (!handle) {
        return nullptr;
    }
    auto instance = std::make_shared<ZIMImpl>();
    instance->handle_ = handle;

    {
        std::lock_guard<std::mutex> lock(zim_handle_map_mutex_);
        zim_handle_map_[handle] = instance;
    }

    zim_register_callback(handle);
    return instance.get();
}

void ZIMImpl::destroy() {
    {
        std::lock_guard<std::mutex> lock(zim_handle_map_mutex_);

        auto tmp_handle = handle_;

        zim_destroy(&handle_);
        zim_handle_map_.erase(tmp_handle);
    }
}

void ZIM::setLogConfig(const ZIMLogConfig &config) {
    struct zim_log_config log_config {};
    ZIMConverter::sZIMLogConfig(&log_config, &config);
    zim_set_log_config(log_config);
}

void ZIM::setAdvancedConfig(const std::string &key, const std::string &value) {
    zim_set_advanced_config(key.c_str(), value.c_str());
}

bool ZIM::setGeofencingConfig(const std::vector<int> &areaList, ZIMGeofencingType type) {
    return zim_set_geofencing_config(areaList.data(), (unsigned int)(areaList.size()),
                                     (zim_geofencing_type)type);
}

void ZIM::setCacheConfig(const ZIMCacheConfig &config) {
    struct zim_cache_config cache_config {};
    ZIMConverter::sZIMCacheConfig(&cache_config, &config);
    zim_set_cache_config(cache_config);
}

void ZIM::setAndroidEnv(void *jvm, void *context) { zim_set_android_env(jvm, context); }

// MARK: Do not access std::unordered_map from outside

ZIMImpl *GetZIMImplInstance(zim_handle handle) {
    // No need to lock here, read only
    auto instance_ptr = zim_handle_map_[handle];
    if (instance_ptr) {
        return instance_ptr.get();
    } else {
        return nullptr;
    }
}

} // namespace zim
