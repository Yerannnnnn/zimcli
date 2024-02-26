#pragma once
#include <cstring>
//
//  ZIMConverter.h
//  ZIM
//
//  Created by Patrick Fu on 2021/6/20.
//  Copyright © 2021 Zego. All rights reserved.
//

namespace zim {

class ZIMConverter {

  public:
    // MARK: - ZIMMessage <- zim_message
    static void cZIMMessage(std::shared_ptr<ZIMMessage> toMessage,
                            struct zim_message &fromMessage) {

        if ((zim_message_type)toMessage->getType() != fromMessage.type) {
            return;
        }

        if (fromMessage.type == zim_message_type_text) {

            auto textMessage = std::static_pointer_cast<ZIMTextMessage>(toMessage);
            textMessage->message = fromMessage.message;

        } else if (fromMessage.type == zim_message_type_command) {

            auto commandMessage = std::static_pointer_cast<ZIMCommandMessage>(toMessage);
            std::vector<uint8_t> message(
                &(fromMessage.command_message)[0],
                &(fromMessage.command_message)[fromMessage.command_message_length]);
            commandMessage->message = message;

        } else if (fromMessage.type == zim_message_type_barrage) {

            auto barrageMessage = std::static_pointer_cast<ZIMBarrageMessage>(toMessage);
            barrageMessage->message = fromMessage.message;

        } else if (fromMessage.type == zim_message_type_image) {

            auto imageMessage = std::static_pointer_cast<ZIMImageMessage>(toMessage);

            imageMessage->fileUID = fromMessage.file_uid ? fromMessage.file_uid : "";
            imageMessage->fileName = fromMessage.file_name ? fromMessage.file_name : "";
            imageMessage->fileSize = fromMessage.file_size;
            imageMessage->fileDownloadUrl =
                fromMessage.file_download_url ? fromMessage.file_download_url : "";
            imageMessage->fileLocalPath =
                fromMessage.file_local_path ? fromMessage.file_local_path : "";

            imageMessage->largeImageDownloadUrl =
                fromMessage.large_image_download_url ? fromMessage.large_image_download_url : "";
            imageMessage->largeImageLocalPath =
                fromMessage.large_image_local_path ? fromMessage.large_image_local_path : "";
            imageMessage->thumbnailDownloadUrl =
                fromMessage.thumbnail_download_url ? fromMessage.thumbnail_download_url : "";
            imageMessage->thumbnailLocalPath =
                fromMessage.thumbnail_local_path ? fromMessage.thumbnail_local_path : "";
            imageMessage->originalImageWidth = fromMessage.original_image_width;
            imageMessage->originalImageHeight = fromMessage.original_image_height;
            imageMessage->largeImageWidth = fromMessage.large_image_width;
            imageMessage->largeImageHeight = fromMessage.large_image_height;
            imageMessage->thumbnailWidth = fromMessage.thumbnail_width;
            imageMessage->thumbnailHeight = fromMessage.thumbnail_height;

        } else if (fromMessage.type == zim_message_type_file) {

            auto fileMessage = std::static_pointer_cast<ZIMFileMessage>(toMessage);

            fileMessage->fileUID = fromMessage.file_uid ? fromMessage.file_uid : "";
            fileMessage->fileName = fromMessage.file_name ? fromMessage.file_name : "";
            fileMessage->fileSize = fromMessage.file_size;
            fileMessage->fileDownloadUrl =
                fromMessage.file_download_url ? fromMessage.file_download_url : "";
            fileMessage->fileLocalPath =
                fromMessage.file_local_path ? fromMessage.file_local_path : "";

        } else if (fromMessage.type == zim_message_type_audio) {

            auto audioMessage = std::static_pointer_cast<ZIMAudioMessage>(toMessage);

            audioMessage->fileUID = fromMessage.file_uid ? fromMessage.file_uid : "";
            audioMessage->fileName = fromMessage.file_name ? fromMessage.file_name : "";
            audioMessage->fileSize = fromMessage.file_size;
            audioMessage->fileDownloadUrl =
                fromMessage.file_download_url ? fromMessage.file_download_url : "";
            audioMessage->fileLocalPath =
                fromMessage.file_local_path ? fromMessage.file_local_path : "";

            audioMessage->audioDuration = static_cast<unsigned int>(fromMessage.media_duration);

        } else if (fromMessage.type == zim_message_type_video) {

            auto videoMessage = std::static_pointer_cast<ZIMVideoMessage>(toMessage);

            videoMessage->fileUID = fromMessage.file_uid ? fromMessage.file_uid : "";
            videoMessage->fileName = fromMessage.file_name ? fromMessage.file_name : "";
            videoMessage->fileSize = fromMessage.file_size;
            videoMessage->fileDownloadUrl =
                fromMessage.file_download_url ? fromMessage.file_download_url : "";
            videoMessage->fileLocalPath =
                fromMessage.file_local_path ? fromMessage.file_local_path : "";

            videoMessage->videoDuration = static_cast<unsigned int>(fromMessage.media_duration);
            videoMessage->videoFirstFrameLocalPath = fromMessage.video_first_frame_local_path
                                                         ? fromMessage.video_first_frame_local_path
                                                         : "";
            videoMessage->videoFirstFrameDownloadUrl =
                fromMessage.video_first_frame_download_url
                    ? fromMessage.video_first_frame_download_url
                    : "";
            videoMessage->videoFirstFrameWidth = fromMessage.video_first_frame_width;
            videoMessage->videoFirstFrameHeight = fromMessage.video_first_frame_height;

        } else if (fromMessage.type == zim_message_type_system) {

            auto sysMessage = std::static_pointer_cast<ZIMSystemMessage>(toMessage);
            sysMessage->message = fromMessage.message;

        } else if (fromMessage.type == zim_message_type_revoke) {
            auto revokeMessage = std::static_pointer_cast<ZIMRevokeMessage>(toMessage);
            revokeMessage->revokeType = (ZIMRevokeType)fromMessage.revoke_type;
            revokeMessage->revokeTimestamp = fromMessage.revoke_timestamp;
            revokeMessage->operatedUserID =
                fromMessage.operated_userid ? fromMessage.operated_userid : "";
            revokeMessage->originalMessageType = (ZIMMessageType)fromMessage.original_message_type;
            revokeMessage->originalTextMessageContent = fromMessage.original_text_message;
            revokeMessage->revokeExtendedData = fromMessage.revoke_extended_data;
            revokeMessage->revokeStatus = (ZIMMessageRevokeStatus)fromMessage.revoke_status;

        } else if (fromMessage.type == zim_message_type_custom) {

            auto customMessage = std::static_pointer_cast<ZIMCustomMessage>(toMessage);
            customMessage->message = fromMessage.message;
            customMessage->subType = fromMessage.sub_type;
            customMessage->searchedContent = fromMessage.searched_content;
        } else if (fromMessage.type == zim_message_type_combine) {
            auto combineMessage = std::static_pointer_cast<ZIMCombineMessage>(toMessage);
            combineMessage->summary =
                fromMessage.combine_summary ? fromMessage.combine_summary : "";
            combineMessage->title = fromMessage.combine_title ? fromMessage.combine_title : "";
            combineMessage->combineID = fromMessage.combine_id ? fromMessage.combine_id : "";
            for (unsigned int i = 0; i < fromMessage.combine_message_list_length; i++) {
                struct zim_message sMessage = *(fromMessage.combine_message_list + i);
                std::shared_ptr<ZIMMessage> message = ZIMConverter::oZIMMessage(&sMessage);
                combineMessage->messageList.emplace_back(message);
            }
        } else {
        }

        toMessage->messageID = fromMessage.message_id;
        toMessage->localMessageID = fromMessage.local_message_id;
        toMessage->timestamp = fromMessage.timestamp;
        toMessage->senderUserID = fromMessage.sender_user_id;
        toMessage->conversationID = fromMessage.conversation_id;
        toMessage->conversationType = (ZIMConversationType)fromMessage.conversation_type;
        toMessage->conversationSeq = fromMessage.conversation_seq;
        toMessage->direction = (ZIMMessageDirection)fromMessage.direction;
        toMessage->sentStatus = (ZIMMessageSentStatus)fromMessage.sent_status;
        toMessage->receiptStatus = (ZIMMessageReceiptStatus)fromMessage.receipt_status;
        toMessage->orderKey = fromMessage.order_key;
        toMessage->userInserted = fromMessage.is_user_inserted;
        toMessage->extendedData = fromMessage.extended_data;
        toMessage->localExtendedData = fromMessage.local_extended_data;
        toMessage->broadcastMessage = fromMessage.is_broadcast_message;
        oZIMMessageReactionList(fromMessage.reactions, fromMessage.reaction_length,
                                toMessage->reactions);
        toMessage->isMentionAll = fromMessage.is_mention_all;
        toMessage->serverMessage = fromMessage.is_server_message;
        oZIMMessageMentionedUserIDs(fromMessage.mentioned_user_ids,
                                    fromMessage.mentioned_user_ids_length,
                                    toMessage->mentionedUserIDs);
    }

  public:
    // MARK: - object -> struct
    static void
    sZIMMessageReactionUserQueryConfig(struct zim_message_reaction_users_query_config *sConfig,
                                       const ZIMMessageReactionUserQueryConfig *oConfig) {
        if (!oConfig) {
            return;
        }
        sConfig->count = oConfig->count;
        sConfig->next_flag = oConfig->nextFlag;
        sConfig->reaction_type = const_cast<char *>(oConfig->reactionType.c_str());
    };

    static void sNewZIMMessage(struct zim_message &sMessage, const ZIMMessage *oMessage) {
        if (!oMessage) {
            return;
        }
        memset(&sMessage, 0, sizeof(struct zim_message));
        if (oMessage->type == ZIM_MESSAGE_TYPE_TEXT) {
            ZIMTextMessage *textMessage = (ZIMTextMessage *)oMessage;
            char *message = const_cast<char *>(textMessage->message.c_str());
            sMessage.message = message;
            sMessage.type = zim_message_type_text;
        } else if (oMessage->type == ZIM_MESSAGE_TYPE_COMMAND) {
            ZIMCommandMessage *commandMessage = (ZIMCommandMessage *)oMessage;
            sMessage.command_message = commandMessage->message.data();
            sMessage.command_message_length = (unsigned int)commandMessage->message.size();
            sMessage.type = zim_message_type_command;
        } else if (oMessage->type == ZIM_MESSAGE_TYPE_BARRAGE) {
            ZIMBarrageMessage *barrageMessage = (ZIMBarrageMessage *)oMessage;
            sMessage.message = const_cast<char *>(barrageMessage->message.c_str());
            sMessage.type = zim_message_type_barrage;
        } else if (oMessage->type == ZIM_MESSAGE_TYPE_IMAGE) {
            ZIMImageMessage *imageMessage = (ZIMImageMessage *)oMessage;
            sMessage.file_local_path = const_cast<char *>(imageMessage->fileLocalPath.c_str());
            sMessage.file_uid = const_cast<char *>(imageMessage->getFileUID().c_str());
            sMessage.file_name = const_cast<char *>(imageMessage->fileName.c_str());
            sMessage.file_size = imageMessage->getFileSize();
            sMessage.file_download_url = const_cast<char *>(imageMessage->fileDownloadUrl.c_str());
            sMessage.large_image_download_url =
                const_cast<char *>(imageMessage->largeImageDownloadUrl.c_str());
            sMessage.large_image_local_path =
                const_cast<char *>(imageMessage->getLargeImageLocalPath().c_str());
            sMessage.thumbnail_download_url =
                const_cast<char *>(imageMessage->thumbnailDownloadUrl.c_str());
            sMessage.thumbnail_local_path =
                const_cast<char *>(imageMessage->getThumbnailLocalPath().c_str());
            sMessage.type = zim_message_type_image;
            sMessage.original_image_width = imageMessage->getOriginalImageWidth();
            sMessage.original_image_height = imageMessage->getOriginalImageHeight();
            sMessage.large_image_width = imageMessage->getLargeImageWidth();
            sMessage.large_image_height = imageMessage->getLargeImageHeight();
            sMessage.thumbnail_width = imageMessage->getThumbnailImageWidth();
            sMessage.thumbnail_height = imageMessage->getThumbnailImageHeight();
        } else if (oMessage->type == ZIM_MESSAGE_TYPE_FILE) {
            ZIMFileMessage *fileMessage = (ZIMFileMessage *)oMessage;
            sMessage.file_local_path = const_cast<char *>(fileMessage->fileLocalPath.c_str());
            sMessage.file_uid = const_cast<char *>(fileMessage->getFileUID().c_str());
            sMessage.file_name = const_cast<char *>(fileMessage->fileName.c_str());
            sMessage.file_size = fileMessage->getFileSize();
            sMessage.file_download_url = const_cast<char *>(fileMessage->fileDownloadUrl.c_str());
            sMessage.type = zim_message_type_file;
        } else if (oMessage->type == ZIM_MESSAGE_TYPE_AUDIO) {
            ZIMAudioMessage *audioMessage = (ZIMAudioMessage *)oMessage;
            sMessage.file_local_path = const_cast<char *>(audioMessage->fileLocalPath.c_str());
            sMessage.file_uid = const_cast<char *>(audioMessage->getFileUID().c_str());
            sMessage.file_name = const_cast<char *>(audioMessage->fileName.c_str());
            sMessage.file_size = audioMessage->getFileSize();
            sMessage.file_download_url = const_cast<char *>(audioMessage->fileDownloadUrl.c_str());
            sMessage.media_duration = audioMessage->audioDuration;
            sMessage.type = zim_message_type_audio;
        } else if (oMessage->type == ZIM_MESSAGE_TYPE_VIDEO) {
            ZIMVideoMessage *videoMessage = (ZIMVideoMessage *)oMessage;
            sMessage.file_local_path = const_cast<char *>(videoMessage->fileLocalPath.c_str());
            sMessage.file_uid = const_cast<char *>(videoMessage->getFileUID().c_str());
            sMessage.file_name = const_cast<char *>(videoMessage->fileName.c_str());
            sMessage.file_size = videoMessage->getFileSize();
            sMessage.file_download_url = const_cast<char *>(videoMessage->fileDownloadUrl.c_str());
            sMessage.media_duration = videoMessage->videoDuration;
            sMessage.video_first_frame_local_path =
                const_cast<char *>(videoMessage->getVideoFirstFrameLocalPath().c_str());
            sMessage.video_first_frame_download_url =
                const_cast<char *>(videoMessage->videoFirstFrameDownloadUrl.c_str());
            sMessage.video_first_frame_width = videoMessage->getVideoFirstFrameWidth();
            sMessage.video_first_frame_height = videoMessage->getVideoFirstFrameHeight();
            sMessage.type = zim_message_type_video;
        } else if (oMessage->type == ZIM_MESSAGE_TYPE_SYSTEM) {
            ZIMSystemMessage *systemMessage = (ZIMSystemMessage *)oMessage;
            char *message = const_cast<char *>(systemMessage->message.c_str());
            sMessage.message = message;
            sMessage.type = zim_message_type_system;
        } else if (oMessage->type == ZIM_MESSAGE_TYPE_REVOKE) {
            ZIMRevokeMessage *revokeMessage = (ZIMRevokeMessage *)oMessage;
            sMessage.revoke_type = (zim_revoke_type)revokeMessage->getRevokeType();
            sMessage.revoke_timestamp = revokeMessage->getRevokeTimestamp();
            sMessage.operated_userid =
                const_cast<char *>(revokeMessage->getOperatedUserID().c_str());
            sMessage.original_message_type =
                (zim_message_type)revokeMessage->getOriginalMessageType();
            sMessage.original_text_message =
                const_cast<char *>(revokeMessage->getOriginalTextMessageContent().c_str());
            sMessage.revoke_extended_data =
                const_cast<char *>(revokeMessage->getRevokeExtendedData().c_str());
            sMessage.revoke_status = (zim_message_revoke_status)revokeMessage->getRevokeStatus();
            sMessage.type = zim_message_type_revoke;
        } else if (oMessage->type == ZIM_MESSAGE_TYPE_CUSTOM) {
            ZIMCustomMessage *customMessage = (ZIMCustomMessage *)oMessage;
            char *message = const_cast<char *>(customMessage->message.c_str());
            sMessage.message = message;
            sMessage.sub_type = customMessage->subType;
            sMessage.type = zim_message_type_custom;
            char *searchedContent = const_cast<char *>(customMessage->searchedContent.c_str());
            sMessage.searched_content = searchedContent;
        } else if (oMessage->type == ZIM_MESSAGE_TYPE_COMBINE) {
            ZIMCombineMessage *combineMessage = (ZIMCombineMessage *)oMessage;
            sMessage.combine_id = const_cast<char *>(combineMessage->combineID.c_str());
            sMessage.combine_title = const_cast<char *>(combineMessage->title.c_str());
            sMessage.combine_summary = const_cast<char *>(combineMessage->summary.c_str());
            sMessage.combine_message_list_length = (unsigned int)combineMessage->messageList.size();
            sMessage.combine_message_list = new zim_message[sMessage.combine_message_list_length];
            sMessage.type = zim_message_type_combine;
            sNewZIMMessageList(sMessage.combine_message_list, combineMessage->messageList);
        } else {
            sMessage.type = zim_message_type_unknown;
        }

        sMessage.conversation_seq = oMessage->conversationSeq;
        sMessage.message_id = oMessage->messageID;
        sMessage.local_message_id = oMessage->localMessageID;
        sMessage.direction = (zim_message_direction)oMessage->direction;
        sMessage.order_key = oMessage->orderKey;
        sMessage.conversation_type = (zim_conversation_type)oMessage->conversationType;
        sMessage.sender_user_id = const_cast<char *>(oMessage->senderUserID.c_str());
        sMessage.conversation_id = const_cast<char *>(oMessage->conversationID.c_str());
        sMessage.timestamp = oMessage->timestamp;
        sMessage.sent_status = (zim_message_sent_status)oMessage->sentStatus;
        sMessage.receipt_status = (zim_message_receipt_status)oMessage->receiptStatus;
        sMessage.is_user_inserted = oMessage->userInserted;
        sMessage.extended_data = const_cast<char *>(oMessage->extendedData.c_str());
        sMessage.local_extended_data = const_cast<char *>(oMessage->localExtendedData.c_str());
        sMessage.is_broadcast_message = oMessage->broadcastMessage;
        sMessage.is_mention_all = oMessage->isMentionAll;
        sMessage.is_server_message = oMessage->serverMessage;
        if (oMessage->mentionedUserIDs.size() > 0) {
            sMessage.mentioned_user_ids_length = (unsigned int)oMessage->mentionedUserIDs.size();
            sMessage.mentioned_user_ids = new char *[sMessage.mentioned_user_ids_length];
            for (unsigned int i = 0; i < sMessage.mentioned_user_ids_length; i++) {
                sMessage.mentioned_user_ids[i] =
                    const_cast<char *>(oMessage->mentionedUserIDs.at(i).c_str());
            }
        }
    }

    static void sDelZIMMessage(struct zim_message &sMessage) {
        if (sMessage.mentioned_user_ids) {
            delete[] sMessage.mentioned_user_ids;
            sMessage.mentioned_user_ids = nullptr;
            sMessage.mentioned_user_ids_length = 0;
        }
        if (sMessage.type == zim_message_type_combine && sMessage.combine_message_list) {
            for (unsigned int i = 0; i < sMessage.combine_message_list_length; i++) {
                if (sMessage.combine_message_list[i].type == zim_message_type_combine) {
                    sDelZIMMessage(sMessage.combine_message_list[i]);
                }
            }
            delete[] sMessage.combine_message_list;
            sMessage.combine_message_list = nullptr;
            sMessage.combine_message_list_length = 0;
        }
    }

    static void sNewZIMMessageList(struct zim_message *message_list,
                                   const std::vector<std::shared_ptr<ZIMMessage>> &messageList) {
        if (messageList.empty()) {
            return;
        }

        for (unsigned int i = 0; i < (unsigned int)messageList.size(); i++) {
            ZIMConverter::sNewZIMMessage(message_list[i], messageList.at(i).get());
        }
    }

    static void sDelZIMMessageList(struct zim_message *message_list, unsigned int list_length) {
        for (unsigned int i = 0; i < list_length; i++) {
            ZIMConverter::sDelZIMMessage(message_list[i]);
        }
    }

    static void sZIMConversationQueryConfig(struct zim_conversation_query_config &sConfig,
                                            const ZIMConversationQueryConfig &oConfig) {
        sConfig.next_order_key = oConfig.nextConversation ? oConfig.nextConversation->orderKey : -1;
        sConfig.count = oConfig.count;
    }

    static void sZIMConversationDeleteConfig(struct zim_conversation_delete_config &sConfig,
                                             const ZIMConversationDeleteConfig &oConfig) {
        sConfig.is_also_delete_server_conversation = oConfig.isAlsoDeleteServerConversation;
    }

    static void sZIMLogConfig(struct zim_log_config *sConfig, const ZIMLogConfig *oConfig) {
        memset(sConfig, 0, sizeof(struct zim_log_config));
        if (!oConfig) {
            return;
        }
        //memcpy(sConfig->log_path, oConfig->logPath.c_str(), ZIM_MAX_COMMON_LEN - 1);
        sConfig->log_path = const_cast<char *>(oConfig->logPath.c_str());
        sConfig->log_size = oConfig->logSize;
    }

    static void sZIMCacheConfig(struct zim_cache_config *sConfig, const ZIMCacheConfig *oConfig) {
        memset(sConfig, 0, sizeof(struct zim_cache_config));
        if (!oConfig) {
            return;
        }
        //memcpy(sConfig->cache_path, oConfig->cachePath.c_str(), ZIM_MAX_COMMON_LEN - 1);
        sConfig->cache_path = const_cast<char *>(oConfig->cachePath.c_str());
    }

    static void sZIMLoginConfig(struct zim_login_config &sConfig, const ZIMLoginConfig *oConfig) {
        memset(&sConfig, 0, sizeof(struct zim_login_config));
        if (!oConfig) {
            return;
        }
        sConfig.user_name = const_cast<char *>(oConfig->userName.c_str());
        sConfig.token = const_cast<char *>(oConfig->token.c_str());
        sConfig.is_offline_login = oConfig->isOfflineLogin;
    }

    static void sZIMQueryMemberConfig(struct zim_room_member_query_config *sConfig,
                                      const ZIMRoomMemberQueryConfig *oConfig) {
        memset(sConfig, 0, sizeof(struct zim_room_member_query_config));
        if (!oConfig) {
            return;
        }
        //memcpy(sConfig->next_flag, oConfig->nextFlag.c_str(), ZIM_MAX_COMMON_LEN - 1);
        sConfig->next_flag = const_cast<char *>(oConfig->nextFlag.c_str());
        sConfig->count = oConfig->count;
    }

    static void sZIMCallInviteConfig(struct zim_call_invite_config &sConfig,
                                     const ZIMCallInviteConfig *oConfig) {
        if (!oConfig) {
            return;
        }

        sConfig.timeout = oConfig->timeout;
        sConfig.extended_data = const_cast<char *>(oConfig->extendedData.c_str());
        sConfig.mode = (zim_call_invitation_mode)oConfig->mode;
        bool is_enable_offline_push = (oConfig->pushConfig != nullptr);
        sConfig.enable_offline_push = is_enable_offline_push;

        if (is_enable_offline_push) {
            sZIMPushConfig(sConfig.push_config, oConfig->pushConfig);
        }
    }

    static void sZIMCallJoinConfig(struct zim_call_join_config &sConfig,
                                   const ZIMCallJoinConfig *oConfig) {
        if (!oConfig) {
            return;
        }

        sConfig.extended_data = const_cast<char *>(oConfig->extendedData.c_str());
    }

    static void sZIMCallingInviteConfig(struct zim_calling_invite_config &sConfig,
                                        const ZIMCallingInviteConfig *oConfig) {
        if (!oConfig) {
            return;
        }

        bool is_enable_offline_push = (oConfig->pushConfig != nullptr);
        sConfig.enable_offline_push = is_enable_offline_push;

        if (is_enable_offline_push) {
            sZIMPushConfig(sConfig.push_config, oConfig->pushConfig);
        }
    }

    static void sZIMCallQuitConfig(struct zim_call_quit_config &sConfig,
                                   const ZIMCallQuitConfig *oConfig) {
        if (!oConfig) {
            return;
        }

        sConfig.extended_data = const_cast<char *>(oConfig->extendedData.c_str());
        bool is_enable_offline_push = (oConfig->pushConfig != nullptr);
        sConfig.enable_offline_push = is_enable_offline_push;

        if (is_enable_offline_push) {
            sZIMPushConfig(sConfig.push_config, oConfig->pushConfig);
        }
    }

    static void sZIMCallEndConfig(struct zim_call_end_config &sConfig,
                                  const ZIMCallEndConfig *oConfig) {
        if (!oConfig) {
            return;
        }

        sConfig.extended_data = const_cast<char *>(oConfig->extendedData.c_str());
        bool is_enable_offline_push = (oConfig->pushConfig != nullptr);
        sConfig.enable_offline_push = is_enable_offline_push;

        if (is_enable_offline_push) {
            sZIMPushConfig(sConfig.push_config, oConfig->pushConfig);
        }
    }

    static void sZIMQueryCallListConfig(struct zim_query_call_list_config &sConfig,
                                        const ZIMCallInvitationQueryConfig *oConfig) {
        if (!oConfig) {
            return;
        }

        sConfig.count = oConfig->count;
        sConfig.next_flag = oConfig->nextFlag;
    }

    static void sZIMBlacklistQueryConfig(struct zim_blacklist_query_config &sConfig,
                                         const ZIMBlacklistQueryConfig *oConfig) {
        if (!oConfig) {
            return;
        }

        sConfig.count = oConfig->count;
        sConfig.next_flag = static_cast<unsigned int>(oConfig->nextFlag);
    }

    static void
    sZIMFriendApplicationListQueryConfig(struct zim_friend_application_list_query_config &sConfig,
                                         const ZIMFriendApplicationListQueryConfig *oConfig) {
        if (!oConfig) {
            return;
        }

        sConfig.count = oConfig->count;
        sConfig.next_flag = static_cast<unsigned int>(oConfig->nextFlag);
    }

    static void sZIMFriendAddConfig(struct zim_friend_add_config &sConfig,
                                    const ZIMFriendAddConfig *oConfig) {
        if (!oConfig) {
            return;
        }

        sConfig.friend_attributes = new zim_friend_attribute[oConfig->friendAttributes.size()];
        sConfig.friend_attributes_length =
            static_cast<unsigned int>(oConfig->friendAttributes.size());
        unsigned int i = 0;
        for (auto &it : oConfig->friendAttributes) {
            sConfig.friend_attributes[i].key = it.first.c_str();
            sConfig.friend_attributes[i].value = it.second.c_str();
            i++;
        }

        sConfig.friend_alias = oConfig->friendAlias.c_str();
        sConfig.wording = oConfig->wording.c_str();
    }

    static void sZIMFriendSearchConfig(struct zim_friend_search_config &sConfig,
                                       const ZIMFriendSearchConfig *oConfig) {
        if (!oConfig) {
            return;
        }
        sConfig.count = oConfig->count;
        sConfig.next_flag = oConfig->nextFlag;
        sConfig.is_also_match_friend_alias = oConfig->isAlsoMatchFriendAlias;
        sConfig.keywords_length = (unsigned int)oConfig->keywords.size();
        if (sConfig.keywords_length > 0) {
            sConfig.keywords = new char *[sConfig.keywords_length];
            for (unsigned int i = 0; i < sConfig.keywords_length; i++) {
                sConfig.keywords[i] = const_cast<char *>(oConfig->keywords[i].c_str());
            }
        }
    }

    static void sZIMFriendCheckConfig(struct zim_friend_relation_check_config &sConfig,
                                      const ZIMFriendRelationCheckConfig *oConfig) {
        if (!oConfig) {
            return;
        }

        sConfig.type = (zim_friend_relation_check_type)oConfig->type;
    }

    static void sZIMFriendListQueryConfig(struct zim_friend_list_query_config &sConfig,
                                          const ZIMFriendListQueryConfig *oConfig) {
        if (!oConfig) {
            return;
        }

        sConfig.count = oConfig->count;
        sConfig.next_flag = static_cast<unsigned int>(oConfig->nextFlag);
    }

    static void
    sZIMFriendApplicationRejectConfig(struct zim_friend_application_reject_config &sConfig,
                                      const ZIMFriendApplicationRejectConfig *oConfig) {

        if (!oConfig) {
            return;
        }

        bool is_enable_offline_push = (oConfig->pushConfig != nullptr);
        sConfig.enable_offline_push = is_enable_offline_push;

        if (is_enable_offline_push) {
            sZIMPushConfig(sConfig.push_config, oConfig->pushConfig);
        }
    }

    static void sZIMFriendApplicationSendConfig(struct zim_friend_application_send_config &sConfig,
                                                const ZIMFriendApplicationSendConfig *oConfig) {

        if (!oConfig) {
            return;
        }
        bool is_enable_offline_push = (oConfig->pushConfig != nullptr);
        sConfig.enable_offline_push = is_enable_offline_push;

        if (is_enable_offline_push) {
            sZIMPushConfig(sConfig.push_config, oConfig->pushConfig);
        }

        sConfig.friend_attributes = new zim_friend_attribute[oConfig->friendAttributes.size()];
        sConfig.friend_attributes_length = (unsigned int)oConfig->friendAttributes.size();
        unsigned int i = 0;
        for (auto &it : oConfig->friendAttributes) {
            sConfig.friend_attributes[i].key = it.first.c_str();
            sConfig.friend_attributes[i].value = it.second.c_str();
            i++;
        }

        sConfig.friend_alias = oConfig->friendAlias.c_str();
        sConfig.wording = oConfig->wording.c_str();
    }

    static void
    sZIMFriendApplicationAcceptConfig(struct zim_friend_application_accept_config &sConfig,
                                      const ZIMFriendApplicationAcceptConfig *oConfig) {

        if (!oConfig) {
            return;
        }
        bool is_enable_offline_push = (oConfig->pushConfig != nullptr);
        sConfig.enable_offline_push = is_enable_offline_push;

        if (is_enable_offline_push) {
            sZIMPushConfig(sConfig.push_config, oConfig->pushConfig);
        }

        sConfig.friend_attributes = new zim_friend_attribute[oConfig->friendAttributes.size()];
        sConfig.friend_attributes_length = (unsigned int)oConfig->friendAttributes.size();
        unsigned int i = 0;
        for (auto &it : oConfig->friendAttributes) {
            sConfig.friend_attributes[i].key = it.first.c_str();
            sConfig.friend_attributes[i].value = it.second.c_str();
            i++;
        }

        sConfig.friend_alias = oConfig->friendAlias.c_str();
    }

    static void sZIMCallCancelConfig(struct zim_call_cancel_config &sConfig,
                                     const ZIMCallCancelConfig *oConfig) {
        if (!oConfig) {
            return;
        }
        sConfig.extended_data = const_cast<char *>(oConfig->extendedData.c_str());

        bool is_enable_offline_push = (oConfig->pushConfig != nullptr);
        sConfig.enable_offline_push = is_enable_offline_push;

        if (is_enable_offline_push) {
            sZIMPushConfig(sConfig.push_config, oConfig->pushConfig);
        }
    }

    static void sZIMCallAcceptConfig(struct zim_call_accept_config &sConfig,
                                     const ZIMCallAcceptConfig *oConfig) {
        if (!oConfig) {
            return;
        }
        sConfig.extended_data = const_cast<char *>(oConfig->extendedData.c_str());
    }

    static void sZIMCallRejectConfig(struct zim_call_reject_config &sConfig,
                                     const ZIMCallRejectConfig *oConfig) {
        if (!oConfig) {
            return;
        }
        sConfig.extended_data = const_cast<char *>(oConfig->extendedData.c_str());
    }

    static void sZIMVoIPConfig(struct zim_voip_config &sConfig, const ZIMVoIPConfig *oConfig) {
        if (!oConfig) {
            return;
        }
        sConfig.ios_voip_handle_value = const_cast<char *>(oConfig->iOSVoIPHandleValue.c_str());
        sConfig.ios_voip_handle_type = static_cast<zim_cx_handle_type>(oConfig->iOSVoIPHandleType);
        sConfig.ios_voip_has_video = oConfig->iOSVoIPHasVideo;
    }

    static void sZIMPushConfig(struct zim_push_config &sConfig, const ZIMPushConfig *oConfig) {
        if (!oConfig) {
            return;
        }
        sConfig.title = const_cast<char *>(oConfig->title.c_str());
        sConfig.content = const_cast<char *>(oConfig->content.c_str());
        sConfig.payload = const_cast<char *>(oConfig->payload.c_str());
        sConfig.resources_id = const_cast<char *>(oConfig->resourcesID.c_str());
        sConfig.enable_badge = oConfig->enableBadge;
        sConfig.badge_increment = oConfig->badgeIncrement;
        if (oConfig->voIPConfig == nullptr) {
            sConfig.enable_voip_config = false;
        } else {
            sConfig.enable_voip_config = true;
            sZIMVoIPConfig(sConfig.voip_config, oConfig->voIPConfig);
        }
    }

    static void sZIMMessageSendConfig(struct zim_message_send_config &sConfig,
                                      const ZIMMessageSendConfig *oConfig) {
        if (!oConfig) {
            return;
        }

        bool is_enable_offline_push = (oConfig->pushConfig != nullptr);
        sConfig.priority = (zim_message_priority)oConfig->priority;
        sConfig.has_receipt = oConfig->hasReceipt;
        sConfig.is_notify_mentioned_users = oConfig->isNotifyMentionedUsers;
        sConfig.enable_offline_push = is_enable_offline_push;

        if (is_enable_offline_push) {
            sZIMPushConfig(sConfig.push_config, oConfig->pushConfig);
        }
    }

    static void sNewZIMMessageQueryConfig(struct zim_message_query_config &sConfig,
                                          const ZIMMessageQueryConfig *oConfig) {
        //memset(sConfig, 0, sizeof(struct zim_query_message_config));
        if (!oConfig) {
            return;
        }

        if (oConfig->nextMessage) {
            sConfig.next_message = new zim_message{};
            sNewZIMMessage(*sConfig.next_message, oConfig->nextMessage.get());
        } else {
            sConfig.next_message = nullptr;
        }

        sConfig.count = oConfig->count;
        sConfig.reverse = oConfig->reverse;
    }

    static void sDelZIMMessageQueryConfig(struct zim_message_query_config &sConfig) {
        if (sConfig.next_message) {
            ZIMConverter::sDelZIMMessage(*sConfig.next_message);
            delete sConfig.next_message;
            sConfig.next_message = nullptr;
        }
    }

    static void sZIMMessageDeleteConfig(struct zim_message_delete_config &sConfig,
                                        const ZIMMessageDeleteConfig *oConfig) {
        if (!oConfig) {
            return;
        }
        sConfig.is_also_delete_server_message = oConfig->isAlsoDeleteServerMessage;
    }

    static void sZIMMessageRevokeConfig(struct zim_message_revoke_config &sConfig,
                                        const ZIMMessageRevokeConfig *oConfig) {
        if (!oConfig) {
            return;
        }

        sConfig.enable_offline_push = (oConfig->config != nullptr);
        sConfig.revoke_extended_data = const_cast<char *>(oConfig->revokeExtendedData.c_str());

        if (sConfig.enable_offline_push) {
            sZIMPushConfig(sConfig.push_config, oConfig->config);
        }
    }

    static void sZIMGroupMuteConfig(struct zim_group_mute_config &sConfig,
                                    const ZIMGroupMuteConfig &oConfig) {

        sConfig.mode = static_cast<zim_group_mute_mode>(oConfig.mode);
        sConfig.duration = oConfig.duration;
        sConfig.mute_role_list = const_cast<int *>(oConfig.roles.data());
        sConfig.mute_role_list_length = static_cast<unsigned int>(oConfig.roles.size());
    }

    static void sZIMGroupMemberListMuteConfig(struct zim_group_member_list_mute_config &sConfig,
                                              const ZIMGroupMemberMuteConfig &oConfig) {

        sConfig.duration = oConfig.duration;
    }

    static void
    sZIMGroupMemberMutedListQueryConfig(struct zim_group_muted_members_query_config &sConfig,
                                        const ZIMGroupMemberMutedListQueryConfig &oConfig) {

        sConfig.next_flag = oConfig.nextFlag;
        sConfig.count = oConfig.count;
    }

    static void sNewZIMMessageSearchConfig(struct zim_message_search_config &sConfig,
                                           const ZIMMessageSearchConfig &oConfig) {
        if (oConfig.nextMessage) {
            sConfig.next_message = new zim_message;
            sNewZIMMessage(*sConfig.next_message, oConfig.nextMessage.get());
        } else {
            sConfig.next_message = nullptr;
        }

        sConfig.count = oConfig.count;
        sConfig.order = (zim_message_order)oConfig.order;

        sConfig.keywords_length = (unsigned int)oConfig.keywords.size();
        if (sConfig.keywords_length > 0) {
            sConfig.keywords = new char *[sConfig.keywords_length];

            for (unsigned int i = 0; i < sConfig.keywords_length; i++) {
                sConfig.keywords[i] = const_cast<char *>(oConfig.keywords[i].c_str());
            }
        }

        sConfig.message_types_length = (unsigned int)oConfig.messageTypes.size();
        if (sConfig.message_types_length > 0) {
            sConfig.message_types = new zim_message_type[sConfig.message_types_length];

            for (unsigned int i = 0; i < sConfig.message_types_length; i++) {
                sConfig.message_types[i] = (zim_message_type)oConfig.messageTypes[i];
            }
        }

        sConfig.sub_message_types_length = (unsigned int)oConfig.subMessageTypes.size();
        if (sConfig.sub_message_types_length > 0) {
            sConfig.sub_message_types = new int[sConfig.sub_message_types_length];

            for (unsigned int i = 0; i < sConfig.sub_message_types_length; i++) {
                sConfig.sub_message_types[i] = (int)oConfig.subMessageTypes[i];
            }
        }

        sConfig.sender_user_ids_length = (unsigned int)oConfig.senderUserIDs.size();
        if (sConfig.sender_user_ids_length > 0) {
            sConfig.sender_user_ids = new char *[sConfig.sender_user_ids_length];

            for (unsigned int i = 0; i < sConfig.sender_user_ids_length; i++) {
                sConfig.sender_user_ids[i] = const_cast<char *>(oConfig.senderUserIDs[i].c_str());
            }
        }

        sConfig.start_time = oConfig.startTime;
        sConfig.end_time = oConfig.endTime;
    }

    static void sDelZIMMessageSearchConfig(struct zim_message_search_config &sConfig) {
        if (sConfig.next_message) {
            ZIMConverter::sDelZIMMessage(*sConfig.next_message);
            delete sConfig.next_message;
            sConfig.next_message = nullptr;
        }

        if (sConfig.keywords) {
            delete[] sConfig.keywords;
            sConfig.keywords = nullptr;
        }

        if (sConfig.message_types) {
            delete[] sConfig.message_types;
            sConfig.message_types = nullptr;
        }

        if (sConfig.sub_message_types) {
            delete[] sConfig.sub_message_types;
            sConfig.sub_message_types = nullptr;
        }

        if (sConfig.sender_user_ids) {
            delete[] sConfig.sender_user_ids;
            sConfig.sender_user_ids = nullptr;
        }
    }

    static void sNewZIMConversationSearchConfig(struct zim_conversation_search_config &sConfig,
                                                const ZIMConversationSearchConfig &oConfig) {

        sConfig.next_flag = oConfig.nextFlag;
        sConfig.total_conversation_count = oConfig.totalConversationCount;
        sConfig.conversation_message_count = oConfig.conversationMessageCount;

        sConfig.keywords_length = (unsigned int)oConfig.keywords.size();
        if (sConfig.keywords_length > 0) {
            sConfig.keywords = new char *[sConfig.keywords_length];

            for (unsigned int i = 0; i < sConfig.keywords_length; i++) {
                sConfig.keywords[i] = const_cast<char *>(oConfig.keywords[i].c_str());
            }
        }

        sConfig.message_types_length = (unsigned int)oConfig.messageTypes.size();
        if (sConfig.message_types_length > 0) {
            sConfig.message_types = new zim_message_type[sConfig.message_types_length];

            for (unsigned int i = 0; i < sConfig.message_types_length; i++) {
                sConfig.message_types[i] = (zim_message_type)oConfig.messageTypes[i];
            }
        }

        sConfig.sub_message_types_length = (unsigned int)oConfig.subMessageTypes.size();
        if (sConfig.sub_message_types_length > 0) {
            sConfig.sub_message_types = new int[sConfig.sub_message_types_length];

            for (unsigned int i = 0; i < sConfig.sub_message_types_length; i++) {
                sConfig.sub_message_types[i] = (int)oConfig.subMessageTypes[i];
            }
        }

        sConfig.sender_user_ids_length = (unsigned int)oConfig.senderUserIDs.size();
        if (sConfig.sender_user_ids_length > 0) {
            sConfig.sender_user_ids = new char *[sConfig.sender_user_ids_length];

            for (unsigned int i = 0; i < sConfig.sender_user_ids_length; i++) {
                sConfig.sender_user_ids[i] = const_cast<char *>(oConfig.senderUserIDs[i].c_str());
            }
        }

        sConfig.start_time = oConfig.startTime;
        sConfig.end_time = oConfig.endTime;
    }

    static void sDelZIMConversationSearchConfig(struct zim_conversation_search_config &sConfig) {

        if (sConfig.keywords) {
            delete[] sConfig.keywords;
            sConfig.keywords = nullptr;
        }

        if (sConfig.message_types) {
            delete[] sConfig.message_types;
            sConfig.message_types = nullptr;
        }

        if (sConfig.sub_message_types) {
            delete[] sConfig.sub_message_types;
            sConfig.sub_message_types = nullptr;
        }

        if (sConfig.sender_user_ids) {
            delete[] sConfig.sender_user_ids;
            sConfig.sender_user_ids = nullptr;
        }
    }

    // Group
    static void sZIMGroupInfo(struct zim_group_info &sInfo, const ZIMGroupInfo &oInfo) {
        sInfo.group_id = const_cast<char *>(oInfo.groupID.c_str());
        sInfo.group_name = const_cast<char *>(oInfo.groupName.c_str());
        sInfo.group_avatar_url = const_cast<char *>(oInfo.groupAvatarUrl.c_str());
    }

  public:
    // MARK: - struct -> object

    static void oZIMMessageReaction(const struct zim_message_reaction &sInfo,
                                    ZIMMessageReaction &oInfo) {
        oInfo.totalCount = sInfo.total_count;
        oInfo.reactionType = sInfo.reaction_type;
        oInfo.conversationType = (ZIMConversationType)sInfo.conversation_type;
        oInfo.conversationID = sInfo.conversation_id;
        oInfo.messageID = sInfo.message_id;
        oInfo.isSelfIncluded = sInfo.self_included;
        oZIMMessageReactionUserInfoList(sInfo.user_list, sInfo.user_info_length, oInfo.userList);
    }

    static void
    oZIMMessageReactionUserInfoList(const struct zim_message_reaction_user_info *infos,
                                    unsigned int infos_length,
                                    std::vector<ZIMMessageReactionUserInfo> &userInfoList) {
        if (!infos) {
            return;
        }
        for (unsigned int i = 0; i < infos_length; ++i) {
            ZIMMessageReactionUserInfo userInfo{};
            userInfo.userID = (*(infos + i)).user_id;
            userInfoList.emplace_back(userInfo);
        }
    }

    static void oZIMMessageReactionList(const struct zim_message_reaction *infos,
                                        unsigned int infos_length,
                                        std::vector<ZIMMessageReaction> &oInfoList) {
        oInfoList.clear();
        oInfoList.reserve(infos_length);
        for (unsigned int i = 0; i < infos_length; ++i) {
            ZIMMessageReaction info{};
            oZIMMessageReaction(infos[i], info);
            oInfoList.emplace_back(info);
        }
    }

    static void oZIMMessageMentionedUserIDs(char **user_ids, unsigned int ids_length,
                                            std::vector<std::string> &oIDs) {
        oIDs.clear();
        oIDs.reserve(ids_length);
        for (unsigned int i = 0; i < ids_length; i++) {
            oIDs.emplace_back(user_ids[i]);
        }
    }

    static void oZIMMessageMentionedInfoList(const struct zim_message_mentioned_info *infos,
                                             unsigned int infos_length,
                                             std::vector<ZIMMessageMentionedInfo> &oInfoList) {
        oInfoList.clear();
        oInfoList.reserve(infos_length);
        for (unsigned int i = 0; i < infos_length; i++) {
            ZIMMessageMentionedInfo oInfo;
            oInfo.fromUserID = infos[i].from_user_id;
            oInfo.messageID = infos[i].message_id;
            oInfo.type = (ZIMMessageMentionedType)infos[i].type;

            oInfoList.emplace_back(oInfo);
        }
    }

    static void oZIMUserInfo(ZIMUserInfo *oInfo, struct zim_user_info *sInfo) {
        if (!sInfo) {
            return;
        }
        oInfo->userID = sInfo->user_id;
        oInfo->userName = sInfo->user_name;
        oInfo->userAvatarUrl = sInfo->user_avatar_url;
    }

    static void oZIMRoomMemberInfo(ZIMRoomMemberInfo *oInfo, struct zim_room_member_info *sInfo) {
        if (!sInfo) {
            return;
        }
        oInfo->userID = sInfo->user_info.user_id;
        oInfo->userName = sInfo->user_info.user_name;
        oInfo->userAvatarUrl = sInfo->user_info.user_avatar_url;
    }

    static void oZIMUserFullInfo(ZIMUserFullInfo *oInfo, struct zim_user_full_info *sInfo) {
        if (!sInfo) {
            return;
        }
        oInfo->baseInfo.userID = sInfo->base_info.user_id;
        oInfo->baseInfo.userName = sInfo->base_info.user_name;
        oInfo->baseInfo.userAvatarUrl = sInfo->base_info.user_avatar_url;
        oInfo->userAvatarUrl = sInfo->user_avatar_url;
        oInfo->extendedData = sInfo->user_extended_data;
    }

    static std::shared_ptr<ZIMMessage> oZIMMessage(struct zim_message *sMessage) {
        if (!sMessage) {
            return nullptr;
        }

        std::shared_ptr<ZIMMessage> oMessage = nullptr;
        // 将 -1 定义为空消息
        if (sMessage->type == zim_message_type_empty) {
            return oMessage;
        }

        if (sMessage->type == zim_message_type_text) {
            oMessage = std::make_shared<ZIMTextMessage>();
            auto textMessage = std::static_pointer_cast<ZIMTextMessage>(oMessage);
            textMessage->message = sMessage->message;
            textMessage->type = ZIM_MESSAGE_TYPE_TEXT;

        } else if (sMessage->type == zim_message_type_command) {
            oMessage = std::make_shared<ZIMCommandMessage>();
            auto commandMessage = std::static_pointer_cast<ZIMCommandMessage>(oMessage);
            std::vector<uint8_t> message(
                &(sMessage->command_message)[0],
                &(sMessage->command_message)[sMessage->command_message_length]);
            commandMessage->message = message;
            commandMessage->type = ZIM_MESSAGE_TYPE_COMMAND;
        } else if (sMessage->type == zim_message_type_barrage) {
            oMessage = std::make_shared<ZIMBarrageMessage>();
            auto barrageMessage = std::static_pointer_cast<ZIMBarrageMessage>(oMessage);
            barrageMessage->message = sMessage->message;
            barrageMessage->type = ZIM_MESSAGE_TYPE_BARRAGE;

        } else if (sMessage->type == zim_message_type_image) {
            oMessage = std::make_shared<ZIMImageMessage>("");

            auto imageMessage = std::static_pointer_cast<ZIMImageMessage>(oMessage);
            imageMessage->type = ZIM_MESSAGE_TYPE_IMAGE;
            imageMessage->fileUID = sMessage->file_uid ? sMessage->file_uid : "";
            imageMessage->fileName = sMessage->file_name ? sMessage->file_name : "";
            imageMessage->fileSize = sMessage->file_size;
            imageMessage->fileDownloadUrl =
                sMessage->file_download_url ? sMessage->file_download_url : "";
            imageMessage->fileLocalPath =
                sMessage->file_local_path ? sMessage->file_local_path : "";

            imageMessage->largeImageDownloadUrl =
                sMessage->large_image_download_url ? sMessage->large_image_download_url : "";
            imageMessage->largeImageLocalPath =
                sMessage->large_image_local_path ? sMessage->large_image_local_path : "";
            imageMessage->thumbnailDownloadUrl =
                sMessage->thumbnail_download_url ? sMessage->thumbnail_download_url : "";
            imageMessage->thumbnailLocalPath =
                sMessage->thumbnail_local_path ? sMessage->thumbnail_local_path : "";
            imageMessage->originalImageWidth = sMessage->original_image_width;
            imageMessage->originalImageHeight = sMessage->original_image_height;
            imageMessage->largeImageWidth = sMessage->large_image_width;
            imageMessage->largeImageHeight = sMessage->large_image_height;
            imageMessage->thumbnailWidth = sMessage->thumbnail_width;
            imageMessage->thumbnailHeight = sMessage->thumbnail_height;

        } else if (sMessage->type == zim_message_type_file) {
            oMessage = std::make_shared<ZIMFileMessage>("");

            auto fileMessage = std::static_pointer_cast<ZIMFileMessage>(oMessage);
            fileMessage->type = ZIM_MESSAGE_TYPE_FILE;
            fileMessage->fileUID = sMessage->file_uid ? sMessage->file_uid : "";
            fileMessage->fileName = sMessage->file_name ? sMessage->file_name : "";
            fileMessage->fileSize = sMessage->file_size;
            fileMessage->fileDownloadUrl =
                sMessage->file_download_url ? sMessage->file_download_url : "";
            fileMessage->fileLocalPath = sMessage->file_local_path ? sMessage->file_local_path : "";
        } else if (sMessage->type == zim_message_type_audio) {
            oMessage = std::make_shared<ZIMAudioMessage>("", 0);

            auto audioMessage = std::static_pointer_cast<ZIMAudioMessage>(oMessage);
            audioMessage->type = ZIM_MESSAGE_TYPE_AUDIO;
            audioMessage->fileUID = sMessage->file_uid ? sMessage->file_uid : "";
            audioMessage->fileName = sMessage->file_name ? sMessage->file_name : "";
            audioMessage->fileSize = sMessage->file_size;
            audioMessage->fileDownloadUrl =
                sMessage->file_download_url ? sMessage->file_download_url : "";
            audioMessage->fileLocalPath =
                sMessage->file_local_path ? sMessage->file_local_path : "";

            audioMessage->audioDuration = static_cast<unsigned int>(sMessage->media_duration);
        } else if (sMessage->type == zim_message_type_video) {
            oMessage = std::make_shared<ZIMVideoMessage>("", 0);

            auto videoMessage = std::static_pointer_cast<ZIMVideoMessage>(oMessage);
            videoMessage->type = ZIM_MESSAGE_TYPE_VIDEO;
            videoMessage->fileUID = sMessage->file_uid ? sMessage->file_uid : "";
            videoMessage->fileName = sMessage->file_name ? sMessage->file_name : "";
            videoMessage->fileSize = sMessage->file_size;
            videoMessage->fileDownloadUrl =
                sMessage->file_download_url ? sMessage->file_download_url : "";
            videoMessage->fileLocalPath =
                sMessage->file_local_path ? sMessage->file_local_path : "";

            videoMessage->videoDuration = static_cast<unsigned int>(sMessage->media_duration);
            videoMessage->videoFirstFrameLocalPath = sMessage->video_first_frame_local_path
                                                         ? sMessage->video_first_frame_local_path
                                                         : "";
            videoMessage->videoFirstFrameDownloadUrl =
                sMessage->video_first_frame_download_url ? sMessage->video_first_frame_download_url
                                                         : "";
            videoMessage->videoFirstFrameWidth = sMessage->video_first_frame_width;
            videoMessage->videoFirstFrameHeight = sMessage->video_first_frame_height;
        } else if (sMessage->type == zim_message_type_system) {
            oMessage = std::make_shared<ZIMSystemMessage>();
            auto textMessage = std::static_pointer_cast<ZIMSystemMessage>(oMessage);
            textMessage->message = sMessage->message;
            textMessage->type = ZIM_MESSAGE_TYPE_SYSTEM;
        } else if (sMessage->type == zim_message_type_revoke) {
            oMessage = std::make_shared<ZIMRevokeMessage>();
            auto revokeMessage = std::static_pointer_cast<ZIMRevokeMessage>(oMessage);
            revokeMessage->revokeType = (ZIMRevokeType)sMessage->revoke_type;
            revokeMessage->revokeTimestamp = sMessage->revoke_timestamp;
            revokeMessage->operatedUserID = sMessage->operated_userid;
            revokeMessage->originalMessageType = (ZIMMessageType)sMessage->original_message_type;
            revokeMessage->originalTextMessageContent =
                sMessage->original_text_message ? sMessage->original_text_message : "";
            revokeMessage->revokeExtendedData =
                sMessage->revoke_extended_data ? sMessage->revoke_extended_data : "";
            revokeMessage->revokeStatus = (ZIMMessageRevokeStatus)sMessage->revoke_status;
        } else if (sMessage->type == zim_message_type_custom) {
            oMessage = std::make_shared<ZIMCustomMessage>(sMessage->message, sMessage->sub_type);
            auto customMessage = std::static_pointer_cast<ZIMCustomMessage>(oMessage);
            customMessage->searchedContent = sMessage->searched_content;
        } else if (sMessage->type == zim_message_type_combine) {
            oMessage = std::make_shared<ZIMCombineMessage>();
            auto combineMessage = std::static_pointer_cast<ZIMCombineMessage>(oMessage);
            combineMessage->title = sMessage->combine_title ? sMessage->combine_title : "";
            combineMessage->summary = sMessage->combine_summary ? sMessage->combine_summary : "";
            combineMessage->combineID = sMessage->combine_id ? sMessage->combine_id : "";

            for (unsigned int i = 0; i < sMessage->combine_message_list_length; i++) {
                struct zim_message sChildMessage = *(sMessage->combine_message_list + i);
                std::shared_ptr<ZIMMessage> message = ZIMConverter::oZIMMessage(&sChildMessage);
                combineMessage->messageList.emplace_back(message);
            }
        } else {
            // Unknown type message
            oMessage = std::make_shared<ZIMMessage>();
            oMessage->type = ZIM_MESSAGE_TYPE_UNKNOWN;
        }

        oMessage->messageID = sMessage->message_id;
        oMessage->localMessageID = sMessage->local_message_id;
        oMessage->timestamp = sMessage->timestamp;
        oMessage->senderUserID = sMessage->sender_user_id;
        oMessage->conversationID = sMessage->conversation_id;
        oMessage->conversationType = (ZIMConversationType)sMessage->conversation_type;
        oMessage->conversationSeq = sMessage->conversation_seq;
        oMessage->direction = (ZIMMessageDirection)sMessage->direction;
        oMessage->sentStatus = (ZIMMessageSentStatus)sMessage->sent_status;
        oMessage->orderKey = sMessage->order_key;
        oMessage->userInserted = sMessage->is_user_inserted;
        oMessage->receiptStatus = (ZIMMessageReceiptStatus)(sMessage->receipt_status);
        oMessage->extendedData = sMessage->extended_data;
        oMessage->localExtendedData = sMessage->local_extended_data;
        oMessage->broadcastMessage = sMessage->is_broadcast_message;
        oZIMMessageReactionList(sMessage->reactions, sMessage->reaction_length,
                                oMessage->reactions);

        oMessage->isMentionAll = sMessage->is_mention_all;
        oMessage->serverMessage = sMessage->is_server_message;
        oZIMMessageMentionedUserIDs(sMessage->mentioned_user_ids,
                                    sMessage->mentioned_user_ids_length,
                                    oMessage->mentionedUserIDs);
        return oMessage;
    }

    static std::vector<ZIMMessageReceiptInfo>
    oZIMMessageReceiptInfoList(const struct zim_message_receipt_info *read_receipt_list,
                               unsigned int list_length) {
        std::vector<ZIMMessageReceiptInfo> infoList;
        if (!read_receipt_list && list_length == 0) {
            return infoList;
        }

        for (unsigned int i = 0; i < list_length; i++) {
            ZIMMessageReceiptInfo info;
            info.conversationID = read_receipt_list[i].conversation_id;
            info.conversationType = (ZIMConversationType)read_receipt_list[i].conversation_type;
            info.status = (ZIMMessageReceiptStatus)read_receipt_list[i].status;
            info.messageID = read_receipt_list[i].message_id;
            info.readMemberCount = read_receipt_list[i].read_member_count;
            info.unreadMemberCount = read_receipt_list[i].unread_member_count;
            info.isSelfOperated = read_receipt_list[i].is_self_operated;

            infoList.emplace_back(info);
        }

        return infoList;
    }

    static std::vector<ZIMConversationSearchInfo>
    oZIMConversationSearchInfoList(const struct zim_conversation_search_info *global_info_list,
                                   unsigned int info_list_length) {
        std::vector<ZIMConversationSearchInfo> globalInfoList;
        if (!global_info_list || info_list_length == 0) {
            return globalInfoList;
        }

        for (unsigned int i = 0; i < info_list_length; i++) {
            ZIMConversationSearchInfo info;
            info.conversationID = global_info_list[i].conversation_id;
            info.conversationType = (ZIMConversationType)global_info_list[i].conversation_type;
            info.totalMessageCount = global_info_list[i].total_message_count;

            std::vector<std::shared_ptr<ZIMMessage>> messageList;
            for (unsigned int j = 0; j < global_info_list[i].message_list_length; j++) {
                std::shared_ptr<ZIMMessage> message =
                    ZIMConverter::oZIMMessage(&global_info_list[i].message_list[j]);
                messageList.emplace_back(message);
            }

            info.messageList = messageList;

            globalInfoList.emplace_back(info);
        }

        return globalInfoList;
    }

    static std::shared_ptr<ZIMConversation>
    oZIMConversation(struct zim_conversation *sConversation) {
        if (!sConversation) {
            return nullptr;
        }

        std::shared_ptr<ZIMConversation> oConversation = std::make_shared<ZIMConversation>();

        oConversation->conversationID = sConversation->conversation_id;
        oConversation->conversationName = sConversation->conversation_name;
        oConversation->conversationAvatarUrl = sConversation->conversation_avatar_url;
        oConversation->conversationAlias = sConversation->conversation_alias;
        oConversation->type = (ZIMConversationType)sConversation->type;
        oConversation->unreadMessageCount = sConversation->unread_message_count;
        oConversation->orderKey = sConversation->order_key;
        oConversation->lastMessage = ZIMConverter::oZIMMessage(&sConversation->last_message);
        oConversation->notificationStatus =
            (ZIMConversationNotificationStatus)sConversation->notification_status;
        oConversation->isPinned = sConversation->is_pinned;
        oZIMMessageMentionedInfoList(sConversation->mentioned_info_list,
                                     sConversation->mentioned_info_list_length,
                                     oConversation->mentionedInfoList);
        oConversation->draft = sConversation->draft;

        return oConversation;
    }

    static void sZIMConversation(struct zim_conversation &sConversation,
                                 std::shared_ptr<ZIMConversation> conversation) {
        if (conversation.get() == nullptr) {
            return;
        }
        memset(&sConversation, 0, sizeof(struct zim_conversation));

        sConversation.conversation_id = const_cast<char *>(conversation->conversationID.c_str());
        sConversation.conversation_alias =
            const_cast<char *>(conversation->conversationAlias.c_str());
        sConversation.conversation_name =
            const_cast<char *>(conversation->conversationName.c_str());
        sConversation.conversation_avatar_url =
            const_cast<char *>(conversation->conversationAvatarUrl.c_str());
        sConversation.type = (zim_conversation_type)conversation->type;
        sConversation.unread_message_count = conversation->unreadMessageCount;
        sConversation.order_key = conversation->orderKey;
        ZIMConverter::sNewZIMMessage(sConversation.last_message, conversation->lastMessage.get());
        sConversation.notification_status =
            (zim_conversation_notification_status)conversation->notificationStatus;
        sConversation.is_pinned = conversation->isPinned;
        //sConversation.draft = const_cast<char *>(conversation->draft.c_str());
    }

    static void oZIMConversationChangeInfo(ZIMConversationChangeInfo &oInfo,
                                           struct zim_conversation_change_info &sInfo) {
        oInfo.event = (ZIMConversationEvent)sInfo.event;
        oInfo.conversation = ZIMConverter::oZIMConversation(&sInfo.conversation);
    }

    static void oZIMMessageSentStatusChangeInfo(ZIMMessageSentStatusChangeInfo &oInfo,
                                                struct zim_message_sent_status_change_info &sInfo) {
        oInfo.status = (ZIMMessageSentStatus)sInfo.status;
        oInfo.message = ZIMConverter::oZIMMessage(&sInfo.message);
        oInfo.reason = sInfo.reason;
    }

    static void oZIMRoomInfo(ZIMRoomInfo *oInfo, struct zim_room_info *sInfo) {
        if (!sInfo) {
            return;
        }
        oInfo->roomID = sInfo->room_id;
        oInfo->roomName = sInfo->room_name;
    }

    static void oZIMRoomFullInfo(ZIMRoomFullInfo *oInfo, struct zim_room_full_info *sInfo) {
        if (!sInfo) {
            return;
        }
        oZIMRoomInfo(&(oInfo->baseInfo), &(sInfo->base_info));
    }

    static void oZIMGroupBaseInfo(ZIMGroupInfo *oInfo, struct zim_group_info *sInfo) {
        if (!sInfo) {
            return;
        }

        oInfo->groupID = sInfo->group_id;
        oInfo->groupName = sInfo->group_name;
        oInfo->groupAvatarUrl = sInfo->group_avatar_url;
    }

    static void oZIMGroupAdvancedConfig(ZIMGroupAdvancedConfig *oInfo,
                                        struct zim_group_advanced_config *sInfo) {
        if (!sInfo) {
            return;
        }

        oInfo->groupNotice = sInfo->group_notice;
        for (unsigned int i = 0; i < static_cast<unsigned int>(sInfo->group_attributes_length);
             ++i) {
            oInfo->groupAttributes.emplace(sInfo->group_attributes[i].key,
                                           sInfo->group_attributes[i].value);
        }
    }

    static void oZIMGroupMuteInfo(ZIMGroupMuteInfo *oInfo,
                                  const struct zim_group_mute_info &sInfo) {

        oInfo->mode = static_cast<ZIMGroupMuteMode>(sInfo.mute_mode);
        oInfo->expiredTime = sInfo.mute_expired_time;
        if (sInfo.mute_role_list_length > 0) {
            oInfo->roles = std::vector<ZIMGroupMemberRole>(
                sInfo.mute_role_list, sInfo.mute_role_list + sInfo.mute_role_list_length);
        }
    }

    static void oZIMGroup(ZIMGroup *oInfo, const struct zim_group *sInfo) {
        if (!sInfo) {
            return;
        }
        oInfo->baseInfo.groupID = sInfo->base_info.group_id;
        oInfo->baseInfo.groupName = sInfo->base_info.group_name;
        oInfo->baseInfo.groupAvatarUrl = sInfo->base_info.group_avatar_url;
        oInfo->notificationStatus = (ZIMGroupMessageNotificationStatus)(sInfo->notification_status);
    }

    static void oZIMGroupInfo(ZIMGroupFullInfo *oInfo, struct zim_group_full_info *sInfo) {
        if (!sInfo) {
            return;
        }

        oZIMGroupBaseInfo(&(oInfo->baseInfo), &(sInfo->base_info));
        oInfo->groupNotice = sInfo->group_notice;
        oInfo->notificationStatus = (ZIMGroupMessageNotificationStatus)(sInfo->notification_status);
        for (unsigned int i = 0; i < static_cast<unsigned int>(sInfo->group_attributes_length);
             ++i) {
            oInfo->groupAttributes.emplace(sInfo->group_attributes[i].key,
                                           sInfo->group_attributes[i].value);
        }
        oZIMGroupMuteInfo(&(oInfo->mutedInfo), sInfo->muted_info);
    }

    static void oZIMGroupMemberInfo(ZIMGroupMemberInfo *oInfo,
                                    const struct zim_group_member_info *sInfo) {
        if (!sInfo) {
            return;
        }

        oInfo->userID = sInfo->user_id;
        oInfo->userName = sInfo->user_name;
        oInfo->userAvatarUrl = sInfo->member_avatar_url;
        oInfo->memberAvatarUrl = sInfo->member_avatar_url;
        oInfo->memberNickname = sInfo->member_nick_name;
        oInfo->memberRole = sInfo->member_role;
        oInfo->muteExpiredTime = sInfo->mute_expired_time;
    }

    static void oZIMErrorUserInfo(ZIMErrorUserInfo *oInfo,
                                  const struct zim_error_user_info *sInfo) {
        if (!sInfo) {
            return;
        }

        oInfo->userID = sInfo->user_id;
        oInfo->reason = sInfo->reason;
    }

    static void oZIMFriendRelationInfo(ZIMFriendRelationInfo *oInfo,
                                       const struct zim_friend_relation_info *sInfo) {
        if (!sInfo) {
            return;
        }

        oInfo->type = (ZIMUserRelationType)sInfo->type;
        oInfo->userID = sInfo->user_id;
    }

    static void oZIMCallUserInfo(ZIMCallUserInfo *oInfo, const struct zim_call_user_info *sInfo) {
        if (!sInfo) {
            return;
        }

        oInfo->userID = sInfo->user_id;
        oInfo->state = (ZIMCallUserState)sInfo->user_state;
        oInfo->extendedData = sInfo->extended_data;
    }

    static void oZIMCallInfo(ZIMCallInfo *oInfo, struct zim_call_info *sInfo) {
        if (!sInfo) {
            return;
        }

        oInfo->caller = sInfo->caller;
        oInfo->callID = sInfo->call_id;
        oInfo->state = (ZIMCallState)sInfo->state;
        oInfo->mode = (ZIMCallInvitationMode)sInfo->mode;
        oInfo->extendedData = sInfo->extended_data;
        oInfo->createTime = sInfo->create_time;
        oInfo->endTime = sInfo->end_time;
        //oInfo->callDuration = sInfo->call_duration;
        //oInfo->userDuration = sInfo->user_duration;

        oInfo->callUserList.resize(sInfo->call_user_list_length);
        for (unsigned int i = 0; i < static_cast<unsigned int>(sInfo->call_user_list_length); ++i) {
            ZIMCallUserInfo UserInfo{};
            UserInfo.userID = sInfo->call_user_list[i].user_id;
            UserInfo.extendedData = sInfo->call_user_list[i].extended_data;
            UserInfo.state = (ZIMCallUserState)sInfo->call_user_list[i].user_state;
            oInfo->callUserList.at(i) = UserInfo;
        }
    }

    static void oZIMGroupOperatedInfo(ZIMGroupOperatedInfo *oInfo,
                                      struct zim_group_operated_info *sInfo) {
        if (!sInfo) {
            return;
        }

        oInfo->operatedUserInfo.userID = sInfo->user_id;
        oInfo->operatedUserInfo.userName = sInfo->user_name;
        oInfo->operatedUserInfo.memberNickname = sInfo->member_nick_name;
        oInfo->operatedUserInfo.memberRole = sInfo->member_role;

        oInfo->userID = sInfo->user_id;
        oInfo->userName = sInfo->user_name;
        oInfo->memberNickname = sInfo->member_nick_name;
        oInfo->memberRole = sInfo->member_role;
    }

    static void oZIMCallInvitationSentInfo(ZIMCallInvitationSentInfo &oInfo,
                                           struct zim_call_invitation_sent_info *sInfo) {
        if (!sInfo) {
            return;
        }
        oInfo.timeout = sInfo->timeout;
        oInfo.errorInvitees.resize(sInfo->error_invitees_length);
        oInfo.errorUserList.resize(sInfo->old_error_invitees_length);
        for (unsigned int i = 0; i < static_cast<unsigned int>(sInfo->error_invitees_length); i++) {
            ZIMErrorUserInfo UserInfo{};
            UserInfo.userID = sInfo->error_invitees[i].user_id;
            UserInfo.reason = sInfo->error_invitees[i].reason;
            oInfo.errorUserList.at(i) = UserInfo;
        }

        for (unsigned int i = 0; i < static_cast<unsigned int>(sInfo->old_error_invitees_length);
             i++) {
            ZIMCallUserInfo UserInfo{};
            UserInfo.userID = sInfo->old_error_invitees[i].user_id;
            UserInfo.state = (ZIMCallUserState)sInfo->old_error_invitees[i].user_state;
            UserInfo.extendedData = sInfo->old_error_invitees[i].extended_data;
            oInfo.errorInvitees.at(i) = UserInfo;
        }
    }

    static void oZIMCallInvitationReceivedInfo(ZIMCallInvitationReceivedInfo &oInfo,
                                               struct zim_call_invitation_received_info *sInfo) {
        if (!sInfo) {
            return;
        }
        oInfo.timeout = sInfo->timeout;
        oInfo.inviter = sInfo->inviter;
        oInfo.caller = sInfo->caller;
        oInfo.createTime = sInfo->create_time;
        oInfo.mode = (ZIMCallInvitationMode)sInfo->mode;
        oInfo.extendedData = std::string(sInfo->extended_data);

        auto userListLength = sInfo->call_user_list_length;
        if (userListLength != 0) {
            oInfo.callUserList.resize(userListLength);
            for (unsigned int i = 0; i < userListLength; i++) {
                ZIMConverter::oZIMCallUserInfo(&oInfo.callUserList.at(i),
                                               &sInfo->call_user_list[i]);
            }
        }
    }

    static void oZIMCallInvitationCancelledInfo(ZIMCallInvitationCancelledInfo &oInfo,
                                                struct zim_call_invitation_cancelled_info *sInfo) {
        if (!sInfo) {
            return;
        }
        oInfo.inviter = sInfo->inviter;
        oInfo.mode = (ZIMCallInvitationMode)sInfo->mode;
        oInfo.extendedData = std::string(sInfo->extended_data);
    }

    static void oZIMCallInvitationAcceptedInfo(ZIMCallInvitationAcceptedInfo &oInfo,
                                               struct zim_call_invitation_accepted_info *sInfo) {
        if (!sInfo) {
            return;
        }
        oInfo.invitee = sInfo->invitee;
        oInfo.extendedData = std::string(sInfo->extended_data);
    }

    static void oZIMCallInvitationRejectedInfo(ZIMCallInvitationRejectedInfo &oInfo,
                                               struct zim_call_invitation_rejected_info *sInfo) {
        if (!sInfo) {
            return;
        }
        oInfo.invitee = sInfo->invitee;
        oInfo.extendedData = std::string(sInfo->extended_data);
    }

    static void oZIMCallInvitationTimeoutInfo(ZIMCallInvitationTimeoutInfo &oInfo,
                                              struct zim_call_invitation_timeout_info *sInfo) {

        if (!sInfo) {
            return;
        }
        oInfo.mode = (ZIMCallInvitationMode)sInfo->mode;
    }

    static void oZIMCallInvitationCreatedInfo(ZIMCallInvitationCreatedInfo &oInfo,
                                              struct zim_call_invitation_created_info *sInfo) {
        if (!sInfo) {
            return;
        }
        oInfo.mode = (ZIMCallInvitationMode)sInfo->mode;
        oInfo.caller = sInfo->caller;
        oInfo.extendedData = sInfo->extended_data;
        oInfo.timeout = sInfo->timeout;
        oInfo.createTime = sInfo->create_time;
        auto call_user_list_length = sInfo->call_user_list_length;
        std::vector<ZIMCallUserInfo> callUserInfoList;
        auto call_user_list = sInfo->call_user_list;
        if (call_user_list_length != 0) {
            callUserInfoList.resize(call_user_list_length);
            for (unsigned int i = 0; i < call_user_list_length; i++) {

                ZIMConverter::oZIMCallUserInfo(&callUserInfoList.at(i), &call_user_list[i]);
            }
        }
        oInfo.callUserList = callUserInfoList;
    }

    static void oZIMCallInvitationEndedInfo(ZIMCallInvitationEndedInfo &oInfo,
                                            struct zim_call_invitation_ended_info *sInfo) {
        if (!sInfo) {
            return;
        }

        oInfo.caller = sInfo->caller;
        oInfo.operatedUserID = sInfo->operated_user_id;
        oInfo.extendedData = sInfo->extended_data;
        oInfo.mode = (ZIMCallInvitationMode)sInfo->mode;
        oInfo.endTime = sInfo->end_time;
    }

    static void oZIMCallUserStateChangedInfo(ZIMCallUserStateChangeInfo &oInfo,
                                             struct zim_event_call_user_state_changed_info *sInfo) {
        if (!sInfo) {
            return;
        }
        auto call_user_list_length = sInfo->call_user_list_length;
        auto call_user_list = sInfo->call_user_list;
        std::vector<ZIMCallUserInfo> callUserInfoList;
        if (call_user_list_length != 0) {
            callUserInfoList.resize(call_user_list_length);
            for (unsigned int i = 0; i < call_user_list_length; i++) {
                ZIMConverter::oZIMCallUserInfo(&callUserInfoList.at(i), &call_user_list[i]);
            }
        }
        oInfo.callUserList = callUserInfoList;
    }

    static void oZIMCallingInvitationSentInfo(ZIMCallingInvitationSentInfo &oInfo,
                                              struct zim_calling_invitation_sent_info *sInfo) {
        if (!sInfo) {
            return;
        }
        oInfo.errorUserList.resize(sInfo->error_invitees_length);
        for (unsigned int i = 0; i < static_cast<unsigned int>(sInfo->error_invitees_length); i++) {
            ZIMErrorUserInfo UserInfo{};
            UserInfo.userID = sInfo->error_invitees[i].user_id;
            UserInfo.reason = sInfo->error_invitees[i].reason;
            oInfo.errorUserList.at(i) = UserInfo;
        }
    }

    static void oZIMCallJoinSentInfo(ZIMCallJoinSentInfo &oInfo,
                                     struct zim_call_join_sent_info *sInfo) {
        if (!sInfo) {
            return;
        }
        oInfo.extendedData = sInfo->extended_data ? sInfo->extended_data : "";
        oInfo.createTime = sInfo->create_time;
        oInfo.joinTime = sInfo->join_time;
        auto call_user_list_length = sInfo->call_user_list_length;
        auto call_user_list = sInfo->call_user_list;
        std::vector<ZIMCallUserInfo> callUserInfoList;
        if (call_user_list_length != 0) {
            callUserInfoList.resize(call_user_list_length);
            for (unsigned int i = 0; i < call_user_list_length; i++) {
                ZIMConverter::oZIMCallUserInfo(&callUserInfoList.at(i), &call_user_list[i]);
            }
        }
        oInfo.callUserList = callUserInfoList;
    }

    static void oZIMCallQuitSentInfo(ZIMCallQuitSentInfo &oInfo,
                                     struct zim_call_quit_sent_info *sInfo) {
        if (!sInfo) {
            return;
        }
        oInfo.acceptTime = sInfo->accept_time;
        oInfo.createTime = sInfo->create_time;
        oInfo.quitTime = sInfo->quit_time;
    }

    static void oZIMError(ZIMError *oError, struct zim_error *sError) {
        oError->code = (ZIMErrorCode)sError->code;
        oError->message = sError->message;
    }

    static void oZIMFriendInfo(ZIMFriendInfo &oInfo, const struct zim_friend_info *sInfo) {
        if (!sInfo) {
            return;
        }

        oInfo.createTime = sInfo->create_time;
        oInfo.friendAlias = sInfo->friend_alias;
        oInfo.wording = sInfo->wording;
        oInfo.userID = sInfo->user_info.user_id;
        oInfo.userName = sInfo->user_info.user_name;
        oInfo.userAvatarUrl = sInfo->user_info.user_avatar_url;
        for (unsigned int i = 0; i < static_cast<unsigned int>(sInfo->friend_attributes_length);
             ++i) {
            oInfo.friendAttributes.emplace(sInfo->friend_attributes[i].key,
                                           sInfo->friend_attributes[i].value);
        }
    }

    static void oZIMFriendApplicationInfo(ZIMFriendApplicationInfo &oInfo,
                                          const struct zim_friend_application_info *sInfo) {
        if (!sInfo) {
            return;
        }

        ZIMUserInfo applyUser{};
        oInfo.createTime = sInfo->create_time;
        oInfo.updateTime = sInfo->update_time;
        oInfo.wording = sInfo->wording;
        oInfo.state = (ZIMFriendApplicationState)sInfo->state;
        oInfo.type = (ZIMFriendApplicationType)sInfo->type;
        struct zim_user_info user_info = sInfo->apply_user;
        oZIMUserInfo(&applyUser, &user_info);
        oInfo.applyUser = applyUser;
    }
};

} // namespace zim
