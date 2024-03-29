﻿#pragma once

//
//  ZIM.h
//  ZIM
//
//  Copyright © 2021 Zego. All rights reserved.
//

#include "ZIMDefines.h"
#include "ZIMErrorCode.h"
#include "ZIMEventHandler.h"

namespace zim {

// ZIM SDK main class
//
class ZIM {

  protected:
    virtual ~ZIM() {}

  public:
    // Gets the SDK's version number.
    //
    // When the SDK is running, the developer finds that it does not match the expected situation and submits the problem and related logs to the ZEGO technical staff for locating. The ZEGO technical staff may need the information of the engine version to assist in locating the problem.
    // Developers can also collect this information as the version information of the engine used by the app, so that the SDK corresponding to each version of the app on the line.
    //
    // @return SDK version
    static std::string getVersion();

    static ZIM *getInstance();

    // MARK: - Main

    // Create a ZIM instance.
    //
    // You need to create and initialize an ZIM instance before calling any other function.
    // The SDK supports the creation of multiple ZIM instances.
    //
    // @param config This parameter is used for configuration items before create, please apply appID from the ZEGO Admin Console https://console.zego.im
    static ZIM *create(const ZIMAppConfig &config);

    // @deprecated Deprecated since 2.3.0, pleace use the method with the same name method.
    // Create a ZIM instance.
    //
    // You need to create and initialize an ZIM instance before calling any other function.
    // The SDK supports the creation of multiple ZIM instances.
    //
    // @param appID Application ID issued by ZEGO for developers, please apply from the ZEGO Admin Console https://console.zego.im
    static ZIM *create(unsigned int appID);

    // Destroy the ZIM instance.
    //
    // Used to release resources used by ZIM.
    virtual void destroy() = 0;

    // Set log related configuration.
    //
    // When you need to customize the log file size and path, you need to call this function to set the log configuration.
    // It must be set before calling [create] to take effect. If it is set after [create], it will take effect at the next [create].
    //
    // @param config Log configuration object
    static void setLogConfig(const ZIMLogConfig &config);

    static void setAdvancedConfig(const std::string &key, const std::string &value);

    static bool setGeofencingConfig(const std::vector<int> &areaList, ZIMGeofencingType type);

    // Set cache related configuration.
    //
    // Supported version: 1.1.0 and above.
    //
    // Detailed description: Example Set the SDK cache file path. Because the SDK has a default path, it is generally not recommended that you set your own path unless there is a strong need to do so.
    //
    // Default value:Android：/storage/Android/data/[packageName]/files/ZIMCaches
    // iOS：~/Library/Caches/ZIMCaches
    // macOS：（sandbox）~/Library/Containers/[Bundle ID]/Data/Library/Caches/ZIMCaches / ~/Library/Caches/ZIMCaches
    // Windows：C:\Users\[Your UserName]\AppData\[App Name]ZEGO.SDK\ZIMCaches
    //
    // Call timing: It must be called before [create].
    //
    // Note: If the developer calls after [create], the SDK saves the configuration until it takes effect the next time [Create] is invoked.
    //
    // Related callbacks: In addition to getting the login result in the callback parameter, the developer will also receive the [onConnectionStateChanged] callback during the login request and after the login is successful/failed to determine the current user's login status.
    //
    // Life cycle: Set before calling [create] and takes effect when calling [create]. If the developer does not set the new logging configuration the next time [create] is created, the previous configuration will still take effect.
    //
    // Platform difference: The default path varies with platforms. Please refer to the default value.
    // @param config cache configuration object

    static void setCacheConfig(const ZIMCacheConfig &config);

    // Sets the Context for the Android platform. It must be called before calling [create] on the Android platform.
    //
    // @param Java VM Object
    // @param context Android Context, it must be valid in the SDK lifecycle.
    //
    static void setAndroidEnv(void *jvm, void *context);

    // Upload log and call after setting up log path
    //
    // Description: After calling [create] to create an instance, the log report can be called.
    virtual void uploadLog(ZIMLogUploadedCallback callback) = 0;

    // Set the event notification callbacks that need to be handled. If the eventHandler is set to [nullptr], all the callbacks set previously will be cleared.
    //
    // Description: Invoking this function multiple times will **overwrite** the handler set by the last invoke to this function.
    //
    // @param handler Event notification callback. Developers should override callbacks to focus on specific notifications based on their own business scenarios.
    virtual void setEventHandler(std::shared_ptr<ZIMEventHandler> handler) = 0;

    // Supported version: 2.4.0 and above.

    // Detailed description: Log in to the ZIM service. [login] is the most important step of the ZIM function. You need to log in before using any other functions.

    // Call timing: This function must be called after calling [create] to create an instance and before calling other instance functions.

    // Note: Before using ZIM's single chat, room, message sending and receiving functions, you must first call this function to log in, and the UI can be displayed to the user through the login result.

    // Privacy protection statement: remind users not to pass in sensitive information involving personal privacy in the userID parameter, including but not limited to mobile phone number, ID number, passport number, real name, etc.

    // Related callbacks: In addition to getting the login result in the callback parameter, the developer will also receive the [onConnectionStateChanged] callback during the login request and after the login is successful/failed to determine the current user's login status.
    // @param userInfo Unique ID used to identify the user. Note that the userID must be unique under the same appID, otherwise mutual kicks out will occur.
    // @param callback
    virtual void login(const ZIMUserInfo &userInfo, ZIMLoggedInCallback callback) = 0;

    // Login, you must log in before using all functions.
    //
    // Description: After invoking this function, you will receive the [onConnectionStateChanged] callback,
    // which can be used to determine whether the login is successful.
    //
    // @param userID Unique ID used to identify the user. Note that the userID must be unique under the same appID, otherwise mutual kicks out will occur.
    // @param token The token issued by the developer's business server, used to ensure security. The generation rules are detailed in ZEGO document website.
    virtual void login(const ZIMUserInfo &userInfo, const std::string &token,
                       ZIMLoggedInCallback callback) = 0;

    virtual void login(const std::string &userID, const ZIMLoginConfig &config,
                       ZIMLoggedInCallback callback) = 0;

    // Log out, you cannot receive messages after logging out.
    //
    // Description: After invoking this function, you will receive the [onConnectionStateChanged] callback.
    virtual void logout() = 0;

    // Update the authentication token.
    //
    // Description: After receiving the [onTokenWillExpire] callback, you needs to request your authentication server to regenerate a token in time.
    //
    // @param token The token issued by the developer's business server, used to ensure security. The generation rules are detailed in ZEGO document website.
    // @param callback Callback of the renewing token result.
    virtual void renewToken(const std::string &token, ZIMTokenRenewedCallback callback) = 0;

    /// Available since: 2.2.0 or above.
    ///
    /// Description: After user logs in, calling this interface could update the user's own user name.
    ///
    /// When to call: After the user is logged in.
    ///
    /// Privacy reminder: Try not to pass in sensitive information involving personal privacy, including but not limited to mobile phone numbers, ID numbers, passport numbers, real names, etc.
    ///
    /// Related callbacks: [onUserNameUpdatedCallback].
    ///
    /// Related APIs: [updatedUserExtended] and [queryUsersInfo].
    /// @param userName User name wanted to changed to.
    /// @param callback The callback of the update user name.
    virtual void updateUserName(const std::string &userName,
                                ZIMUserNameUpdatedCallback callback) = 0;

    /// Supported versions: 2.3.0 and above.
    ///
    /// Detail description: After the user logs in, calling this interface can set or update the user's own user avatar URL.
    //
    /// Call timing/notify timing: After the user logs in.
    //
    /// Note: The user avatar itself needs to be stored by the developer, and ZIM only saves its user information as a pass-through URL.
    //
    /// Usage Restriction: There is no limit on special characters and a maximum of 500 bytes.
    //
    /// Related callback: [ZIMUserAvatarUrlUpdatedCallback].
    //
    /// Related interface: [queryUsersInfo].

    virtual void updateUserAvatarUrl(const std::string &userAvatarUrl,
                                     ZIMUserAvatarUrlUpdatedCallback callback) = 0;

    /// Available since: 2.2.0 or above.
    ///
    /// Description: After user logs in, calling this interface could update the user's own user extended data.
    ///
    /// When to call: After the user is logged in.
    ///
    /// Privacy reminder: Try not to pass in sensitive information involving personal privacy, including but not limited to mobile phone numbers, ID numbers, passport numbers, real names, etc.
    ///
    /// Related callbacks: [ onUserNameUpdatedCallback ].
    ///
    /// Related APIs: [ updatedUserExtended ] and [ queryUsersInfo ].
    /// @param extendedData User extended data wanted to changed to .
    /// @param callback The callback of the update user extended data.
    virtual void updateUserExtendedData(const std::string &extendedData,
                                        ZIMUserExtendedDataUpdatedCallback callback) = 0;

    /// Available since: 2.0.0 and above.
    ///
    /// Description: Through this interface, you can query and obtain the corresponding UserInfo by userID.
    ///
    /// When to call /Trigger: It is available only after calling [create] to create the instance and then calling [login] to login.
    ///
    /// Related callbacks: [ZIMUsersInfoQueriedCallback]
    ///
    /// @param userIDs userID list.
    /// @param callback Callback of the result.
    virtual void queryUsersInfo(const std::vector<std::string> &userIDs,
                                const ZIMUsersInfoQueryConfig &config,
                                ZIMUsersInfoQueriedCallback callback) = 0;

    // MARK: - Message

    // @deprecated Deprecated since 2.4.0, please use [sendMessage] instead.
    // Send peer-to-peer messages.
    //
    // Description: After invoking this function, you will receive the [MessageSent] callback,
    // which can be used to determine whether the message is sent successfully.
    //
    // @param message The message to be sent.
    // @param toUserID The ID of the user who will receive the message.
    // @param config Related configuration for sending single chat messages.
    // @param callback Callback of the sending result of the message.
    virtual void sendPeerMessage(ZIMMessage *message, const std::string &toUserID,
                                 const ZIMMessageSendConfig &config,
                                 ZIMMessageSentCallback callback) = 0;

    // @deprecated Deprecated since 2.4.0, please use [sendMessage] instead.
    // Send room messages.
    //
    // Description: After invoking this function, you will receive the [MessageSent] callback,
    // which can be used to determine whether the message is sent successfully.
    //
    // @param message The message to be sent.
    // @param toRoomID The ID of the room which will receive the message.
    // @param config Related configuration for sending room messages.
    // @param callback Callback of the sending result of the message.
    virtual void sendRoomMessage(ZIMMessage *message, const std::string &toRoomID,
                                 const ZIMMessageSendConfig &config,
                                 ZIMMessageSentCallback callback) = 0;

    // @deprecated Deprecated since 2.4.0, please use [sendMessage] instead.
    // Send group message.
    //
    // Supported versions: 2.0.0 and above.
    //
    // Detail description: This interface is called when a group chat message needs to be sent.
    //
    // Business scenario: This interface can be used when sending group messages.
    //
    // Call timing/Notification timing: This interface is called when a group chat message needs to be sent.
    //
    // Usage limit: 10 times/s, available after login, unavailable after logout.
    //
    // Note: pushconfig only needs to be filled in when you need to use the offline push function. The properties in ZIMMessage are read-only and do not need to be modified.
    //
    // Scope of influence: Using this method will trigger the receivePeerMessage callback of the message receiver, and will trigger the onConversationChanged callback of the sender and receiver. If the session where the message is located does not have message DND set, the conversationTotalUnreadMessageCountUpdated callback will be triggered.
    //
    // Related callbacks: [ZIMMessageSentCallback], [receiveGroupMessage], [onConversationChanged], [conversationTotalUnreadMessageCountUpdated].
    // Related interfaces: [queryHistoryMessage], [deleteMessageByConversationID], [deleteMessage]
    // @param message The message to be sent.
    // @param toRoomID The ID of the group which will receive the message.
    // @param config Related configuration for sending group messages..
    // @param callback Callback of the sending result of the message.

    virtual void sendGroupMessage(ZIMMessage *message, const std::string &toGroupID,
                                  const ZIMMessageSendConfig &config,
                                  ZIMMessageSentCallback callback) = 0;

    // Supported versions: 2.4.0 and above.

    // Detailed description: This method can be used to send messages in single chat, room and group chat.

    // Business scenario: When you need to send message to the target user, target message room, and target group chat after logging in, send it through this interface.

    // Call timing/Notification timing: It can be called after login.

    // Usage limit: no more than 10/s, available after login, unavailable after logout.

    // Related callback: [ZIMMessageSentCallback], [ZIMMessageSendNotification], [onReceivePeMessage], [onReceiveRoomMessage], [onReceiveGroupMessage], [onConversationChanged], [onConversationTotalUnreadMessageCountUpdated].

    // Related interfaces: [queryHistoryMessage], [deleteAllMessage], [deleteMessages],[sendMediaMessage]
    // @param message The message to be sent.
    // @param toConversationID The conversation ID the message needs to be sent.
    // @param conversationType Conversation type, supports single chat, room and group chat.
    // @param config Related configuration for sending messages.
    // @param notification Related notifications when messages are sent.
    // @param callback Callback of the sending result of the message.

    virtual void sendMessage(std::shared_ptr<ZIMMessage> message,
                             const std::string &toConversationID,
                             ZIMConversationType conversationType,
                             const ZIMMessageSendConfig &config,
                             std::shared_ptr<ZIMMessageSendNotification> notification,
                             ZIMMessageSentCallback callback) = 0;

    // @deprecated Deprecated since 2.4.0, pleace use the method with the same name method.
    // Supported versions: 2.1.0 and above.
    //
    // Detailed description: This method can be used to send messages in single chat, room and group chat.
    //
    // Business scenario: When you need to send media to the target user, target message room, and target group chat after logging in, send it through this interface.
    //
    // Call timing/Notification timing: It can be called after login.
    //
    // Usage limit: no more than 10/s, available after login, unavailable after logout.
    //
    // Impact: [onReceivePeerMessage]/[ReceiveGroupMessage] sessions and session-scoped [onReceiveGroupMessage] sessions did not fire message receiver's [ConversationR] fires [onversationTotalUnreadMessageCountUpdated] objection.
    //
    // Note: Only required if you need to use the threaded update feature when pushing configuration. Push notifications are not supported, nor are [onContationChanged] and [ConTotalUnreadMessageCountUpdated] supported if media messages are broadcast to the world.
    //
    // Related: [ZIMMessageSentCallback], [ZIMMediaUploadingProgress], [onReceivePeMessage], [onReceiveRoomMessage], [onReceiveGroupMessage], [onConversationChanged], [onConversationTotalUnreadMessageCountUpdated].
    // Related interfaces: [queryHistoryMessage], [deleteAllMessage], [deleteMessages]
    //
    // @param message The message to be sent.
    // @param toConversationID The ID of the conversation which will receive the message.
    // @param conversationType The conversation type.
    // @param config Related configuration for sending group messages.
    // @param progress Callback of the sending progress of the message.
    // @param callback Callback of the sending result of the message.
    virtual void sendMediaMessage(ZIMMediaMessage *message, const std::string &toConversationID,
                                  ZIMConversationType conversationType,
                                  const ZIMMessageSendConfig &config,
                                  ZIMMediaUploadingProgress progress,
                                  ZIMMessageSentCallback callback) = 0;

    // Supported versions: 2.4.0 and above.

    // Detailed description: This method can be used to send messages in single chat, room and group chat.

    // Business scenario: When you need to send media to the target user, target message room, and target group chat after logging in, send it through this interface.

    // Call timing/Notification timing: It can be called after login.

    // Usage limit: no more than 10/s, available after login, unavailable after logout.

    // Impact: [onReceivePeerMessage]/[ReceiveGroupMessage] sessions and session-scoped [onReceiveGroupMessage] sessions did not fire message receiver's [ConversationR] fires [onversationTotalUnreadMessageCountUpdated] objection.

    // Note: Only required if you need to use the threaded update feature when pushing configuration. Push notifications are not supported, nor are [onContationChanged] and [ConTotalUnreadMessageCountUpdated] supported if media messages are broadcast to the world.

    // Related: [ZIMMessageSentCallback], [ZIMMediaMessageSendNotification], [onReceivePeMessage], [onReceiveRoomMessage], [onReceiveGroupMessage], [onConversationChanged], [onConversationTotalUnreadMessageCountUpdated].

    // Related interfaces: [queryHistoryMessage], [deleteAllMessage], [deleteMessages]
    // @param message When using the message to be sent, modify the type of message according to the type of multimedia message. For example, when sending image messages, use ZIMImageMessage.
    // @param toConversationID The conversation ID of the message recipient, supports single chat, room and group chat.
    // @param conversationType Conversation type, supports single chat, room and group chat.
    // @param config Related configuration for sending messages.
    // @param notification Relevant notifications when sending media messages, including upload progress, etc.
    // @param callback Result callback for sending media messages.

    virtual void sendMediaMessage(std::shared_ptr<ZIMMediaMessage> message,
                                  const std::string &toConversationID,
                                  ZIMConversationType conversationType,
                                  const ZIMMessageSendConfig &config,
                                  std::shared_ptr<ZIMMediaMessageSendNotification> notification,
                                  ZIMMessageSentCallback callback) = 0;

    // Supported versions: 2.1.0 and above.
    //
    // Detailed description: This method can be used to download the content of media messages, including the original image, large image, thumbnail image, file message, audio message, video message and the first frame of the image message.
    //
    // Service scenario: After the user receives a message, if the message is a media message, he can call this API to download its content.
    //
    // Invoke timing/notification timing: can be invoked after logging in and receiving a media message.
    //
    // Related callbacks: [ZIMMediaDownloadedCallback], [ZIMMediadownloadingProgress].
    //
    // @param message The message to be downloaded.
    // @param fileType The file type to be downloaded.
    // @param progress Callback of the downloading progress of the message.
    // @param callback Callback of the downloading result of the message.
    virtual void downloadMediaFile(ZIMMediaMessage *message, ZIMMediaFileType fileType,
                                   ZIMMediaDownloadingProgress progress,
                                   ZIMMediaDownloadedCallback callback) = 0;

    // Supported versions: 2.4.0 and above.
    //
    // Detailed description: This method can be used to download the content of media messages, including the original image, large image, thumbnail image, file message, audio message, video message and the first frame of the image message.
    //
    // Service scenario: After the user receives a message, if the message is a media message, he can call this API to download its content.
    //
    // Invoke timing/notification timing: can be invoked after logging in and receiving a media message.
    //
    // Related callbacks: [ZIMMediaDownloadedCallback], [ZIMMediadownloadingProgress].
    //
    // @param message The message to be downloaded.
    // @param fileType The file type to be downloaded.
    // @param progress Callback of the downloading progress of the message.
    // @param callback Callback of the downloading result of the message.
    virtual void downloadMediaFile(std::shared_ptr<ZIMMediaMessage> message,
                                   ZIMMediaFileType fileType, ZIMMediaDownloadingProgress progress,
                                   ZIMMediaDownloadedCallback callback) = 0;

    // Update the local expandable field of the message.
    //
    // Description: After the user logs in, calling this interface allows updating the local expandable field of the message.
    //
    // @param extended_data The expandable message field visible only on this end can store additional information locally and currently has a length limit of 128K. If you have special requirements, please contact ZEGO technical support for configuration.
    // @param message Message body to be updated.
    // @param callback Callback function to modify local extension fields.
    virtual void
    updateMessageLocalExtendedData(const std::string &localExtendedData,
                                   const std::shared_ptr<ZIMMessage> &message,
                                   ZIMMessageLocalExtendedDataUpdatedCallback callback) = 0;

    // Query historical messages.
    //
    // Supported versions: 2.0.0 and above.
    //
    // Detailed description: This method is used to query historical messages.
    //
    // Business scenario: When you need to obtain past historical messages, you can call this interface to query historical messages by paging.
    //
    // Call timing/Notification timing: Called when historical messages need to be queried.
    //
    // Restrictions: Effective after login, invalid after logout.
    //
    // Related callbacks: [ZIMMessageQueriedCallback]
    // @param conversationID The session ID of the queried historical message.
    // @param conversationType conversation type.
    // @param config Query the configuration of historical messages.
    // @param callback Returns the result of querying historical messages.

    virtual void queryHistoryMessage(const std::string &conversationID,
                                     ZIMConversationType conversationType,
                                     const ZIMMessageQueryConfig &config,
                                     ZIMMessageQueriedCallback callback) = 0;

    // Delete all message by conversation id and conversation type
    //
    // Supported versions: 2.0.0 and above.
    //
    // Detail description: This method implements the function of deleting messages.
    //
    // Business scenario: The user needs to delete a message. When the user does not need to display a message, this method can be used to delete it.
    //
    // Call timing/Notification timing: Called when the message needs to be deleted.
    //
    // Note: The impact of deleting messages is limited to this account.
    //
    // Restrictions: Effective after login, invalid after logout.
    //
    // Scope of influence: If the deleted message is the latest message of the session, the [conversationChanged] callback will be triggered, and if the message is unread, the [conversationTotalUnreadMessageCountUpdated] callback will be triggered.
    //
    // @param conversationID The session ID of the deleted message.
    // @param conversationType conversation type.
    // @param config Delete the configuration of the message.
    // @param callback Returns the result of deleting the message.
    virtual void deleteAllMessage(const std::string &conversationID,
                                  ZIMConversationType conversationType,
                                  const ZIMMessageDeleteConfig &config,
                                  ZIMMessageDeletedCallback callback) = 0;

    //Supported versions: 2.14.0 and above.
    //
    //Detail description: This method implements the function of deleting all messages for all sessions.
    //
    //Business scenario: The user needs to delete a message. When the user does not need to display a message, this method can be used to delete it.
    //
    //Call timing/Notification timing: Called when you need to delete all messages for all sessions.
    //
    //Note: The sdk deletes lastMessage for each conversation, but does not trigger [onConversationChanged] for each conversation.
    //To update the last message of the conversation, call [queryConversationList] again to retrieve the conversation list from the sdk.
    //IsAlsoDeleteServerMessage decided whether to delete a server message, the impact of deleting messages is limited to this account.
    //
    //Restrictions: Effective after login.
    //
    //Impacts on other APIs: Call the interface trigger [onMessageDeleted] callback, if there are unread messages at this time, will trigger  [onConversationTotalUnreadMessageCountUpdated] callback.
    //
    //Related callbacks：[ZIMConversationMessagesAllDeletedCallback]、[onMessageDeleted]、[onConversationTotalUnreadMessageCountUpdated].
    // @param config Delete the configuration of message.
    // @param callback Returns the result of deleting all messages.

    virtual void
    deleteAllConversationMessages(const ZIMMessageDeleteConfig &config,
                                  ZIMConversationMessagesAllDeletedCallback callback) = 0;

    // Delete message.
    //
    // Supported versions: 2.0.0 and above.
    //
    // Detail description: This method implements the function of deleting messages.
    //
    // Business scenario: The user needs to delete a message. When the user does not need to display a message, this method can be used to delete it.
    //
    // Call timing/Notification timing: Called when the message needs to be deleted.
    //
    // Note: The impact of deleting messages is limited to this account.
    //
    // Restrictions: Effective after login, invalid after logout.
    //
    // Scope of influence: If the deleted message is the latest message of the session, the [conversationChanged] callback will be triggered, and if the message is unread, the [conversationTotalUnreadMessageCountUpdated] callback will be triggered.
    //
    // @param messageList List of deleted messages.
    // @param conversationID The session ID of the deleted message.
    // @param conversationType conversation type.
    // @param config Delete the configuration of the message.
    // @param callback Returns the result of deleting the message.

    virtual void deleteMessages(const std::vector<std::shared_ptr<ZIMMessage>> &messageList,
                                const std::string &conversationID,
                                ZIMConversationType conversationType,
                                const ZIMMessageDeleteConfig &config,
                                ZIMMessageDeletedCallback callback) = 0;

    // Revoke a message sent by self.
    //
    // Available since: 2.5.0 and above.
    //
    // Description: This method implements the function of revoking messages.
    //
    // Use cases: The user needs to recall a message. This method can be used when the user does not want other users to see the message.
    //
    // When to call /Trigger: Called when the message needs to be revoked.
    //
    // Note: Room message revoke is not supported. If the revoked message is the latest message of the session, the [conversationChanged] callback will be triggered, and if the message is unread, the [conversationTotalUnreadMessageCountUpdated] callback will be triggered.
    //
    // @param message The message needs to be revoke.
    // @param config The configuration of the revoking operation.
    // @param callback The result of revoking the message.
    virtual void revokeMessage(std::shared_ptr<ZIMMessage> message,
                               const ZIMMessageRevokeConfig &config,
                               ZIMMessageRevokedCallback callback) = 0;

    // Supported Versions: 2.4.0 and above.

    // Detail description: This method can insert a message directly to the local DB on the client side.

    // Business scenario: The developer can combine the system message type, and convert the callback notification (for example, invite someone into the group, remove someone from the group, etc.) to the system message type on the client side and insert it into the local DB to achieve the effect of the system prompt .

    // Call timing/Notification timing: It can be called after login.

    // Related callback: [ZIMMessageInsertedCallback]

    // Related interfaces: [queryHistoryMessage], [deleteAllMessage], [deleteMessages]
    //
    // @param message The message to be insert.
    // @param conversationID Conversation ID.
    // @param conversationType Conversation type.
    // @param senderUserID The sender ID of this message.
    // @param callback The result callback of the inserted message.
    virtual void insertMessageToLocalDB(std::shared_ptr<ZIMMessage> message,
                                        const std::string &conversationID,
                                        ZIMConversationType conversationType,
                                        const std::string &senderUserID,
                                        ZIMMessageInsertedCallback callback) = 0;

    // Set the receipt of a batch of messages to become read
    //
    // Available since: 2.5.0 and above.
    //
    // Description: This method can set the receipt of a batch of messages to become read.
    //
    // Use cases: Developers can use this method to set a batch of messages with receipts that have been read. If the sender is online, it will receive the [onMessageReceiptChanged] callback.
    //
    // When to call /Trigger: Callable after login. It is recommended to set the settings for the messages that need to be read on the message list page. It is not recommended to mix with [sendConversationMessageReceiptRead].
    //
    // Restrictions: Only support the settings for received messages with receipt status as PROCESSING.
    //
    // Related callbacks: [ZIMMessageReceiptsReadSentCallback].
    //
    // Related APIs: [sendMessage].
    //
    // @param messageList To set the list of read messages.
    // @param conversationID Conversation ID.
    // @param conversationType Conversation type.
    // @param callback Set the result callback of the read message.
    virtual void
    sendMessageReceiptsRead(const std::vector<std::shared_ptr<ZIMMessage>> &messageList,
                            const std::string &conversationID, ZIMConversationType conversationType,
                            ZIMMessageReceiptsReadSentCallback callback) = 0;

    // Set the receipt of a batch of messages to become read
    //
    // Available since: 2.5.0 and above.
    //
    // Description: This method can query the receipt information of a batch of messages, including the status, the number of unread users and the number of read users.
    //
    // Use cases: If you need to query the receipt status of the message, the number of unread users and the number of read users, you can call this interface.
    //
    // When to call /Trigger: Callable after login. If you need to query the detailed member list, you can query through the interface [queryGroupMessageReceiptReadMemberList] or [queryGroupMessageReceiptUnreadMemberList].
    //
    // Restrictions: Only messages whose statuses are not NONE and UNKNOWN are supported.
    //
    // Related callbacks: [ZIMMessageReceiptsInfoQueriedCallback].
    //
    // Related APIs: [queryGroupMessageReceiptReadMemberList], [queryGroupMessageReceiptUnreadMemberList].
    //
    // @param messageList The list of messages to query.
    // @param conversationID Conversation ID.
    // @param conversationType Conversation type.
    // @param callback Callback for the result of querying message receipt information.
    virtual void
    queryMessageReceiptsInfo(const std::vector<std::shared_ptr<ZIMMessage>> &messageList,
                             const std::string &conversationID,
                             ZIMConversationType conversationType,
                             ZIMMessageReceiptsInfoQueriedCallback callback) = 0;

    // Search local message list.
    //
    // Supported versions: 2.9.0 and above.
    //
    // Detailed description: This method is used to search local messages.
    //
    // Business scenario: When it is necessary to search for past local messages based on keywords or other conditions, this interface can be called to perform pagination search for local messages.
    //
    // Restrictions: Effective after login, invalid after logout. Searching is not supported in the room scene (conversationType=1).
    //
    // Related callbacks: [ZIMMessagesSearchedCallback].
    //
    // @param conversationID The conversation ID of the local message to be searchd.
    // @param conversationType conversation type.
    // @param config Search the configuration of local messages.
    // @param callback Callback of the search local message result.
    virtual void searchLocalMessages(const std::string &conversationID,
                                     ZIMConversationType conversationType,
                                     const ZIMMessageSearchConfig &config,
                                     ZIMMessagesSearchedCallback callback) = 0;

    // Search global local message list.
    //
    // Supported versions: 2.9.0 and above.
    //
    // Detailed description: This method is used to search global local messages.
    //
    // Business scenario: When there is a need to perform a global search of previous local messages based on keywords or other conditions, you can invoke this interface to search local messages by grouping them according to conversations.
    //
    // Restrictions: Effective after login, invalid after logout. Searching global is not supported in the room scene (conversationType=1).
    //
    // Related callbacks: [ZIMMessagesGlobalSearchedCallback].
    //
    // @param config Search global the configuration of local messages.
    // @param callback Callback of the search global local message result.
    virtual void searchGlobalLocalMessages(const ZIMMessageSearchConfig &config,
                                           ZIMMessagesGlobalSearchedCallback callback) = 0;

    // Search local conversations on local messages.
    //
    // Supported versions: 2.9.0 and above.
    //
    // Detailed description: This method is used for searching local conversations based on local messages.
    //
    // Business scenario: When there is a need to search conversations using keywords or other conditions, this API can be called for searching.
    //
    // Call timing/Notification timing: It should be invoked when there is a requirement to search conversations using keywords or other conditions.
    //
    // Restrictions: effective after logging in and becomes invalid after logging out. Searching is not supported in room scenarios (conversationType=1).
    //
    // Related callbacks: [ZIMConversationsSearchedCallback].
    //
    // @param config Global search conversation config.
    // @param callback Callback of the search conversations result.
    virtual void searchLocalConversations(const ZIMConversationSearchConfig &config,
                                          ZIMConversationsSearchedCallback callback) = 0;

    //Available sinces: 2.10.0 and above.
    //
    //Detail description: Message reaction refers to the user's response to a message. It can generally be used to add or remove emoticons for single chat or group chat messages, as well as to initiate group voting, confirm group results, and other operations.
    //
    //Use cases: Users need to express their position on a certain message, such as liking, and this method can be used to express their position.
    //
    //Note: Room message reaction is not supported.
    //
    //Restrictions: You can only use it after logging in. And only supports message reactions for single chat and group chat
    //
    //Related callbacks: If the addition is successful, the [onMessageReactionsChanged] callback will be triggered. If the reaction is made to the latest message in the conversation, the [onConversationChanged] callback will be triggered when the addition is successful.
    //
    // @param reactionType Type of reaction, defined by you, with a maximum length of 32 bytes.
    // @param message The message needs reaction.
    // @param callback Returns the result of adding message reaction.
    virtual void addMessageReaction(const std::string &reactionType,
                                    const std::shared_ptr<ZIMMessage> &message,
                                    ZIMMessageReactionAddedCallback callback) = 0;

    //Available sinces: 2.10.0 and above.
    //
    //Detail description: Delete the reaction made by the local user.
    //
    //Use cases: Users need to delete the status of a message that has already been stated, which can be done using this method.
    //
    //Note: Room message reaction is not supported.
    //
    //Restrictions: You can only use it after logging in. And only supports message statements for single chat and group chat
    //
    //Related callbacks: If the deletion is successful, the [onMessageReactionsChanged] callback will be triggered. If the reaction is deleted from the latest message in the conversation, the [onConversationChanged] callback will be triggered when the addition is successful.
    //
    // @param reactionType Reaction type. It must be the type of reaction made by the local user.
    // @param message The message needs reaction delete.
    // @param callback Returns the result of deleting reaction of the message.
    virtual void deleteMessageReaction(const std::string &reactionType,
                                       const std::shared_ptr<ZIMMessage> &message,
                                       ZIMMessageReactionDeletedCallback callback) = 0;

    //Available sinces: 2.10.0 and above.
    //
    //Detail description: This method is used to query specific user information under a certain state of a message.
    //
    //Use cases: When it is necessary to obtain specific user information under a certain state of a message, this interface can be called to query state user messages in a paginated manner.
    //
    //Restrictions: You can only use it after logging in. And only supports message statements for single chat and group chat
    //
    // @param message The message needs querying reaction user list.
    // @param config reaction user query config.
    // @param callback the result of querying reaction user list.
    virtual void
    queryMessageReactionUserList(const std::shared_ptr<ZIMMessage> &message,
                                 const ZIMMessageReactionUserQueryConfig &config,
                                 ZIMMessageReactionUserListQueriedCallback callback) = 0;
    // MARK: - Conversation

    // Available since: 2.8.0 and above.
    //
    // Description: This method displays the session list of the logged in user.
    //
    // Use cases: When you need to know the relevant information of the specified conversation, you can call this interface to obtain the data source.
    //
    // When to call /Trigger: Can be invoked after login.
    //
    // Restrictions:There is no limit to the frequency of use, available after login, unavailable after logout.
    //
    // Related callbacks: [ZIMConversationQueriedCallback].
    virtual void queryConversation(const std::string &conversationID,
                                   ZIMConversationType conversationType,
                                   ZIMConversationQueriedCallback callback) = 0;

    // Query the conversation list.
    //
    // Available since: 2.0.0 and above.
    //
    // Description: This method displays the session list of the logged in user.
    //
    // Use cases: This interface can be invoked to get the data source when you need to display an existing message session after logging in.
    //
    // When to call /Trigger: Can be invoked after login.
    //
    // Restrictions:There is no limit to the frequency of use, available after login, unavailable after logout.
    //
    // Caution: NextConversation is the riveting point of the query message, which can be null for the first query. In subsequent query, the earliest conversation can be used as nextConversation to query earlier sessions. In paging query, Count in [ZIMConversationQueryConfig] fill each pull the number of sessions.
    //
    // Related callbacks: [ZIMConversationListQueriedCallback].
    //
    // Related APIs: [deleteConversation] Deletes the session. [clearConversationUnreadMessageCount] clear session readings.
    // @param config Configuration for session queries.
    // @param callback Callback for session query.
    virtual void queryConversationList(const ZIMConversationQueryConfig &config,
                                       ZIMConversationListQueriedCallback callback) = 0;

    //Available since: 2.8.0 and above.
    //
    // Description: This method displays the pinned conversation list of the logged in user.
    //
    // Use cases: This interface can be invoked to get the data source when you need to display
    // an existing pinned message conversation after logging in.
    //
    // When to call /Trigger: Can be invoked after login.
    //
    // Restrictions:There is no limit to the frequency of use, available after login, unavailable after logout.
    //
    // Caution: NextConversation is the riveting point of the query message, which can be null for the first query.
    // In subsequent query, the earliest conversation can be used as nextConversation to query earlier sessions.
    // In paging query, Count in [ZIMConversationQueryConfig] fill each pull the number of sessions.

    virtual void queryConversationPinnedList(const ZIMConversationQueryConfig &config,
                                             ZIMConversationPinnedListQueriedCallback callback) = 0;
    // Clear session unreads.
    //
    // Available since: 2.0.0 and above.
    //
    // Description: Used to clear unread for the current user target session.
    //
    // Use cases: This interface is called when a chat page is entered from a session and the original message readings of the session need to be cleared.
    //
    // When to call /Trigger: Called when a target needs to be cleared without readings.
    //
    // Restrictions: Valid after login, invalid after logout.
    //
    // Impacts on other APIs: Calling this method will trigger a total readings not updated callback [conversationTotalUnreadMessageCountUpdated], would trigger a session to update callbacks [conversationChanged].
    //
    // Related callbacks:[ZIMConversationUnreadMessageCountClearedCallback].
    //
    // Related APIs:[conversationTotalUnreadMessageCountUpdated]、[conversationChanged].
    //
    // @param conversationID conversationID.
    // @param conversationType conversation type.
    // @param callback Callback to remove session unreads.

    virtual void clearConversationUnreadMessageCount(
        const std::string &conversationID, ZIMConversationType conversationType,
        ZIMConversationUnreadMessageCountClearedCallback callback) = 0;

    //Available since: 2.12.0 and above.
    //
    //Description: Used to clear unread for all sessions.
    //
    //Use cases: You can call this interface when you need to clear all unread sessions to zero.
    //
    //When to call /Trigger: Called when all session readings need to be cleared.
    //
    //Restrictions: Valid after login, invalid after logout.
    //
    //Impacts on other APIs: Calling this method will trigger a total readings not updated callback [onConversationTotalUnreadMessageCountUpdated].
    //
    //Related callbacks:[ZIMConversationTotalUnreadMessageCountClearedCallback].
    //
    //Related APIs:[onConversationTotalUnreadMessageCountUpdated].
    virtual void clearConversationTotalUnreadMessageCount(
        ZIMConversationTotalUnreadMessageCountClearedCallback callback) = 0;

    // Delete the conversation.
    //
    // Available since: 2.0.0 and above.
    //
    // Description: This interface is invoked when a session needs to be deleted. All members in the session can invoke this interface.
    //
    // Use cases: You can invoke this interface implementation to delete an entire session when it is no longer needed.
    //
    // When to call /Trigger: his parameter is invoked when a session needs to be deleted and can be invoked after a ZIM instance is created. The call takes effect after login and becomes invalid after logout.
    //
    // Impacts on other APIs: call success will trigger onConversationchanged callback, if the deleted session include unread message triggers [onConversationTotalUnreadMessageCountUpdated] callback.
    //
    // Related callbacks: [ZIMConversationDeletedCallback]
    //
    // @param conversationID conversationID.
    // @param conversationType conversation type.
    // @param config delete the session's configuration.
    // @param callback Callback to delete session.

    virtual void
    updateConversationPinnedState(bool isPinned, const std::string &conversationID,
                                  ZIMConversationType conversationType,
                                  ZIMConversationPinnedStateUpdatedCallback callback) = 0;

    // Available since: 2.0.0 and above.
    //
    // Description: This interface is invoked when a session needs to be deleted. All members in the session can invoke this interface.
    //
    // Use cases: You can invoke this interface implementation to delete an entire session when it is no longer needed.
    //
    // When to call /Trigger: his parameter is invoked when a session needs to be deleted and can be invoked after a ZIM instance is created. The call takes effect after login and becomes invalid after logout.
    //
    // Impacts on other APIs: call success will trigger onConversationchanged callback, if the deleted session include unread message triggers [onConversationTotalUnreadMessageCountUpdated] callback.
    //
    // Related callbacks: [ZIMConversationDeletedCallback]

    virtual void deleteConversation(const std::string &conversationID,
                                    ZIMConversationType conversationType,
                                    const ZIMConversationDeleteConfig &config,
                                    ZIMConversationDeletedCallback callback) = 0;

    //Available since: 2.12.0 and above.
    //
    //Description: This interface is invoked when all sessions needs to be deleted. All members in sessions can invoke this interface.
    //
    //Use cases: If you want to delete all sessions when they are no longer needed, you can call this interface implementation.
    //
    //When to call /Trigger: his parameter is invoked when sessions needs to be deleted and can be invoked after a ZIM instance is created. The call takes effect after login and becomes invalid after logout.
    //
    //Impacts on other APIs: If deleted sessions include unread message will trigger the [onConversationTotalUnreadMessageCountUpdated] callback, the call is successful at login, and the other end will trigger [onConversationsAllDeleted] callback.
    //
    //Related callbacks: [ZIMConversationsAllDeletedCallback]
    virtual void deleteAllConversations(const ZIMConversationDeleteConfig &config,
                                        ZIMConversationsAllDeletedCallback callback) = 0;

    // Set the conversation notification state.
    //
    // Available since: 2.0.0 and above.
    //
    // Description: This method enables DND by selecting whether the unread of the target session is updated when a message is received.
    //
    // Use cases: If the user selects MESSAGE DO not Disturb (DND), the user can call the corresponding method.
    //
    // Default value: Message DND is disabled by default.
    //
    // When to call /Trigger: If the target session exists after login, invoke this interface if you want to enable the DND status of the target session.
    //
    // Restrictions:  Valid after login, invalid after logout.
    //
    // Impacts on other APIs: After the DND state is enabled, receiving messages is not triggered [conversationTotalUnreadMessageCountUpdated].
    //
    // Related callbacks: [ZIMConversationNotificationStatusSetCallback].
    //
    // Related APIs: [conversationTotalUnreadMessageCountUpdated].
    //
    // @param status the session notification state.
    // @param conversationID conversationID.
    // @param conversationType conversation type.
    // @param callback Callback for setting session notification status..

    virtual void
    setConversationNotificationStatus(ZIMConversationNotificationStatus status,
                                      const std::string &conversationID,
                                      ZIMConversationType conversationType,
                                      ZIMConversationNotificationStatusSetCallback callback) = 0;

    // Set all received receipts of the conversation to be read.
    //
    // Available since: 2.5.0 and above.
    //
    // Description: Set all received receipts of the conversation to be read.
    //
    // Use cases: Set all received receipt messages in the entire conversation to be read, and the sender of the message receipt in the conversation will receive the [onConversationMessageReceiptChanged] callback from ZIMEventHandler.
    //
    // When to call /Trigger: It can be called after login. It is recommended to call before entering the message list page. In the message list page, it is recommended to call [sendMessageReceiptsRead] to batch set the messages that need to be read.
    //
    // Note: Only single chat conversation are allowed.
    //
    // Related callbacks: [ZIMConversationMessageReceiptReadSentCallback].
    //
    // Related APIs: [sendMessageReceiptsRead], [sendMessage].
    //
    // @param conversationID Conversation ID.
    // @param conversationType Conversation type, only Peer type is supported.
    // @param callback Set Conversation read callback.
    virtual void
    sendConversationMessageReceiptRead(const std::string &conversationID,
                                       ZIMConversationType conversationType,
                                       ZIMConversationMessageReceiptReadSentCallback callback) = 0;
    virtual void
    queryCombineMessageDetail(std::shared_ptr<ZIMCombineMessage> &message,
                              const ZIMCombineMessageDetailQueriedCallback &callback) = 0;

    // Available since: 2.14.0 and above.
    //
    // Description: When you need to set a draft for a session, call this interface, and members of the session can call this interface.
    //
    // Use cases: This interface can be invoked when you need to temporarily save the text message that the user is editing but has not yet sent.
    //
    // When to call /Trigger:  Call when you need to set session draft, call after creating ZIM instance, take effect after login, invalid after logout.
    //
    // Impacts on other APIs: A successful call triggers the [onConversationchanged] callback.
    //
    // Related callbacks: [ZIMConversationDraftSetCallback]
    // @param draft Drafts that need to be set.
    // @param conversationID Conversation ID.
    // @param conversationType Conversation type, only Peer type is supported.
    // @param callback Set Conversation draft callback.
    virtual void setConversationDraft(const std::string &draft, const std::string &conversationID,
                                      ZIMConversationType conversationType,
                                      ZIMConversationDraftSetCallback callback) = 0;

    // MARK: - Room

    // Create a room.
    //
    // Description: When a room is created, other users can join this room through [joinRoom] function.
    // When everyone leaves the room, the room will be automatically destroyed.
    //
    // @param roomInfo The configuration information of the room to be created.
    // @param callback Callback of the result of creating the room.
    virtual void createRoom(const ZIMRoomInfo &roomInfo, ZIMRoomCreatedCallback callback) = 0;

    // Create a room.
    //
    // Available since: 1.3.0.
    //
    // Description: When a room is created, other users can join this room through [joinRoom] function.
    //
    // Use cases: When you need to create a multi-person chat scene, you can create a room by this API.
    //
    // When to call: It can be called after creating a ZIM instance through [create].
    //
    // Caution: When everyone leaves the room, the room will be automatically destroyed.
    //
    // Related callbacks: The result of the room creation can be obtained through the [ZIMRoomCreatedCallback] callback.
    //
    // Related APIs: You can join the room through [joinRoom] and leave the room with [leaveRoom].
    //
    // @param roomInfo The configuration information of the room to be created.
    // @param config The advanced information of the room to be created.
    // @param callback Callback of the result of creating the room.
    virtual void createRoom(const ZIMRoomInfo &roomInfo, const ZIMRoomAdvancedConfig &config,
                            ZIMRoomCreatedCallback callback) = 0;

    // Join a room.
    //
    // Description: If the room does not exist, the join fails and you need to call [createRoom] to create the room first.
    // When everyone leaves the room, the room will be automatically destroyed.
    //
    // @param roomID ID of the room to join.
    // @param callback Callback of the result of joining the room.
    virtual void joinRoom(const std::string &roomID, ZIMRoomJoinedCallback callback) = 0;

    //Supported version: 2.1.0.
    //
    // Detail description : After calling this API, If the room already exists, join the room directly;
    // if the room does not exist, create a room and join. At the same time, if the room does not exist,
    // after calling this interface, the room advanced properties set by the user will take effect.
    //
    // Business scenario: When you need to enter a multi-person chat scene with custom attributes,
    // and you do not need to distinguish whether the room is created or added, you can enter a room through this interface.
    //
    // When to call: It can be called after logging in.
    //
    // Note: When everyone leaves the room, the room will be automatically destroyed,
    // and a user can be in a maximum of 5 rooms at the same time.
    // [enterRoom] is equivalent to [createRoom] and [joinRoom], so you only need to choose one of the APIs.
    //
    // Related callbacks: The result of entering the room can be obtained through the [onRoomEntered] callback.
    //
    // Related APIs: You can enter the room through [enterRoom], and leave the room through [leaveRoom].

    virtual void enterRoom(const ZIMRoomInfo &roomInfo, const ZIMRoomAdvancedConfig &config,
                           ZIMRoomEnteredCallback callback) = 0;

    // Leave a room.
    //
    // Description: If the current user is not in this room, the exit fails.
    // When everyone leaves the room, the room will be automatically destroyed.
    //
    // @param roomID ID of the room to leave.
    // @param callback Callback of the result of joining the room.
    virtual void leaveRoom(const std::string &roomID, ZIMRoomLeftCallback callback) = 0;

    // Query the list of members in the room.
    //
    // Description: After joining a room, you can use this function to get the list of members in the room.
    //
    // @param roomID ID of the room to query.
    // @param config Configuration of query room member operation.
    // @param callback Callback for the result of querying room members list.
    virtual void queryRoomMemberList(const std::string &roomID,
                                     const ZIMRoomMemberQueryConfig &config,
                                     ZIMRoomMemberQueriedCallback callback) = 0;

    // Available since: 2.8.0 and above.
    //
    // Description: This method can query the information of up to ten users in the specified room of the logged-in user.
    //
    // Use cases: When you need to know the user information in the specified room, you can call this interface to obtain the data source.
    //
    // When to call /Trigger: Can be invoked after login.
    //
    // Restrictions: Available after login, unavailable after logout, up to ten users can be queried at one time.

    virtual void queryRoomMembers(const std::vector<std::string> &userIDs,
                                  const std::string &roomID,
                                  ZIMRoomMembersQueriedCallback callback) = 0;

    // Query the number of online members in the room.
    //
    // Description: After joining a room, you can use this function to get the number of online members in the room.
    //
    // @param roomID ID of the room to query.
    // @param callback Callback for the result of querying room online members count.
    virtual void queryRoomOnlineMemberCount(const std::string &roomID,
                                            ZIMRoomOnlineMemberCountQueriedCallback callback) = 0;

    // Set room attributes (use this for all additions and changes).
    //
    // Available since: 1.3.0.
    //
    // Description: Used to set room properties.
    //
    // @param roomAttributes room attributes will be set.
    // @param roomID ID of the room to set.
    // @param config config of the room to set.
    // @param callback Callback for the result of setting room attributes.
    virtual void
    setRoomAttributes(const std::unordered_map<std::string, std::string> &roomAttributes,
                      const std::string &roomID, ZIMRoomAttributesSetConfig *config,
                      ZIMRoomAttributesOperatedCallback callback) = 0;

    // Delete room attributes.
    //
    // Available since: 1.3.0.
    //
    // Description: Used to delete room attributes.
    //
    // @param keys room attributes keys will be deleted.
    // @param roomID ID of the room to deleted.
    // @param config config of the room to deleted.
    // @param callback Callback for the result of operation room attributes.
    virtual void deleteRoomAttributes(const std::vector<std::string> &keys,
                                      const std::string &roomID,
                                      ZIMRoomAttributesDeleteConfig *config,
                                      ZIMRoomAttributesOperatedCallback callback) = 0;

    // Open combination room attribute operation.
    //
    // Available since: 1.3.0.
    //
    // Description: Used to turn on the combination of room attributes.
    //
    // @param roomID ID of the room to operation.
    // @param config config of the room to turn on the combination of room attributes.
    virtual void
    beginRoomAttributesBatchOperation(const std::string &roomID,
                                      ZIMRoomAttributesBatchOperationConfig *config) = 0;

    // Complete the property operation of the combined room.
    //
    // Available since: 1.3.0.
    //
    // Description: After completing the operation of combining room attributes,
    // all the setting/deleting operations from the last call to beginRoomAttributesBatchOperation
    // to this operation will be completed for the room.
    //
    // @param roomID ID of the room to operation.
    // @param callback Callback for the result of operation combination of room attributes.
    virtual void
    endRoomAttributesBatchOperation(const std::string &roomID,
                                    ZIMRoomAttributesBatchOperatedCallback callback) = 0;

    // Query all properties of the room.
    //
    // Available since: 1.3.0.
    //
    // Used to query room attributes.
    //
    // @param roomID ID of the room to queried.
    // @param callback Callback for the result of quering room attributes.
    virtual void queryRoomAllAttributes(const std::string &roomID,
                                        ZIMRoomAttributesQueriedCallback callback) = 0;

    // Supported Versions: 2.4.0 and above.

    // Detail description: Call this API to set room user properties of members in the room.

    // Business scenario: If you need to set a level for members in the room, you can use this interface to set a state.

    // Default: [ZIMRoomMemberAttributesSetConfig] Default constructor isDeleteAfterOwnerLeft is true.

    // Call timing/Notification timing: After logging in and calling in the relevant room.

    // Usage limit: A maximum of 500 user attributes can be set in each room and stored in the key-value mode. If you need to increase the attribute limit, please contact ZEGO technical support. The total length of user attribute key-values owned by each user in a room cannot exceed 144 bytes, and the number of key-values cannot exceed 30 pairs. The length of a single key-value cannot exceed 8 bytes for a Key and 64 bytes for a Value. If you need to raise the cap, please contact ZEGO technical support. After the room is destroyed, the user-defined user properties are also destroyed.

    // Relevant callback: [ZIMRoomMembersAttributesOperatedCallback],[onRoomMemberAttributesUpdated].

    // Related interfaces: [queryRoomMembersAttributes], [queryRoomMemberAttributesList].
    // @param attributes Room member attributes to be set.
    // @param userIDs A list of userIDs to set.
    // @param roomID Room ID.
    // @param config Behavior configuration of the operation.
    // @param callback Operation result for setting room members attributes.
    virtual void
    setRoomMembersAttributes(const std::unordered_map<std::string, std::string> &attributes,
                             const std::vector<std::string> &userIDs, const std::string &roomID,
                             const ZIMRoomMemberAttributesSetConfig &config,
                             ZIMRoomMembersAttributesOperatedCallback callback) = 0;

    // Available since:2.4.0 or later.

    // Description:Call this API to batch query the room user attributes of the members in the room.

    // Use cases:Use this interface when you need to specify that you want to query some room users.

    // Restrictions:The maximum call frequency is 5 times within 30 seconds by default, and the maximum query time is 100 people.

    // Related callbacks:[ZIMRoomMembersAttributesQueriedCallback].

    // Related APIs: [setRoomMembersAttributes]、[queryRoomMemberAttributesList].

    // Runtime lifecycle: It is available after logging in and joining the corresponding room, but unavailable after leaving the corresponding room.
    // @param userIDs A list of userIDs to query.
    // @param roomID Room ID.
    // @param callback Callback for the result of batch query of room user attributes.
    virtual void queryRoomMembersAttributes(const std::vector<std::string> &userIDs,
                                            const std::string &roomID,
                                            ZIMRoomMembersAttributesQueriedCallback callback) = 0;

    // Available since:2.4.0 or later.

    // Description:paginate the room user properties that have room property members in the room.

    // Use cases:This interface is used when you need to query all room users.

    // Restrictions:The maximum call frequency is 5 times within 30 seconds by default, and the maximum query time is 100 people.

    // Related callbacks:[ZIMRoomMemberAttributesListQueriedCallback].

    // Related APIs: [setRoomMembersAttributes]、[queryRoomMembersAttributes].

    // Runtime lifecycle: It is available after logging in and joining the corresponding room, but unavailable after leaving the corresponding room.
    // @param roomID Room ID.
    // @param config Behavior configuration of the operation.
    // @param callback Result callback for querying member attributes in the room.
    virtual void
    queryRoomMemberAttributesList(const std::string &roomID,
                                  const ZIMRoomMemberAttributesQueryConfig &config,
                                  ZIMRoomMemberAttributesListQueriedCallback callback) = 0;
    // MARK: - Group

    // Create group.
    //
    // Available since: 2.0.0 and above.
    //
    // Description: You can call this interface to create a group, and the person who calls this interface is the group leader. An empty string if the group name is left blank.
    //
    // Use cases: You can use this interface to create a chat scenario and join a group.
    //
    // When to call: After you create a ZIM instance with [create] and login with [login].
    //
    // Restrictions: Available after login, unavailable after logout. UserIDs support a maximum of 100 users and a group supports a maximum of 500 users.
    //
    // Impacts on other APIs: You can use [joinGroup] to join a group, [leaveGroup] to leave a group, or [dismissGroup] to dismiss a group.
    //
    // Related callbacks: The result of creating the group is obtained through the [ZIMGroupCreatedCallback] callback.
    //
    // @param groupInfo Configuration information for the group to be created.
    // @param userIDs List of users invited to the group.
    // @param callback Callback for the result of creating a group.
    virtual void createGroup(const ZIMGroupInfo &groupInfo, const std::vector<std::string> &userIDs,
                             ZIMGroupCreatedCallback callback) = 0;

    // Create a group with the andvanced info such as group attributes and group notice.
    //
    // Available since: 2.0.0 and above.
    //
    // Description: You can call this interface to create a group, and the person who calls this interface is the group leader. An empty string if the group name is left blank.
    //
    // Use cases: You can use this interface to create a chat scenario and join a group.
    //
    // When to call: After you create a ZIM instance with [create] and login with [login].
    //
    // Restrictions: Available after login, unavailable after logout. UserIDs support a maximum of 100 users and a group supports a maximum of 500 users.
    //
    // Impacts on other APIs: You can use [joinGroup] to join a group, [leaveGroup] to leave a group, or [dismissGroup] to dismiss a group.
    //
    // Related callbacks: The result of creating the group is obtained through the [ZIMGroupCreatedCallback] callback.
    // @param groupInfo Configuration information for the group to be created..
    // @param userIDs List of users invited to the group.
    // @param config advanced config.
    // @param callback Callback for the result of creating a group.

    virtual void createGroup(const ZIMGroupInfo &groupInfo, const std::vector<std::string> &userIDs,
                             const ZIMGroupAdvancedConfig &config,
                             ZIMGroupCreatedCallback callback) = 0;

    // Disband the group.
    //
    // Available since: 2.0.0 and above.
    //
    // Description: When a group is created, you can use [dismissGroup] to dismiss it.
    //
    // Use cases: After you create a chat group, you do not need to use this interface to dissolve the group.
    //
    // When to call /Trigger: This parameter can be called after a group is created by using [createGroup].
    //
    // Caution: A non-group owner cannot dissolve a group.
    //
    // Impacts on other APIs: Through callback can get [ZIMGroupDismissedCallback] dissolution results of the room, through [onGroupStateChanged] listen callback can get the room status.
    //
    // Related callbacks: You can use [createGroup] to create a group, [joinGroup] to join a group, and [leaveGroup] to leave a group.
    // @param groupID The ID of the group to be disbanded.
    //  @param callback Callback for the result of disbanding the group.

    virtual void dismissGroup(const std::string &groupID, ZIMGroupDismissedCallback callback) = 0;

    // Join a group.
    //
    // Available since: 2.0.0 and above.
    //
    // Description: After a group is created, other users can use [joinGroup] to join the group.
    //
    // Use cases: This interface is used to join a group in a chat scenario.
    //
    // When to call /Trigger: The ZIM instance can be invoked after being created by [create] and logged in.
    //
    // Caution: Available after login, unavailable after logout. If you have joined a group, the join succeeds. A group is limited to 500 people and fails to join when it is full.
    //
    // Related callbacks: To get the result of joining the room, call [ZIMGroupJoinedCallback].
    //
    // Related APIs: You can use [createGroup] to create a group, [leaveGroup] to leave a group, or [dismissGroup] to dismiss a group.
    //
    // @param groupID The group ID to join.
    // @param callback Callback for the result of joining the group.
    virtual void joinGroup(const std::string &groupID, ZIMGroupJoinedCallback callback) = 0;

    // Leave the group.
    //
    // Available since: 2.0.0 and above.
    //
    // Description: After a user joins a group, the user can leave the group through this interface.
    //
    // Use cases: This interface is used to exit a chat group.
    //
    // When to call /Trigger: It can be invoked after a ZIM instance is created through [create] and logged in.
    //
    // Restrictions: Available after login, unavailable after logout.
    //
    // Caution: When the group owner quits the group, the identity of the group owner will be automatically transferred to the earliest member who joined the group. When all members exit the group, the group is automatically dissolved.
    //
    // Impacts on other APIs: You can use [createGroup] to create a group, [joinGroup] to join a group, or [dismissGroup] to dismiss a group.
    //
    // Related callbacks: The result of leaving the room can be obtained by the [ZIMGroupLeftCallback] callback.
    // @param groupID The group ID to leave..
    // @param callback Callback for the result of leaving the group.

    virtual void leaveGroup(const std::string &groupID, ZIMGroupLeftCallback callback) = 0;

    // Invite users into the group.
    //
    // Available since: 2.0.0 and above.
    //
    // Description: After a group is created, users can add multiple users to the group through this interface. The interface can be invoked by both the master and members of the group.
    //
    // Use cases: This interface allows you to invite others to join a group chat.
    //
    // When to call /Trigger: The ZIM instance can be invoked after being created by [create] and logged in.
    //
    // Restrictions: The maximum number of userIDs users can join the group is 100. If the number of users reaches 100, the interface callback will notify the user. The maximum number of people in a group is 500.
    //
    // Caution: This interface does not require the peer's consent or the peer's online status. The service layer determines the number of invited users.
    //
    // Related callbacks: Through the callback [ZIMGroupUserInvitedCallback] can add multiple users into the group's results.
    //
    // Related APIs: KickGroupMember can be used to kick a target user out of the group.
    //
    // @param userIDs List of invited users.
    // @param groupID The ID of the group that will invite users to the group.
    // @param callback Callback for the result of inviting the user to the group.

    virtual void inviteUsersIntoGroup(const std::vector<std::string> &userIDs,
                                      const std::string &groupID,
                                      ZIMGroupUsersInvitedCallback callback) = 0;

    // kick some members out of the group.
    //
    // Available since: 2.0.0 and above.
    //
    // Description: After a user joins a group, you can use this method to remove the user from the group.
    //
    // Use cases: You can use this method to remove one or more users from the group.
    //
    // When to call /Trigger: The ZIM instance can be invoked after being created by [create] and logged in.
    //
    // Restrictions: You can't kick someone unless you're the leader of the group.
    //
    // Caution: This interface does not require the peer's consent or the peer's online status. It cannot accept group-related callbacks after being kicked out. History messages and sessions remain after being kicked out and can still enter the group.
    //
    // Related callbacks: Through the callback [ZIMGroupMemberKickedCallback] can get the user kicked out the results of the group.
    //
    // Related APIs: You can invite a target user into a group through [inviteUsersIntoGroup].
    //
    // @param userIDs List of users who have been kicked out of the group.
    // @param groupID The group ID of the member who will be kicked out.
    // @param callback Callback for the result of being kicked out of the group.

    virtual void kickGroupMembers(const std::vector<std::string> &userIDs,
                                  const std::string &groupID,
                                  ZIMGroupMemberKickedCallback callback) = 0;

    // Transfer the group owner.
    //
    // Available since: 2.0.0 and above.
    //
    // Description: After a group is created, the group owner can use this method to assign the group owner to a specified user.
    //
    // Use cases: In a group chat scenario, you can transfer the group master through this interface.
    //
    // When to call /Trigger: The ZIM instance can be invoked after being created by [create] and logged in.
    //
    // Restrictions: You cannot transfer a group owner if you are not a group owner.
    //
    // Related APIs: Through the callback [ZIMGroupOwnerTransferredCallback] can get the result of the transfer of the group manager.
    // @param toUserID The converted group owner ID.
    // @param groupID The group ID of the group owner to be replaced.
    // @param callback The callback of the transfer group owner.

    virtual void transferGroupOwner(const std::string &toUserID, const std::string &groupID,
                                    ZIMGroupOwnerTransferredCallback callback) = 0;

    // Update the group name.
    //
    // Available since: 2.0.0 and above.
    //
    // Description: After a group is created, users can call this method to change the group name.
    //
    // Use cases: After creating a group, you need to change the group name.
    //
    // When to call /Trigger: The ZIM instance can be invoked after being created by [create] and logged in.
    //
    // Restrictions: Group members and group owners can change the group name. The maximum length of the name is 100 bytes.
    //
    // Related APIs: Through the callback [ZIMGroupNameUpdatedCallback] can get the result of the change of name, through [onGroupNoticeUpdated] can get update group name information.
    //
    // @param groupName The updated group name.
    // @param groupID The group ID whose group name will be updated.
    // @param callback Callback for the result of updating the group name.

    virtual void updateGroupName(const std::string &groupName, const std::string &groupID,
                                 ZIMGroupNameUpdatedCallback callback) = 0;

    // Supported versions: 2.3.0 and above.
    //
    // Detail description: After a group is created, the user can modify the group avatar URL by calling this method.
    //
    // Business scenario: After creating a group, the user needs to change the group avatar URL.
    //
    // Invocation timing/notification timing: It can be invoked after creating a ZIM instance through [create] and logging in.
    //
    // Usage restrictions: Group members and group owners can modify the group avatar, with a maximum length of 500 bytes.
    //
    // Related callbacks: The result of changing the group name can be obtained through the [ZIMGroupAvatarUrlUpdatedCallback] callback, and the updated group avatar information can be obtained through the [onGroupAvatarUrlUpdated] callback.
    virtual void updateGroupAvatarUrl(const std::string &groupAvatarUrl, const std::string &groupID,
                                      ZIMGroupAvatarUrlUpdatedCallback callback) = 0;

    // Update group announcements.
    //
    // Available since: 2.0.0 and above.
    //
    // Description: When a group is created, users can use this method to update the group bulletin.
    //
    // Use cases: You need to update the group bulletin in the group.
    //
    // When to call /Trigger: The ZIM instance can be invoked after being created by [create] and logged in.
    //
    // Restrictions: Only group members can update the group bulletin. The maximum number of bytes is 300. There is no special character limit.
    //
    // Related callbacks: Through callback [ZIMGroupNoticeUpdateCallback] can get update group of the results announcement, announcement by [onGroupNoticeUpdated] can get update group information.
    //
    // @param groupNotice Pre-updated group announcements.
    // @param groupID The group ID of the group announcement that will be updated.
    // @param callback Callback to update the results of group announcements.

    virtual void updateGroupNotice(const std::string &groupNotice, const std::string &groupID,
                                   ZIMGroupNoticeUpdatedCallback callback) = 0;

    virtual void muteGroup(bool isMute, const std::string &groupID,
                           const ZIMGroupMuteConfig &config, ZIMGroupMutedCallback callback) = 0;

    virtual void muteGroupMembers(bool isMute, const std::vector<std::string> &userIDs,
                                  const std::string &groupID,
                                  const ZIMGroupMemberMuteConfig &config,
                                  ZIMGroupMembersMutedCallback callback) = 0;

    virtual void queryGroupMemberMutedList(const std::string &groupID,
                                           const ZIMGroupMemberMutedListQueryConfig &config,
                                           ZIMGroupMemberMutedListQueriedCallback callback) = 0;

    // Query group information.
    //
    // Available since: 2.0.0 and above.
    //
    // Description: Query information about a created group.
    //
    // Use cases: You need to obtain group information for display.
    //
    // When to call /Trigger: The ZIM instance can be invoked after being created by [create] and logged in.
    //
    // Related callbacks: Through the callback [ZIMGroupInfoQueriedCallback] can query the result of the group information.
    //
    // @param groupID The group ID of the group information to be queried.
    // @param callback Callback for the result of querying group information.
    virtual void queryGroupInfo(const std::string &groupID,
                                ZIMGroupInfoQueriedCallback callback) = 0;

    // Add or update group attributes.
    //
    // Available since: 2.0.0 and above.
    //
    // Description: If a group already exists, all users of the group can use this method to set group properties.
    //
    // Use cases: Added extended field information about group description, such as group family, label, and industry category.
    //
    // When to call /Trigger: The ZIM instance can be invoked after being created by [create] and logged in.
    //
    // Restrictions: Only group members can set group properties.
    //
    // Related callbacks: Through the callback [ZIMGroupAttributesOperatedCallback] can get the result of the set of properties.
    //
    // Related APIs: [deleteGroupAttributes] can be used to deleteGroupAttributes, [queryGroupAttributes] can be used to queryGroupAttributes, [queryAllGroupAttributes] can be used to queryAllGroupAttributes.
    // @param groupAttributes group properties.
    // @param groupID The groupID of the group attribute being set.
    // @param callback Callback for setting group properties.

    virtual void
    setGroupAttributes(const std::unordered_map<std::string, std::string> &groupAttributes,
                       const std::string &groupID, ZIMGroupAttributesOperatedCallback callback) = 0;

    // Delete group attributes.
    //
    // Available since: 2.0.0 and above.
    //
    // Description: When a group already exists, you can use this method to delete group attributes. Both the master and members of the interface group can be invoked.
    //
    // Use cases: Deleted the extended field of the group description.
    //
    // When to call /Trigger: The ZIM instance can be invoked after being created by [create] and logged in.
    //
    // Restrictions: Only group members can delete group attributes.
    //
    // Related callbacks: Through the callback [ZIMGroupAttributesOperatedCallback] can delete the result of the group of attributes.
    //
    // Related APIs: You can use [setGroupAttributes] to setGroupAttributes, [queryGroupAttributes] to queryGroupAttributes, and [queryAllGroupAttributes] to queryAllGroupAttributes.
    // @param keys The key of the group attribute to delete.
    // @param groupID The group ID of the group attribute to be deleted.
    // @param callback Callback for the result of removing the group property.

    virtual void deleteGroupAttributes(const std::vector<std::string> &keys,
                                       const std::string &groupID,
                                       ZIMGroupAttributesOperatedCallback callback) = 0;
    //  Query group attributes by keys.
    //
    // Available since: 2.0.0 and above.
    //
    // Description: After a group is created, you can use this method to query the specified group properties.
    //
    // Use cases: You need to query the scenarios to display the specified group attributes.
    //
    // When to call /Trigger: After creating a ZIM instance with [create] and logging in with [login].
    //
    // Restrictions: Available after login, unavailable after logout.
    //
    // Related callbacks: Through the callback [ZIMGroupAttributesQuriedCallback] can get query attributes of the specified group of results.
    //
    // Related APIs: [queryAllGroupAttributes] Queries all group attributes.
    // @param keys The group ID of the group attribute to be queried.
    // @param groupID The group ID of the group attribute to be queried.
    // @param callback Callback for the result of querying group properties.

    virtual void queryGroupAttributes(const std::vector<std::string> &keys,
                                      const std::string &groupID,
                                      ZIMGroupAttributesQueriedCallback callback) = 0;

    // Query all attributes of the group.
    //
    // Available since: 2.0.0 and above.
    //
    // Description: After a group is created, you can use this method to query all group attributes.
    //
    // Use cases: Scenarios where all group attributes need to be queried.
    //
    // When to call /Trigger: The ZIM instance can be invoked after being created by [create] and logged in.
    //
    // Related callbacks: Through callback can get query [ZIMGroupAttributesQuriedCallback] all the results of the group of attributes.
    //
    // Related APIs: [queryGroupAttributes] Queries the attributes of the specified group.
    // @param groupID The group ID of all group attributes to be queried.
    // @param callback Callback for querying the result of all attributes of the group.

    virtual void queryGroupAllAttributes(const std::string &groupID,
                                         ZIMGroupAttributesQueriedCallback callback) = 0;

    // Set the group member role.
    //
    // Available since: 2.0.0 and above.
    //
    // Description: After a group is created, you can use this method to set the roles of group members.
    //
    // Use cases: The ZIM instance can be invoked after being created by [create] and logged in.
    //
    // When to call /Trigger: If the primary role of a group is 1 and the default role of other members is 3, you can invoke this interface to change the role.
    //
    // Caution: Non-group master unavailable.
    //
    // Related callbacks: Through the callback [ZIMGroupMemberRoleUpdatedCallback] can be set up to get the results of the group of members of the role.
    // @param role Set of group roles.
    // @param forUserID User ID for which group role is set.
    // @param groupID The group ID of the group role to be set.
    // @param callback Callback to set the result of the group member role.

    virtual void setGroupMemberRole(ZIMGroupMemberRole role, const std::string &forUserID,
                                    const std::string &groupID,
                                    ZIMGroupMemberRoleUpdatedCallback callback) = 0;

    // Set group member nickname.
    //
    // Available since: 2.0.0 and above.
    //
    // Description: After a group is created, you can use this method to set nicknames for group members.
    //
    // Use cases: Nicknames need to be set for group members.
    //
    // When to call /Trigger: The ZIM instance can be invoked after being created by [create] and logged in.
    //
    // Restrictions: Available after login, unavailable after logout. The owner of a group can change his or her own nickname, while the members can change only their own nickname.
    //
    // Caution: A group name can contain a maximum of 100 characters.
    //
    // Related callbacks: Through the callback [ZIMGroupMemberNicknameUpdatedCallback] can be set up to get the results of the group members nickname.
    // @param nickname Set member nickname.
    // @param forUserID User ID for which group nickname is set.
    // @param groupID The group ID of the group member's nickname is set.
    // @param callback Callback for the result of setting the group member's nickname.

    virtual void setGroupMemberNickname(const std::string &nickname, const std::string &forUserID,
                                        const std::string &groupID,
                                        ZIMGroupMemberNicknameUpdatedCallback callback) = 0;

    // Query group member information.
    //
    // Available since: 2.0.0 and above.
    //
    // Description: After a group is created, you can use this method to query information about a specified group member.
    //
    // Use cases: You need to obtain the specified group member information for display or interaction.
    //
    // When to call /Trigger: The ZIM instance can be invoked after being created by [create] and logged in.
    //
    // Restrictions: Available after login, unavailable after logout.
    //
    // Related callbacks: Through the callback [ZIMGroupMemberInfoQueriedCallback] can get the query specifies the result of group membership information.
    // @param userID User ID of the queried member information.
    // @param groupID The ID of the group whose member information will be queried.
    // @param callback Callback for the result of querying group member information.

    virtual void queryGroupMemberInfo(const std::string &userID, const std::string &groupID,
                                      ZIMGroupMemberInfoQueriedCallback callback) = 0;

    // Query group list.
    //
    // Available since: 2.0.0 and above.
    //
    // Description: Query the list of all groups.
    //
    // Use cases: You need to get a list of groups to display.
    //
    // When to call /Trigger: The ZIM instance can be invoked after being created by [create] and logged in.
    //
    // Restrictions: Available after login, unavailable after logout.
    //
    // Related callbacks: Through the callback [ZIMGroupMemberListQueiedCallback] can get a check list of all current group results.
    // @param callback Callback for querying the result of the group list.
    virtual void queryGroupList(ZIMGroupListQueriedCallback callback) = 0;

    // Query the list of group members.
    //
    // Available since: 2.0.0 and above.
    //
    // Description: After a group is created, you can use this method to query the group member list.
    //
    // Use cases: You need to obtain the specified group member list for display or interaction.
    //
    // When to call /Trigger: The ZIM instance can be invoked after being created by [create] and logged in.
    //
    // Restrictions: Available after login, unavailable after logout.
    //
    // Related callbacks: Through the callback [ZIMGroupMemberListQueriedCallback] can query the result of the group member list.
    // @param groupID The group ID of the group member list to be queried.
    // @param config Group member query configuration.
    // @param callback Callback for querying the list of group members.
    virtual void queryGroupMemberList(const std::string &groupID,
                                      const ZIMGroupMemberQueryConfig &config,
                                      ZIMGroupMemberListQueriedCallback callback) = 0;

    // Available since: 2.2.0 or above.
    //
    // Description: Query the number of group members in a group.
    //
    // When to call: The ZIM instance can be invoked after being created by [create] and logged in.
    //
    // Restrictions: This function can only query the group that the user has entered.
    //
    // Related callbacks: [ZIMGroupMemberCountQueriedCallback].
    //
    // @param groupID The group ID of the group to be queried.
    // @param callback Callback for querying the number of groups.
    virtual void queryGroupMemberCount(const std::string &groupID,
                                       ZIMGroupMemberCountQueriedCallback callback) = 0;

    // Query the specific read member list of a message sent by a group.
    //
    // Available since: 2.5.0 and above.
    //
    // Description: This method can query the specific read member list of a message sent by a group.
    //
    // Use cases: Developers can use this method to query the specific read member list of a message they send.
    //
    // When to call: Callable after login.
    //
    // Restrictions: only supports querying the messages sent by the local end, and the receipt status of the messages is not NONE and UNKNOWN. If the user is not in the group, or has been kicked out of the group, the corresponding member list cannot be found.
    //
    // Related callbacks: [ZIMGroupMessageReceiptMemberListQueriedCallback].
    //
    // @param message The messages to be queried
    // @param groupID The group ID.
    // @param config The query configuration
    // @param callback Result of the specific read member list.
    virtual void queryGroupMessageReceiptReadMemberList(
        std::shared_ptr<ZIMMessage> message, const std::string &groupID,
        const ZIMGroupMessageReceiptMemberQueryConfig &config,
        ZIMGroupMessageReceiptMemberListQueriedCallback callback) = 0;

    // Query the specific unread member list of a message sent by a group.
    //
    // Available since: 2.5.0 and above.
    //
    // Description: This method can query the specific unread member list of a message sent by a group.
    //
    // Use cases: Developers can use this method to query the specific unread member list of a message they send.
    //
    // When to call: Callable after login.
    //
    // Restrictions: only supports querying the messages sent by the local end, and the receipt status of the messages is not NONE and UNKNOWN. If the user is not in the group, or has been kicked out of the group, the corresponding member list cannot be found.
    //
    // Related callbacks: [ZIMGroupMessageReceiptMemberListQueriedCallback].
    //
    // @param message The messages to be queried
    // @param groupID The group ID.
    // @param config The query configuration
    // @param callback Result of the specific unread member list.
    virtual void queryGroupMessageReceiptUnreadMemberList(
        std::shared_ptr<ZIMMessage> message, const std::string &groupID,
        const ZIMGroupMessageReceiptMemberQueryConfig &config,
        ZIMGroupMessageReceiptMemberListQueriedCallback callback) = 0;

    // Search local group members.
    //
    // Supported versions: 2.9.0 and above.
    //
    // Detailed description: This method is used to search for local groups.
    //
    // Business scenario: When you need to search for local groups by keywords or other criteria, you can call this interface to search.
    //
    // Call timing/Notification timing: After calling [login]
    //
    // Restrictions: Takes effect after login, becomes invalid after logout.
    //
    // @param config Configuration for searching groups.
    // @param callback Callback of the search groups result.
    virtual void searchLocalGroups(const ZIMGroupSearchConfig &config,
                                   ZIMGroupsSearchedCallback callback) = 0;

    // Search local group members.
    //
    // Supported versions: 2.9.0 and above.
    //
    // Detailed description: This method is used to search for group members.
    //
    // Business scenario: When you need to search for local group members by keywords or other criteria, you can call this interface to search.
    //
    // Restrictions: Takes effect after login, becomes invalid after logout.
    //
    // Related callbacks: [ZIMGroupMembersSearchedCallback].
    //
    // @param groupID Group ID of the joined group.
    // @param config The configuration for searching group members.
    // @param callback The configuration for searching group members.
    virtual void searchLocalGroupMembers(const std::string &groupID,
                                         const ZIMGroupMemberSearchConfig &config,
                                         ZIMGroupMembersSearchedCallback callback) = 0;

    // MARK: - CallInvite

    // Initiate a call invitation.
    //
    // Supported versions: 2.0.0 and above.
    //
    // Detail description: When the caller initiates a call invitation, the called party can use [callAccept] to accept the call invitation or [callReject] to reject the invitation.
    //
    // Business scenario: When you need to initiate a call invitation, you can create a unique callid through this interface to maintain this call invitation.
    //
    // When to call: It can be called after creating a ZIM instance through [create].
    //
    // Note: The call invitation has a timeout period, and the call invitation will end when the timeout period expires.
    // @param invitees list of invitees.
    // @param config Call Invitation Related Configuration.
    // @param callback Callback for initiating a call invitation.
    virtual void callInvite(const std::vector<std::string> &invitees,
                            const ZIMCallInviteConfig &config,
                            ZIMCallInvitationSentCallback callback) = 0;

    virtual void callingInvite(const std::vector<std::string> &invitees, const std::string &callID,
                               const ZIMCallingInviteConfig &config,
                               ZIMCallingInvitationSentCallback callback) = 0;

    virtual void callJoin(const std::string &callID, const ZIMCallJoinConfig &config,
                          ZIMCallJoinSentCallback callback) = 0;

    // Cancel the call invitation.
    //
    // Supported versions: 2.0.0 and above.
    //
    // Detail description: After the caller initiates a call invitation, the call invitation can be canceled through this interface before the timeout period.
    //
    // Business scenario: When you need to cancel the call invitation initiated before, you can cancel the call invitation through this interface.
    //
    // When to call: It can be called after creating a ZIM instance through [create].
    //
    // Note: Canceling the call invitation after the timeout period of the call invitation expires will fail.
    // @param invitees list of invitees.
    // @param callID The ID of the call invitation to cancel.
    // @param config Cancel the related configuration of call invitation.
    // @param callback Callback for canceling a call invitation.

    virtual void callCancel(const std::vector<std::string> &invitees, const std::string &callID,
                            ZIMCallCancelConfig config, ZIMCallCancelSentCallback callback) = 0;

    // Accept the call invitation.
    //
    // Supported versions: 2.0.0 and above.
    //
    // Detail description: When the calling party initiates a call invitation, the called party can accept the call invitation through this interface.
    //
    // Service scenario: When you need to accept the call invitation initiated earlier, you can accept the call invitation through this interface.
    //
    // When to call: It can be called after creating a ZIM instance through [create].
    //
    // Note: The callee will fail to accept an uninvited callid.
    // @param callID The call invitation ID to accept.
    // @param config Configuration related to accepting call invitations.
    // @param callback Callback to accept call invitation.

    virtual void callAccept(const std::string &callID, const ZIMCallAcceptConfig &config,
                            ZIMCallAcceptanceSentCallback callback) = 0;

    // Reject the call invitation.
    //
    // Supported versions: 2.0.0 and above.
    //
    // Detail description: When the calling party initiates a call invitation, the called party can reject the call invitation through this interface.
    //
    // Service scenario: When you need to reject the call invitation initiated earlier, you can use this interface to reject the call invitation.
    //
    // When to call: It can be called after creating a ZIM instance through [create].
    //
    // Note: The callee will fail to reject the uninvited callid.
    // @param callID The ID of the call invitation to be rejected.
    // @param config Related configuration for rejecting call invitations.
    // @param callback Callback for rejecting call invitations.
    virtual void callReject(const std::string &callID, const ZIMCallRejectConfig &config,
                            ZIMCallRejectionSentCallback callback) = 0;

    /// Supported versions: 2.9.0 and above.
    ///
    /// Detail description: When the Lord's call initiates the invitation and is called and accepts it, the current call can be exited through the interface.
    ///
    /// When to call: It can be called after creating a ZIM instance through [create].
    ///
    /// Note: The callee will fail to quit the uninvited callid.
    ///
    /// Related callbacks: [ZIMCallQuitSentCallback].
    /// @param callID  The ID of the call invitation to be quit.
    /// @param config Related configuration for quit call invitations.
    /// @param callback Callback for quit call invitations.
    virtual void callQuit(const std::string &callID, const ZIMCallQuitConfig &config,
                          ZIMCallQuitSentCallback callback) = 0;

    /// Supported versions: 2.9.0 and above.
    ///
    /// Detail description: End The call in advanced mode.
    ///
    /// When to call: The call was in advanced mode and the user status was Accepted.
    ///
    /// Note: User calls that are not in the call will fail. ZupdatIM SDK Does not have service logic after the call ends, and developers can customize the development logic after the end.
    ///
    /// Related callbacks: [ZIMCallEndSentCallback].
    /// @param callID The ID of the call invitation to be ended.
    /// @param config Related configuration for end call invitations.
    /// @param callback Callback for end call invitations.
    virtual void callEnd(const std::string &callID, const ZIMCallEndConfig &config,
                         ZIMCallEndSentCallback callback) = 0;

    /// Supported versions: 2.9.0 and above.
    ///
    /// Detail description: Users can call the call invitation list through the query.
    ///
    /// Service scenario: Users can use the query call invitation list for interface display or other functions.
    ///
    /// When to call: Run [create] to create a ZIM instance, which can be invoked after login.
    ///
    ///Related callbacks: [ZIMCallInvitationListQueriedCallback].
    ///
    /// @param config Query the relevant configuration of the call invitation list.
    /// @param callback Inquire the results of the call invitation list.
    virtual void queryCallInvitationList(const ZIMCallInvitationQueryConfig &config,
                                         ZIMCallInvitationListQueriedCallback callback) = 0;

    virtual void addUsersToBlacklist(const std::vector<std::string> &userIDs,
                                     ZIMBlacklistUsersAddedCallback callback) = 0;

    virtual void removeUsersFromBlacklist(const std::vector<std::string> &userIDs,
                                          ZIMBlacklistUsersRemovedCallback callback) = 0;

    virtual void queryBlacklist(const ZIMBlacklistQueryConfig &config,
                                ZIMBlacklistQueriedCallback callback) = 0;

    virtual void checkUserIsInBlacklist(const std::string &userID,
                                        ZIMBlacklistCheckedCallback callback) = 0;

    virtual void addFriend(const std::string &userID, const ZIMFriendAddConfig &config,
                           ZIMFriendAddedCallback callback) = 0;

    virtual void sendFriendApplication(const std::string &userID,
                                       const ZIMFriendApplicationSendConfig &config,
                                       ZIMFriendApplicationSentCallback callback) = 0;
    virtual void deleteFriends(const std::vector<std::string> &userIDs,
                               const ZIMFriendDeleteConfig &config,
                               ZIMFriendsDeletedCallback callback) = 0;

    virtual void searchLocalFriends(const ZIMFriendSearchConfig &config,
                                    ZIMFriendsSearchedCallback callback) = 0;

    virtual void checkFriendsRelation(const std::vector<std::string> &userIDs,
                                      const ZIMFriendRelationCheckConfig &config,
                                      ZIMFriendsRelationCheckedCallback callback) = 0;

    virtual void updateFriendAlias(const std::string &friendAlias, const std::string &userID,
                                   ZIMFriendAliasUpdatedCallback callback) = 0;

    virtual void
    updateFriendAttributes(const std::unordered_map<std::string, std::string> &friendAttributes,
                           const std::string &userID,
                           ZIMFriendAttributesUpdatedCallback callback) = 0;

    virtual void queryFriendsInfo(const std::vector<std::string> &userIDs,
                                  ZIMFriendsInfoQueriedCallback callback) = 0;

    virtual void acceptFriendApplication(const std::string &userID,
                                         const ZIMFriendApplicationAcceptConfig &config,
                                         ZIMFriendApplicationAcceptedCallback callback) = 0;

    virtual void rejectFriendApplication(const std::string &userID,
                                         const ZIMFriendApplicationRejectConfig &config,
                                         ZIMFriendApplicationRejectedCallback callback) = 0;

    virtual void queryFriendList(const ZIMFriendListQueryConfig &config,
                                 ZIMFriendListQueriedCallback callback) = 0;

    virtual void queryFriendApplicationList(const ZIMFriendApplicationListQueryConfig &config,
                                            ZIMFriendApplicationListQueriedCallback callback) = 0;
};

} // namespace zim

#include "internal/ZIMImpl.h"
