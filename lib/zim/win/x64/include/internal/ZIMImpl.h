#pragma once

//
//  ZIMImpl.h
//  ZIM
//
//  Created by Patrick Fu on 2021/6/10.
//  Copyright © 2021 Zego. All rights reserved.
//

#include <future>
#include <mutex>
#include <unordered_map>

// clang-format off
#include "include/zim.h"
#include "ZIMConverter.h"

#if defined(__APPLE_OS__) || defined(__APPLE__)
#include "TargetConditionals.h"
#endif

#ifdef _WIN32
#include <windows.h>
#include <wchar.h>
#include <sstream>
#include <locale>
#include <codecvt>
#define ZegoStrncpy(destination, source, max_count) strncpy_s(destination, max_count + 1, source, max_count)

#elif TARGET_OS_OSX || TARGET_OS_IPHONE

#define ZegoStrncpy strncpy

#elif defined(ANDROID)

#define ZegoStrncpy strncpy

#elif defined(__unix__)

#define ZegoStrncpy strncpy
#include <string.h>

#endif

// clang-format on

namespace zim {

class ZIMImpl : public ZIM {
  public:
    // MARK: - Main

    void stlVector2CharsArray(const std::vector<std::string> &source, char **destination) {
        for (unsigned int i = 0; i < static_cast<unsigned int>(source.size()); ++i) {
            destination[i] = new char[source[i].length() + 1];
            destination[i][source[i].length()] = '\0';
            ZegoStrncpy(destination[i], source[i].c_str(), (size_t)source[i].size());
        }
    }

    void destroy() override;

    void setEventHandler(std::shared_ptr<ZIMEventHandler> handler) override {
        this->event_handler_ = handler;
    }

    void uploadLog(ZIMLogUploadedCallback callback) override {

        int sequence = 0;
        zim_upload_log(handle_, &sequence);
        {
            std::lock_guard<std::mutex> lock(this->log_uploaded_callbacks_mutex_);
            this->log_uploaded_callbacks_[sequence] = callback;
        }
    }

    void login(const ZIMUserInfo &userInfo, ZIMLoggedInCallback callback) override {
        login(userInfo, "", callback);
    }

    void login(const ZIMUserInfo &userInfo, const std::string &token,
               ZIMLoggedInCallback callback) override {
        zim_sequence sequence = ++sequence_;

        struct zim_login_config login_config {};
        login_config.user_name = const_cast<char *>(userInfo.userName.c_str());
        login_config.token = const_cast<char *>(token.c_str());
        login_config.is_offline_login = false;

        {
            std::lock_guard<std::mutex> lock(this->logged_in_callbacks_mutex_);
            this->logged_in_callbacks_[sequence] = callback;
        }
        zim_login(handle_, userInfo.userID.c_str(), login_config, &sequence);
    }

    /*
        zim_handle handle, const char *user_id,
        struct zim_login_config config, zim_sequence *sequence
    */
    void login(const std::string &userID, const ZIMLoginConfig &config,
               ZIMLoggedInCallback callback) override {
        zim_sequence sequence = ++sequence_;

        struct zim_login_config login_config {};
        ZIMConverter::sZIMLoginConfig(login_config, &config);

        {
            std::lock_guard<std::mutex> lock(this->logged_in_callbacks_mutex_);
            this->logged_in_callbacks_[sequence] = callback;
        }
        zim_login(handle_, userID.c_str(), login_config, &sequence);
    }

    void logout() override { zim_logout(handle_); }

    void renewToken(const std::string &token, ZIMTokenRenewedCallback callback) override {
        zim_sequence sequence = ++sequence_;
        {
            std::lock_guard<std::mutex> lock(this->renew_token_callbacks_mutex_);
            this->renew_token_callbacks_[sequence] = callback;
        }
        zim_renew_token(handle_, token.c_str(), &sequence);
    }

    void updateUserName(const std::string &userName, ZIMUserNameUpdatedCallback callback) override {
        zim_sequence sequence = ++sequence_;
        {
            std::lock_guard<std::mutex> lock(this->update_user_name_callbacks_mutex_);
            this->update_user_name_callbacks_[sequence] = callback;
        }
        zim_update_user_name(handle_, userName.c_str(), &sequence);
    }

    void updateUserAvatarUrl(const std::string &userAvatarUrl,
                             ZIMUserAvatarUrlUpdatedCallback callback) override {
        zim_sequence sequence = ++sequence_;
        {
            std::lock_guard<std::mutex> lock(this->update_user_avatar_callbacks_mutex_);
            this->update_user_avatar_callbacks_[sequence] = callback;
        }
        zim_update_user_avatar_url(handle_, userAvatarUrl.c_str(), &sequence);
    }

    void updateUserExtendedData(const std::string &extendedData,
                                ZIMUserExtendedDataUpdatedCallback callback) override {
        zim_sequence sequence = ++sequence_;
        {
            std::lock_guard<std::mutex> lock(this->update_user_extended_data_callbacks_mutex_);
            this->update_user_extended_data_callbacks_[sequence] = callback;
        }
        zim_update_user_extended_data(handle_, extendedData.c_str(), &sequence);
    }

    void queryUsersInfo(const std::vector<std::string> &userIDs,
                        const ZIMUsersInfoQueryConfig &oConfig,
                        ZIMUsersInfoQueriedCallback callback) override {
        zim_sequence sequence = ++sequence_;

        auto char_user_list = new char *[userIDs.size()];
        stlVector2CharsArray(userIDs, char_user_list);

        zim_users_info_query_config user_info_config{};

        if (&oConfig) {
            user_info_config.is_query_from_server = oConfig.isQueryFromServer;
        }

        {
            std::lock_guard<std::mutex> lock(this->users_info_queried_callbacks_mutex_);
            this->users_info_queried_callbacks_[sequence] = callback;
        }
        zim_query_users_info(handle_, (const char **)char_user_list, (unsigned int)userIDs.size(),
                             user_info_config, &sequence);
        for (unsigned int i = 0; i < static_cast<unsigned int>(userIDs.size()); ++i) {
            delete[](char_user_list[i]);
        }

        delete[] char_user_list;
        char_user_list = NULL;
    }

  public:
    // MARK: - Message
    void sendPeerMessage(ZIMMessage *message, const std::string &toUserID,
                         const ZIMMessageSendConfig &config,
                         ZIMMessageSentCallback callback) override {
        struct zim_message sMessage {};
        ZIMConverter::sNewZIMMessage(sMessage, message);

        zim_sequence sequence = ++sequence_;

        zim_message_send_config send_config{};
        ZIMConverter::sZIMMessageSendConfig(send_config, &config);
        {
            std::lock_guard<std::mutex> lock(this->message_sent_callbacks_mutex_);
            this->message_sent_callbacks_[sequence] = callback;
        }

        zim_send_peer_message(handle_, sMessage, toUserID.c_str(), &send_config, &sequence);
        ZIMConverter::sDelZIMMessage(sMessage);
    }

    void sendRoomMessage(ZIMMessage *message, const std::string &toRoomID,
                         const ZIMMessageSendConfig &config,
                         ZIMMessageSentCallback callback) override {
        struct zim_message sMessage {};
        ZIMConverter::sNewZIMMessage(sMessage, message);

        zim_sequence sequence = ++sequence_;

        zim_message_send_config send_config{};
        ZIMConverter::sZIMMessageSendConfig(send_config, &config);
        // 房间消息不支持离线推送
        send_config.enable_offline_push = false;

        {
            std::lock_guard<std::mutex> lock(this->message_sent_callbacks_mutex_);
            this->message_sent_callbacks_[sequence] = callback;
        }
        zim_send_room_message(handle_, sMessage, toRoomID.c_str(), &send_config, &sequence);
        ZIMConverter::sDelZIMMessage(sMessage);
    }

    void sendGroupMessage(ZIMMessage *message, const std::string &toGroupID,
                          const ZIMMessageSendConfig &config,
                          ZIMMessageSentCallback callback) override {
        struct zim_message sMessage {};
        ZIMConverter::sNewZIMMessage(sMessage, message);

        zim_sequence sequence = ++sequence_;

        zim_message_send_config send_config{};
        ZIMConverter::sZIMMessageSendConfig(send_config, &config);

        {
            std::lock_guard<std::mutex> lock(this->message_sent_callbacks_mutex_);
            this->message_sent_callbacks_[sequence] = callback;
        }
        zim_send_group_message(handle_, sMessage, toGroupID.c_str(), &send_config, &sequence);
        ZIMConverter::sDelZIMMessage(sMessage);
    }

    void sendMessage(std::shared_ptr<ZIMMessage> message, const std::string &toConversationID,
                     ZIMConversationType conversationType, const ZIMMessageSendConfig &config,
                     std::shared_ptr<ZIMMessageSendNotification> notification,
                     ZIMMessageSentCallback callback) override {
        struct zim_message sMessage {};
        ZIMConverter::sNewZIMMessage(sMessage, message.get());

        zim_sequence sequence = ++sequence_;

        zim_message_send_config send_config{};
        ZIMConverter::sZIMMessageSendConfig(send_config, &config);

        {
            std::lock_guard<std::mutex> lock(this->send_message_notification_callbacks_mutex_);
            this->send_message_notification_callbacks_[sequence] = notification;
        }
        {
            std::lock_guard<std::mutex> lock(this->message_sent_callbacks_mutex_);
            this->message_sent_callbacks_[sequence] = callback;
        }
        if (message->getMessageID() == 0 && message->getLocalMessageID() == 0) {
            std::lock_guard<std::mutex> lock(this->message_obj_map_mutex_);
            this->message_obj_map_[sequence] = message;
        }

        zim_send_message(handle_, sMessage, toConversationID.c_str(),
                         (zim_conversation_type)conversationType, send_config, &sequence);
        ZIMConverter::sDelZIMMessage(sMessage);
    }

    void sendMediaMessage(ZIMMediaMessage *message, const std::string &toConversationID,
                          ZIMConversationType conversationType, const ZIMMessageSendConfig &config,
                          ZIMMediaUploadingProgress progress,
                          ZIMMessageSentCallback callback) override {
        struct zim_message sMessage {};
        ZIMConverter::sNewZIMMessage(sMessage, message);

        zim_sequence sequence = ++sequence_;

        zim_message_send_config send_config{};
        ZIMConverter::sZIMMessageSendConfig(send_config, &config);

        {
            std::lock_guard<std::mutex> lock(this->media_send_progress_callbacks_mutex_);
            this->media_send_progress_callbacks_[sequence] = progress;
        }
        {
            std::lock_guard<std::mutex> lock(this->message_sent_callbacks_mutex_);
            this->message_sent_callbacks_[sequence] = callback;
        }

        zim_send_media_message(handle_, sMessage, toConversationID.c_str(),
                               (zim_conversation_type)conversationType, send_config, &sequence);
        ZIMConverter::sDelZIMMessage(sMessage);
    }

    void sendMediaMessage(std::shared_ptr<ZIMMediaMessage> message,
                          const std::string &toConversationID, ZIMConversationType conversationType,
                          const ZIMMessageSendConfig &config,
                          std::shared_ptr<ZIMMediaMessageSendNotification> notification,
                          ZIMMessageSentCallback callback) override {
        struct zim_message sMessage {};
        ZIMConverter::sNewZIMMessage(sMessage, message.get());

        zim_sequence sequence = ++sequence_;

        zim_message_send_config send_config{};
        ZIMConverter::sZIMMessageSendConfig(send_config, &config);

        {
            std::lock_guard<std::mutex> lock(
                this->send_media_message_notification_callbacks_mutex_);
            this->send_media_message_notification_callbacks_[sequence] = notification;
        }
        {
            std::lock_guard<std::mutex> lock(this->message_sent_callbacks_mutex_);
            this->message_sent_callbacks_[sequence] = callback;
        }
        if (message->getMessageID() == 0 && message->getLocalMessageID() == 0) {
            std::lock_guard<std::mutex> lock(this->message_obj_map_mutex_);
            this->message_obj_map_[sequence] = message;
        }

        zim_send_media_message(handle_, sMessage, toConversationID.c_str(),
                               (zim_conversation_type)conversationType, send_config, &sequence);
        ZIMConverter::sDelZIMMessage(sMessage);
    }

    void downloadMediaFile(ZIMMediaMessage *message, ZIMMediaFileType fileType,
                           ZIMMediaDownloadingProgress progress,
                           ZIMMediaDownloadedCallback callback) override {
        zim_sequence sequence = ++sequence_;
        struct zim_message sMessage {};
        ZIMConverter::sNewZIMMessage(sMessage, message);

        {
            std::lock_guard<std::mutex> lock(this->media_downloading_progress_callbacks_mutex_);
            this->media_downloading_progress_callbacks_[sequence] = progress;
        }
        {
            std::lock_guard<std::mutex> lock(this->media_download_callbacks_mutex_);
            this->media_download_callbacks_[sequence] = callback;
        }

        zim_download_media_file(handle_, sMessage, (zim_media_file_type)fileType, &sequence);
        ZIMConverter::sDelZIMMessage(sMessage);
    }

    void downloadMediaFile(std::shared_ptr<ZIMMediaMessage> message, ZIMMediaFileType fileType,
                           ZIMMediaDownloadingProgress progress,
                           ZIMMediaDownloadedCallback callback) override {
        zim_sequence sequence = ++sequence_;
        struct zim_message sMessage {};
        ZIMConverter::sNewZIMMessage(sMessage, message.get());

        {
            std::lock_guard<std::mutex> lock(this->media_downloading_progress_callbacks_mutex_);
            this->media_downloading_progress_callbacks_[sequence] = progress;
        }
        {
            std::lock_guard<std::mutex> lock(this->media_download_callbacks_mutex_);
            this->media_download_callbacks_[sequence] = callback;
        }
        if (message->getMessageID() == 0 && message->getLocalMessageID() == 0) {
            std::lock_guard<std::mutex> lock(this->message_obj_map_mutex_);
            this->message_obj_map_[sequence] = message;
        }

        zim_download_media_file(handle_, sMessage, (zim_media_file_type)fileType, &sequence);
        ZIMConverter::sDelZIMMessage(sMessage);
    }

    void queryHistoryMessage(const std::string &conversationID,
                             ZIMConversationType conversationType,
                             const ZIMMessageQueryConfig &config,
                             ZIMMessageQueriedCallback callback) override {

        zim_sequence sequence = ++sequence_;
        zim_message_query_config query_config{};
        ZIMConverter::sNewZIMMessageQueryConfig(query_config, &config);

        {
            std::lock_guard<std::mutex> lock(this->message_queried_callbacks_mutex_);
            this->message_queried_callbacks_[sequence] = callback;
        }

        zim_query_history_message(handle_, conversationID.c_str(),
                                  (zim_conversation_type)conversationType, query_config, &sequence);

        ZIMConverter::sDelZIMMessageQueryConfig(query_config);
    }

    void deleteAllMessage(const std::string &conversationID, ZIMConversationType conversationType,
                          const ZIMMessageDeleteConfig &config,
                          ZIMMessageDeletedCallback callback) override {

        zim_sequence sequence = ++sequence_;
        zim_message_delete_config delete_config{};

        ZIMConverter::sZIMMessageDeleteConfig(delete_config, &config);

        {
            std::lock_guard<std::mutex> lock(this->message_deleted_callbacks_mutex_);
            this->message_deleted_callbacks_[sequence] = callback;
        }

        zim_delete_all_message(handle_, conversationID.c_str(),
                               (zim_conversation_type)conversationType, delete_config, &sequence);
    }

    void
    deleteAllConversationMessages(const ZIMMessageDeleteConfig &config,
                                  ZIMConversationMessagesAllDeletedCallback callback) override {

        zim_sequence sequence = ++sequence_;
        zim_message_delete_config delete_config{};

        ZIMConverter::sZIMMessageDeleteConfig(delete_config, &config);

        {
            std::lock_guard<std::mutex> lock(
                this->conversation_messages_all_deleted_callbacks_mutex_);
            this->conversation_messages_all_deleted_callbacks_[sequence] = callback;
        }

        zim_delete_all_conversation_messages(handle_, delete_config, &sequence);
    }

    void deleteMessages(const std::vector<std::shared_ptr<ZIMMessage>> &messageList,
                        const std::string &conversationID, ZIMConversationType conversationType,
                        const ZIMMessageDeleteConfig &config,
                        ZIMMessageDeletedCallback callback) override {
        zim_sequence sequence = ++sequence_;
        zim_message_delete_config delete_config{};

        ZIMConverter::sZIMMessageDeleteConfig(delete_config, &config);

        unsigned int message_list_length = (unsigned int)messageList.size();
        struct zim_message *message_list = new zim_message[message_list_length];
        ZIMConverter::sNewZIMMessageList(message_list, messageList);

        {
            std::lock_guard<std::mutex> lock(this->message_deleted_callbacks_mutex_);
            this->message_deleted_callbacks_[sequence] = callback;
        }

        zim_delete_messages(handle_, message_list, message_list_length, conversationID.c_str(),
                            (zim_conversation_type)conversationType, delete_config, &sequence);

        ZIMConverter::sDelZIMMessageList(message_list, message_list_length);
        delete[] message_list;
    }

    void
    updateMessageLocalExtendedData(const std::string &localExtendedData,
                                   const std::shared_ptr<ZIMMessage> &message,
                                   ZIMMessageLocalExtendedDataUpdatedCallback callback) override {
        zim_sequence sequence = ++sequence_;
        struct zim_message sMessage {};
        ZIMConverter::sNewZIMMessage(sMessage, message.get());
        {
            std::lock_guard<std::mutex> lock(
                this->message_local_extended_data_updated_callbacks_mutex_);
            this->message_local_extended_data_updated_callbacks_[sequence] = callback;
        }
        zim_update_message_local_extended_data(handle_, localExtendedData.c_str(), sMessage,
                                               &sequence);
        ZIMConverter::sDelZIMMessage(sMessage);
    }

    void revokeMessage(std::shared_ptr<ZIMMessage> message, const ZIMMessageRevokeConfig &config,
                       ZIMMessageRevokedCallback callback) override {
        zim_sequence sequence = ++sequence_;
        zim_message_revoke_config revoke_config{};

        ZIMConverter::sZIMMessageRevokeConfig(revoke_config, &config);
        struct zim_message sMessage {};
        ZIMConverter::sNewZIMMessage(sMessage, message.get());
        {
            std::lock_guard<std::mutex> lock(this->message_revoked_callbacks_mutex_);
            this->message_revoked_callbacks_[sequence] = callback;
        }

        zim_revoke_message(handle_, sMessage, revoke_config, &sequence);
        ZIMConverter::sDelZIMMessage(sMessage);
    }

    void insertMessageToLocalDB(std::shared_ptr<ZIMMessage> message,
                                const std::string &conversationID,
                                ZIMConversationType conversationType,
                                const std::string &senderUserID,
                                ZIMMessageInsertedCallback callback) override {

        struct zim_message sMessage {};
        ZIMConverter::sNewZIMMessage(sMessage, message.get());

        zim_sequence sequence = ++sequence_;
        {
            std::lock_guard<std::mutex> lock(this->message_inserted_callbacks_mutex_);
            this->message_inserted_callbacks_[sequence] = callback;
        }
        if (message->getMessageID() == 0 && message->getLocalMessageID() == 0) {
            std::lock_guard<std::mutex> lock(this->message_obj_map_mutex_);
            this->message_obj_map_[sequence] = message;
        }

        zim_insert_message_to_local_db(handle_, sMessage, conversationID.c_str(),
                                       (zim_conversation_type)conversationType,
                                       senderUserID.c_str(), &sequence);
        ZIMConverter::sDelZIMMessage(sMessage);
    }

    void sendMessageReceiptsRead(const std::vector<std::shared_ptr<ZIMMessage>> &messageList,
                                 const std::string &conversationID,
                                 ZIMConversationType conversationType,
                                 ZIMMessageReceiptsReadSentCallback callback) override {

        unsigned int message_list_length = (unsigned int)messageList.size();
        struct zim_message *message_list = new zim_message[message_list_length];
        ZIMConverter::sNewZIMMessageList(message_list, messageList);

        zim_sequence sequence = ++sequence_;
        {
            std::lock_guard<std::mutex> lock(this->message_receipts_read_callbacks_mutex_);
            this->message_receipts_read_callbacks_[sequence] = callback;
        }

        zim_send_message_receipts_read(handle_, message_list, message_list_length,
                                       conversationID.c_str(),
                                       (zim_conversation_type)conversationType, &sequence);

        ZIMConverter::sDelZIMMessageList(message_list, message_list_length);
        delete[] message_list;
    }

    void queryMessageReceiptsInfo(const std::vector<std::shared_ptr<ZIMMessage>> &messageList,
                                  const std::string &conversationID,
                                  ZIMConversationType conversationType,
                                  ZIMMessageReceiptsInfoQueriedCallback callback) override {
        unsigned int message_list_length = (unsigned int)messageList.size();
        struct zim_message *message_list = new zim_message[message_list_length];
        ZIMConverter::sNewZIMMessageList(message_list, messageList);

        zim_sequence sequence = ++sequence_;
        {
            std::lock_guard<std::mutex> lock(this->message_receipts_queried_callbacks_mutex_);
            this->message_receipts_queried_callbacks_[sequence] = callback;
        }

        zim_query_message_receipts_info(handle_, message_list, message_list_length,
                                        conversationID.c_str(),
                                        (zim_conversation_type)conversationType, &sequence);

        ZIMConverter::sDelZIMMessageList(message_list, message_list_length);
        delete[] message_list;
    }

    void searchLocalMessages(const std::string &conversationID,
                             ZIMConversationType conversationType,
                             const ZIMMessageSearchConfig &config,
                             ZIMMessagesSearchedCallback callback) override {

        struct zim_message_search_config c_config {};
        ZIMConverter::sNewZIMMessageSearchConfig(c_config, config);

        zim_sequence sequence = ++sequence_;
        {
            std::lock_guard<std::mutex> lock(this->message_searched_callbacks_mutex_);
            this->message_searched_callbacks_[sequence] = callback;
        }

        zim_search_local_messages(handle_, conversationID.c_str(),
                                  (zim_conversation_type)conversationType, c_config, &sequence);

        ZIMConverter::sDelZIMMessageSearchConfig(c_config);
    }

    void searchGlobalLocalMessages(const ZIMMessageSearchConfig &config,
                                   ZIMMessagesGlobalSearchedCallback callback) override {

        struct zim_message_search_config c_config {};
        ZIMConverter::sNewZIMMessageSearchConfig(c_config, config);

        zim_sequence sequence = ++sequence_;
        {
            std::lock_guard<std::mutex> lock(this->message_global_searched_callbacks_mutex_);
            this->message_global_searched_callbacks_[sequence] = callback;
        }

        zim_search_global_local_messages(handle_, c_config, &sequence);

        ZIMConverter::sDelZIMMessageSearchConfig(c_config);
    }

    void searchLocalConversations(const ZIMConversationSearchConfig &config,
                                  ZIMConversationsSearchedCallback callback) override {
        struct zim_conversation_search_config c_config {};
        ZIMConverter::sNewZIMConversationSearchConfig(c_config, config);

        zim_sequence sequence = ++sequence_;
        {
            std::lock_guard<std::mutex> lock(this->conversations_searched_callbacks_mutex_);
            this->conversations_searched_callbacks_[sequence] = callback;
        }

        zim_search_local_conversations(handle_, c_config, &sequence);

        ZIMConverter::sDelZIMConversationSearchConfig(c_config);
    }

    void addMessageReaction(const std::string &reactionType,
                            const std::shared_ptr<ZIMMessage> &message,
                            ZIMMessageReactionAddedCallback callback) override {
        zim_sequence sequence = ++sequence_;
        struct zim_message sMessage {};
        ZIMConverter::sNewZIMMessage(sMessage, message.get());

        {
            std::lock_guard<std::mutex> lock(this->reaction_added_callbacks_mutex_);
            this->reaction_added_callbacks_[sequence] = callback;
        }
        zim_add_message_reaction(handle_, reactionType.c_str(), sMessage, &sequence);
        ZIMConverter::sDelZIMMessage(sMessage);
    }

    void deleteMessageReaction(const std::string &reactionType,
                               const std::shared_ptr<ZIMMessage> &message,
                               ZIMMessageReactionDeletedCallback callback) override {
        zim_sequence sequence = ++sequence_;
        struct zim_message sMessage {};
        ZIMConverter::sNewZIMMessage(sMessage, message.get());

        {
            std::lock_guard<std::mutex> lock(this->reaction_deleted_callbacks_mutex_);
            this->reaction_deleted_callbacks_[sequence] = callback;
        }
        zim_delete_message_reaction(handle_, reactionType.c_str(), sMessage, &sequence);
        ZIMConverter::sDelZIMMessage(sMessage);
    }

    void queryMessageReactionUserList(const std::shared_ptr<ZIMMessage> &message,
                                      const ZIMMessageReactionUserQueryConfig &config,
                                      ZIMMessageReactionUserListQueriedCallback callback) override {
        zim_sequence sequence = ++sequence_;
        struct zim_message sMessage {};
        ZIMConverter::sNewZIMMessage(sMessage, message.get());
        struct zim_message_reaction_users_query_config sConfig {};
        ZIMConverter::sZIMMessageReactionUserQueryConfig(&sConfig, &config);
        {
            std::lock_guard<std::mutex> lock(this->reaction_user_list_queried_callbacks_mutex_);
            this->reaction_user_list_queried_callbacks_[sequence] = callback;
        }

        zim_query_message_reaction_user_list(handle_, sMessage, sConfig, &sequence);
        ZIMConverter::sDelZIMMessage(sMessage);
    }

  public:
    // MARK: - Conversation
    void queryConversation(const std::string &conversationID, ZIMConversationType conversationType,
                           ZIMConversationQueriedCallback callback) override {
        zim_sequence sequence = ++sequence_;

        {
            std::lock_guard<std::mutex> lock(this->conversation_queried_callbacks_mutex_);
            this->conversation_queried_callbacks_[sequence] = callback;
        }

        zim_query_conversation(handle_, conversationID.c_str(),
                               (zim_conversation_type)conversationType, &sequence);
    }

    void queryConversationList(const ZIMConversationQueryConfig &config,
                               ZIMConversationListQueriedCallback callback) override {
        zim_sequence sequence = ++sequence_;
        struct zim_conversation_query_config query_config {};
        ZIMConverter::sZIMConversationQueryConfig(query_config, config);

        {
            std::lock_guard<std::mutex> lock(this->conversation_list_queried_callbacks_mutex_);
            this->conversation_list_queried_callbacks_[sequence] = callback;
        }

        zim_query_conversation_list(handle_, query_config, &sequence);
    }

    void queryConversationPinnedList(const ZIMConversationQueryConfig &config,
                                     ZIMConversationPinnedListQueriedCallback callback) override {
        zim_sequence sequence = ++sequence_;
        struct zim_conversation_query_config query_config {};
        ZIMConverter::sZIMConversationQueryConfig(query_config, config);

        {
            std::lock_guard<std::mutex> lock(
                this->conversation_pinned_list_queried_callbacks_mutex_);
            this->conversation_pinned_list_queried_callbacks_[sequence] = callback;
        }

        zim_query_conversation_pinned_list(handle_, query_config, &sequence);
    }

    void clearConversationUnreadMessageCount(
        const std::string &conversationID, ZIMConversationType conversationType,
        ZIMConversationUnreadMessageCountClearedCallback callback) override {
        zim_sequence sequence = ++sequence_;

        {
            std::lock_guard<std::mutex> lock(
                this->conversation_unread_count_cleared_callbacks_mutex_);
            this->conversation_unread_count_cleared_callbacks_[sequence] = callback;
        }

        zim_clear_conversation_unread_message_count(
            handle_, conversationID.c_str(), (zim_conversation_type)conversationType, &sequence);
    }

    void clearConversationTotalUnreadMessageCount(
        ZIMConversationTotalUnreadMessageCountClearedCallback callback) override {
        zim_sequence sequence = ++sequence_;

        {
            std::lock_guard<std::mutex> lock(
                this->conversations_all_unread_count_cleared_callbacks_mutex_);
            this->conversations_all_unread_count_cleared_callbacks_[sequence] = callback;
        }

        zim_clear_conversation_total_unread_message_count(handle_, &sequence);
    }

    void
    updateConversationPinnedState(bool isPinned, const std::string &conversationID,
                                  ZIMConversationType conversationType,
                                  ZIMConversationPinnedStateUpdatedCallback callback) override {
        zim_sequence sequence = ++sequence_;

        {
            std::lock_guard<std::mutex> lock(
                this->conversation_pinned_state_updated_callbacks_mutex_);
            this->conversation_pinned_state_updated_callbacks_[sequence] = callback;
        }

        zim_update_conversation_pinned_state(handle_, isPinned, conversationID.c_str(),
                                             (zim_conversation_type)conversationType, &sequence);
    }

    void deleteConversation(const std::string &conversationID, ZIMConversationType conversationType,
                            const ZIMConversationDeleteConfig &config,
                            ZIMConversationDeletedCallback callback) override {
        zim_sequence sequence = ++sequence_;
        struct zim_conversation_delete_config delete_config {};
        ZIMConverter::sZIMConversationDeleteConfig(delete_config, config);

        {
            std::lock_guard<std::mutex> lock(this->conversation_deleted_callbacks_mutex_);
            this->conversation_deleted_callbacks_[sequence] = callback;
        }

        zim_delete_conversation(handle_, conversationID.c_str(),
                                (zim_conversation_type)conversationType, delete_config, &sequence);
    }

    void deleteAllConversations(const ZIMConversationDeleteConfig &config,
                                ZIMConversationsAllDeletedCallback callback) override {
        zim_sequence sequence = ++sequence_;
        struct zim_conversation_delete_config delete_config {};
        ZIMConverter::sZIMConversationDeleteConfig(delete_config, config);

        {
            std::lock_guard<std::mutex> lock(this->conversations_all_deleted_callbacks_mutex_);
            this->conversations_all_deleted_callbacks_[sequence] = callback;
        }

        zim_delete_all_conversations(handle_, delete_config, &sequence);
    }

    void sendConversationMessageReceiptRead(
        const std::string &conversationID, ZIMConversationType conversationType,
        ZIMConversationMessageReceiptReadSentCallback callback) override {
        zim_sequence sequence = ++sequence_;

        {
            std::lock_guard<std::mutex> lock(this->conversation_receipt_read_callbacks_mutex_);
            this->conversation_receipt_read_callbacks_[sequence] = callback;
        }

        zim_send_conversation_message_receipt_read(
            handle_, conversationID.c_str(), (zim_conversation_type)conversationType, &sequence);
    }

    void
    queryCombineMessageDetail(std::shared_ptr<ZIMCombineMessage> &message,
                              const ZIMCombineMessageDetailQueriedCallback &callback) override {
        zim_sequence sequence = ++sequence_;

        {
            std::lock_guard<std::mutex> lock(this->message_combine_queried_callbacks_mutex_);
            this->message_combine_queried_callbacks_[sequence] = callback;
        }
        struct zim_message sMessage {};
        ZIMConverter::sNewZIMMessage(sMessage, message.get());
        zim_query_combine_message_detail(handle_, sMessage, &sequence);
        ZIMConverter::sDelZIMMessage(sMessage);
    }
    void setConversationDraft(const std::string &draft, const std::string &conversationID,
                              ZIMConversationType conversationType,
                              ZIMConversationDraftSetCallback callback) override {
        zim_sequence sequence = ++sequence_;

        {
            std::lock_guard<std::mutex> lock(this->conversation_draft_set_callbacks_mutex_);
            this->conversation_draft_set_callbacks_[sequence] = callback;
        }

        zim_set_conversation_draft(handle_, draft.c_str(), conversationID.c_str(),
                                   (zim_conversation_type)conversationType, &sequence);
    }

  public:
    // MARK: - Room

    void createRoom(const ZIMRoomInfo &roomInfo, ZIMRoomCreatedCallback callback) override {

        struct zim_room_info sInfo {};

        sInfo.room_id = const_cast<char *>(roomInfo.roomID.c_str());
        sInfo.room_name = const_cast<char *>(roomInfo.roomName.c_str());

        zim_sequence sequence = ++sequence_;
        {
            std::lock_guard<std::mutex> lock(this->room_created_callbacks_mutex_);
            this->room_created_callbacks_[sequence] = callback;
        }
        zim_create_room(handle_, sInfo, nullptr, &sequence);
    }

    void createRoom(const ZIMRoomInfo &roomInfo, const ZIMRoomAdvancedConfig &config,
                    ZIMRoomCreatedCallback callback) override {

        struct zim_room_info sInfo {};

        sInfo.room_id = const_cast<char *>(roomInfo.roomID.c_str());
        sInfo.room_name = const_cast<char *>(roomInfo.roomName.c_str());

        zim_sequence sequence = ++sequence_;

        auto attribute = new zim_room_attribute[config.roomAttributes.size()];
        unsigned int i = 0;
        for (auto &it : config.roomAttributes) {
            memset(&attribute[i], 0, sizeof(struct zim_room_attribute));
            char *cKey = const_cast<char *>(it.first.c_str());
            char *cValue = const_cast<char *>(it.second.c_str());
            attribute[i].key = cKey;
            attribute[i].value = cValue;
            i++;
        }

        zim_room_advanced_config sConfig{};
        sConfig.room_attributes = attribute;
        sConfig.room_attributes_length = (unsigned int)config.roomAttributes.size();
        sConfig.room_destroy_delay_time = config.roomDestroyDelayTime;

        {
            std::lock_guard<std::mutex> lock(this->room_created_callbacks_mutex_);
            this->room_created_callbacks_[sequence] = callback;
        }

        zim_create_room(handle_, sInfo, &sConfig, &sequence);

        if (attribute) {
            delete[] attribute;
            attribute = nullptr;
        }
    }

    void joinRoom(const std::string &roomID, ZIMRoomJoinedCallback callback) override {
        zim_sequence sequence = ++sequence_;
        {
            std::lock_guard<std::mutex> lock(this->room_joined_callbacks_mutex_);
            this->room_joined_callbacks_[sequence] = callback;
        }
        zim_join_room(handle_, roomID.c_str(), &sequence);
    }

    void enterRoom(const ZIMRoomInfo &roomInfo, const ZIMRoomAdvancedConfig &config,
                   ZIMRoomEnteredCallback callback) override {
        struct zim_room_info sInfo {};

        sInfo.room_id = const_cast<char *>(roomInfo.roomID.c_str());
        sInfo.room_name = const_cast<char *>(roomInfo.roomName.c_str());

        zim_sequence sequence = ++sequence_;

        auto attribute = new zim_room_attribute[config.roomAttributes.size()];
        unsigned int i = 0;
        for (auto &it : config.roomAttributes) {
            memset(&attribute[i], 0, sizeof(struct zim_room_attribute));
            char *cKey = const_cast<char *>(it.first.c_str());
            char *cValue = const_cast<char *>(it.second.c_str());
            attribute[i].key = cKey;
            attribute[i].value = cValue;
            //attribute[i].sequence = 0;
            i++;
        }

        zim_room_advanced_config sConfig{};
        sConfig.room_attributes = attribute;
        sConfig.room_attributes_length = (unsigned int)config.roomAttributes.size();
        sConfig.room_destroy_delay_time = config.roomDestroyDelayTime;

        {
            std::lock_guard<std::mutex> lock(this->room_entered_callbacks_mutex_);
            this->room_entered_callbacks_[sequence] = callback;
        }
        zim_enter_room(handle_, sInfo, &sConfig, &sequence);

        if (attribute) {
            delete[] attribute;
            attribute = nullptr;
        }
    }

    void leaveRoom(const std::string &roomID, ZIMRoomLeftCallback callback) override {
        zim_sequence sequence = ++sequence_;
        {
            std::lock_guard<std::mutex> lock(this->room_left_callbacks_mutex_);
            this->room_left_callbacks_[sequence] = callback;
        }
        zim_leave_room(handle_, roomID.c_str(), &sequence);
    }

    void queryRoomMemberList(const std::string &roomID, const ZIMRoomMemberQueryConfig &config,
                             ZIMRoomMemberQueriedCallback callback) override {

        zim_room_member_query_config query_member_config{};
        ZIMConverter::sZIMQueryMemberConfig(&query_member_config, &config);

        zim_sequence sequence = ++sequence_;
        {
            std::lock_guard<std::mutex> lock(this->member_queried_callbacks_mutex_);
            this->member_queried_callbacks_[sequence] = callback;
        }
        zim_query_room_member_list(handle_, roomID.c_str(), query_member_config, &sequence);
    }

    void queryRoomMembers(const std::vector<std::string> &userIDs, const std::string &room_id,
                          ZIMRoomMembersQueriedCallback callback) override {
        zim_sequence sequence = ++sequence_;

        auto user_ids = new char *[userIDs.size()];

        for (unsigned int i = 0; i < userIDs.size(); i++) {
            user_ids[i] = const_cast<char *>(userIDs[i].c_str());
        }

        {
            std::lock_guard<std::mutex> lock(this->members_queried_callbacks_mutex_);
            this->members_queried_callbacks_[sequence] = callback;
        }

        zim_query_room_members(handle_, (const char **)user_ids, (unsigned int)userIDs.size(),
                               room_id.c_str(), &sequence);

        if (user_ids) {
            delete[] user_ids;
            user_ids = nullptr;
        }
    }

    void queryRoomOnlineMemberCount(const std::string &roomID,
                                    ZIMRoomOnlineMemberCountQueriedCallback callback) override {
        zim_sequence sequence = ++sequence_;
        {
            std::lock_guard<std::mutex> lock(this->online_count_queried_callbacks_mutex_);
            this->online_count_queried_callbacks_[sequence] = callback;
        }
        zim_query_room_online_member_count(handle_, roomID.c_str(), &sequence);
    }

    void queryGroupMessageReceiptReadMemberList(
        std::shared_ptr<ZIMMessage> message, const std::string &groupID,
        const ZIMGroupMessageReceiptMemberQueryConfig &config,
        ZIMGroupMessageReceiptMemberListQueriedCallback callback) override {
        zim_sequence sequence = ++sequence_;
        zim_message c_message{};
        zim_group_message_receipt_member_query_config c_config;
        c_config.count = config.count;
        c_config.next_flag = config.nextFlag;

        ZIMConverter::sNewZIMMessage(c_message, message.get());
        {
            std::lock_guard<std::mutex> lock(this->group_receipt_detail_queried_callbacks_mutex_);
            this->group_receipt_detail_queried_callbacks_[sequence] = callback;
        }

        zim_query_group_message_receipt_read_member_list(handle_, c_message, groupID.c_str(),
                                                         c_config, &sequence);
        ZIMConverter::sDelZIMMessage(c_message);
    }

    void queryGroupMessageReceiptUnreadMemberList(
        std::shared_ptr<ZIMMessage> message, const std::string &groupID,
        const ZIMGroupMessageReceiptMemberQueryConfig &config,
        ZIMGroupMessageReceiptMemberListQueriedCallback callback) override {
        zim_sequence sequence = ++sequence_;
        zim_message c_message{};
        zim_group_message_receipt_member_query_config c_config;
        c_config.count = config.count;
        c_config.next_flag = config.nextFlag;

        ZIMConverter::sNewZIMMessage(c_message, message.get());

        {
            std::lock_guard<std::mutex> lock(this->group_receipt_detail_queried_callbacks_mutex_);
            this->group_receipt_detail_queried_callbacks_[sequence] = callback;
        }

        zim_query_group_message_receipt_unread_member_list(handle_, c_message, groupID.c_str(),
                                                           c_config, &sequence);
        ZIMConverter::sDelZIMMessage(c_message);
    }

    void searchLocalGroups(const ZIMGroupSearchConfig &config,
                           ZIMGroupsSearchedCallback callback) override {
        zim_sequence sequence = ++sequence_;

        auto keywords = new char *[config.keywords.size()];
        for (unsigned int i = 0; i < config.keywords.size(); i++) {
            keywords[i] = const_cast<char *>(config.keywords[i].c_str());
        }

        zim_group_search_config c_config;
        c_config.count = config.count;
        c_config.next_flag = config.nextFlag;
        c_config.keywords = keywords;
        c_config.keywords_length = (unsigned int)config.keywords.size();
        c_config.is_also_match_group_member_user_name = config.isAlsoMatchGroupMemberUserName;
        c_config.is_also_match_group_member_nick_name = config.isAlsoMatchGroupMemberNickname;

        {
            std::lock_guard<std::mutex> lock(this->groups_searched_callbacks_mutex_);
            this->groups_searched_callbacks_[sequence] = callback;
        }

        zim_search_local_groups(handle_, c_config, &sequence);

        if (keywords) {
            delete[] keywords;
            keywords = nullptr;
        }
    }

    void searchLocalGroupMembers(const std::string &groupID,
                                 const ZIMGroupMemberSearchConfig &config,
                                 ZIMGroupMembersSearchedCallback callback) override {
        zim_sequence sequence = ++sequence_;

        auto keywords = new char *[config.keywords.size()];
        for (unsigned int i = 0; i < config.keywords.size(); i++) {
            keywords[i] = const_cast<char *>(config.keywords[i].c_str());
        }

        zim_group_member_search_config c_config;
        c_config.count = config.count;
        c_config.next_flag = config.nextFlag;
        c_config.keywords = keywords;
        c_config.keywords_length = (unsigned int)config.keywords.size();
        c_config.is_also_match_group_member_nick_name = config.isAlsoMatchGroupMemberNickname;

        {
            std::lock_guard<std::mutex> lock(this->group_members_searched_callbacks_mutex_);
            this->group_members_searched_callbacks_[sequence] = callback;
        }

        zim_search_local_group_members(handle_, groupID.c_str(), c_config, &sequence);

        if (keywords) {
            delete[] keywords;
            keywords = nullptr;
        }
    }

    void setRoomAttributes(const std::unordered_map<std::string, std::string> &roomAttributes,
                           const std::string &roomID, ZIMRoomAttributesSetConfig *config,
                           ZIMRoomAttributesOperatedCallback callback) override {
        zim_sequence sequence = ++sequence_;
        auto attribute = new zim_room_attribute[roomAttributes.size()];
        unsigned int i = 0;
        for (auto &it : roomAttributes) {
            memset(&attribute[i], 0, sizeof(struct zim_room_attribute));
            char *cKey = const_cast<char *>(it.first.c_str());
            char *cValue = const_cast<char *>(it.second.c_str());
            attribute[i].key = cKey;
            attribute[i].value = cValue;
            i++;
        }

        {
            std::lock_guard<std::mutex> lock(this->room_attributes_operated_callbacks_mutex_);
            this->room_attributes_operated_callbacks_[sequence] = callback;
        }

        zim_room_attributes_set_config sConfig{false, false, false};
        if (config) {
            sConfig.is_force = config->isForce;
            sConfig.is_delete_after_owner_left = config->isDeleteAfterOwnerLeft;
            sConfig.is_update_owner = config->isUpdateOwner;
        }

        zim_set_room_attributes(handle_, attribute, (unsigned int)roomAttributes.size(),
                                roomID.c_str(), sConfig, &sequence);

        if (attribute) {
            delete[] attribute;
            attribute = nullptr;
        }
    }

    void deleteRoomAttributes(const std::vector<std::string> &keys, const std::string &roomID,
                              ZIMRoomAttributesDeleteConfig *config,
                              ZIMRoomAttributesOperatedCallback callback) override {
        zim_sequence sequence = ++sequence_;

        auto keys_str_list = new char *[keys.size()];

        for (unsigned int i = 0; i < keys.size(); i++) {
            keys_str_list[i] = const_cast<char *>(keys[i].c_str());
        }

        {
            std::lock_guard<std::mutex> lock(this->room_attributes_operated_callbacks_mutex_);
            this->room_attributes_operated_callbacks_[sequence] = callback;
        }

        zim_room_attributes_delete_config sConfig{false};
        if (config) {
            sConfig.is_force = config->isForce;
        }

        zim_delete_room_attributes(handle_, (const char **)keys_str_list, (unsigned int)keys.size(),
                                   roomID.c_str(), sConfig, &sequence);

        if (keys_str_list) {
            delete[] keys_str_list;
            keys_str_list = nullptr;
        }
    }

    void queryRoomAllAttributes(const std::string &roomID,
                                ZIMRoomAttributesQueriedCallback callback) override {
        zim_sequence sequence = ++sequence_;

        {
            std::lock_guard<std::mutex> lock(this->room_all_attributes_queried_callbacks_mutex_);
            this->room_all_attributes_queried_callbacks_[sequence] = callback;
        }

        zim_query_room_all_attributes(handle_, roomID.c_str(), &sequence);
    }

    void beginRoomAttributesBatchOperation(const std::string &roomID,
                                           ZIMRoomAttributesBatchOperationConfig *config) override {
        zim_sequence sequence = ++sequence_;

        zim_room_attributes_batch_operation_config sConfig{false, false, false};
        if (config) {
            sConfig.is_force = config->isForce;
            sConfig.is_delete_after_owner_left = config->isDeleteAfterOwnerLeft;
            sConfig.is_update_owner = config->isUpdateOwner;
        }

        zim_begin_room_attributes_batch_operation(handle_, roomID.c_str(), sConfig, &sequence);
    }

    void endRoomAttributesBatchOperation(const std::string &roomID,
                                         ZIMRoomAttributesBatchOperatedCallback callback) override {
        zim_sequence sequence = ++sequence_;

        {
            std::lock_guard<std::mutex> lock(this->room_attributes_batch_operated_callbacks_mutex_);
            this->room_attributes_batch_operated_callbacks_[sequence] = callback;
        }

        zim_end_room_attributes_batch_operation(handle_, roomID.c_str(), &sequence);
    }

    void setRoomMembersAttributes(const std::unordered_map<std::string, std::string> &attributes,
                                  const std::vector<std::string> &userIDs,
                                  const std::string &roomID,
                                  const ZIMRoomMemberAttributesSetConfig &config,
                                  ZIMRoomMembersAttributesOperatedCallback callback) override {
        zim_sequence sequence = ++sequence_;

        char **user_ids = new char *[userIDs.size()];
        auto member_attributes = new zim_room_member_attribute[attributes.size()];

        unsigned int user_ids_count = 0;
        unsigned int member_attributes_count = 0;

        for (const auto &attribute : attributes) {
            member_attributes[member_attributes_count].key =
                const_cast<char *>(attribute.first.c_str());
            member_attributes[member_attributes_count].value =
                const_cast<char *>(attribute.second.c_str());

            ++member_attributes_count;
        }

        for (const auto &id : userIDs) {
            user_ids[user_ids_count] = const_cast<char *>(id.c_str());
            ++user_ids_count;
        }

        zim_room_member_attributes_set_config sConfig{};
        sConfig.is_delete_after_owner_left = config.isDeleteAfterOwnerLeft;

        {
            std::lock_guard<std::mutex> lock(
                this->room_members_attributes_operated_callbacks_mutex_);
            this->room_members_attributes_operated_callbacks_[sequence] = callback;
        }

        zim_set_room_members_attributes(handle_, member_attributes, member_attributes_count,
                                        (const char **)user_ids, user_ids_count, roomID.c_str(),
                                        sConfig, &sequence);

        if (member_attributes) {
            delete[] member_attributes;
            member_attributes = nullptr;
        }
        if (user_ids) {
            delete[] user_ids;
            user_ids = nullptr;
        }
    }

    void queryRoomMembersAttributes(const std::vector<std::string> &userIDs,
                                    const std::string &roomID,
                                    ZIMRoomMembersAttributesQueriedCallback callback) override {
        zim_sequence sequence = ++sequence_;

        char **user_ids = new char *[userIDs.size()];
        unsigned int user_ids_count = 0;

        for (const auto &id : userIDs) {
            user_ids[user_ids_count] = const_cast<char *>(id.c_str());
            ++user_ids_count;
        }

        {
            std::lock_guard<std::mutex> lock(
                this->room_members_attributes_queried_callbacks_mutex_);
            this->room_members_attributes_queried_callbacks_[sequence] = callback;
        }

        zim_query_room_members_attributes(handle_, (const char **)user_ids, user_ids_count,
                                          roomID.c_str(), &sequence);

        if (user_ids) {
            delete[] user_ids;
            user_ids = nullptr;
        }
    }

    void
    queryRoomMemberAttributesList(const std::string &roomID,
                                  const ZIMRoomMemberAttributesQueryConfig &config,
                                  ZIMRoomMemberAttributesListQueriedCallback callback) override {

        zim_room_member_attributes_query_config sConfig{};
        sConfig.next_flag = const_cast<char *>(config.nextFlag.c_str());
        sConfig.count = config.count;

        zim_sequence sequence = ++sequence_;

        {
            std::lock_guard<std::mutex> lock(
                this->room_member_attributes_list_queried_callbacks_mutex_);
            this->room_member_attributes_list_queried_callbacks_[sequence] = callback;
        }

        zim_query_room_member_attributes_list(handle_, roomID.c_str(), sConfig, &sequence);
    }

    void createGroup(const ZIMGroupInfo &groupInfo, const std::vector<std::string> &userIDs,
                     ZIMGroupCreatedCallback callback) override {
        zim_sequence sequence = ++sequence_;

        zim_group_info group_info;
        group_info.group_id = const_cast<char *>(groupInfo.groupID.c_str());
        group_info.group_name = const_cast<char *>(groupInfo.groupName.c_str());
        group_info.group_avatar_url = const_cast<char *>(groupInfo.groupAvatarUrl.c_str());

        auto char_user_list = new char *[userIDs.size() + 1];
        stlVector2CharsArray(userIDs, char_user_list);

        {
            std::lock_guard<std::mutex> lock(this->group_created_callbacks_mutex_);
            this->group_created_callbacks_[sequence] = callback;
        }

        zim_create_group(handle_, group_info, (const char **)char_user_list,
                         (unsigned int)(userIDs.size()), nullptr, &sequence);
        for (unsigned int i = 0; i < static_cast<unsigned int>(userIDs.size()); ++i) {
            delete[](char_user_list[i]);
        }

        delete[] char_user_list;
        char_user_list = NULL;
    }

    void createGroup(const ZIMGroupInfo &groupInfo, const std::vector<std::string> &userIDs,
                     const ZIMGroupAdvancedConfig &advancedInfo,
                     ZIMGroupCreatedCallback callback) override {
        zim_sequence sequence = ++sequence_;

        zim_group_info group_info{};
        ZIMConverter::sZIMGroupInfo(group_info, groupInfo);

        auto char_user_list = new char *[userIDs.size()];
        stlVector2CharsArray(userIDs, char_user_list);

        zim_group_advanced_config advanced_config{};
        advanced_config.group_notice = const_cast<char *>(advancedInfo.groupNotice.c_str());

        unsigned int count_attributes = 0;

        advanced_config.group_attributes_length = (unsigned int)advancedInfo.groupAttributes.size();
        advanced_config.group_attributes =
            new zim_group_attribute[advanced_config.group_attributes_length];

        for (auto it : advancedInfo.groupAttributes) {
            advanced_config.group_attributes[count_attributes].key =
                new char[it.first.length() + 1];
            advanced_config.group_attributes[count_attributes].key[it.first.length()] = '\0';
            ZegoStrncpy(advanced_config.group_attributes[count_attributes].key, it.first.c_str(),
                        it.first.length());
            advanced_config.group_attributes[count_attributes].value =
                new char[it.second.length() + 1];
            advanced_config.group_attributes[count_attributes].value[it.second.length()] = '\0';
            ZegoStrncpy(advanced_config.group_attributes[count_attributes].value, it.second.c_str(),
                        it.second.length());

            ++count_attributes;
        }

        {
            std::lock_guard<std::mutex> lock(this->group_created_callbacks_mutex_);
            this->group_created_callbacks_[sequence] = callback;
        }

        zim_create_group(handle_, group_info, (const char **)char_user_list,
                         (unsigned int)(userIDs.size()), &advanced_config, &sequence);
        for (unsigned int i = 0; i < userIDs.size(); ++i) {
            delete char_user_list[i];
        }

        for (unsigned int i = 0; i < count_attributes; ++i) {
            delete[] advanced_config.group_attributes[i].key;
            delete[] advanced_config.group_attributes[i].value;
        }

        delete[] char_user_list;
        delete[] advanced_config.group_attributes;
    }

    void dismissGroup(const std::string &groupID, ZIMGroupDismissedCallback callback) override {
        zim_sequence sequence = ++sequence_;

        {
            std::lock_guard<std::mutex> lock(this->group_dismissed_callbacks_mutex_);
            this->group_dismissed_callbacks_[sequence] = callback;
        }

        zim_dismiss_group(handle_, groupID.c_str(), &sequence);
    }

    void joinGroup(const std::string &groupID, ZIMGroupJoinedCallback callback) override {
        zim_sequence sequence = ++sequence_;

        {
            std::lock_guard<std::mutex> lock(this->group_joined_callbacks_mutex_);
            this->group_joined_callbacks_[sequence] = callback;
        }

        zim_join_group(handle_, groupID.c_str(), &sequence);
    }

    void leaveGroup(const std::string &groupID, ZIMGroupLeftCallback callback) override {
        zim_sequence sequence = ++sequence_;

        {
            std::lock_guard<std::mutex> lock(this->group_left_callbacks_mutex_);
            this->group_left_callbacks_[sequence] = callback;
        }

        zim_leave_group(handle_, groupID.c_str(), &sequence);
    }

    void inviteUsersIntoGroup(const std::vector<std::string> &userIDs, const std::string &groupID,
                              ZIMGroupUsersInvitedCallback callback) override {
        zim_sequence sequence = ++sequence_;

        auto char_user_list = new char *[userIDs.size() + 1];
        stlVector2CharsArray(userIDs, char_user_list);

        {
            std::lock_guard<std::mutex> lock(this->group_users_invited_callbacks_mutex_);
            this->group_users_invited_callbacks_[sequence] = callback;
        }

        zim_invite_users_into_group(handle_, (const char **)char_user_list,
                                    (unsigned int)(userIDs.size()), groupID.c_str(), &sequence);

        for (unsigned int i = 0; i < static_cast<unsigned int>(userIDs.size()); ++i) {
            delete[](char_user_list[i]);
        }

        delete[] char_user_list;
        char_user_list = NULL;
    }

    void kickGroupMembers(const std::vector<std::string> &userIDs, const std::string &groupID,
                          ZIMGroupMemberKickedCallback callback) override {
        zim_sequence sequence = ++sequence_;

        auto char_user_list = new char *[userIDs.size() + 1];
        stlVector2CharsArray(userIDs, char_user_list);

        {
            std::lock_guard<std::mutex> lock(this->group_member_kickedout_callbacks_mutex_);
            this->group_member_kickedout_callbacks_[sequence] = callback;
        }
        zim_kick_group_members(handle_, (const char **)char_user_list,
                               (unsigned int)(userIDs.size()), groupID.c_str(), &sequence);

        for (unsigned int i = 0; i < static_cast<unsigned int>(userIDs.size()); ++i) {
            delete[](char_user_list[i]);
        }

        delete[] char_user_list;
        char_user_list = NULL;
    }

    void transferGroupOwner(const std::string &toUserID, const std::string &groupID,
                            ZIMGroupOwnerTransferredCallback callback) override {
        zim_sequence sequence = ++sequence_;

        {
            std::lock_guard<std::mutex> lock(this->group_owner_transferred_callbacks_mutex_);
            this->group_owner_transferred_callbacks_[sequence] = callback;
        }
        zim_transfer_group_owner(handle_, toUserID.c_str(), groupID.c_str(), &sequence);
    }

    void updateGroupName(const std::string &groupName, const std::string &groupID,
                         ZIMGroupNameUpdatedCallback callback) override {
        zim_sequence sequence = ++sequence_;
        {
            std::lock_guard<std::mutex> lock(this->group_name_updated_callbacks_mutex_);
            this->group_name_updated_callbacks_[sequence] = callback;
        }
        zim_update_group_name(handle_, groupName.c_str(), groupID.c_str(), &sequence);
    }

    void updateGroupAvatarUrl(const std::string &groupAvatarUrl, const std::string &groupID,
                              ZIMGroupAvatarUrlUpdatedCallback callback) override {
        zim_sequence sequence = ++sequence_;

        {
            std::lock_guard<std::mutex> lock(this->group_avatar_updated_callbacks_mutex_);
            this->group_avatar_updated_callbacks_[sequence] = callback;
        }

        zim_update_group_avatar_url(handle_, groupAvatarUrl.c_str(), groupID.c_str(), &sequence);
    }

    void updateGroupNotice(const std::string &groupNotice, const std::string &groupID,
                           ZIMGroupNoticeUpdatedCallback callback) override {
        zim_sequence sequence = ++sequence_;

        {
            std::lock_guard<std::mutex> lock(this->group_notice_updated_callbacks_mutex_);
            this->group_notice_updated_callbacks_[sequence] = callback;
        }

        zim_update_group_notice(handle_, groupNotice.c_str(), groupID.c_str(), &sequence);
    }

    void muteGroup(bool isMute, const std::string &groupID, const ZIMGroupMuteConfig &config,
                   ZIMGroupMutedCallback callback) override {
        zim_sequence sequence = ++sequence_;

        {
            std::lock_guard<std::mutex> lock(this->group_muted_callbacks_mutex_);
            this->group_muted_callbacks_[sequence] = callback;
        }

        zim_group_mute_config sConfig{};
        ZIMConverter::sZIMGroupMuteConfig(sConfig, config);
        zim_mute_group(handle_, isMute, groupID.c_str(), sConfig, &sequence);
    }

    void muteGroupMembers(bool isMute, const std::vector<std::string> &userIDs,
                          const std::string &groupID, const ZIMGroupMemberMuteConfig &config,
                          ZIMGroupMembersMutedCallback callback) override {
        zim_sequence sequence = ++sequence_;

        {
            std::lock_guard<std::mutex> lock(this->group_member_list_muted_callbacks_mutex_);
            this->group_member_list_muted_callbacks_[sequence] = callback;
        }

        zim_group_member_list_mute_config sConfig{};
        ZIMConverter::sZIMGroupMemberListMuteConfig(sConfig, config);

        std::vector<const char *> cch_vec;
        for (const auto &id : userIDs) {
            cch_vec.push_back(id.c_str());
        }

        zim_mute_group_members(handle_, isMute, groupID.c_str(), cch_vec.data(),
                               static_cast<unsigned int>(cch_vec.size()), sConfig, &sequence);
    }

    void queryGroupMemberMutedList(const std::string &groupID,
                                   const ZIMGroupMemberMutedListQueryConfig &config,
                                   ZIMGroupMemberMutedListQueriedCallback callback) override {
        zim_sequence sequence = ++sequence_;

        {
            std::lock_guard<std::mutex> lock(
                this->group_member_muted_list_queried_callbacks_mutex_);
            this->group_member_muted_list_queried_callbacks_[sequence] = callback;
        }

        zim_group_muted_members_query_config sConfig{};
        ZIMConverter::sZIMGroupMemberMutedListQueryConfig(sConfig, config);

        zim_query_group_muted_member_list(handle_, groupID.c_str(), sConfig, &sequence);
    }

    void queryGroupInfo(const std::string &groupID, ZIMGroupInfoQueriedCallback callback) override {
        zim_sequence sequence = ++sequence_;

        {
            std::lock_guard<std::mutex> lock(this->group_info_queried_callbacks_mutex_);
            this->group_info_queried_callbacks_[sequence] = callback;
        }

        zim_query_group_info(handle_, groupID.c_str(), &sequence);
    }

    void setGroupAttributes(const std::unordered_map<std::string, std::string> &groupAttributes,
                            const std::string &groupID,
                            ZIMGroupAttributesOperatedCallback callback) override {
        zim_sequence sequence = ++sequence_;

        auto c_attributes = new zim_group_attribute[groupAttributes.size()];

        unsigned int i = 0;

        for (auto &it : groupAttributes) {
            memset(&c_attributes[i], 0, sizeof(struct zim_group_attribute));
            char *cKey = const_cast<char *>(it.first.c_str());
            char *cValue = const_cast<char *>(it.second.c_str());
            c_attributes[i].key = cKey;
            c_attributes[i].value = cValue;
            i++;
        }

        {
            std::lock_guard<std::mutex> lock(this->group_attributes_operated_callbacks_mutex_);
            this->group_attributes_operated_callbacks_[sequence] = callback;
        }

        zim_set_group_attributes(handle_, &c_attributes[0], i, groupID.c_str(), &sequence);

        delete[] c_attributes;
        c_attributes = NULL;
    }

    void deleteGroupAttributes(const std::vector<std::string> &keys, const std::string &groupID,
                               ZIMGroupAttributesOperatedCallback callback) override {
        zim_sequence sequence = ++sequence_;

        auto c_attributes_keys = new char *[keys.size()];

        stlVector2CharsArray(keys, c_attributes_keys);

        {
            std::lock_guard<std::mutex> lock(this->group_attributes_operated_callbacks_mutex_);
            this->group_attributes_operated_callbacks_[sequence] = callback;
        }

        zim_delete_group_attributes(handle_, (const char **)c_attributes_keys,
                                    (unsigned int)keys.size(), groupID.c_str(), &sequence);

        for (unsigned int i = 0; i < static_cast<unsigned int>(keys.size()); ++i) {
            delete[](c_attributes_keys[i]);
        }

        delete[] c_attributes_keys;
        c_attributes_keys = NULL;
    }

    void queryGroupAttributes(const std::vector<std::string> &keys, const std::string &groupID,
                              ZIMGroupAttributesQueriedCallback callback) override {
        zim_sequence sequence = ++sequence_;

        auto c_attributes_keys = new char *[keys.size()];

        stlVector2CharsArray(keys, c_attributes_keys);

        {
            std::lock_guard<std::mutex> lock(this->group_attributes_queried_callbacks_mutex_);
            this->group_attributes_queried_callbacks_[sequence] = callback;
        }

        zim_query_group_attributes(handle_, (const char **)c_attributes_keys,
                                   (unsigned int)keys.size(), groupID.c_str(), &sequence);

        for (unsigned int i = 0; i < static_cast<unsigned int>(keys.size()); ++i) {
            delete[](c_attributes_keys[i]);
        }

        delete[] c_attributes_keys;
        c_attributes_keys = NULL;
    }

    void queryGroupAllAttributes(const std::string &groupID,
                                 ZIMGroupAttributesQueriedCallback callback) override {
        zim_sequence sequence = ++sequence_;

        {
            std::lock_guard<std::mutex> lock(this->group_attributes_queried_callbacks_mutex_);
            this->group_attributes_queried_callbacks_[sequence] = callback;
        }

        zim_query_group_all_attributes(handle_, groupID.c_str(), &sequence);
    }

    void setGroupMemberRole(ZIMGroupMemberRole role, const std::string &forUserID,
                            const std::string &groupID,
                            ZIMGroupMemberRoleUpdatedCallback callback) override {
        zim_sequence sequence = ++sequence_;

        {
            std::lock_guard<std::mutex> lock(this->group_member_role_set_callbacks_mutex_);
            this->group_member_role_set_callbacks_[sequence] = callback;
        }
        zim_set_group_member_role(handle_, role, forUserID.c_str(), groupID.c_str(), &sequence);
    }

    void setGroupMemberNickname(const std::string &nickname, const std::string &forUserID,
                                const std::string &groupID,
                                ZIMGroupMemberNicknameUpdatedCallback callback) override {
        zim_sequence sequence = ++sequence_;

        {
            std::lock_guard<std::mutex> lock(this->group_member_nickname_set_callbacks_mutex_);
            this->group_member_nickname_set_callbacks_[sequence] = callback;
        }
        zim_set_group_member_nickname(handle_, nickname.c_str(), forUserID.c_str(), groupID.c_str(),
                                      &sequence);
    }

    void queryGroupMemberInfo(const std::string &userID, const std::string &groupID,
                              ZIMGroupMemberInfoQueriedCallback callback) override {
        zim_sequence sequence = ++sequence_;

        {
            std::lock_guard<std::mutex> lock(this->group_member_info_queried_callbacks_mutex_);
            this->group_member_info_queried_callbacks_[sequence] = callback;
        }
        zim_query_group_member_info(handle_, userID.c_str(), groupID.c_str(), &sequence);
    }

    void queryGroupList(ZIMGroupListQueriedCallback callback) override {
        zim_sequence sequence = ++sequence_;

        {
            std::lock_guard<std::mutex> lock(this->group_list_queried_callbacks_mutex_);
            this->group_list_queried_callbacks_[sequence] = callback;
        }

        zim_query_group_list(handle_, &sequence);
    }

    void queryGroupMemberList(const std::string &groupID, const ZIMGroupMemberQueryConfig &config,
                              ZIMGroupMemberListQueriedCallback callback) override {
        zim_sequence sequence = ++sequence_;

        zim_group_member_query_config c_config;

        c_config.count = config.count;
        c_config.next_flag = config.nextFlag;

        {
            std::lock_guard<std::mutex> lock(this->group_member_list_queried_callbacks_mutex_);
            this->group_member_list_queried_callbacks_[sequence] = callback;
        }

        zim_query_group_member_list(handle_, groupID.c_str(), c_config, &sequence);
    }

    void queryGroupMemberCount(const std::string &groupID,
                               ZIMGroupMemberCountQueriedCallback callback) override {
        zim_sequence sequence = ++sequence_;

        {
            std::lock_guard<std::mutex> lock(this->group_count_queried_callbacks_mutex_);
            this->group_count_queried_callbacks_[sequence] = callback;
        }

        zim_query_group_member_count(handle_, groupID.c_str(), &sequence);
    }

    void setConversationNotificationStatus(
        ZIMConversationNotificationStatus status, const std::string &conversationID,
        ZIMConversationType type, ZIMConversationNotificationStatusSetCallback callback) override {
        zim_sequence sequence = ++sequence_;

        {
            std::lock_guard<std::mutex> lock(
                this->conversation_notification_status_set_callbacks_mutex_);
            this->conversation_notification_status_set_callbacks_[sequence] = callback;
        }

        zim_set_conversation_notification_status(
            handle_, (zim_conversation_notification_status)status, conversationID.c_str(),
            (zim_conversation_type)type, &sequence);
    }

    void callInvite(const std::vector<std::string> &invitees, const ZIMCallInviteConfig &config,
                    ZIMCallInvitationSentCallback callback) override {
        zim_sequence sequence = ++sequence_;
        auto invitees_str_list = new char *[invitees.size()];

        for (unsigned int i = 0; i < invitees.size(); i++) {
            invitees_str_list[i] = const_cast<char *>(invitees[i].c_str());
        }
        zim_call_invite_config inviteConfig = {};
        ZIMConverter::sZIMCallInviteConfig(inviteConfig, &config);

        {
            std::lock_guard<std::mutex> lock(this->call_invitation_sent_callbacks_mutex_);
            this->call_invitation_sent_callbacks_[sequence] = callback;
        }

        zim_call_invite(handle_, (const char **)invitees_str_list, (unsigned int)invitees.size(),
                        inviteConfig, &sequence);

        delete[] invitees_str_list;
    }

    void callCancel(const std::vector<std::string> &invitees, const std::string &callID,
                    ZIMCallCancelConfig config, ZIMCallCancelSentCallback callback) override {
        zim_sequence sequence = ++sequence_;

        struct zim_call_cancel_config cancelConfig = {};

        ZIMConverter::sZIMCallCancelConfig(cancelConfig, &config);

        auto invitees_str_list = new char *[invitees.size()];

        for (unsigned int i = 0; i < invitees.size(); i++) {
            invitees_str_list[i] = const_cast<char *>(invitees[i].c_str());
        }

        {
            std::lock_guard<std::mutex> lock(this->call_cancel_sent_callbacks_mutex_);
            this->call_cancel_sent_callbacks_[sequence] = callback;
        }

        zim_call_cancel(handle_, callID.c_str(), (const char **)invitees_str_list,
                        (unsigned int)invitees.size(), cancelConfig, &sequence);

        delete[] invitees_str_list;
    }

    void callAccept(const std::string &callID, const ZIMCallAcceptConfig &config,
                    ZIMCallAcceptanceSentCallback callback) override {
        zim_sequence sequence = ++sequence_;
        struct zim_call_accept_config acceptConfig = {};
        ZIMConverter::sZIMCallAcceptConfig(acceptConfig, &config);

        {
            std::lock_guard<std::mutex> lock(this->call_acceptance_sent_callbacks_mutex_);
            this->call_acceptance_sent_callbacks_[sequence] = callback;
        }

        zim_call_accept(handle_, callID.c_str(), acceptConfig, &sequence);
    }

    void callReject(const std::string &callID, const ZIMCallRejectConfig &config,
                    ZIMCallRejectionSentCallback callback) override {
        zim_sequence sequence = ++sequence_;
        struct zim_call_reject_config rejectConfig = {};
        ZIMConverter::sZIMCallRejectConfig(rejectConfig, &config);
        {
            std::lock_guard<std::mutex> lock(this->call_rejection_sent_callbacks_mutex_);
            this->call_rejection_sent_callbacks_[sequence] = callback;
        }
        zim_call_reject(handle_, callID.c_str(), rejectConfig, &sequence);
    }

    void callQuit(const std::string &callID, const ZIMCallQuitConfig &config,
                  ZIMCallQuitSentCallback callback) override {
        zim_sequence sequence = ++sequence_;

        zim_call_quit_config callQuitConfig = {};
        ZIMConverter::sZIMCallQuitConfig(callQuitConfig, &config);

        {
            std::lock_guard<std::mutex> lock(this->call_quit_sent_callbacks_mutex_);
            this->call_quit_sent_callbacks_[sequence] = callback;
        }

        zim_call_quit(handle_, callID.c_str(), callQuitConfig, &sequence);
    }

    void callEnd(const std::string &callID, const ZIMCallEndConfig &config,
                 ZIMCallEndSentCallback callback) override {
        zim_sequence sequence = ++sequence_;

        zim_call_end_config callEndConfig = {};
        ZIMConverter::sZIMCallEndConfig(callEndConfig, &config);

        {
            std::lock_guard<std::mutex> lock(this->call_end_sent_callbacks_mutex_);
            this->call_end_sent_callbacks_[sequence] = callback;
        }

        zim_call_end(handle_, callID.c_str(), callEndConfig, &sequence);
    }

    void callingInvite(const std::vector<std::string> &invitees, const std::string &callID,
                       const ZIMCallingInviteConfig &config,
                       ZIMCallingInvitationSentCallback callback) override {
        zim_sequence sequence = ++sequence_;
        auto invitees_str_list = new char *[invitees.size()];

        for (unsigned int i = 0; i < invitees.size(); i++) {
            invitees_str_list[i] = const_cast<char *>(invitees[i].c_str());
        }

        zim_calling_invite_config inviteConfig = {};
        ZIMConverter::sZIMCallingInviteConfig(inviteConfig, &config);

        {
            std::lock_guard<std::mutex> lock(this->calling_invitation_sent_callbacks_mutex_);
            this->calling_invitation_sent_callbacks_[sequence] = callback;
        }

        zim_calling_invite(handle_, callID.c_str(), (const char **)invitees_str_list,
                           (unsigned int)invitees.size(), inviteConfig, &sequence);

        delete[] invitees_str_list;
    }

    void callJoin(const std::string &callID, const ZIMCallJoinConfig &config,
                  ZIMCallJoinSentCallback callback) override {
        zim_sequence sequence = ++sequence_;

        zim_call_join_config joinConfig = {};
        ZIMConverter::sZIMCallJoinConfig(joinConfig, &config);

        {
            std::lock_guard<std::mutex> lock(this->call_join_sent_callbacks_mutex_);
            this->call_join_sent_callbacks_[sequence] = callback;
        }

        zim_call_join(handle_, callID.c_str(), joinConfig, &sequence);
    }

    void queryCallInvitationList(const ZIMCallInvitationQueryConfig &config,
                                 ZIMCallInvitationListQueriedCallback callback) override {

        zim_sequence sequence = ++sequence_;
        zim_query_call_list_config queryCallListConfig = {};
        ZIMConverter::sZIMQueryCallListConfig(queryCallListConfig, &config);

        {
            std::lock_guard<std::mutex> lock(this->query_call_list_callbacks_mutex_);
            this->query_call_list_callbacks_[sequence] = callback;
        }

        zim_query_call_list(handle_, queryCallListConfig, &sequence);
    }

    // Internal Function for reflection

    void addFriend(const std::string &userId, const ZIMFriendAddConfig &config,
                   ZIMFriendAddedCallback callback) override {

        zim_sequence sequence = ++sequence_;
        {
            std::lock_guard<std::mutex> lock(this->add_friend_application_callbacks_mutex_);
            this->add_friend_application_callbacks_[sequence] = callback;
        }
        zim_friend_add_config new_config{};
        ZIMConverter::sZIMFriendAddConfig(new_config, &config);
        zim_add_friend(handle_, userId.c_str(), new_config, &sequence);
        delete[] new_config.friend_attributes;
    }

    void sendFriendApplication(const std::string &userID,
                               const ZIMFriendApplicationSendConfig &config,
                               ZIMFriendApplicationSentCallback callback) override {
        zim_sequence sequence = ++sequence_;
        {
            std::lock_guard<std::mutex> lock(this->send_friend_application_callbacks_mutex_);
            this->send_friend_application_callbacks_[sequence] = callback;
        }

        zim_friend_application_send_config new_config{};
        ZIMConverter::sZIMFriendApplicationSendConfig(new_config, &config);
        zim_send_friend_application(handle_, userID.c_str(), new_config, &sequence);
        delete[] new_config.friend_attributes;
    }

    void deleteFriends(const std::vector<std::string> &userIDs, const ZIMFriendDeleteConfig &config,
                       ZIMFriendsDeletedCallback callback) override {
        zim_sequence sequence = ++sequence_;

        {
            std::lock_guard<std::mutex> lock(this->friend_deleted_callbacks_mutex_);
            this->friend_deleted_callbacks_[sequence] = callback;
        }

        zim_friend_delete_config new_config{};

        auto user_id_str_list = new char *[userIDs.size()];

        for (unsigned int i = 0; i < userIDs.size(); i++) {
            user_id_str_list[i] = const_cast<char *>(userIDs[i].c_str());
        }
        new_config.type = (zim_friend_delete_type)config.type;
        zim_delete_friends(handle_, (const char **)user_id_str_list, (unsigned int)userIDs.size(),
                           new_config, &sequence);
        delete[] user_id_str_list;
    }

    void searchLocalFriends(const ZIMFriendSearchConfig &config,
                            ZIMFriendsSearchedCallback callback) override {
        zim_sequence sequence = ++sequence_;

        {
            std::lock_guard<std::mutex> lock(this->friend_searched_callbacks_mutex_);
            this->friend_searched_callbacks_[sequence] = callback;
        }
        zim_friend_search_config new_config{};
        ZIMConverter::sZIMFriendSearchConfig(new_config, &config);
        zim_search_local_friends(handle_, new_config, &sequence);
    }

    void checkFriendsRelation(const std::vector<std::string> &userIDs,
                              const ZIMFriendRelationCheckConfig &config,
                              ZIMFriendsRelationCheckedCallback callback) override {
        zim_sequence sequence = ++sequence_;

        {
            std::lock_guard<std::mutex> lock(this->friend_relation_checked_callbacks_mutex_);
            this->friend_relation_checked_callbacks_[sequence] = callback;
        }

        auto user_id_str_list = new char *[userIDs.size()];

        for (unsigned int i = 0; i < userIDs.size(); i++) {
            user_id_str_list[i] = const_cast<char *>(userIDs[i].c_str());
        }

        zim_friend_relation_check_config new_config{};

        ZIMConverter::sZIMFriendCheckConfig(new_config, &config);
        zim_check_friends_relation(handle_, (const char **)user_id_str_list,
                                   (unsigned int)userIDs.size(), new_config, &sequence);

        delete[] user_id_str_list;
    }

    void updateFriendAlias(const std::string &friendAlias, const std::string &userID,
                           ZIMFriendAliasUpdatedCallback callback) override {
        zim_sequence sequence = ++sequence_;
        {
            std::lock_guard<std::mutex> lock(this->friends_alias_updated_callbacks_mutex_);
            this->friends_alias_updated_callbacks_[sequence] = callback;
        }
        zim_update_friend_alias(handle_, friendAlias.c_str(), userID.c_str(), &sequence);
    }

    void
    updateFriendAttributes(const std::unordered_map<std::string, std::string> &friendAttributes,
                           const std::string &userID,
                           ZIMFriendAttributesUpdatedCallback callback) override {

        zim_sequence sequence = ++sequence_;
        {
            std::lock_guard<std::mutex> lock(this->friends_attributes_updated_callbacks_mutex_);
            this->friends_attributes_updated_callbacks_[sequence] = callback;
        }
        struct zim_friend_attribute *new_friend_attributes = nullptr;
        if (!friendAttributes.empty()) {
            new_friend_attributes = new zim_friend_attribute[friendAttributes.size()];
            unsigned int i = 0;
            for (auto &it : friendAttributes) {
                new_friend_attributes[i].key = it.first.c_str();
                new_friend_attributes[i].value = it.second.c_str();
                i++;
            }
        }
        zim_update_friend_attributes(handle_, new_friend_attributes,
                                     (unsigned int)friendAttributes.size(), userID.c_str(),
                                     &sequence);
        delete[] new_friend_attributes;
    }

    void queryFriendsInfo(const std::vector<std::string> &userIDs,
                          ZIMFriendsInfoQueriedCallback callback) override {
        zim_sequence sequence = ++sequence_;

        auto user_id_str_list = new char *[userIDs.size()];

        for (unsigned int i = 0; i < userIDs.size(); i++) {
            user_id_str_list[i] = const_cast<char *>(userIDs[i].c_str());
        }
        {
            std::lock_guard<std::mutex> lock(this->friends_info_queried_callbacks_mutex_);
            this->friends_info_queried_callbacks_[sequence] = callback;
        }

        zim_query_friends_info(handle_, (const char **)user_id_str_list,
                               (unsigned int)userIDs.size(), &sequence);
    }

    void acceptFriendApplication(const std::string &userID,
                                 const ZIMFriendApplicationAcceptConfig &config,
                                 ZIMFriendApplicationAcceptedCallback callback) override {

        zim_sequence sequence = ++sequence_;
        {
            std::lock_guard<std::mutex> lock(this->friend_application_acceoted_callback_mutex_);
            this->friend_application_acceoted_callback_[sequence] = callback;
        }
        zim_friend_application_accept_config new_config{};
        ZIMConverter::sZIMFriendApplicationAcceptConfig(new_config, &config);
        zim_accept_friend_application(handle_, userID.c_str(), new_config, &sequence);
        if (config.friendAttributes.size() > 0) {
            delete[] new_config.friend_attributes;
        }
    }

    void rejectFriendApplication(const std::string &userID,
                                 const ZIMFriendApplicationRejectConfig &config,
                                 ZIMFriendApplicationRejectedCallback callback) override {

        zim_sequence sequence = ++sequence_;
        {
            std::lock_guard<std::mutex> lock(this->friend_application_rejected_callback_mutex_);
            this->friend_application_rejected_callback_[sequence] = callback;
        }
        zim_friend_application_reject_config new_config{};
        ZIMConverter::sZIMFriendApplicationRejectConfig(new_config, &config);
        zim_friend_reject_application(handle_, userID.c_str(), new_config, &sequence);
    }

    void queryFriendList(const ZIMFriendListQueryConfig &config,
                         ZIMFriendListQueriedCallback callback) override {
        zim_sequence sequence = ++sequence_;
        {
            std::lock_guard<std::mutex> lock(this->friend_list_queried_callback_mutex_);
            this->friend_list_queried_callback_[sequence] = callback;
        }
        zim_friend_list_query_config query_config{};
        ZIMConverter::sZIMFriendListQueryConfig(query_config, &config);
        zim_query_friend_list(handle_, query_config, &sequence);
    }

    void queryFriendApplicationList(const ZIMFriendApplicationListQueryConfig &config,
                                    ZIMFriendApplicationListQueriedCallback callback) override {
        zim_sequence sequence = ++sequence_;
        {
            std::lock_guard<std::mutex> lock(this->friend_application_list_queried_callbaks_mutex_);
            this->friend_application_list_queried_callbaks_[sequence] = callback;
        }
        zim_friend_application_list_query_config query_config = {};
        ZIMConverter::sZIMFriendApplicationListQueryConfig(query_config, &config);
        zim_query_friend_application_list(handle_, query_config, &sequence);
    }

    void addUsersToBlacklist(const std::vector<std::string> &userIDs,
                             ZIMBlacklistUsersAddedCallback callback) override {
        zim_sequence sequence = ++sequence_;

        auto user_id_str_list = new char *[userIDs.size()];

        for (unsigned int i = 0; i < userIDs.size(); i++) {
            user_id_str_list[i] = const_cast<char *>(userIDs[i].c_str());
        }

        {
            std::lock_guard<std::mutex> lock(this->blacklist_users_added_callbacks_mutex_);
            this->blacklist_users_added_callbacks_[sequence] = callback;
        }

        zim_add_users_to_blacklist(handle_, (const char **)user_id_str_list,
                                   static_cast<unsigned int>(userIDs.size()), &sequence);
        delete[] user_id_str_list;
    }

    void removeUsersFromBlacklist(const std::vector<std::string> &userIDs,
                                  ZIMBlacklistUsersRemovedCallback callback) override {
        zim_sequence sequence = ++sequence_;

        auto user_id_str_list = new char *[userIDs.size()];

        for (unsigned int i = 0; i < userIDs.size(); i++) {
            user_id_str_list[i] = const_cast<char *>(userIDs[i].c_str());
        }

        {
            std::lock_guard<std::mutex> lock(this->blacklist_users_removed_callbacks_mutex_);
            this->blacklist_users_removed_callbacks_[sequence] = callback;
        }

        zim_remove_users_from_blacklist(handle_, (const char **)user_id_str_list,
                                        static_cast<unsigned int>(userIDs.size()), &sequence);
        delete[] user_id_str_list;
    }

    void queryBlacklist(const ZIMBlacklistQueryConfig &config,
                        ZIMBlacklistQueriedCallback callback) override {
        zim_sequence sequence = ++sequence_;
        {
            std::lock_guard<std::mutex> lock(this->blacklist_queried_callbacks_mutex_);
            this->blacklist_queried_callbacks_[sequence] = callback;
        }
        zim_blacklist_query_config queryConfig = {};
        ZIMConverter::sZIMBlacklistQueryConfig(queryConfig, &config);

        zim_query_blacklist(handle_, queryConfig, &sequence);
    }

    void checkUserIsInBlacklist(const std::string &userID,
                                ZIMBlacklistCheckedCallback callback) override {
        zim_sequence sequence = ++sequence_;
        {
            std::lock_guard<std::mutex> lock(this->blacklist_checked_callbacks_mutex_);
            this->blacklist_checked_callbacks_[sequence] = callback;
        }
        zim_check_user_is_in_blacklist(handle_, userID.c_str(), &sequence);
    }

  public:
    zim_handle handle_ = nullptr;

  public:
    std::shared_ptr<ZIMEventHandler> event_handler_;

  public:
    std::unordered_map<zim_sequence, ZIMLoggedInCallback> logged_in_callbacks_;
    std::mutex logged_in_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMTokenRenewedCallback> renew_token_callbacks_;
    std::mutex renew_token_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMUserNameUpdatedCallback> update_user_name_callbacks_;
    std::mutex update_user_name_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMUserAvatarUrlUpdatedCallback> update_user_avatar_callbacks_;
    std::mutex update_user_avatar_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMUserExtendedDataUpdatedCallback>
        update_user_extended_data_callbacks_;
    std::mutex update_user_extended_data_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMUsersInfoQueriedCallback> users_info_queried_callbacks_;
    std::mutex users_info_queried_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMMessageQueriedCallback> message_queried_callbacks_;
    std::mutex message_queried_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMMessageSentCallback> message_sent_callbacks_;
    std::mutex message_sent_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMMessageInsertedCallback> message_inserted_callbacks_;
    std::mutex message_inserted_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMMessageReceiptsReadSentCallback>
        message_receipts_read_callbacks_;
    std::mutex message_receipts_read_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMMessageReceiptsInfoQueriedCallback>
        message_receipts_queried_callbacks_;
    std::mutex message_receipts_queried_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMMediaDownloadedCallback> media_download_callbacks_;
    std::mutex media_download_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMMediaUploadingProgress> media_send_progress_callbacks_;
    std::mutex media_send_progress_callbacks_mutex_;

    std::unordered_map<zim_sequence, std::shared_ptr<ZIMMessageSendNotification>>
        send_message_notification_callbacks_;
    std::mutex send_message_notification_callbacks_mutex_;

    std::unordered_map<zim_sequence, std::shared_ptr<ZIMMediaMessageSendNotification>>
        send_media_message_notification_callbacks_;
    std::mutex send_media_message_notification_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMMediaDownloadingProgress>
        media_downloading_progress_callbacks_;
    std::mutex media_downloading_progress_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMMessageDeletedCallback> message_deleted_callbacks_;
    std::mutex message_deleted_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMConversationMessagesAllDeletedCallback>
        conversation_messages_all_deleted_callbacks_;
    std::mutex conversation_messages_all_deleted_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMMessageLocalExtendedDataUpdatedCallback>
        message_local_extended_data_updated_callbacks_;
    std::mutex message_local_extended_data_updated_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMMessageRevokedCallback> message_revoked_callbacks_;
    std::mutex message_revoked_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMMessagesSearchedCallback> message_searched_callbacks_;
    std::mutex message_searched_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMMessagesGlobalSearchedCallback>
        message_global_searched_callbacks_;
    std::mutex message_global_searched_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMConversationsSearchedCallback>
        conversations_searched_callbacks_;
    std::mutex conversations_searched_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMConversationQueriedCallback>
        conversation_queried_callbacks_;
    std::mutex conversation_queried_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMMessageReactionAddedCallback> reaction_added_callbacks_;
    std::mutex reaction_added_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMMessageReactionDeletedCallback> reaction_deleted_callbacks_;
    std::mutex reaction_deleted_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMMessageReactionUserListQueriedCallback>
        reaction_user_list_queried_callbacks_;
    std::mutex reaction_user_list_queried_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMConversationListQueriedCallback>
        conversation_list_queried_callbacks_;
    std::mutex conversation_list_queried_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMConversationPinnedListQueriedCallback>
        conversation_pinned_list_queried_callbacks_;
    std::mutex conversation_pinned_list_queried_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMConversationUnreadMessageCountClearedCallback>
        conversation_unread_count_cleared_callbacks_;
    std::mutex conversation_unread_count_cleared_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMConversationTotalUnreadMessageCountClearedCallback>
        conversations_all_unread_count_cleared_callbacks_;
    std::mutex conversations_all_unread_count_cleared_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMConversationPinnedStateUpdatedCallback>
        conversation_pinned_state_updated_callbacks_;
    std::mutex conversation_pinned_state_updated_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMConversationDeletedCallback>
        conversation_deleted_callbacks_;
    std::mutex conversation_deleted_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMConversationsAllDeletedCallback>
        conversations_all_deleted_callbacks_;
    std::mutex conversations_all_deleted_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMConversationMessageReceiptReadSentCallback>
        conversation_receipt_read_callbacks_;
    std::mutex conversation_receipt_read_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMCombineMessageDetailQueriedCallback>
        message_combine_queried_callbacks_;
    std::mutex message_combine_queried_callbacks_mutex_;
    std::unordered_map<zim_sequence, ZIMConversationDraftSetCallback>
        conversation_draft_set_callbacks_;
    std::mutex conversation_draft_set_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMRoomCreatedCallback> room_created_callbacks_;
    std::mutex room_created_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMRoomJoinedCallback> room_joined_callbacks_;
    std::mutex room_joined_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMRoomEnteredCallback> room_entered_callbacks_;
    std::mutex room_entered_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMRoomLeftCallback> room_left_callbacks_;
    std::mutex room_left_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMRoomOnlineMemberCountQueriedCallback>
        online_count_queried_callbacks_;
    std::mutex online_count_queried_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMLogUploadedCallback> log_uploaded_callbacks_;
    std::mutex log_uploaded_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMRoomAttributesOperatedCallback>
        room_attributes_operated_callbacks_;
    std::mutex room_attributes_operated_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMRoomAttributesQueriedCallback>
        room_all_attributes_queried_callbacks_;
    std::mutex room_all_attributes_queried_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMRoomAttributesBatchOperatedCallback>
        room_attributes_batch_operated_callbacks_;
    std::mutex room_attributes_batch_operated_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMRoomMembersAttributesOperatedCallback>
        room_members_attributes_operated_callbacks_;
    std::mutex room_members_attributes_operated_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMRoomMembersAttributesQueriedCallback>
        room_members_attributes_queried_callbacks_;
    std::mutex room_members_attributes_queried_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMRoomMemberAttributesListQueriedCallback>
        room_member_attributes_list_queried_callbacks_;
    std::mutex room_member_attributes_list_queried_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMRoomMemberQueriedCallback> member_queried_callbacks_;
    std::mutex member_queried_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMRoomMembersQueriedCallback> members_queried_callbacks_;
    std::mutex members_queried_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMGroupCreatedCallback> group_created_callbacks_;
    std::mutex group_created_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMGroupDismissedCallback> group_dismissed_callbacks_;
    std::mutex group_dismissed_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMGroupJoinedCallback> group_joined_callbacks_;
    std::mutex group_joined_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMGroupLeftCallback> group_left_callbacks_;
    std::mutex group_left_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMGroupUsersInvitedCallback> group_users_invited_callbacks_;
    std::mutex group_users_invited_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMGroupMemberKickedCallback>
        group_member_kickedout_callbacks_;
    std::mutex group_member_kickedout_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMGroupOwnerTransferredCallback>
        group_owner_transferred_callbacks_;
    std::mutex group_owner_transferred_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMGroupNameUpdatedCallback> group_name_updated_callbacks_;
    std::mutex group_name_updated_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMGroupAvatarUrlUpdatedCallback>
        group_avatar_updated_callbacks_;
    std::mutex group_avatar_updated_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMGroupNoticeUpdatedCallback> group_notice_updated_callbacks_;
    std::mutex group_notice_updated_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMGroupMutedCallback> group_muted_callbacks_;
    std::mutex group_muted_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMGroupMembersMutedCallback>
        group_member_list_muted_callbacks_;
    std::mutex group_member_list_muted_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMGroupMemberMutedListQueriedCallback>
        group_member_muted_list_queried_callbacks_;
    std::mutex group_member_muted_list_queried_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMGroupInfoQueriedCallback> group_info_queried_callbacks_;
    std::mutex group_info_queried_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMGroupAttributesOperatedCallback>
        group_attributes_operated_callbacks_;
    std::mutex group_attributes_operated_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMGroupAttributesQueriedCallback>
        group_attributes_queried_callbacks_;
    std::mutex group_attributes_queried_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMGroupMemberRoleUpdatedCallback>
        group_member_role_set_callbacks_;
    std::mutex group_member_role_set_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMGroupMemberNicknameUpdatedCallback>
        group_member_nickname_set_callbacks_;
    std::mutex group_member_nickname_set_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMGroupMemberInfoQueriedCallback>
        group_member_info_queried_callbacks_;
    std::mutex group_member_info_queried_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMGroupListQueriedCallback> group_list_queried_callbacks_;
    std::mutex group_list_queried_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMGroupMemberListQueriedCallback>
        group_member_list_queried_callbacks_;
    std::mutex group_member_list_queried_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMGroupMemberCountQueriedCallback>
        group_count_queried_callbacks_;
    std::mutex group_count_queried_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMGroupMessageReceiptMemberListQueriedCallback>
        group_receipt_detail_queried_callbacks_;
    std::mutex group_receipt_detail_queried_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMGroupsSearchedCallback> groups_searched_callbacks_;
    std::mutex groups_searched_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMGroupMembersSearchedCallback>
        group_members_searched_callbacks_;
    std::mutex group_members_searched_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMConversationNotificationStatusSetCallback>
        conversation_notification_status_set_callbacks_;
    std::mutex conversation_notification_status_set_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMCallInvitationSentCallback> call_invitation_sent_callbacks_;
    std::mutex call_invitation_sent_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMCallingInvitationSentCallback>
        calling_invitation_sent_callbacks_;
    std::mutex calling_invitation_sent_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMCallJoinSentCallback> call_join_sent_callbacks_;
    std::mutex call_join_sent_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMCallQuitSentCallback> call_quit_sent_callbacks_;
    std::mutex call_quit_sent_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMCallEndSentCallback> call_end_sent_callbacks_;
    std::mutex call_end_sent_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMCallInvitationListQueriedCallback>
        query_call_list_callbacks_;
    std::mutex query_call_list_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMCallCancelSentCallback> call_cancel_sent_callbacks_;
    std::mutex call_cancel_sent_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMCallAcceptanceSentCallback> call_acceptance_sent_callbacks_;
    std::mutex call_acceptance_sent_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMCallRejectionSentCallback> call_rejection_sent_callbacks_;
    std::mutex call_rejection_sent_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMFriendApplicationListQueriedCallback>
        friend_application_list_queried_callbaks_;
    std::mutex friend_application_list_queried_callbaks_mutex_;

    std::unordered_map<zim_sequence, ZIMFriendListQueriedCallback> friend_list_queried_callback_;
    std::mutex friend_list_queried_callback_mutex_;

    std::unordered_map<zim_sequence, ZIMFriendApplicationRejectedCallback>
        friend_application_rejected_callback_;
    std::mutex friend_application_rejected_callback_mutex_;

    std::unordered_map<zim_sequence, ZIMFriendAttributesUpdatedCallback>
        friends_attributes_updated_callbacks_;
    std::mutex friends_attributes_updated_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMFriendAliasUpdatedCallback>
        friends_alias_updated_callbacks_;
    std::mutex friends_alias_updated_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMFriendsRelationCheckedCallback>
        friend_relation_checked_callbacks_;
    std::mutex friend_relation_checked_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMFriendsSearchedCallback> friend_searched_callbacks_;
    std::mutex friend_searched_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMFriendsDeletedCallback> friend_deleted_callbacks_;
    std::mutex friend_deleted_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMFriendApplicationSentCallback>
        send_friend_application_callbacks_;
    std::mutex send_friend_application_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMFriendAddedCallback> add_friend_application_callbacks_;
    std::mutex add_friend_application_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMFriendApplicationAcceptedCallback>
        friend_application_acceoted_callback_;
    std::mutex friend_application_acceoted_callback_mutex_;

    std::unordered_map<zim_sequence, ZIMFriendsInfoQueriedCallback> friends_info_queried_callbacks_;
    std::mutex friends_info_queried_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMBlacklistUsersAddedCallback>
        blacklist_users_added_callbacks_;
    std::mutex blacklist_users_added_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMBlacklistUsersRemovedCallback>
        blacklist_users_removed_callbacks_;
    std::mutex blacklist_users_removed_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMBlacklistQueriedCallback> blacklist_queried_callbacks_;
    std::mutex blacklist_queried_callbacks_mutex_;

    std::unordered_map<zim_sequence, ZIMBlacklistCheckedCallback> blacklist_checked_callbacks_;
    std::mutex blacklist_checked_callbacks_mutex_;

  public:
    std::unordered_map<zim_sequence, std::shared_ptr<ZIMMessage>> message_obj_map_;
    std::mutex message_obj_map_mutex_;

  private:
    std::atomic<int> sequence_{0};
};

} // namespace zim

// MARK: - Callback bridge

#include "ZIMBridge.h"

// MARK: - Function implementation

#ifdef ZIM_MAIN_CONFIG
#include "ZIMInternal.h"
#endif
