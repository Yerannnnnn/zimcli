#pragma once

//
//  ZIMBridge.h
//  ZIM
//
//  Created by Patrick Fu on 2021/6/26.
//  Copyright © 2021 Zego. All rights reserved.
//

namespace zim {

extern ZIMImpl *GetZIMImplInstance(zim_handle handle);

class ZIMBridge {
  public:
    // MARK: - Callback functions implementation

    // MARK: Main callbacks
    static void on_log_uploaded(zim_handle handle, struct zim_error error_info,
                                zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMLogUploadedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->log_uploaded_callbacks_mutex_);
            callback = zim->log_uploaded_callbacks_[sequence];
            zim->log_uploaded_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(error);
    }

    static void on_error(zim_handle handle, struct zim_error error_info) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        if (!zim->event_handler_) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        zim->event_handler_->onError(zim, error);
    }

    static void on_logged_in(zim_handle handle, struct zim_error error_info,
                             zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMLoggedInCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->logged_in_callbacks_mutex_);
            callback = zim->logged_in_callbacks_[sequence];
            zim->logged_in_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(error);
    }

    static void on_connection_state_changed(zim_handle handle, enum zim_connection_state state,
                                            enum zim_connection_event event,
                                            const char *extended_data) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        if (!zim->event_handler_) {
            return;
        }

        zim->event_handler_->onConnectionStateChanged(zim, ZIMConnectionState(state),
                                                      ZIMConnectionEvent(event), extended_data);
    }

    static void on_room_state_changed(zim_handle handle, enum zim_room_state state,
                                      enum zim_room_event event, const char *extended_data,
                                      const char *room_id) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim || !room_id) {
            return;
        }

        if (!zim->event_handler_) {
            return;
        }

        zim->event_handler_->onRoomStateChanged(zim, ZIMRoomState(state), ZIMRoomEvent(event),
                                                extended_data, room_id);
    }

    static void on_token_will_expire(zim_handle handle, unsigned int second) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        if (!zim->event_handler_) {
            return;
        }

        zim->event_handler_->onTokenWillExpire(zim, second);
    }

    static void on_user_info_updated(zim_handle handle, struct zim_user_full_info info) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        if (!zim->event_handler_) {
            return;
        }

        ZIMUserFullInfo userInfo;
        ZIMConverter::oZIMUserFullInfo(&userInfo, &info);

        zim->event_handler_->onUserInfoUpdated(zim, userInfo);
    }

    static void zim_on_token_renewed(zim_handle handle, const char *token,
                                     struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMTokenRenewedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->renew_token_callbacks_mutex_);
            callback = zim->renew_token_callbacks_[sequence];
            zim->renew_token_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(token ? token : "", error);
    }

    static void on_user_name_updated(zim_handle handle, const char *name,
                                     struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMUserNameUpdatedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->update_user_name_callbacks_mutex_);
            callback = zim->update_user_name_callbacks_[sequence];
            zim->update_user_name_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(name ? name : "", error);
    }

    static void on_user_avatar_updated(zim_handle handle, const char *avatar,
                                       struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMUserAvatarUrlUpdatedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->update_user_avatar_callbacks_mutex_);
            callback = zim->update_user_avatar_callbacks_[sequence];
            zim->update_user_avatar_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(avatar ? avatar : "", error);
    }

    static void on_user_extended_data_updated(zim_handle handle, const char *extended_data,
                                              struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMUserExtendedDataUpdatedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->update_user_extended_data_callbacks_mutex_);
            callback = zim->update_user_extended_data_callbacks_[sequence];
            zim->update_user_extended_data_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(extended_data ? extended_data : "", error);
    }

    static void on_users_info_queried(zim_handle handle,
                                      const struct zim_user_full_info *user_info_list,
                                      unsigned int info_length,
                                      const struct zim_error_user_info *error_user_info_list,
                                      unsigned int error_info_list_length,
                                      struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMUsersInfoQueriedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->users_info_queried_callbacks_mutex_);
            callback = zim->users_info_queried_callbacks_[sequence];
            zim->users_info_queried_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }
        std::vector<ZIMUserFullInfo> userList;
        for (unsigned int i = 0; i < info_length; ++i) {
            ZIMUserFullInfo userInfo;
            auto c_user_info = user_info_list[i];
            ZIMConverter::oZIMUserFullInfo(&userInfo, &c_user_info);
            userList.push_back(userInfo);
        }
        std::vector<ZIMErrorUserInfo> errorUsers;
        for (unsigned int i = 0; i < error_info_list_length; ++i) {
            ZIMErrorUserInfo errorUser;
            auto c_error_user = error_user_info_list[i];
            ZIMConverter::oZIMErrorUserInfo(&errorUser, &c_error_user);
            errorUsers.push_back(errorUser);
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(userList, errorUsers, error);
    }

    static void on_room_attributes_updated(zim_handle handle, const char *room_id,
                                           zim_room_attributes_update_info info,
                                           const char * /*extended_data*/) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim || !room_id) {
            return;
        }

        if (!zim->event_handler_) {
            return;
        }

        ZIMRoomAttributesUpdateInfo zinfo;
        zinfo.action = (ZIMRoomAttributesUpdateAction)info.room_attributes_action;
        for (unsigned int i = 0; i < info.room_attributes_length; i++) {
            zinfo.roomAttributes.insert(
                std::make_pair(info.room_attributes[i].key, info.room_attributes[i].value));
        }
        zim->event_handler_->onRoomAttributesUpdated(zim, zinfo, room_id);
    }

    static void on_room_attributes_batch_updated(zim_handle handle, const char *room_id,
                                                 zim_room_attributes_update_info *infos,
                                                 unsigned int info_length,
                                                 const char * /*extended_data*/) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim || !room_id) {
            return;
        }

        if (!zim->event_handler_) {
            return;
        }

        std::vector<ZIMRoomAttributesUpdateInfo> zinfo;

        for (unsigned int info_seq = 0; info_seq < info_length; info_seq++) {
            ZIMRoomAttributesUpdateInfo info;
            info.action = (ZIMRoomAttributesUpdateAction)infos[info_seq].room_attributes_action;

            for (unsigned int i = 0; i < infos[info_seq].room_attributes_length; i++) {
                info.roomAttributes.insert(
                    std::make_pair(infos[info_seq].room_attributes[i].key,
                                   infos[info_seq].room_attributes[i].value));
            }

            zinfo.push_back(info);
        }

        zim->event_handler_->onRoomAttributesBatchUpdated(zim, zinfo, room_id);
    }

    static void on_room_members_attributes_updated(
        zim_handle handle, struct zim_room_member_attributes_update_info *infos,
        unsigned int infos_length, struct zim_room_operated_info operated_info,
        const char *room_id) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim || !room_id) {
            return;
        }

        if (!zim->event_handler_) {
            return;
        }

        std::vector<ZIMRoomMemberAttributesUpdateInfo> infosVec;
        for (unsigned int i = 0; i < infos_length; ++i) {
            ZIMRoomMemberAttributesUpdateInfo info;

            info.attributesInfo.userID = infos[i].attributes_info.user_id;

            for (unsigned int j = 0; j < infos[i].attributes_info.attributes_length; ++j) {
                info.attributesInfo.attributes.emplace(
                    infos[i].attributes_info.attributes[j].key,
                    infos[i].attributes_info.attributes[j].value);
            }

            infosVec.emplace_back(info);
        }

        ZIMRoomOperatedInfo operatedInfo;
        operatedInfo.userID = operated_info.user_id;

        zim->event_handler_->onRoomMemberAttributesUpdated(zim, infosVec, operatedInfo, room_id);
    }

    // MARK: Message callbacks

    static void on_message_sent(zim_handle handle, struct zim_message message,
                                struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMMessageSentCallback callback = nullptr;
        std::shared_ptr<ZIMMessage> oMessage = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->message_sent_callbacks_mutex_);
            callback = zim->message_sent_callbacks_[sequence];
            {
                std::lock_guard<std::mutex> lock_2(zim->message_obj_map_mutex_);
                if (zim->message_obj_map_.find(sequence) != zim->message_obj_map_.end()) {

                    oMessage = zim->message_obj_map_[sequence];
                    zim->message_obj_map_.erase(sequence);
                }
            }

            zim->message_sent_callbacks_.erase(sequence);
            zim->media_send_progress_callbacks_.erase(sequence);
            zim->send_message_notification_callbacks_.erase(sequence);
            zim->send_media_message_notification_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        if (oMessage) {
            ZIMConverter::cZIMMessage(oMessage, message);
        } else {
            oMessage = ZIMConverter::oZIMMessage(&message);
        }

        callback(oMessage, error);
    }

    static void on_message_attached(zim_handle handle, struct zim_message message,
                                    zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        std::shared_ptr<ZIMMessageSendNotification> messageNotification = nullptr;
        std::shared_ptr<ZIMMediaMessageSendNotification> mediaMessageNotification = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->send_message_notification_callbacks_mutex_);
            if (zim->send_message_notification_callbacks_.find(sequence) !=
                zim->send_message_notification_callbacks_.end()) {
                messageNotification = zim->send_message_notification_callbacks_[sequence];
            }
        }

        {
            std::lock_guard<std::mutex> lock(zim->send_media_message_notification_callbacks_mutex_);
            if (zim->send_media_message_notification_callbacks_.find(sequence) !=
                zim->send_media_message_notification_callbacks_.end()) {
                mediaMessageNotification =
                    zim->send_media_message_notification_callbacks_[sequence];
            }
        }

        std::shared_ptr<ZIMMessage> oMessage = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->message_obj_map_mutex_);
            if (zim->message_obj_map_.find(sequence) != zim->message_obj_map_.end()) {
                oMessage = zim->message_obj_map_[sequence];
                zim->message_obj_map_.erase(sequence);
            }
        }

        if (oMessage) {
            ZIMConverter::cZIMMessage(oMessage, message);
        } else {
            oMessage = ZIMConverter::oZIMMessage(&message);
        }

        if (messageNotification) {
            if (messageNotification->onMessageAttached) {
                messageNotification->onMessageAttached(oMessage);
            }
        }

        if (mediaMessageNotification) {
            if (mediaMessageNotification->onMessageAttached) {
                mediaMessageNotification->onMessageAttached(
                    std::static_pointer_cast<ZIMMediaMessage>(oMessage));
            }
        }
    }

    static void on_receive_peer_message(zim_handle handle, const struct zim_message *message_list,
                                        unsigned int message_list_length,
                                        const char *from_user_id) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        if (!zim->event_handler_) {
            return;
        }

        std::vector<std::shared_ptr<ZIMMessage>> messageList;
        for (unsigned int i = 0; i < message_list_length; i++) {
            struct zim_message sMessage = *(message_list + i);
            std::shared_ptr<ZIMMessage> message = ZIMConverter::oZIMMessage(&sMessage);
            messageList.emplace_back(std::move(message));
        }

        zim->event_handler_->onReceivePeerMessage(zim, messageList, from_user_id);
    }

    static void on_receive_room_message(zim_handle handle, const struct zim_message *message_list,
                                        unsigned int message_list_length,
                                        const char *from_room_id) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        if (!zim->event_handler_) {
            return;
        }

        std::vector<std::shared_ptr<ZIMMessage>> messageList;
        for (unsigned int i = 0; i < message_list_length; i++) {
            struct zim_message sMessage = *(message_list + i);
            std::shared_ptr<ZIMMessage> message = ZIMConverter::oZIMMessage(&sMessage);
            messageList.emplace_back(std::move(message));
        }

        zim->event_handler_->onReceiveRoomMessage(zim, messageList, from_room_id);
    }

    static void on_receive_group_message(zim_handle handle, const struct zim_message *message_list,
                                         unsigned int message_list_length,
                                         const char *from_group_id) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        if (!zim->event_handler_) {
            return;
        }

        std::vector<std::shared_ptr<ZIMMessage>> messageList;
        for (unsigned int i = 0; i < message_list_length; i++) {
            struct zim_message sMessage = *(message_list + i);
            std::shared_ptr<ZIMMessage> message = ZIMConverter::oZIMMessage(&sMessage);
            messageList.emplace_back(std::move(message));
        }

        zim->event_handler_->onReceiveGroupMessage(zim, messageList, from_group_id);
    }

    static void on_message_queried(zim_handle handle, const char *conversation_id,
                                   zim_conversation_type type,
                                   const struct zim_message *message_list,
                                   unsigned int message_list_length, struct zim_error error_info,
                                   zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMMessageQueriedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->message_queried_callbacks_mutex_);
            callback = zim->message_queried_callbacks_[sequence];
            zim->message_queried_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        std::vector<std::shared_ptr<ZIMMessage>> messageList;
        for (unsigned int i = 0; i < message_list_length; i++) {
            struct zim_message sMessage = *(message_list + i);
            std::shared_ptr<ZIMMessage> message = ZIMConverter::oZIMMessage(&sMessage);
            messageList.emplace_back(std::move(message));
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(conversation_id, (ZIMConversationType)type, messageList, error);
    }

    static void on_message_local_extended_data_updated(zim_handle handle,
                                                       struct zim_message message,
                                                       struct zim_error error_info,
                                                       zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }
        ZIMMessageLocalExtendedDataUpdatedCallback callback = nullptr;
        {
            std::lock_guard<std::mutex> lock(
                zim->message_local_extended_data_updated_callbacks_mutex_);
            callback = zim->message_local_extended_data_updated_callbacks_[sequence];
            zim->message_local_extended_data_updated_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        std::shared_ptr<ZIMMessage> message_ptr = ZIMConverter::oZIMMessage(&message);

        callback(message_ptr, error);
    }

    static void on_message_deleted(zim_handle handle, const char *conversation_id,
                                   zim_conversation_type type, struct zim_error error_info,
                                   zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMMessageDeletedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->message_deleted_callbacks_mutex_);
            callback = zim->message_deleted_callbacks_[sequence];
            zim->message_deleted_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(conversation_id, (ZIMConversationType)type, error);
    }

    static void on_conversation_messages_all_deleted(zim_handle handle, struct zim_error error_info,
                                                     zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMConversationMessagesAllDeletedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(
                zim->conversation_messages_all_deleted_callbacks_mutex_);
            callback = zim->conversation_messages_all_deleted_callbacks_[sequence];
            zim->conversation_messages_all_deleted_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(error);
    }

    static void on_message_revoked(zim_handle handle, struct zim_message message,
                                   struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMMessageRevokedCallback callback = nullptr;
        {
            std::lock_guard<std::mutex> lock(zim->message_revoked_callbacks_mutex_);
            callback = zim->message_revoked_callbacks_[sequence];
            zim->message_revoked_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        std::shared_ptr<ZIMMessage> message_ptr = ZIMConverter::oZIMMessage(&message);

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(message_ptr, error);
    }

    static void on_message_receipts_acked(zim_handle handle, const char *conversation_id,
                                          enum zim_conversation_type type,
                                          const long long *error_message_id_list,
                                          unsigned int list_length, struct zim_error error_info,
                                          zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMMessageReceiptsReadSentCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->message_receipts_read_callbacks_mutex_);
            callback = zim->message_receipts_read_callbacks_[sequence];
            zim->message_receipts_read_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        std::vector<long long> errorMessageIDs;
        for (unsigned int i = 0; i < list_length; i++) {
            errorMessageIDs.push_back(error_message_id_list[i]);
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(conversation_id, (ZIMConversationType)type, errorMessageIDs, error);
    }

    static void on_message_receipts_queried(
        zim_handle handle, const struct zim_message_receipt_info *read_receipt_list,
        unsigned int list_length, const long long *error_message_id_list,
        unsigned int id_list_length, struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMMessageReceiptsInfoQueriedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->message_receipts_queried_callbacks_mutex_);
            callback = zim->message_receipts_queried_callbacks_[sequence];
            zim->message_receipts_queried_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        std::vector<long long> error_id_list;
        for (unsigned int i = 0; i < id_list_length; ++i) {
            error_id_list.emplace_back(error_message_id_list[i]);
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        auto infos = ZIMConverter::oZIMMessageReceiptInfoList(read_receipt_list, list_length);

        callback(infos, error_id_list, error);
    }

    static void on_messages_searched(zim_handle handle, const char *conversation_id,
                                     enum zim_conversation_type conversation_type,
                                     const struct zim_message *message_list,
                                     unsigned int message_list_length,
                                     const struct zim_message *next_message,
                                     struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMMessagesSearchedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->message_searched_callbacks_mutex_);
            callback = zim->message_searched_callbacks_[sequence];
            zim->message_searched_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        std::vector<std::shared_ptr<ZIMMessage>> messageList;
        for (unsigned int i = 0; i < message_list_length; i++) {
            struct zim_message sMessage = *(message_list + i);
            std::shared_ptr<ZIMMessage> message = ZIMConverter::oZIMMessage(&sMessage);
            messageList.emplace_back(std::move(message));
        }

        std::shared_ptr<ZIMMessage> nextMessage;
        if (next_message) {
            zim_message *next_message_ptr = const_cast<zim_message *>(next_message);
            nextMessage = ZIMConverter::oZIMMessage(next_message_ptr);
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(conversation_id, (ZIMConversationType)conversation_type, messageList, nextMessage,
                 error);
    }

    static void on_messages_global_searched(zim_handle handle,
                                            const struct zim_message *message_list,
                                            unsigned int message_list_length,
                                            const struct zim_message *next_message,
                                            struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMMessagesGlobalSearchedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->message_global_searched_callbacks_mutex_);
            callback = zim->message_global_searched_callbacks_[sequence];
            zim->message_global_searched_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        std::vector<std::shared_ptr<ZIMMessage>> messageList;
        for (unsigned int i = 0; i < message_list_length; i++) {
            struct zim_message sMessage = *(message_list + i);
            std::shared_ptr<ZIMMessage> message = ZIMConverter::oZIMMessage(&sMessage);
            messageList.emplace_back(std::move(message));
        }

        std::shared_ptr<ZIMMessage> nextMessage;
        if (next_message) {
            zim_message *next_message_ptr = const_cast<zim_message *>(next_message);
            nextMessage = ZIMConverter::oZIMMessage(next_message_ptr);
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(messageList, nextMessage, error);
    }

    static void
    on_conversations_searched(zim_handle handle,
                              const struct zim_conversation_search_info *global_info_list,
                              unsigned int global_info_length, unsigned int next_flag,
                              struct zim_error error_info, zim_sequence sequence) {

        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMConversationsSearchedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->conversations_searched_callbacks_mutex_);
            callback = zim->conversations_searched_callbacks_[sequence];
            zim->conversations_searched_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        std::vector<ZIMConversationSearchInfo> globalInfoList =
            ZIMConverter::oZIMConversationSearchInfoList(global_info_list, global_info_length);

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(globalInfoList, next_flag, error);
    }

    static void on_message_reactions_changed(zim_handle handle,
                                             const struct zim_message_reaction *reactions,
                                             unsigned int reaction_length) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        if (!zim->event_handler_) {
            return;
        }

        std::vector<ZIMMessageReaction> reactionList;
        ZIMConverter::oZIMMessageReactionList(reactions, reaction_length, reactionList);

        zim->event_handler_->onMessageReactionsChanged(zim, reactionList);
    }

    static void on_message_reaction_users_queried(
        zim_handle handle, const struct zim_message message, const char *reaction_type,
        const struct zim_message_reaction_user_info *user_infos, unsigned int user_info_length,
        long long next_flag, unsigned int total_count, struct zim_error error_info,
        zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMMessageReactionUserListQueriedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->reaction_user_list_queried_callbacks_mutex_);
            callback = zim->reaction_user_list_queried_callbacks_[sequence];
            zim->reaction_user_list_queried_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;
        std::vector<ZIMMessageReactionUserInfo> userList;
        ZIMConverter::oZIMMessageReactionUserInfoList(user_infos, user_info_length, userList);
        std::shared_ptr<ZIMMessage> oMessage =
            ZIMConverter::oZIMMessage(const_cast<zim_message *>(&message));

        callback(oMessage, userList, reaction_type, next_flag, total_count, error);
    }

    static void on_combine_message_detail_queried(zim_handle handle,
                                                  const struct zim_message message,
                                                  struct zim_error error_info,
                                                  zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMCombineMessageDetailQueriedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->message_combine_queried_callbacks_mutex_);
            callback = zim->message_combine_queried_callbacks_[sequence];
            zim->message_combine_queried_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;
        std::shared_ptr<ZIMMessage> oMessage =
            ZIMConverter::oZIMMessage(const_cast<zim_message *>(&message));

        callback(std::static_pointer_cast<ZIMCombineMessage>(oMessage), error);
    }

    static void on_message_reaction_deleted(zim_handle handle, struct zim_message_reaction reaction,
                                            struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMMessageReactionDeletedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->reaction_deleted_callbacks_mutex_);
            callback = zim->reaction_deleted_callbacks_[sequence];
            zim->reaction_deleted_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        ZIMMessageReaction oReaction{};
        ZIMConverter::oZIMMessageReaction(reaction, oReaction);
        callback(oReaction, error);
    }
    static void on_message_reaction_added(zim_handle handle, struct zim_message_reaction reaction,
                                          struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMMessageReactionAddedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->reaction_added_callbacks_mutex_);
            callback = zim->reaction_added_callbacks_[sequence];
            zim->reaction_added_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        ZIMMessageReaction oReaction{};
        ZIMConverter::oZIMMessageReaction(reaction, oReaction);
        callback(oReaction, error);
    }

    // MARK: Conversation callbacks
    static void on_conversation_queried(zim_handle handle, struct zim_conversation conversation,
                                        struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMConversationQueriedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->conversation_queried_callbacks_mutex_);
            callback = zim->conversation_queried_callbacks_[sequence];
            zim->conversation_queried_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        std::shared_ptr<ZIMConversation> conversation_ptr;

        conversation_ptr = ZIMConverter::oZIMConversation(&conversation);

        callback(conversation_ptr, error);
    }

    static void on_conversation_list_queried(zim_handle handle,
                                             const struct zim_conversation *conversation_list,
                                             unsigned int conversation_list_length,
                                             struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMConversationListQueriedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->conversation_list_queried_callbacks_mutex_);
            callback = zim->conversation_list_queried_callbacks_[sequence];
            zim->conversation_list_queried_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        std::vector<std::shared_ptr<ZIMConversation>> conversationList;
        for (unsigned int i = 0; i < conversation_list_length; i++) {
            struct zim_conversation sConversation = *(conversation_list + i);
            std::shared_ptr<ZIMConversation> conversation =
                ZIMConverter::oZIMConversation(&sConversation);
            conversationList.emplace_back(std::move(conversation));
        }

        callback(conversationList, error);
    }

    static void on_conversation_pinned_list_queried(
        zim_handle handle, const struct zim_conversation *conversation_list,
        unsigned int conversation_list_length, struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMConversationPinnedListQueriedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(
                zim->conversation_pinned_list_queried_callbacks_mutex_);
            callback = zim->conversation_pinned_list_queried_callbacks_[sequence];
            zim->conversation_pinned_list_queried_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        std::vector<std::shared_ptr<ZIMConversation>> conversationList;
        for (unsigned int i = 0; i < conversation_list_length; i++) {
            struct zim_conversation sConversation = *(conversation_list + i);
            std::shared_ptr<ZIMConversation> conversation =
                ZIMConverter::oZIMConversation(&sConversation);
            conversationList.emplace_back(std::move(conversation));
        }

        callback(conversationList, error);
    }

    static void on_conversation_pinned_state_updated(zim_handle handle, const char *conversation_id,
                                                     zim_conversation_type type,
                                                     struct zim_error error_info,
                                                     zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMConversationPinnedStateUpdatedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(
                zim->conversation_pinned_state_updated_callbacks_mutex_);
            callback = zim->conversation_pinned_state_updated_callbacks_[sequence];
            zim->conversation_pinned_state_updated_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(conversation_id, (ZIMConversationType)type, error);
    }

    static void on_conversation_deleted(zim_handle handle, const char *conversation_id,
                                        zim_conversation_type type, struct zim_error error_info,
                                        zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMConversationDeletedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->conversation_deleted_callbacks_mutex_);
            callback = zim->conversation_deleted_callbacks_[sequence];
            zim->conversation_deleted_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(conversation_id, (ZIMConversationType)type, error);
    }

    static void on_conversation_unread_message_count_cleared(zim_handle handle,
                                                             const char *conversation_id,
                                                             zim_conversation_type type,
                                                             struct zim_error error_info,
                                                             zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMConversationUnreadMessageCountClearedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(
                zim->conversation_unread_count_cleared_callbacks_mutex_);
            callback = zim->conversation_unread_count_cleared_callbacks_[sequence];
            zim->conversation_unread_count_cleared_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(conversation_id, (ZIMConversationType)type, error);
    }

    static void on_conversations_all_deleted(zim_handle handle, struct zim_error error_info,
                                             zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMConversationsAllDeletedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->conversations_all_deleted_callbacks_mutex_);
            callback = zim->conversations_all_deleted_callbacks_[sequence];
            zim->conversations_all_deleted_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(error);
    }

    static void on_conversation_total_unread_message_count_cleared(zim_handle handle,
                                                                   struct zim_error error_info,
                                                                   zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMConversationTotalUnreadMessageCountClearedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(
                zim->conversations_all_unread_count_cleared_callbacks_mutex_);
            callback = zim->conversations_all_unread_count_cleared_callbacks_[sequence];
            zim->conversations_all_unread_count_cleared_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(error);
    }

    static void on_conversation_receipt_acked(zim_handle handle, const char *conversation_id,
                                              zim_conversation_type type,
                                              struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMConversationMessageReceiptReadSentCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->conversation_receipt_read_callbacks_mutex_);
            callback = zim->conversation_receipt_read_callbacks_[sequence];
            zim->conversation_receipt_read_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(conversation_id, (ZIMConversationType)type, error);
    }

    static void on_conversation_draft_set(zim_handle handle, const char *conversation_id,
                                          enum zim_conversation_type type,
                                          struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMConversationDraftSetCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->conversation_draft_set_callbacks_mutex_);
            callback = zim->conversation_draft_set_callbacks_[sequence];
            zim->conversation_draft_set_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(conversation_id, (ZIMConversationType)type, error);
    }

    static void on_conversation_changed(
        zim_handle handle, const struct zim_conversation_change_info *conversation_change_info_list,
        unsigned int list_length) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        if (!zim->event_handler_) {
            return;
        }

        std::vector<ZIMConversationChangeInfo> conversationChangeInfoList;
        for (unsigned int i = 0; i < list_length; i++) {
            struct zim_conversation_change_info sConversationChangeInfo =
                *(conversation_change_info_list + i);
            ZIMConversationChangeInfo oInfo{};
            ZIMConverter::oZIMConversationChangeInfo(oInfo, sConversationChangeInfo);
            conversationChangeInfoList.emplace_back(std::move(oInfo));
        }

        zim->event_handler_->onConversationChanged(zim, conversationChangeInfoList);
    }

    static void
    on_conversation_total_unread_message_count_updated(zim_handle handle,
                                                       unsigned int total_unread_message_count) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        if (!zim->event_handler_) {
            return;
        }

        zim->event_handler_->onConversationTotalUnreadMessageCountUpdated(
            zim, total_unread_message_count);
    }

    static void on_conversations_all_deleted_event(zim_handle handle,
                                                   struct zim_conversations_all_deleted_info info) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        if (!zim->event_handler_) {
            return;
        }

        ZIMConversationsAllDeletedInfo conversationsAllDeletedInfo{};
        conversationsAllDeletedInfo.count = info.count;

        zim->event_handler_->onConversationsAllDeleted(zim, conversationsAllDeletedInfo);
    }

    // MARK: Room callbacks

    static void on_room_created(zim_handle handle, struct zim_room_full_info room_info,
                                struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMRoomCreatedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->room_created_callbacks_mutex_);
            callback = zim->room_created_callbacks_[sequence];
            zim->room_created_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMRoomFullInfo info{};
        ZIMConverter::oZIMRoomFullInfo(&info, &room_info);

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(info, error);
    }

    static void on_room_joined(zim_handle handle, struct zim_room_full_info room_info,
                               struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMRoomJoinedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->room_joined_callbacks_mutex_);
            callback = zim->room_joined_callbacks_[sequence];
            zim->room_joined_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMRoomFullInfo info{};
        ZIMConverter::oZIMRoomFullInfo(&info, &room_info);

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(info, error);
    }

    static void on_room_entered(zim_handle handle, struct zim_room_full_info room_info,
                                struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMRoomEnteredCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->room_entered_callbacks_mutex_);
            callback = zim->room_entered_callbacks_[sequence];
            zim->room_entered_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMRoomFullInfo info{};
        ZIMConverter::oZIMRoomFullInfo(&info, &room_info);

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(info, error);
    }

    static void on_room_left(zim_handle handle, const char *room_id, struct zim_error error_info,
                             zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMRoomLeftCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->room_left_callbacks_mutex_);
            callback = zim->room_left_callbacks_[sequence];
            zim->room_left_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(room_id, error);
    }

    static void on_member_queried(zim_handle handle, const char *room_id,
                                  const struct zim_user_info *member_list,
                                  unsigned int member_list_length, const char *next_flag,
                                  struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMRoomMemberQueriedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->member_queried_callbacks_mutex_);
            callback = zim->member_queried_callbacks_[sequence];
            zim->member_queried_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        std::vector<ZIMUserInfo> memberList;
        for (unsigned int i = 0; i < member_list_length; i++) {
            struct zim_user_info user_info = *(member_list + i);
            ZIMUserInfo userInfo{};
            ZIMConverter::oZIMUserInfo(&userInfo, &user_info);
            memberList.emplace_back(userInfo);
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(room_id, memberList, next_flag ? next_flag : "", error);
    }

    static void on_room_members_queried(zim_handle handle, const char *room_id,
                                        const struct zim_room_member_info *user_list,
                                        unsigned int user_list_length,
                                        const struct zim_error_user_info *error_user_list,
                                        unsigned int error_user_list_length, zim_error error_info,
                                        zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMRoomMembersQueriedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->members_queried_callbacks_mutex_);
            callback = zim->members_queried_callbacks_[sequence];
            zim->members_queried_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        std::vector<ZIMRoomMemberInfo> memberList;
        for (unsigned int i = 0; i < user_list_length; i++) {
            struct zim_room_member_info user_info = *(user_list + i);
            ZIMRoomMemberInfo userInfo{};
            ZIMConverter::oZIMRoomMemberInfo(&userInfo, &user_info);
            memberList.emplace_back(userInfo);
        }

        std::vector<ZIMErrorUserInfo> errorUsers;
        for (unsigned int i = 0; i < error_user_list_length; ++i) {
            struct zim_error_user_info error_user = *(error_user_list + i);
            ZIMErrorUserInfo errorUser{};
            ZIMConverter::oZIMErrorUserInfo(&errorUser, &error_user);
            errorUsers.push_back(errorUser);
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(room_id, memberList, errorUsers, error);
    }

    static void on_room_online_member_count_queried(zim_handle handle, const char *room_id,
                                                    unsigned int count, struct zim_error error_info,
                                                    zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMRoomOnlineMemberCountQueriedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->online_count_queried_callbacks_mutex_);
            callback = zim->online_count_queried_callbacks_[sequence];
            zim->online_count_queried_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(room_id, count, error);
    }

    static void on_room_member_joined(zim_handle handle, struct zim_user_info *member_list,
                                      unsigned int member_list_length, const char *room_id) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        if (!zim->event_handler_) {
            return;
        }

        std::vector<ZIMUserInfo> memberList;
        for (unsigned int i = 0; i < member_list_length; i++) {
            struct zim_user_info user_info = *(member_list + i);
            ZIMUserInfo userInfo{};
            ZIMConverter::oZIMUserInfo(&userInfo, &user_info);
            memberList.emplace_back(userInfo);
        }

        zim->event_handler_->onRoomMemberJoined(zim, memberList, room_id);
    }

    static void on_room_member_left(zim_handle handle, struct zim_user_info *member_list,
                                    unsigned int member_list_length, const char *room_id) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        if (!zim->event_handler_) {
            return;
        }

        std::vector<ZIMUserInfo> memberList;
        for (unsigned int i = 0; i < member_list_length; i++) {
            struct zim_user_info user_info = *(member_list + i);
            ZIMUserInfo userInfo{};
            ZIMConverter::oZIMUserInfo(&userInfo, &user_info);
            memberList.emplace_back(userInfo);
        }

        zim->event_handler_->onRoomMemberLeft(zim, memberList, room_id);
    }

    static void on_room_attributes_operated(zim_handle handle, const char *room_id,
                                            const char **error_key_list,
                                            unsigned int error_key_size,
                                            struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMRoomAttributesOperatedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->room_attributes_operated_callbacks_mutex_);
            callback = zim->room_attributes_operated_callbacks_[sequence];
            zim->room_attributes_operated_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        std::vector<std::string> errorKeyList;
        for (unsigned int i = 0; i < error_key_size; i++) {
            errorKeyList.push_back(error_key_list[i]);
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(room_id, errorKeyList, error);
    }

    static void on_room_attributes_queried(zim_handle handle, const char *room_id,
                                           const struct zim_room_attribute *room_attributes,
                                           unsigned int room_attributes_length,
                                           struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMRoomAttributesQueriedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->room_all_attributes_queried_callbacks_mutex_);
            callback = zim->room_all_attributes_queried_callbacks_[sequence];
            zim->room_all_attributes_queried_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        std::unordered_map<std::string, std::string> attributes;

        for (unsigned int i = 0; i < room_attributes_length; i++) {
            std::string key = room_attributes[i].key;
            std::string value = room_attributes[i].value;

            attributes.insert(std::make_pair(key, value));
        }

        callback(room_id, attributes, error);
    }

    static void on_room_attributes_batch_operated(zim_handle handle, const char *room_id,
                                                  struct zim_error error_info,
                                                  zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMRoomAttributesBatchOperatedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->room_attributes_batch_operated_callbacks_mutex_);
            callback = zim->room_attributes_batch_operated_callbacks_[sequence];
            zim->room_attributes_batch_operated_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(room_id, error);
    }

    static void on_room_members_attributes_operated(
        zim_handle handle, const char *room_id,
        const struct zim_room_member_attributes_operated_info *infos, unsigned int infos_length,
        char **error_users, unsigned int error_users_size, struct zim_error error_info,
        zim_sequence sequence) {

        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMRoomMembersAttributesOperatedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(
                zim->room_members_attributes_operated_callbacks_mutex_);
            callback = zim->room_members_attributes_operated_callbacks_[sequence];
            zim->room_members_attributes_operated_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        std::vector<ZIMRoomMemberAttributesOperatedInfo> roomMemberAttributesUpdateInfos;
        std::vector<std::string> errorUsers;

        for (unsigned int i = 0; i < infos_length; ++i) {
            ZIMRoomMemberAttributesOperatedInfo info;
            info.attributesInfo.userID =
                infos[i].attributes_info.user_id ? infos[i].attributes_info.user_id : "";

            for (unsigned int j = 0; j < infos[i].attributes_info.attributes_length; ++j) {
                info.attributesInfo.attributes.emplace(
                    infos[i].attributes_info.attributes[j].key
                        ? infos[i].attributes_info.attributes[j].key
                        : "",
                    infos[i].attributes_info.attributes[j].value
                        ? infos[i].attributes_info.attributes[j].value
                        : "");
            }
            for (unsigned int j = 0; j < infos[i].error_keys_length; ++j) {
                info.errorKeys.emplace_back(infos[i].error_keys[j] ? infos[i].error_keys[j] : "");
            }

            roomMemberAttributesUpdateInfos.emplace_back(info);
        }

        for (unsigned int i = 0; i < error_users_size; ++i) {
            errorUsers.emplace_back(error_users[i]);
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(room_id, roomMemberAttributesUpdateInfos, errorUsers, error);
    }

    static void on_room_members_attributes_queried(
        zim_handle handle, const char *room_id, const struct zim_room_member_attributes_info *infos,
        unsigned int infos_length, struct zim_error error_info, zim_sequence sequence) {

        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMRoomMembersAttributesQueriedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->room_members_attributes_queried_callbacks_mutex_);
            callback = zim->room_members_attributes_queried_callbacks_[sequence];
            zim->room_members_attributes_queried_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        std::vector<ZIMRoomMemberAttributesInfo> infosVec;

        for (unsigned int i = 0; i < infos_length; ++i) {
            ZIMRoomMemberAttributesInfo info;

            info.userID = infos[i].user_id;

            for (unsigned int j = 0; j < infos[i].attributes_length; ++j) {
                info.attributes.emplace(infos[i].attributes[j].key, infos[i].attributes[j].value);
            }

            infosVec.emplace_back(info);
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(room_id, infosVec, error);
    }

    static void
    on_room_members_attributes_paging_queried(zim_handle handle, const char *room_id,
                                              const struct zim_room_member_attributes_info *infos,
                                              unsigned int infos_length, const char *next_flag,
                                              struct zim_error error_info, zim_sequence sequence) {

        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMRoomMemberAttributesListQueriedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(
                zim->room_member_attributes_list_queried_callbacks_mutex_);
            callback = zim->room_member_attributes_list_queried_callbacks_[sequence];
            zim->room_member_attributes_list_queried_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        std::vector<ZIMRoomMemberAttributesInfo> infosVec;

        for (unsigned int i = 0; i < infos_length; ++i) {
            ZIMRoomMemberAttributesInfo info;

            info.userID = infos[i].user_id;

            for (unsigned int j = 0; j < infos[i].attributes_length; ++j) {
                info.attributes.emplace(infos[i].attributes[j].key, infos[i].attributes[j].value);
            }

            infosVec.emplace_back(info);
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(room_id, infosVec, next_flag, error);
    }

    static void on_group_created(zim_handle handle, struct zim_group_full_info group_info,
                                 const struct zim_group_member_info *user_list,
                                 unsigned int user_list_size,
                                 const struct zim_error_user_info *error_user_list,
                                 unsigned int error_user_list_size, struct zim_error error_info,
                                 zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMGroupCreatedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->group_created_callbacks_mutex_);
            callback = zim->group_created_callbacks_[sequence];
            zim->group_created_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMGroupFullInfo groupInfo;
        ZIMConverter::oZIMGroupInfo(&groupInfo, &group_info);

        std::vector<ZIMGroupMemberInfo> userList;
        for (unsigned int i = 0; i < user_list_size; ++i) {
            ZIMGroupMemberInfo memberInfo;
            ZIMConverter::oZIMGroupMemberInfo(&memberInfo, &(user_list[i]));
            userList.push_back(memberInfo);
        }

        std::vector<ZIMErrorUserInfo> errorUsers;
        for (unsigned int i = 0; i < error_user_list_size; ++i) {
            ZIMErrorUserInfo errorUser;
            ZIMConverter::oZIMErrorUserInfo(&errorUser, &(error_user_list[i]));
            errorUsers.push_back(errorUser);
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(groupInfo, userList, errorUsers, error);
    }

    static void on_group_dismissed(zim_handle handle, const char *group_id,
                                   struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMGroupDismissedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->group_dismissed_callbacks_mutex_);
            callback = zim->group_dismissed_callbacks_[sequence];
            zim->group_dismissed_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(group_id, error);
    }

    static void on_group_joined(zim_handle handle, struct zim_group_full_info group_info,
                                struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMGroupJoinedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->group_joined_callbacks_mutex_);
            callback = zim->group_joined_callbacks_[sequence];
            zim->group_joined_callbacks_.erase(sequence);
        }

        ZIMGroupFullInfo groupInfo;
        ZIMConverter::oZIMGroupInfo(&groupInfo, &group_info);

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(groupInfo, error);
    }

    static void on_group_left(zim_handle handle, const char *group_id, struct zim_error error_info,
                              zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMGroupLeftCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->group_left_callbacks_mutex_);
            callback = zim->group_left_callbacks_[sequence];
            zim->group_left_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(group_id, error);
    }

    static void on_group_invited_users(zim_handle handle, const char *group_id,
                                       const struct zim_group_member_info *user_list,
                                       unsigned int user_list_size,
                                       const struct zim_error_user_info *error_user_list,
                                       unsigned int error_user_list_size,
                                       struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMGroupUsersInvitedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->group_users_invited_callbacks_mutex_);
            callback = zim->group_users_invited_callbacks_[sequence];
            zim->group_users_invited_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        std::vector<ZIMGroupMemberInfo> userList;
        for (unsigned int i = 0; i < user_list_size; ++i) {
            ZIMGroupMemberInfo memberInfo;
            ZIMConverter::oZIMGroupMemberInfo(&memberInfo, &(user_list[i]));
            userList.push_back(memberInfo);
        }

        std::vector<ZIMErrorUserInfo> errorUsers;
        for (unsigned int i = 0; i < error_user_list_size; ++i) {
            ZIMErrorUserInfo errorUser;
            ZIMConverter::oZIMErrorUserInfo(&errorUser, &(error_user_list[i]));
            errorUsers.push_back(errorUser);
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(group_id, userList, errorUsers, error);
    }

    static void on_group_member_kicked_out(zim_handle handle, const char *group_id,
                                           const char **user_list, unsigned int user_list_size,
                                           const struct zim_error_user_info *error_user_list,
                                           unsigned int error_user_list_size,
                                           struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMGroupMemberKickedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->group_member_kickedout_callbacks_mutex_);
            callback = zim->group_member_kickedout_callbacks_[sequence];
            zim->group_member_kickedout_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        std::vector<std::string> userList;
        for (unsigned int i = 0; i < user_list_size; ++i) {
            userList.push_back(user_list[i]);
        }

        std::vector<ZIMErrorUserInfo> errorUsers;
        for (unsigned int i = 0; i < error_user_list_size; ++i) {
            ZIMErrorUserInfo errorUser;
            ZIMConverter::oZIMErrorUserInfo(&errorUser, &(error_user_list[i]));
            errorUsers.push_back(errorUser);
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(group_id, userList, errorUsers, error);
    }

    static void on_group_owner_transferred(zim_handle handle, const char *group_id,
                                           const char *user_id, struct zim_error error_info,
                                           zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMGroupOwnerTransferredCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->group_owner_transferred_callbacks_mutex_);
            callback = zim->group_owner_transferred_callbacks_[sequence];
            zim->group_owner_transferred_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(group_id, user_id, error);
    }

    static void on_group_name_updated(zim_handle handle, const char *group_id,
                                      const char *group_name, struct zim_error error_info,
                                      zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMGroupNameUpdatedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->group_name_updated_callbacks_mutex_);
            callback = zim->group_name_updated_callbacks_[sequence];
            zim->group_name_updated_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(group_id, group_name, error);
    }

    static void on_group_avatar_updated(zim_handle handle, const char *group_id,
                                        const char *group_avatar, struct zim_error error_info,
                                        zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMGroupAvatarUrlUpdatedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->group_avatar_updated_callbacks_mutex_);
            callback = zim->group_avatar_updated_callbacks_[sequence];
            zim->group_avatar_updated_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(group_id, group_avatar, error);
    }

    static void on_group_notice_updated(zim_handle handle, const char *group_id,
                                        const char *group_notice, struct zim_error error_info,
                                        zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMGroupNoticeUpdatedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->group_notice_updated_callbacks_mutex_);
            callback = zim->group_notice_updated_callbacks_[sequence];
            zim->group_notice_updated_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(group_id, group_notice, error);
    }

    static void on_group_muted(zim_handle handle, const char *group_id, bool is_mute,
                               enum zim_group_mute_mode mode, long long expired_timestamp,
                               int *role_list, unsigned int role_list_length,
                               struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMGroupMutedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->group_muted_callbacks_mutex_);
            callback = zim->group_muted_callbacks_[sequence];
            zim->group_muted_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        auto info = ZIMGroupMuteInfo();
        info.mode = static_cast<ZIMGroupMuteMode>(mode);
        info.expiredTime = expired_timestamp;
        if (role_list_length > 0) {
            info.roles = std::vector<ZIMGroupMemberRole>(role_list, role_list + role_list_length);
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(group_id, is_mute, info, error);
    }

    static void on_group_member_list_muted(zim_handle handle, const char *group_id, bool is_mute,
                                           int duration, const char **user_id_list,
                                           unsigned int user_id_list_length,
                                           struct zim_error_user_info *error_user_list,
                                           unsigned int error_user_list_length,
                                           struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMGroupMembersMutedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->group_member_list_muted_callbacks_mutex_);
            callback = zim->group_member_list_muted_callbacks_[sequence];
            zim->group_member_list_muted_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        auto vec_user_id =
            std::vector<std::string>(user_id_list, user_id_list + user_id_list_length);

        std::vector<ZIMErrorUserInfo> errorUsers;
        for (unsigned int i = 0; i < error_user_list_length; ++i) {
            ZIMErrorUserInfo errorUser;
            ZIMConverter::oZIMErrorUserInfo(&errorUser, &(error_user_list[i]));
            errorUsers.push_back(errorUser);
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(group_id, is_mute, duration, vec_user_id, errorUsers, error);
    }

    static void on_group_muted_member_list_queried(zim_handle handle, const char *group_id,
                                                   unsigned long long next_flag,
                                                   struct zim_group_member_info *info_list,
                                                   unsigned int info_list_length,
                                                   struct zim_error error_info,
                                                   zim_sequence sequence) {

        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMGroupMemberMutedListQueriedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->group_member_muted_list_queried_callbacks_mutex_);
            callback = zim->group_member_muted_list_queried_callbacks_[sequence];
            zim->group_member_muted_list_queried_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        std::vector<ZIMGroupMemberInfo> groupMemberList;
        for (unsigned int i = 0; i < info_list_length; ++i) {
            ZIMGroupMemberInfo groupMemberInfo;
            ZIMConverter::oZIMGroupMemberInfo(&groupMemberInfo, (info_list + i));
            groupMemberList.push_back(groupMemberInfo);
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(group_id, next_flag, groupMemberList, error);
    }

    static void on_group_info_queried(zim_handle handle, struct zim_group_full_info group_info,
                                      struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMGroupInfoQueriedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->group_info_queried_callbacks_mutex_);
            callback = zim->group_info_queried_callbacks_[sequence];
            zim->group_info_queried_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMGroupFullInfo groupInfo;
        ZIMConverter::oZIMGroupInfo(&groupInfo, &group_info);

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(groupInfo, error);
    }

    static void on_group_attributes_operated(zim_handle handle, const char *group_id,
                                             const char **error_keys, unsigned int error_keys_size,
                                             struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMGroupAttributesOperatedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->group_attributes_operated_callbacks_mutex_);
            callback = zim->group_attributes_operated_callbacks_[sequence];
            zim->group_attributes_operated_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        std::vector<std::string> errorKeys;
        for (unsigned int i = 0; i < error_keys_size; ++i) {
            errorKeys.push_back(error_keys[i]);
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(group_id, errorKeys, error);
    }

    static void on_group_attributes_queried(zim_handle handle, const char *group_id,
                                            struct zim_group_attribute *attributes,
                                            unsigned int attributes_size,
                                            struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMGroupAttributesQueriedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->group_attributes_queried_callbacks_mutex_);
            callback = zim->group_attributes_queried_callbacks_[sequence];
            zim->group_attributes_queried_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        std::unordered_map<std::string, std::string> groupAttributes;
        for (unsigned int i = 0; i < attributes_size; ++i) {
            groupAttributes.emplace(attributes[i].key, attributes[i].value);
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(group_id, groupAttributes, error);
    }

    static void on_group_member_nickname_updated(zim_handle handle, const char *group_id,
                                                 const char *user_id, const char *nickname,
                                                 struct zim_error error_info,
                                                 zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMGroupMemberNicknameUpdatedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->group_member_nickname_set_callbacks_mutex_);
            callback = zim->group_member_nickname_set_callbacks_[sequence];
            zim->group_member_nickname_set_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(group_id, user_id, nickname, error);
    }

    static void on_group_member_role_updated(zim_handle handle, const char *group_id,
                                             const char *user_id, int role,
                                             struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMGroupMemberRoleUpdatedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->group_member_role_set_callbacks_mutex_);
            callback = zim->group_member_role_set_callbacks_[sequence];
            zim->group_member_role_set_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(group_id, user_id, role, error);
    }

    static void on_group_member_info_queried_updated(zim_handle handle, const char *group_id,
                                                     struct zim_group_member_info infos,
                                                     struct zim_error error_info,
                                                     zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMGroupMemberInfoQueriedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->group_member_info_queried_callbacks_mutex_);
            callback = zim->group_member_info_queried_callbacks_[sequence];
            zim->group_member_info_queried_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMGroupMemberInfo memberInfo;
        ZIMConverter::oZIMGroupMemberInfo(&memberInfo, &infos);

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(group_id, memberInfo, error);
    }

    static void on_group_list_queried(zim_handle handle, const struct zim_group *group_list,
                                      unsigned int group_list_size, struct zim_error error_info,
                                      zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMGroupListQueriedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->group_list_queried_callbacks_mutex_);
            callback = zim->group_list_queried_callbacks_[sequence];
            zim->group_list_queried_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        std::vector<ZIMGroup> groupList;
        for (unsigned int i = 0; i < group_list_size; ++i) {
            ZIMGroup groupInfo;
            ZIMConverter::oZIMGroup(&groupInfo, &(group_list[i]));
            groupList.push_back(groupInfo);
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(groupList, error);
    }

    static void on_group_member_list_queried(zim_handle handle, const char *group_id,
                                             const struct zim_group_member_info *group_member_list,
                                             unsigned int group_member_list_size,
                                             unsigned int next_flag, struct zim_error error_info,
                                             zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMGroupMemberListQueriedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->group_member_list_queried_callbacks_mutex_);
            callback = zim->group_member_list_queried_callbacks_[sequence];
            zim->group_member_list_queried_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        std::vector<ZIMGroupMemberInfo> groupMemberList;
        for (unsigned int i = 0; i < group_member_list_size; ++i) {
            ZIMGroupMemberInfo groupMemberInfo;
            ZIMConverter::oZIMGroupMemberInfo(&groupMemberInfo, &(group_member_list[i]));
            groupMemberList.push_back(groupMemberInfo);
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(group_id, groupMemberList, next_flag, error);
    }

    static void on_group_member_count_queried(zim_handle handle, const char *group_id,
                                              unsigned int count, struct zim_error error_info,
                                              zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMGroupMemberCountQueriedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->group_count_queried_callbacks_mutex_);
            callback = zim->group_count_queried_callbacks_[sequence];
            zim->group_count_queried_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(group_id, count, error);
    }

    static void
    on_group_receipt_member_list_queried(zim_handle handle, const char *group_id,
                                         const struct zim_group_member_info *group_member_info_list,
                                         unsigned int list_length, unsigned int next_flag,
                                         struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMGroupMessageReceiptMemberListQueriedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->group_receipt_detail_queried_callbacks_mutex_);
            callback = zim->group_receipt_detail_queried_callbacks_[sequence];
            zim->group_receipt_detail_queried_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }
        std::vector<ZIMGroupMemberInfo> member_list;

        for (unsigned int i = 0; i < list_length; ++i) {
            ZIMGroupMemberInfo member_info;
            member_info.userID = group_member_info_list[i].user_id;
            member_info.userName = group_member_info_list[i].user_name;
            member_info.memberAvatarUrl = group_member_info_list[i].member_avatar_url;
            member_info.memberNickname = group_member_info_list[i].member_nick_name;
            member_info.memberRole = group_member_info_list[i].member_role;

            member_list.push_back(member_info);
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(group_id, member_list, next_flag, error);
    }

    static void on_groups_searched(zim_handle handle,
                                   const struct zim_group_search_info *group_search_info_list,
                                   unsigned int group_search_info_list_length,
                                   unsigned int next_flag, struct zim_error error_info,
                                   zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMGroupsSearchedCallback callback = nullptr;
        {
            std::lock_guard<std::mutex> lock(zim->groups_searched_callbacks_mutex_);
            callback = zim->groups_searched_callbacks_[sequence];
            zim->groups_searched_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        std::vector<ZIMGroupSearchInfo> groupSearchInfoList;

        for (unsigned int i = 0; i < group_search_info_list_length; ++i) {
            ZIMGroupSearchInfo groupSearchInfo;
            groupSearchInfo.groupInfo = {group_search_info_list[i].group_info.group_id,
                                         group_search_info_list[i].group_info.group_name,
                                         group_search_info_list[i].group_info.group_avatar_url};

            for (unsigned int j = 0; j < group_search_info_list[i].user_list_length; ++j) {
                ZIMGroupMemberInfo groupMemberInfo;
                ZIMConverter::oZIMGroupMemberInfo(&groupMemberInfo,
                                                  &group_search_info_list[i].user_list[j]);
                groupSearchInfo.userList.emplace_back(groupMemberInfo);
            }

            groupSearchInfoList.emplace_back(groupSearchInfo);
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(groupSearchInfoList, next_flag, error);
    }

    static void on_group_members_searched(zim_handle handle, const char *group_id,
                                          const struct zim_group_member_info *user_list,
                                          unsigned int user_list_length, unsigned int next_flag,
                                          struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMGroupMembersSearchedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->group_members_searched_callbacks_mutex_);
            callback = zim->group_members_searched_callbacks_[sequence];
            zim->group_members_searched_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        std::vector<ZIMGroupMemberInfo> memberList;

        for (unsigned int i = 0; i < user_list_length; ++i) {
            ZIMGroupMemberInfo memberInfo;
            ZIMConverter::oZIMGroupMemberInfo(&memberInfo, &user_list[i]);

            memberList.emplace_back(memberInfo);
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(group_id, memberList, next_flag, error);
    }

    static void on_conversation_notification_status_set(zim_handle handle,
                                                        const char *conversation_id,
                                                        zim_conversation_type type,
                                                        struct zim_error error_info,
                                                        zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMConversationNotificationStatusSetCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(
                zim->conversation_notification_status_set_callbacks_mutex_);
            callback = zim->conversation_notification_status_set_callbacks_[sequence];
            zim->conversation_notification_status_set_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        callback(conversation_id, (ZIMConversationType)type, error);
    }

    static void on_group_state_updated(zim_handle handle, enum zim_group_state state,
                                       enum zim_group_event event,
                                       struct zim_group_operated_info operated_user,
                                       struct zim_group_full_info group_info) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        if (!zim->event_handler_) {
            return;
        }

        ZIMGroupFullInfo groupInfo;
        ZIMConverter::oZIMGroupInfo(&groupInfo, &group_info);

        ZIMGroupOperatedInfo operatedInfo;
        ZIMConverter::oZIMGroupOperatedInfo(&operatedInfo, &operated_user);

        zim->event_handler_->onGroupStateChanged(zim, (ZIMGroupState)state, (ZIMGroupEvent)event,
                                                 operatedInfo, groupInfo);
    }

    static void on_group_member_state_updated(zim_handle handle, enum zim_group_member_state state,
                                              enum zim_group_member_event event,
                                              const struct zim_group_member_info *members_list,
                                              unsigned int members_list_size,
                                              struct zim_group_operated_info operated_user,
                                              const char *group_id) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        if (!zim->event_handler_) {
            return;
        }

        ZIMGroupOperatedInfo operatedInfo;
        ZIMConverter::oZIMGroupOperatedInfo(&operatedInfo, &operated_user);

        std::vector<ZIMGroupMemberInfo> membersList;
        for (unsigned int i = 0; i < members_list_size; ++i) {
            ZIMGroupMemberInfo memberInfo;
            ZIMConverter::oZIMGroupMemberInfo(&memberInfo, &(members_list[i]));
            membersList.push_back(memberInfo);
        }

        zim->event_handler_->onGroupMemberStateChanged(zim, (ZIMGroupMemberState)state,
                                                       (ZIMGroupMemberEvent)event, membersList,
                                                       operatedInfo, group_id);
    }

    static void on_group_name_update_notified(zim_handle handle, const char *group_name,
                                              struct zim_group_operated_info operated_user,
                                              const char *group_id) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        if (!zim->event_handler_) {
            return;
        }

        ZIMGroupOperatedInfo operatedInfo;
        ZIMConverter::oZIMGroupOperatedInfo(&operatedInfo, &operated_user);

        zim->event_handler_->onGroupNameUpdated(zim, group_name, operatedInfo, group_id);
    }

    static void on_group_avatar_update_notified(zim_handle handle, const char *group_avatar,
                                                struct zim_group_operated_info operated_user,
                                                const char *group_id) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        if (!zim->event_handler_) {
            return;
        }

        ZIMGroupOperatedInfo operatedInfo;
        ZIMConverter::oZIMGroupOperatedInfo(&operatedInfo, &operated_user);

        zim->event_handler_->onGroupAvatarUrlUpdated(zim, group_avatar, operatedInfo, group_id);
    }

    static void on_group_notice_update_notified(zim_handle handle, const char *group_notice,
                                                struct zim_group_operated_info operated_user,
                                                const char *group_id) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        if (!zim->event_handler_) {
            return;
        }

        ZIMGroupOperatedInfo operatedInfo;
        ZIMConverter::oZIMGroupOperatedInfo(&operatedInfo, &operated_user);

        zim->event_handler_->onGroupNoticeUpdated(zim, group_notice, operatedInfo, group_id);
    }

    static void on_group_mute_info_updated_notified(zim_handle handle,
                                                    const struct zim_group_mute_info mute_info,
                                                    struct zim_group_operated_info operated_info,
                                                    const char *group_id) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        if (!zim->event_handler_) {
            return;
        }

        ZIMGroupOperatedInfo operatedInfo;
        ZIMConverter::oZIMGroupOperatedInfo(&operatedInfo, &operated_info);

        ZIMGroupMuteInfo muteInfo;
        ZIMConverter::oZIMGroupMuteInfo(&muteInfo, mute_info);

        zim->event_handler_->onGroupMutedInfoUpdated(zim, muteInfo, operatedInfo, group_id);
    }

    static void on_group_attributes_updated(zim_handle handle,
                                            const struct zim_group_attributes_update_info *infos,
                                            unsigned int infos_length,
                                            struct zim_group_operated_info operated_user,
                                            const char *group_id) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        if (!zim->event_handler_) {
            return;
        }

        std::vector<ZIMGroupAttributesUpdateInfo> updateInfos;
        for (unsigned int i = 0; i < infos_length; i++) {
            ZIMGroupAttributesUpdateInfo info;
            info.action = (ZIMGroupAttributesUpdateAction)infos[i].action;
            for (unsigned int j = 0; j < infos[i].group_attributes_length; j++) {
                info.groupAttributes.insert(std::make_pair(infos[i].group_attributes[j].key,
                                                           infos[i].group_attributes[j].value));
            }

            updateInfos.emplace_back(info);
        }

        ZIMGroupOperatedInfo operatedInfo;
        ZIMConverter::oZIMGroupOperatedInfo(&operatedInfo, &operated_user);

        zim->event_handler_->onGroupAttributesUpdated(zim, updateInfos, operatedInfo, group_id);
    }

    static void on_group_member_info_updated(zim_handle handle,
                                             const struct zim_group_member_info *group_member_info,
                                             unsigned int group_member_info_size,
                                             struct zim_group_operated_info operated_user,
                                             const char *group_id) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        if (!zim->event_handler_) {
            return;
        }

        ZIMGroupOperatedInfo operatedInfo;
        ZIMConverter::oZIMGroupOperatedInfo(&operatedInfo, &operated_user);

        std::vector<ZIMGroupMemberInfo> memberList;
        for (unsigned int i = 0; i < group_member_info_size; ++i) {
            ZIMGroupMemberInfo memberInfo;
            ZIMConverter::oZIMGroupMemberInfo(&memberInfo, &(group_member_info[i]));

            memberList.push_back(memberInfo);
        }

        zim->event_handler_->onGroupMemberInfoUpdated(zim, memberList, operatedInfo, group_id);
    }

    //Call Invitate
    static void on_call_invitation_received(zim_handle handle,
                                            struct zim_call_invitation_received_info info,
                                            const char *callID) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        if (!zim->event_handler_) {
            return;
        }

        ZIMCallInvitationReceivedInfo receivedInfo{};
        ZIMConverter::oZIMCallInvitationReceivedInfo(receivedInfo, &info);

        zim->event_handler_->onCallInvitationReceived(zim, receivedInfo, callID);
    }

    static void on_call_invitation_cancelled(zim_handle handle,
                                             struct zim_call_invitation_cancelled_info info,
                                             const char *call_id) {

        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        if (!zim->event_handler_) {
            return;
        }
        ZIMCallInvitationCancelledInfo cancelledInfo{};
        ZIMConverter::oZIMCallInvitationCancelledInfo(cancelledInfo, &info);

        zim->event_handler_->onCallInvitationCancelled(zim, cancelledInfo, call_id);
    }

    static void on_call_invitation_accepted(zim_handle handle,
                                            struct zim_call_invitation_accepted_info info,
                                            const char *call_id) {

        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        if (!zim->event_handler_) {
            return;
        }

        ZIMCallInvitationAcceptedInfo acceptedInfo{};

        ZIMConverter::oZIMCallInvitationAcceptedInfo(acceptedInfo, &info);
        zim->event_handler_->onCallInvitationAccepted(zim, acceptedInfo, call_id);
    }

    static void on_call_invitation_rejected(zim_handle handle,
                                            struct zim_call_invitation_rejected_info info,
                                            const char *call_id) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        if (!zim->event_handler_) {
            return;
        }

        ZIMCallInvitationRejectedInfo rejectedInfo{};
        ZIMConverter::oZIMCallInvitationRejectedInfo(rejectedInfo, &info);

        zim->event_handler_->onCallInvitationRejected(zim, rejectedInfo, call_id);
    }

    static void on_call_invitation_timeout(zim_handle handle,
                                           struct zim_call_invitation_timeout_info info,
                                           const char *call_id) {

        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        if (!zim->event_handler_) {
            return;
        }
        ZIMCallInvitationTimeoutInfo timeoutInfo{};
        ZIMConverter::oZIMCallInvitationTimeoutInfo(timeoutInfo, &info);
        if (info.also_invoke_discard_method) {
            zim->event_handler_->onCallInvitationTimeout(zim, call_id);
        }
        zim->event_handler_->onCallInvitationTimeout(zim, timeoutInfo, call_id);
    }

    static void on_call_invitees_answered_timeout(zim_handle handle, const char **invitees,
                                                  unsigned int invitees_length,
                                                  const char *call_id) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        if (!zim->event_handler_) {
            return;
        }

        std::vector<std::string> error_invitees;
        for (unsigned int i = 0; i < invitees_length; i++) {
            std::string error_invitee = invitees[i];
            error_invitees.emplace_back(error_invitee);
        }

        zim->event_handler_->onCallInviteesAnsweredTimeout(zim, error_invitees, call_id);
    }

    static void on_call_invitation_created(zim_handle handle,
                                           struct zim_call_invitation_created_info info,
                                           const char *call_id) {
        ZIMImpl *zim = GetZIMImplInstance(handle);
        if (!zim) {
            return;
        }

        if (!zim->event_handler_) {
            return;
        }

        ZIMCallInvitationCreatedInfo callInvitationCreatedInfo{};
        ZIMConverter::oZIMCallInvitationCreatedInfo(callInvitationCreatedInfo, &info);
        zim->event_handler_->onCallInvitationCreated(zim, callInvitationCreatedInfo, call_id);
    }

    static void on_call_invitation_ended(zim_handle handle,
                                         struct zim_call_invitation_ended_info info,
                                         const char *call_id) {
        ZIMImpl *zim = GetZIMImplInstance(handle);
        if (!zim) {
            return;
        }

        if (!zim->event_handler_) {
            return;
        }

        ZIMCallInvitationEndedInfo callInvitationEndedInfo{};
        ZIMConverter::oZIMCallInvitationEndedInfo(callInvitationEndedInfo, &info);

        zim->event_handler_->onCallInvitationEnded(zim, callInvitationEndedInfo, call_id);
    }

    static void on_call_user_state_changed(zim_handle handle,
                                           struct zim_event_call_user_state_changed_info info) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        if (!zim->event_handler_) {
            return;
        }

        ZIMCallUserStateChangeInfo callUserStateChangedInfo{};
        ZIMConverter::oZIMCallUserStateChangedInfo(callUserStateChangedInfo, &info);
        std::string callID = info.call_id;
        zim->event_handler_->onCallUserStateChanged(zim, callUserStateChangedInfo, callID);
    }

    static void on_call_invitation_sent(zim_handle handle, const char *call_id,
                                        struct zim_call_invitation_sent_info invitation_info,
                                        struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }
        ZIMCallInvitationSentCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->call_invitation_sent_callbacks_mutex_);
            callback = zim->call_invitation_sent_callbacks_[sequence];
            zim->call_invitation_sent_callbacks_.erase(sequence);
        }
        if (!callback) {
            return;
        }

        ZIMCallInvitationSentInfo info{};

        ZIMConverter::oZIMCallInvitationSentInfo(info, &invitation_info);

        ZIMError errorInfo{};
        ZIMConverter::oZIMError(&errorInfo, &error_info);
        callback(call_id, info, errorInfo);
    }

    static void on_call_join_sent_callback(zim_handle handle, const char *call_id,
                                           struct zim_call_join_sent_info info,
                                           struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMCallJoinSentCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->call_join_sent_callbacks_mutex_);
            callback = zim->call_join_sent_callbacks_[sequence];
            zim->call_join_sent_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMCallJoinSentInfo callJoinSentInfo;
        ZIMConverter::oZIMCallJoinSentInfo(callJoinSentInfo, &info);

        ZIMError errorInfo;
        ZIMConverter::oZIMError(&errorInfo, &error_info);

        callback(call_id, callJoinSentInfo, errorInfo);
    }

    static void on_call_cancel_sent(zim_handle handle, const char *call_id,
                                    const char **error_invitees, unsigned int error_invitees_length,
                                    struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMCallCancelSentCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->call_cancel_sent_callbacks_mutex_);
            callback = zim->call_cancel_sent_callbacks_[sequence];
            zim->call_cancel_sent_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        std::vector<std::string> errorInvitees;
        if (error_invitees_length != 0) {
            errorInvitees.resize(error_invitees_length);
            for (unsigned int i = 0; i < error_invitees_length; i++) {
                errorInvitees.at(i) = error_invitees[i];
            }
        }

        ZIMError errorInfo;
        ZIMConverter::oZIMError(&errorInfo, &error_info);

        callback(call_id, errorInvitees, errorInfo);
    }

    static void on_call_acceptance_sent(zim_handle handle, const char *call_id,
                                        struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMCallAcceptanceSentCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->call_acceptance_sent_callbacks_mutex_);
            callback = zim->call_acceptance_sent_callbacks_[sequence];
            zim->call_acceptance_sent_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError errorInfo;
        ZIMConverter::oZIMError(&errorInfo, &error_info);

        callback(call_id, errorInfo);
    }

    static void on_call_rejection_sent(zim_handle handle, const char *call_id,
                                       struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMCallRejectionSentCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->call_rejection_sent_callbacks_mutex_);
            callback = zim->call_rejection_sent_callbacks_[sequence];
            zim->call_rejection_sent_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError errorInfo;
        ZIMConverter::oZIMError(&errorInfo, &error_info);

        callback(call_id, errorInfo);
    }

    static void on_calling_invitation_sent_callback(zim_handle handle, const char *call_id,
                                                    struct zim_calling_invitation_sent_info info,
                                                    struct zim_error error_info,
                                                    zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMCallingInvitationSentCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->calling_invitation_sent_callbacks_mutex_);
            callback = zim->calling_invitation_sent_callbacks_[sequence];
            zim->calling_invitation_sent_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMCallingInvitationSentInfo callingInvitationSentInfo{};

        ZIMConverter::oZIMCallingInvitationSentInfo(callingInvitationSentInfo, &info);

        ZIMError errorInfo;
        ZIMConverter::oZIMError(&errorInfo, &error_info);

        callback(call_id, callingInvitationSentInfo, errorInfo);
    }

    static void on_call_quit_sent_callback(zim_handle handle, const char *call_id,
                                           struct zim_call_quit_sent_info info,
                                           struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMCallQuitSentCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->call_quit_sent_callbacks_mutex_);
            callback = zim->call_quit_sent_callbacks_[sequence];
            zim->call_quit_sent_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMCallQuitSentInfo callQuitSentInfo;
        ZIMConverter::oZIMCallQuitSentInfo(callQuitSentInfo, &info);

        ZIMError errorInfo;
        ZIMConverter::oZIMError(&errorInfo, &error_info);

        callback(call_id, callQuitSentInfo, errorInfo);
    }

    static void on_call_end_sent_callback(zim_handle handle, const char *call_id,
                                          struct zim_call_end_sent_info info,
                                          struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMCallEndSentCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->call_end_sent_callbacks_mutex_);
            callback = zim->call_end_sent_callbacks_[sequence];
            zim->call_end_sent_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMCallEndedSentInfo callEndSentInfo{};
        callEndSentInfo.createTime = info.create_time;
        callEndSentInfo.acceptTime = info.accept_time;
        callEndSentInfo.endTime = info.end_time;

        ZIMError errorInfo;
        ZIMConverter::oZIMError(&errorInfo, &error_info);

        callback(call_id, callEndSentInfo, errorInfo);
    }

    static void on_call_list_queried_callback(zim_handle handle, struct zim_call_info *call_list,
                                              unsigned int call_list_length, long long next_flag,
                                              struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMCallInvitationListQueriedCallback callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->query_call_list_callbacks_mutex_);
            callback = zim->query_call_list_callbacks_[sequence];
            zim->query_call_list_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        std::vector<ZIMCallInfo> callInfos;
        for (unsigned int i = 0; i < call_list_length; ++i) {
            ZIMCallInfo callInfo;
            auto c_call = call_list[i];
            ZIMConverter::oZIMCallInfo(&callInfo, &c_call);
            callInfos.push_back(callInfo);
        }

        ZIMError errorInfo;
        ZIMConverter::oZIMError(&errorInfo, &error_info);

        callback(callInfos, next_flag, errorInfo);
    }

    static void on_send_media_file_progress(zim_handle handle, struct zim_message message,
                                            long long currentSize, long long totalSize,
                                            zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMMediaUploadingProgress callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->media_send_progress_callbacks_mutex_);
            callback = zim->media_send_progress_callbacks_[sequence];
        }

        std::shared_ptr<ZIMMediaMessageSendNotification> notification = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->send_media_message_notification_callbacks_mutex_);
            if (zim->send_media_message_notification_callbacks_.find(sequence) !=
                zim->send_media_message_notification_callbacks_.end()) {
                notification = zim->send_media_message_notification_callbacks_[sequence];
            }
        }

        std::shared_ptr<ZIMMessage> oMessage = nullptr;

        bool is_find_in_map = false;
        {
            std::lock_guard<std::mutex> lock(zim->message_obj_map_mutex_);
            if (zim->message_obj_map_.find(sequence) != zim->message_obj_map_.end()) {
                oMessage = zim->message_obj_map_[sequence];
                zim->message_obj_map_.erase(sequence);
                is_find_in_map = true;
            }
        }

        if (!is_find_in_map) {
            oMessage = ZIMConverter::oZIMMessage(&message);
        }

        if (callback) {
            callback(std::static_pointer_cast<ZIMMediaMessage>(oMessage), currentSize, totalSize);
        } else {
            if (notification) {
                if (notification->onMediaUploadingProgress) {
                    notification->onMediaUploadingProgress(
                        std::static_pointer_cast<ZIMMediaMessage>(oMessage), currentSize,
                        totalSize);
                }
            }
        }
    }

    static void on_download_media_file_progress(zim_handle handle, struct zim_message message,
                                                long long currentSize, long long totalSize,
                                                zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMMediaDownloadingProgress callback = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->media_downloading_progress_callbacks_mutex_);
            callback = zim->media_downloading_progress_callbacks_[sequence];
        }

        if (!callback) {
            return;
        }

        std::shared_ptr<ZIMMessage> oMessage = ZIMConverter::oZIMMessage(&message);
        callback(std::static_pointer_cast<ZIMMediaMessage>(oMessage), currentSize, totalSize);
    }

    static void on_downloaded_file_message(zim_handle handle, struct zim_message message,
                                           struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMMediaDownloadedCallback callback = nullptr;
        std::shared_ptr<ZIMMessage> oMessage = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->media_download_callbacks_mutex_);
            callback = zim->media_download_callbacks_[sequence];
            zim->media_download_callbacks_.erase(sequence);
            zim->media_downloading_progress_callbacks_.erase(sequence);

            {
                std::lock_guard<std::mutex> lock_2(zim->message_obj_map_mutex_);
                if (zim->message_obj_map_.find(sequence) != zim->message_obj_map_.end()) {

                    oMessage = zim->message_obj_map_[sequence];
                    zim->message_obj_map_.erase(sequence);
                }
            }
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        if (oMessage) {
            ZIMConverter::cZIMMessage(oMessage, message);
        } else {
            oMessage = ZIMConverter::oZIMMessage(&message);
        }

        callback(std::static_pointer_cast<ZIMMediaMessage>(oMessage), error);
    }

    static void on_message_inserted(zim_handle handle, struct zim_message message,
                                    struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMMessageInsertedCallback callback = nullptr;
        std::shared_ptr<ZIMMessage> oMessage = nullptr;

        {
            std::lock_guard<std::mutex> lock(zim->message_inserted_callbacks_mutex_);
            callback = zim->message_inserted_callbacks_[sequence];
            zim->message_inserted_callbacks_.erase(sequence);

            {
                std::lock_guard<std::mutex> lock_2(zim->message_obj_map_mutex_);
                if (zim->message_obj_map_.find(sequence) != zim->message_obj_map_.end()) {

                    oMessage = zim->message_obj_map_[sequence];
                    zim->message_obj_map_.erase(sequence);
                }
            }
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        if (oMessage) {
            ZIMConverter::cZIMMessage(oMessage, message);
        } else {
            oMessage = ZIMConverter::oZIMMessage(&message);
        }

        callback(oMessage, error);
    }

    static void
    on_message_receipt_changed(zim_handle handle, bool is_conv_read,
                               const struct zim_message_receipt_info *message_read_receipt_list,
                               unsigned int list_length) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        if (!zim->event_handler_) {
            return;
        }

        auto infos =
            ZIMConverter::oZIMMessageReceiptInfoList(message_read_receipt_list, list_length);
        if (is_conv_read) {
            zim->event_handler_->onConversationMessageReceiptChanged(zim, infos);
        } else {
            zim->event_handler_->onMessageReceiptChanged(zim, infos);
        }
    }

    static void on_message_revoke_received(zim_handle handle,
                                           const struct zim_message *message_list,
                                           unsigned int message_list_length) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        if (!zim->event_handler_) {
            return;
        }

        std::vector<std::shared_ptr<ZIMRevokeMessage>> messageList;
        for (unsigned int i = 0; i < message_list_length; i++) {
            struct zim_message sMessage = *(message_list + i);
            std::shared_ptr<ZIMRevokeMessage> message =
                std::static_pointer_cast<ZIMRevokeMessage>(ZIMConverter::oZIMMessage(&sMessage));
            messageList.emplace_back(std::move(message));
        }

        zim->event_handler_->onMessageRevokeReceived(zim, messageList);
    }

    static void on_broadcast_message_received(zim_handle handle, struct zim_message message) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        if (!zim->event_handler_) {
            return;
        }

        zim->event_handler_->onBroadcastMessageReceived(zim, ZIMConverter::oZIMMessage(&message));
    }

    static void on_message_deleted_event(zim_handle handle, const char *conversation_id,
                                         enum zim_conversation_type conversation_type,
                                         bool is_all_message,
                                         const struct zim_message *message_list,
                                         unsigned int message_list_length,
                                         enum zim_message_delete_type message_delete_type) {

        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        if (!zim->event_handler_) {
            return;
        }

        std::vector<std::shared_ptr<ZIMMessage>> messageList;
        for (unsigned int i = 0; i < message_list_length; i++) {
            struct zim_message sMessage = *(message_list + i);
            auto message = ZIMConverter::oZIMMessage(&sMessage);
            messageList.emplace_back(std::move(message));
        }

        zim::ZIMMessageDeletedInfo info;
        info.conversationID = conversation_id;
        info.conversationType = static_cast<ZIMConversationType>(conversation_type);
        info.isDeleteConversationAllMessage = is_all_message;
        info.messageList = std::move(messageList);
        info.messageDeleteType = static_cast<ZIMMessageDeleteType>(message_delete_type);

        zim->event_handler_->onMessageDeleted(zim, info);
    }

    static void
    on_message_sent_status_changed(zim_handle handle,
                                   const struct zim_message_sent_status_change_info *info_list,
                                   unsigned int list_length) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        if (!zim->event_handler_) {
            return;
        }

        std::vector<ZIMMessageSentStatusChangeInfo> messageSentStatusChangeInfoList;
        for (unsigned int i = 0; i < list_length; i++) {
            struct zim_message_sent_status_change_info sMessageSentStatusChangeInfo =
                *(info_list + i);
            ZIMMessageSentStatusChangeInfo oInfo{};
            ZIMConverter::oZIMMessageSentStatusChangeInfo(oInfo, sMessageSentStatusChangeInfo);
            messageSentStatusChangeInfoList.emplace_back(std::move(oInfo));
        }

        zim->event_handler_->onMessageSentStatusChanged(zim, messageSentStatusChangeInfoList);
    }

    static void on_blacklist_changed_event(zim_handle handle,
                                           const struct zim_user_info *user_infos,
                                           enum zim_blacklist_change_action action,
                                           unsigned int user_info_list_length) {
        ZIMImpl *zim = GetZIMImplInstance(handle);
        if (!zim) {
            return;
        }
        if (!zim->event_handler_) {
            return;
        }
        std::vector<ZIMUserInfo> userInfoList;
        for (unsigned int i = 0; i < user_info_list_length; i++) {
            struct zim_user_info user_info = user_infos[i];
            ZIMUserInfo userInfo{};
            ZIMConverter::oZIMUserInfo(&userInfo, &user_info);
            userInfoList.emplace_back(userInfo);
        }
        zim->event_handler_->onBlacklistChanged(zim, userInfoList,
                                                (ZIMBlacklistChangeAction)action);
    }

    static void
    on_blacklist_users_added_callback(zim_handle handle,
                                      const struct zim_error_user_info *error_user_info_list,
                                      unsigned int error_user_info_list_length,
                                      const struct zim_error error_info, zim_sequence sequence) {

        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMBlacklistUsersAddedCallback callback = nullptr;
        {
            std::lock_guard<std::mutex> lock(zim->blacklist_users_added_callbacks_mutex_);
            callback = zim->blacklist_users_added_callbacks_[sequence];
            zim->blacklist_users_added_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        std::vector<ZIMErrorUserInfo> errorUsers;
        for (unsigned int i = 0; i < error_user_info_list_length; ++i) {
            ZIMErrorUserInfo errorUser;
            ZIMConverter::oZIMErrorUserInfo(&errorUser, &(error_user_info_list[i]));
            errorUsers.push_back(errorUser);
        }

        callback(errorUsers, error);
    }

    static void
    on_blacklist_users_remove_callback(zim_handle handle,
                                       const struct zim_error_user_info *error_user_info_list,
                                       unsigned int error_user_info_list_length,
                                       const struct zim_error error_info, zim_sequence sequence) {

        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMBlacklistUsersRemovedCallback callback = nullptr;
        {
            std::lock_guard<std::mutex> lock(zim->blacklist_users_removed_callbacks_mutex_);
            callback = zim->blacklist_users_removed_callbacks_[sequence];
            zim->blacklist_users_removed_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        std::vector<ZIMErrorUserInfo> errorUsers;
        for (unsigned int i = 0; i < error_user_info_list_length; ++i) {
            ZIMErrorUserInfo errorUser;
            ZIMConverter::oZIMErrorUserInfo(&errorUser, &(error_user_info_list[i]));
            errorUsers.push_back(errorUser);
        }

        callback(errorUsers, error);
    }

    static void on_blacklist_queried_callback(zim_handle handle,
                                              const struct zim_user_info *blacklist_user_infos,
                                              unsigned int blacklist_user_infos_length,
                                              unsigned int next_flag,
                                              const struct zim_error error_info,
                                              zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMBlacklistQueriedCallback callback = nullptr;
        {
            std::lock_guard<std::mutex> lock(zim->blacklist_queried_callbacks_mutex_);
            callback = zim->blacklist_queried_callbacks_[sequence];
            zim->blacklist_queried_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        std::vector<ZIMUserInfo> userInfoList;
        for (unsigned int i = 0; i < blacklist_user_infos_length; ++i) {
            struct zim_user_info user_info = *(blacklist_user_infos + i);
            ZIMUserInfo userInfo;
            ZIMConverter::oZIMUserInfo(&userInfo, &user_info);
            userInfoList.push_back(userInfo);
        }

        callback(userInfoList, next_flag, error);
    }

    static void on_check_user_is_in_blacklist_callback(zim_handle handle, bool is_user_in_blacklist,
                                                       const struct zim_error error_info,
                                                       zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMBlacklistCheckedCallback callback = nullptr;
        {
            std::lock_guard<std::mutex> lock(zim->blacklist_checked_callbacks_mutex_);
            callback = zim->blacklist_checked_callbacks_[sequence];
            zim->blacklist_checked_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }

        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;
        callback(is_user_in_blacklist, error);
    }

    static void
    on_friends_searched_callback(zim_handle handle, const struct zim_friend_info *friend_infos,
                                 unsigned int friend_info_list_length, unsigned int next_flag,
                                 const struct zim_error error_info, zim_sequence sequence) {

        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMFriendsSearchedCallback callback = nullptr;
        {
            std::lock_guard<std::mutex> lock(zim->friend_searched_callbacks_mutex_);
            callback = zim->friend_searched_callbacks_[sequence];
            zim->friend_searched_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }
        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;
        std::vector<ZIMFriendInfo> zim_friend_infos;

        for (unsigned int i = 0; i < friend_info_list_length; ++i) {
            struct zim_friend_info friend_info = *(friend_infos + i);
            ZIMFriendInfo friendInfo{};
            ZIMConverter::oZIMFriendInfo(friendInfo, &friend_info);
            zim_friend_infos.push_back(friendInfo);
        }

        callback(zim_friend_infos, (int32_t)next_flag, error);
    }

    static void on_friend_info_updated_event(zim_handle handle,
                                             const struct zim_friend_info *friend_info_list,
                                             unsigned int friend_info_list_length) {
        ZIMImpl *zim = GetZIMImplInstance(handle);
        if (!zim) {
            return;
        }
        if (!zim->event_handler_) {
            return;
        }

        std::vector<ZIMFriendInfo> zim_friend_infos;

        for (unsigned int i = 0; i < friend_info_list_length; ++i) {
            struct zim_friend_info friend_info = *(friend_info_list + i);
            ZIMFriendInfo friendInfo{};
            ZIMConverter::oZIMFriendInfo(friendInfo, &friend_info);
            zim_friend_infos.push_back(friendInfo);
        }
        zim->event_handler_->onFriendInfoUpdated(zim, zim_friend_infos);
    }

    static void on_friend_list_changed_event(zim_handle handle,
                                             const struct zim_friend_info *friend_info_list,
                                             unsigned int friend_info_list_length,
                                             unsigned int action) {
        ZIMImpl *zim = GetZIMImplInstance(handle);
        if (!zim) {
            return;
        }
        if (!zim->event_handler_) {
            return;
        }

        std::vector<ZIMFriendInfo> zim_friend_infos;

        for (unsigned int i = 0; i < friend_info_list_length; ++i) {
            struct zim_friend_info friend_info = *(friend_info_list + i);
            ZIMFriendInfo friendInfo{};
            ZIMConverter::oZIMFriendInfo(friendInfo, &friend_info);
            zim_friend_infos.push_back(friendInfo);
        }
        ZIMFriendListChangeAction new_action = (ZIMFriendListChangeAction)action;
        zim->event_handler_->onFriendListChanged(zim, zim_friend_infos, new_action);
    }

    static void on_friend_application_updated_event(
        zim_handle handle, const struct zim_friend_application_info *friend_application_info_list,
        unsigned int friend_application_info_list_length) {
        ZIMImpl *zim = GetZIMImplInstance(handle);
        if (!zim) {
            return;
        }
        if (!zim->event_handler_) {
            return;
        }

        std::vector<ZIMFriendApplicationInfo> zim_friend_application_infos;

        for (unsigned int i = 0; i < friend_application_info_list_length; ++i) {
            struct zim_friend_application_info friend_application_info =
                *(friend_application_info_list + i);
            ZIMFriendApplicationInfo friendApplicationInfo{};
            ZIMConverter::oZIMFriendApplicationInfo(friendApplicationInfo,
                                                    &friend_application_info);
            zim_friend_application_infos.push_back(friendApplicationInfo);
        }

        zim->event_handler_->onFriendApplicationUpdated(zim, zim_friend_application_infos);
    }

    static void on_friend_application_changed_event(
        zim_handle handle, enum zim_friend_application_change_action action,
        const struct zim_friend_application_info *friend_application_info_list,
        unsigned int friend_application_info_list_length) {

        ZIMImpl *zim = GetZIMImplInstance(handle);
        if (!zim) {
            return;
        }
        if (!zim->event_handler_) {
            return;
        }

        std::vector<ZIMFriendApplicationInfo> zim_friend_application_infos;

        for (unsigned int i = 0; i < friend_application_info_list_length; ++i) {
            struct zim_friend_application_info friend_application_info =
                *(friend_application_info_list + i);
            ZIMFriendApplicationInfo friendApplicationInfo{};
            ZIMConverter::oZIMFriendApplicationInfo(friendApplicationInfo,
                                                    &friend_application_info);
            zim_friend_application_infos.push_back(friendApplicationInfo);
        }

        ZIMFriendApplicationListChangeAction new_action =
            (ZIMFriendApplicationListChangeAction)action;
        zim->event_handler_->onFriendApplicationListChanged(zim, zim_friend_application_infos,
                                                            new_action);
    }

    static void on_friend_added_callback(zim_handle handle,
                                         const struct zim_friend_info friend_info,
                                         struct zim_error error_info, zim_sequence sequence) {

        ZIMImpl *zim = GetZIMImplInstance(handle);

        if (!zim) {
            return;
        }

        ZIMFriendAddedCallback callback = nullptr;
        {
            std::lock_guard<std::mutex> lock(zim->add_friend_application_callbacks_mutex_);
            callback = zim->add_friend_application_callbacks_[sequence];
            zim->add_friend_application_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }
        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;
        ZIMFriendInfo friendInfo{};
        ZIMConverter::oZIMFriendInfo(friendInfo, &friend_info);
        callback(friendInfo, error);
    }

    static void on_friend_application_sent_callback(
        zim_handle handle, const struct zim_friend_application_info friend_application_info,
        const struct zim_error error_info, zim_sequence sequence) {

        ZIMImpl *zim = GetZIMImplInstance(handle);
        if (!zim) {
            return;
        }

        ZIMFriendApplicationSentCallback callback = nullptr;
        {
            std::lock_guard<std::mutex> lock(zim->send_friend_application_callbacks_mutex_);
            callback = zim->send_friend_application_callbacks_[sequence];
            zim->send_friend_application_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }
        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        ZIMFriendApplicationInfo friendApplicationInfo{};
        ZIMConverter::oZIMFriendApplicationInfo(friendApplicationInfo, &friend_application_info);
        callback(friendApplicationInfo, error);
    }

    static void on_delete_friend_callback(zim_handle handle,
                                          const struct zim_error_user_info *error_user_list,
                                          unsigned int error_user_list_length,
                                          const struct zim_error error_info,
                                          zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);
        if (!zim) {
            return;
        }

        ZIMFriendsDeletedCallback callback = nullptr;
        {
            std::lock_guard<std::mutex> lock(zim->friend_deleted_callbacks_mutex_);
            callback = zim->friend_deleted_callbacks_[sequence];
            zim->friend_deleted_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }
        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        std::vector<ZIMErrorUserInfo> errorUsers;
        for (unsigned int i = 0; i < error_user_list_length; ++i) {
            ZIMErrorUserInfo errorUser{};
            ZIMConverter::oZIMErrorUserInfo(&errorUser, &(error_user_list[i]));
            errorUsers.push_back(errorUser);
        }
        callback(errorUsers, error);
    }

    static void on_check_friend_relation_callback(
        zim_handle handle, const struct zim_friend_relation_info *friend_relation_infos,
        unsigned int friend_relation_info_length, const struct zim_error_user_info *error_user_list,
        unsigned int error_user_list_length, const struct zim_error error_info,
        zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);
        if (!zim) {
            return;
        }

        ZIMFriendsRelationCheckedCallback callback = nullptr;
        {
            std::lock_guard<std::mutex> lock(zim->friend_relation_checked_callbacks_mutex_);
            callback = zim->friend_relation_checked_callbacks_[sequence];
            zim->friend_relation_checked_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }
        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        std::vector<ZIMFriendRelationInfo> relationInfos;
        for (unsigned int i = 0; i < friend_relation_info_length; ++i) {
            ZIMFriendRelationInfo relationInfo{};
            ZIMConverter::oZIMFriendRelationInfo(&relationInfo, &(friend_relation_infos[i]));
            relationInfos.push_back(relationInfo);
        }

        std::vector<ZIMErrorUserInfo> errorUsers;
        for (unsigned int i = 0; i < error_user_list_length; ++i) {
            ZIMErrorUserInfo errorUser{};
            ZIMConverter::oZIMErrorUserInfo(&errorUser, &(error_user_list[i]));
            errorUsers.push_back(errorUser);
        }

        callback(relationInfos, errorUsers, error);
    }

    static void on_update_friend_alias_callback(zim_handle handle,
                                                const struct zim_friend_info friend_info,
                                                const struct zim_error error_info,
                                                zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);
        if (!zim) {
            return;
        }

        ZIMFriendAliasUpdatedCallback callback = nullptr;
        {
            std::lock_guard<std::mutex> lock(zim->friends_alias_updated_callbacks_mutex_);
            callback = zim->friends_alias_updated_callbacks_[sequence];
            zim->friends_alias_updated_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }
        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        ZIMFriendInfo zimFriendInfo{};
        ZIMConverter::oZIMFriendInfo(zimFriendInfo, &friend_info);
        callback(zimFriendInfo, error);
    }

    static void on_update_friend_attributes_callback(zim_handle handle,
                                                     const struct zim_friend_info friend_info,
                                                     const struct zim_error error_info,
                                                     zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);
        if (!zim) {
            return;
        }

        ZIMFriendAttributesUpdatedCallback callback = nullptr;
        {
            std::lock_guard<std::mutex> lock(zim->friends_attributes_updated_callbacks_mutex_);
            callback = zim->friends_attributes_updated_callbacks_[sequence];
            zim->friends_attributes_updated_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }
        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        ZIMFriendInfo zimFriendInfo{};
        ZIMConverter::oZIMFriendInfo(zimFriendInfo, &friend_info);
        callback(zimFriendInfo, error);
    }

    static void on_friends_info_queried_callback(zim_handle handle,
                                                 const struct zim_friend_info *friend_info_list,
                                                 unsigned int friend_info_list_length,
                                                 const struct zim_error_user_info *error_user_list,
                                                 unsigned int error_user_list_length,
                                                 const struct zim_error error_info,
                                                 zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);
        if (!zim) {
            return;
        }

        ZIMFriendsInfoQueriedCallback callback = nullptr;
        {
            std::lock_guard<std::mutex> lock(zim->friends_info_queried_callbacks_mutex_);
            callback = zim->friends_info_queried_callbacks_[sequence];
            zim->friends_info_queried_callbacks_.erase(sequence);
        }

        if (!callback) {
            return;
        }
        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        std::vector<ZIMFriendInfo> zim_friend_infos;

        for (unsigned int i = 0; i < friend_info_list_length; ++i) {
            struct zim_friend_info friend_info = *(friend_info_list + i);
            ZIMFriendInfo friendInfo{};
            ZIMConverter::oZIMFriendInfo(friendInfo, &friend_info);
            zim_friend_infos.push_back(friendInfo);
        }

        std::vector<ZIMErrorUserInfo> errorUsers;
        for (unsigned int i = 0; i < error_user_list_length; ++i) {
            ZIMErrorUserInfo errorUser{};
            ZIMConverter::oZIMErrorUserInfo(&errorUser, &(error_user_list[i]));
            errorUsers.push_back(errorUser);
        }

        callback(zim_friend_infos, errorUsers, error);
    }

    static void on_friend_application_accepted_callback(zim_handle handle,
                                                        const struct zim_friend_info friend_info,
                                                        const struct zim_error error_info,
                                                        zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);
        if (!zim) {
            return;
        }

        ZIMFriendApplicationAcceptedCallback callback = nullptr;
        {
            std::lock_guard<std::mutex> lock(zim->friend_application_acceoted_callback_mutex_);
            callback = zim->friend_application_acceoted_callback_[sequence];
            zim->friend_application_acceoted_callback_.erase(sequence);
        }

        if (!callback) {
            return;
        }
        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        ZIMFriendInfo friendInfo{};
        ZIMConverter::oZIMFriendInfo(friendInfo, &friend_info);
        callback(friendInfo, error);
    }

    static void on_friend_application_reject_callback(zim_handle handle,
                                                      const struct zim_user_info user_info,
                                                      const struct zim_error error_info,
                                                      zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);
        if (!zim) {
            return;
        }

        ZIMFriendApplicationRejectedCallback callback = nullptr;
        {
            std::lock_guard<std::mutex> lock(zim->friend_application_rejected_callback_mutex_);
            callback = zim->friend_application_rejected_callback_[sequence];
            zim->friend_application_rejected_callback_.erase(sequence);
        }

        if (!callback) {
            return;
        }
        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        ZIMUserInfo userInfo{};

        struct zim_user_info new_user_info = user_info;

        ZIMConverter::oZIMUserInfo(&userInfo, &new_user_info);
        callback(userInfo, error);
    }

    static void
    on_query_friend_list_callback(zim_handle handle, const struct zim_friend_info *friend_info_list,
                                  unsigned int friend_info_list_length, unsigned int next_flag,
                                  const struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);
        if (!zim) {
            return;
        }

        ZIMFriendListQueriedCallback callback = nullptr;
        {
            std::lock_guard<std::mutex> lock(zim->friend_list_queried_callback_mutex_);
            callback = zim->friend_list_queried_callback_[sequence];
            zim->friend_list_queried_callback_.erase(sequence);
        }

        if (!callback) {
            return;
        }
        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        std::vector<ZIMFriendInfo> zim_friend_infos;

        for (unsigned int i = 0; i < friend_info_list_length; ++i) {
            struct zim_friend_info friend_info = *(friend_info_list + i);
            ZIMFriendInfo friendInfo{};
            ZIMConverter::oZIMFriendInfo(friendInfo, &friend_info);
            zim_friend_infos.push_back(friendInfo);
        }

        callback(zim_friend_infos, next_flag, error);
    }

    static void on_query_friend_application_list_callback(
        zim_handle handle, const struct zim_friend_application_info *friend_application_info_list,
        unsigned int friend_application_info_list_length, unsigned int next_flag,
        const struct zim_error error_info, zim_sequence sequence) {
        ZIMImpl *zim = GetZIMImplInstance(handle);
        if (!zim) {
            return;
        }

        ZIMFriendApplicationListQueriedCallback callback = nullptr;
        {
            std::lock_guard<std::mutex> lock(zim->friend_application_list_queried_callbaks_mutex_);
            callback = zim->friend_application_list_queried_callbaks_[sequence];
            zim->friend_application_list_queried_callbaks_.erase(sequence);
        }

        if (!callback) {
            return;
        }
        ZIMError error;
        error.code = (ZIMErrorCode)error_info.code;
        error.message = error_info.message;

        std::vector<ZIMFriendApplicationInfo> zim_friend_application_infos;

        for (unsigned int i = 0; i < friend_application_info_list_length; ++i) {
            struct zim_friend_application_info friend_application_info =
                *(friend_application_info_list + i);
            ZIMFriendApplicationInfo friendApplicationInfo{};
            ZIMConverter::oZIMFriendApplicationInfo(friendApplicationInfo,
                                                    &friend_application_info);
            zim_friend_application_infos.push_back(friendApplicationInfo);
        }
        callback(zim_friend_application_infos, next_flag, error);
    }
};

} // namespace zim
