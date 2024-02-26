#pragma once

//
//  ZIMEventHandler.h
//  ZIM
//
//  Copyright © 2021 Zego. All rights reserved.
//

namespace zim {

class ZIM;

// Callback
//
class ZIMEventHandler {

  public:
    // MARK: - Main

    // The callback for error information.
    //
    // When an exception occurs in the SDK, the callback will prompt detailed information.
    //
    // @param zim ZIM instance.
    // @param errorCode Error code, please refer to the error codes document.
    // @param desc A detailed description of the error.

    virtual void onError(ZIM * /*zim*/, const ZIMError & /*errorInfo*/) {}

    // The event callback when the connection state changes.
    //
    // @param zim ZIM instance.
    // @param state The current connection state after changed.
    // @param event The event that caused the connection state to change.
    // @param extendedData Extra information when the event occurs, a standard JSON string.

    virtual void onConnectionStateChanged(ZIM * /*zim*/, ZIMConnectionState /*state*/,
                                          ZIMConnectionEvent /*event*/,
                                          const std::string & /*extendedData*/) {}

    // The event callback when the room connection status changes.
    //
    // Available since:  1.1.0 or above.
    //
    // Description:event callback when the room connection status changes.
    //
    // When to call::After creating a ZIM instance through [create], you can call this interface.
    //
    // Related APIs:through [onTokenWillExpire], the callback will be received when the token is about to expire.

    virtual void onRoomStateChanged(ZIM * /*zim*/, ZIMRoomState /*state*/, ZIMRoomEvent /*event*/,
                                    const std::string & /*extendedData*/,
                                    const std::string & /*roomID*/) {}

    // A reminder callback that the token is about to expire.
    //
    // When receiving this callback, you should invoke the [renewToken] function to update the token in time.
    //
    // @param zim ZIM instance.
    // @param second The remaining second before the token expires.

    virtual void onTokenWillExpire(ZIM * /*zim*/, unsigned int /*second*/) {}

    virtual void onUserInfoUpdated(ZIM * /*zim*/, const ZIMUserFullInfo & /*info*/) {}

    // MARK: - Message

    // The callback for receiving peer-to-peer message.
    //
    // When receiving peer-to-peer message from other user, you will receive this callback.
    //
    // @param zim ZIM instance.
    // @param messageList List of received messages.
    // @param fromUserID The user ID of the message sender.

    virtual void
    onReceivePeerMessage(ZIM * /*zim*/,
                         const std::vector<std::shared_ptr<ZIMMessage>> & /*messageList*/,
                         const std::string & /*fromUserID*/) {}

    // The callback for receiving room message.
    //
    // This callback will be triggered when new message is received in a room.
    //
    // @param zim ZIM instance.
    // @param messageList List of received messages.
    // @param fromRoomID ID of the room where the message was received

    virtual void
    onReceiveRoomMessage(ZIM * /*zim*/,
                         const std::vector<std::shared_ptr<ZIMMessage>> & /*messageList*/,
                         const std::string & /*fromRoomID*/) {}

    // The callback for receiving group message.
    //
    // This callback will be triggered when new message is received in a group.
    //
    // @param zim ZIM instance.
    // @param messageList List of received messages.
    // @param fromGroupID ID of the group where the message was received

    virtual void
    onReceiveGroupMessage(ZIM * /*zim*/,
                          const std::vector<std::shared_ptr<ZIMMessage>> & /*messageList*/,
                          const std::string & /*fromGroupID*/) {}

    // Received notification callback when the message receiver confirms that the message has been read.
    //
    // Available since: 2.5.0 and above.
    //
    // Description: When the message receiver confirms that the message has been read, the message sender knows through this callback.
    //
    // When to call /Trigger: Trigger a notification when the message receiver has read the message.
    //
    // Related APIs: Triggered when the peer calls via [sendMessageReceiptsRead].
    // @param zim ZIM instance.
    // @param infos Receipt information.

    virtual void onMessageReceiptChanged(ZIM * /*zim*/,
                                         const std::vector<ZIMMessageReceiptInfo> & /*infos*/) {}

    // Received notification callback when some one else sends a message and then revoke a message sent by themselves.
    //
    // Available since: 2.5.0 and above.
    //
    // Description: This callback is received when some one else sends a message and then revoke.
    //
    // When to call /Trigger: This callback occurs when a ZIM instance is created with [create] and the other user revoke a message.
    //
    // Related APIs: You can revoke message to other members via [revokeMessage].
    // @param zim ZIM instance.
    // @param messageList List of received messages..

    virtual void onMessageRevokeReceived(
        ZIM * /*zim*/, const std::vector<std::shared_ptr<ZIMRevokeMessage>> & /*messageList*/) {}
    // Supported version: 2.10.0 or later.
    //
    // Description: The callback is received when the server interface [SendMessageToAllUsers] sends a message.
    //
    // Call time: This callback is received after logging in to the ZIM service and the server call the [SendMessageToAllUsers] interface to send a message.
    // @param zim ZIM instance.
    // @param message Received message.
    virtual void onBroadcastMessageReceived(ZIM * /*zim*/,
                                            const std::shared_ptr<ZIMMessage> & /*message*/) {}

    virtual void onMessageDeleted(ZIM * /*zim*/, const ZIMMessageDeletedInfo & /*deletedInfo*/) {}

    virtual void onMessageSentStatusChanged(
        ZIM * /*zim*/,
        const std::vector<ZIMMessageSentStatusChangeInfo> & /*messageSentStatusChangeInfoList*/) {}

    //Supported version: 2.10.0 or later.
    //
    //Description: The callback is received when reactions change
    //
    //When to Call: This callback can be registered after the ZIM instance is created by [create] and before login.
    //
    //When to Trigger: After other users add or delete reactions to messages in private or group chats.
    //
    //Related APIs: [addMessageReaction]、[deleteMessageReaction]
    // @param zim ZIM instance.
    // @param reactions The list of changed reactions.
    virtual void onMessageReactionsChanged(ZIM * /*zim*/,
                                           const std::vector<ZIMMessageReaction> & /*reactions*/) {}
    // MARK: - Conversation

    // Received notification callback for session update.
    //
    // Available since: 2.0.0 and above.
    //
    // Description: Trigger this callback to return notification of session updates when a session is added, deleted, or modified.
    //
    // When to call /Trigger: Notifications are triggered when a new message is updated in the session, or when the session itself is added, deleted, or modified.
    //
    // Caution: ConversationID is the same as single chat toUserID and group chat GroupID.
    //
    // Related APIs: Through [sendPeerMessage] , [sendGroupMessage], [sendRoomMessage], [deleteConversation] [deleteMessage], [deleteMessageByConversationID] trigger.
    // @param zim ZIM instance.
    // @param conversationChangeInfoList Changed information about the conversation.

    virtual void onConversationChanged(
        ZIM * /*zim*/,
        const std::vector<ZIMConversationChangeInfo> & /*conversationChangeInfoList*/) {}

    // Received notification callback for session update.
    //
    // Available since: 2.0.0 and above.
    //
    // Description: Trigger this callback to return notification of session updates when a session is added, deleted, or modified.
    //
    // When to call /Trigger: Notifications are triggered when a new message is updated in the session, or when the session itself is added, deleted, or modified.
    //
    // Caution: ConversationID is the same as single chat toUserID and group chat GroupID.
    //
    // Related APIs: Through [sendPeerMessage] , [sendGroupMessage], [sendRoomMessage], [deleteConversation] [deleteMessage], [deleteMessageByConversationID] trigger.
    // @param zim ZIM instance.
    // @param totalUnreadMessageCount Updated total unreads.
    virtual void
    onConversationTotalUnreadMessageCountUpdated(ZIM * /*zim*/,
                                                 unsigned int /*totalUnreadMessageCount*/) {}

    // Received notification callback when the message receiver has read this receipt message.
    //
    // Available since: 2.5.0 and above.
    //
    // Description: When the message receiver has read the session, the message sender knows through this callback.
    //
    // When to call /Trigger: Trigger a notification when the message receiver has read the session.
    //
    // Related APIs: triggered when the peer calls via [sendConversationMessageReceiptRead].
    // @param zim ZIM instance.
    // @param infos Receipt information.
    virtual void
    onConversationMessageReceiptChanged(ZIM * /*zim*/,
                                        const std::vector<ZIMMessageReceiptInfo> & /*infos*/) {}

    //Available since: 2.12.0 and above.
    //
    //Description: When multiple login ends delete all sessions, the local end triggers the callback to notify the local end that all sessions are deleted.
    //
    //When to call /Trigger: When another end clears all unread sessions, the local end triggers a notification.
    //
    //Related APIs: Triggered by [deleteAllConversations].
    virtual void onConversationsAllDeleted(ZIM * /*zim*/,
                                           const ZIMConversationsAllDeletedInfo & /*info*/) {}

    // MARK: - Room

    // Callback when other members join the room.
    //
    // After joining a room, this callback will be triggered when other members also join this room.
    //
    // @param zim ZIM instance.
    // @param memberList List of members who joined the room.
    // @param roomID The ID of the room where this event occurred.

    virtual void onRoomMemberJoined(ZIM * /*zim*/, const std::vector<ZIMUserInfo> & /*memberList*/,
                                    const std::string & /*roomID*/) {}

    // Callback when other members leave the room.
    //
    // After joining a room, this callback will be triggered when other members leave this room.
    //
    // @param zim ZIM instance.
    // @param memberList List of members who left the room.
    // @param roomID The ID of the room where this event occurred.

    virtual void onRoomMemberLeft(ZIM * /*zim*/, const std::vector<ZIMUserInfo> & /*memberList*/,
                                  const std::string & /*roomID*/) {}

    // Event callback when the room attributes changes.
    //
    // Available since:  1.3.0.
    //
    // Description:When the room attribute in the room changes, it will be notified through this callback.
    //
    // @param zim ZIM instance.
    // @param updateInfo The info of the room attributes changed.
    // @param roomID The ID of the room where this event occurred.

    virtual void onRoomAttributesUpdated(ZIM * /*zim*/,
                                         const ZIMRoomAttributesUpdateInfo & /*updateInfo*/,
                                         const std::string & /*roomID*/) {}

    // Event callback when the room attributes changes.
    //
    // Available since:  1.3.0.
    //
    // Description:When the room attribute in the room changes, it will be notified through this callback.
    //
    // @param zim ZIM instance.
    // @param updateInfos The infos of the room attributes changed.
    // @param roomID The ID of the room where this event occurred.

    virtual void
    onRoomAttributesBatchUpdated(ZIM * /*zim*/,
                                 const std::vector<ZIMRoomAttributesUpdateInfo> & /*updateInfos*/,
                                 const std::string & /*roomID*/) {}

    // Room user property update callback.
    // @param ZIM instance.
    // @param infos The infos of the room member attributes changed.
    // @param operatedInfo Room operation information.
    // @param roomID Room ID.
    virtual void onRoomMemberAttributesUpdated(
        ZIM * /*zim*/, const std::vector<ZIMRoomMemberAttributesUpdateInfo> & /*infos*/,
        ZIMRoomOperatedInfo /*operatedInfo*/, const std::string & /*roomID*/) {}

    // MARK: - Group

    // Group state change notification callback.
    //
    // Description: allback notification of group status change.
    //
    // Use cases: Scenarios that require interaction based on the group status.
    //
    // When to call /Trigger: A notification is triggered when a group is created, joined, left, or dismissed.
    //
    // Related APIs: [createGroup] : creates a group. [joinGroup] : joins a group. [leaveGroup], leave the group. [dismissGroup]; dismiss the group.
    // @param zim ZIM instance.
    // @param state The status of the group after the change.
    // @param event Group related events.
    // @param operatedInfo Group information that has been operated.
    // @param groupInfo Information about group status changes.

    virtual void onGroupStateChanged(ZIM * /*zim*/, ZIMGroupState /*state*/,
                                     ZIMGroupEvent /*event*/,
                                     const ZIMGroupOperatedInfo & /*operatedInfo*/,
                                     const ZIMGroupFullInfo & /*groupInfo*/) {}

    // Group name update notification callback.
    //
    // Description: Group name change notification callback.
    //
    // Use cases: If the group name is changed, you need to synchronize the latest group name.
    //
    // When to call /Trigger: The group name is changed. Procedure
    //
    // Related APIs: [updateGroupName] : updates the group name.
    // @param zim ZIM instance.
    // @param groupName The updated group name.
    // @param operatedInfo Operation information after the group name is updated.
    // @param groupID The groupID where the group name update occurred.

    virtual void onGroupNameUpdated(ZIM * /*zim*/, const std::string & /*groupName*/,
                                    const ZIMGroupOperatedInfo & /*operatedInfo*/,
                                    const std::string & /*groupID*/) {}

    virtual void onGroupAvatarUrlUpdated(ZIM * /*zim*/, const std::string & /*groupAvatarUrl*/,
                                         const ZIMGroupOperatedInfo & /*operatedInfo*/,
                                         const std::string & /*groupID*/) {}

    // Notification callback for group announcement updates.
    //
    // Description: Group bulletin Change notification callback.
    //
    // Use cases: If a group bulletin changes, you need to synchronize the latest bulletin content.
    //
    // When to call /Trigger: The group bulletin is changed. Procedure
    //
    // Related APIs: [updateGroupNotice], which updates the group notice.
    // @param zim ZIM instance.
    // @param groupNotice Updated group announcement.
    // @param operatedInfo The group announces the updated operation information.
    // @param groupID The groupID where the group announcement update occurred.

    virtual void onGroupNoticeUpdated(ZIM * /*zim*/, const std::string & /*groupNotice*/,
                                      const ZIMGroupOperatedInfo & /*operatedInfo*/,
                                      const std::string & /*groupID*/) {}

    // Group attributes update notification callback.
    //
    // Description: Group attribute change notification callback.
    //
    // Use cases: When group attributes are changed, you need to synchronize the latest group attributes.
    //
    // When to call /Trigger: Triggered when group properties are set, updated, or deleted.
    //
    // Impacts on other APIs:  [setGroupAttributes] updates group attributes. [deleteGroupAttributes], delete the group attribute.
    // @param zim ZIM instance.
    // @param groupAttributes Information after group attribute update.
    // @param operatedInfo Operation information after the group attribute is updated.
    // @param groupID The groupID for sending group attribute updates.

    virtual void onGroupMutedInfoUpdated(ZIM * /*zim*/, const ZIMGroupMuteInfo & /*mutedInfo*/,
                                         const ZIMGroupOperatedInfo & /*operatedInfo*/,
                                         const std::string & /*groupID*/) {}

    virtual void onGroupAttributesUpdated(
        ZIM * /*zim*/, const std::vector<ZIMGroupAttributesUpdateInfo> & /*groupAttributes*/,
        const ZIMGroupOperatedInfo & /*operatedInfo*/, const std::string & /*groupID*/) {}

    // Group member state change notification callback.
    //
    // Description: Group member status change notification callback.
    //
    // Use cases: Scenarios that require interaction based on group member states.
    //
    // When to call /Trigger: Notification is triggered when a group is created, joined, left, or dismissed, or a user is invited to join or kicked out of the group.
    //
    // Related APIs: [createGroup] : creates a group. [joinGroup] : joins a group. [leaveGroup], leave the group. [dismissGroup]; dismiss the group. [intiveUsersIntoGroup], which invites users to join the group. [kickoutGroupMember] kicks the user out of the group.
    // @param zim ZIM instance.
    // @param state Updated membership status.
    // @param event Updated member events.
    // @param userList Updated member information.
    // @param operatedInfo Updated operational information.
    // @param groupID The groupID where the member state change occurred.

    virtual void onGroupMemberStateChanged(ZIM * /*zim*/, ZIMGroupMemberState /*state*/,
                                           ZIMGroupMemberEvent /*event*/,
                                           const std::vector<ZIMGroupMemberInfo> & /*userList*/,
                                           const ZIMGroupOperatedInfo & /*operatedInfo*/,
                                           const std::string & /*groupID*/) {}

    // Group member information change notification.
    //
    // Description: Return the operation result of changing group member information.
    //
    // Use cases: After the basic information of group members is changed, you need to display or interact with group members on the page.
    //
    // When to call /Trigger: The result is displayed after the group member information is changed.
    //
    // Related APIs: [setGroupMemberNickname] : updates the nickname of a group member. [setGroupMemberRole] : updates the group member role. [transferGroupOwner], group master transfer.
    // @param zim ZIM instance.
    // @param groupMemberInfos Updated member information.
    // @param operatedInfo Updated operational information.
    // @param groupID The groupID where the group member information has been changed.

    virtual void onGroupMemberInfoUpdated(
        ZIM * /*zim*/, const std::vector<ZIMGroupMemberInfo> & /*groupMemberInfos*/,
        const ZIMGroupOperatedInfo & /*operatedInfo*/, const std::string & /*groupID*/) {}

    // MARK: - CallInvite

    // The notification callback for the call invitation received by the invitee.
    //
    // Supported versions: 2.0.0 and above.
    //
    // Detail description: After the inviter initiates a call invitation, the invitee will receive this callback when the invitee is online.
    //
    // Business scenario: The invitee will call this callback after the inviter sends a call invitation.
    //
    // When to call: After creating a ZIM instance through [create].
    //
    // Note: If the user is not in the invitation list or not online, this callback will not be called.
    // @param zim ZIM instance.
    // @param info Information about received call invitations.
    // @param callID Received CallID.

    virtual void onCallInvitationReceived(ZIM * /*zim*/,
                                          const ZIMCallInvitationReceivedInfo & /*info*/,
                                          const std::string & /*callID*/) {}

    // The notification callback received by the invitee to cancel the invitation.
    //
    // Supported versions: 2.0.0 and above.
    //
    // Detail description: After the inviter cancels the call invitation, this callback will be received when the invitee is online.
    //
    // Business scenario: The invitee will call this callback after the inviter cancels the call invitation.
    //
    // When to call: After creating a ZIM instance through [create].
    //
    // Note: If the user is not in the cancel invitation list or is offline, this callback will not be called.
    //
    // Related interface: [callCancel].
    // @param zim ZIM instance.
    // @param info Information about canceled call invitations.
    // @param callID Cancelled callID.

    virtual void onCallInvitationCancelled(ZIM * /*zim*/,
                                           const ZIMCallInvitationCancelledInfo & /*info*/,
                                           const std::string & /*callID*/) {}

    // @deprecated Deprecated since 2.9.0, pleace use  [onCallUserStateChanged] instead.
    // The notification callback received by the inviter that invitee has accepted the invitation.
    //
    // Supported versions: 2.0.0 and above.
    //
    // Detail description: After the invitee accepts the call invitation, this callback will be received when the inviter is online.
    //
    // Business scenario: The inviter will receive this callback after the inviter accepts the call invitation.
    //
    // When to call: After creating a ZIM instance through [create].
    //
    // Note: This callback will not be called if the user is not online.
    //
    // Related interface: [callAccept].
    //
    // @param zim ZIM instance.
    // @param info Information about the call invitation.
    // @param callID The accepted callID.

    virtual void onCallInvitationAccepted(ZIM * /*zim*/,
                                          const ZIMCallInvitationAcceptedInfo & /*info*/,
                                          const std::string & /*callID*/) {}

    // @deprecated Deprecated since 2.9.0, pleace use [onCallUserStateChanged] instead.
    // The inviter receives the notification callback that the invitee reject the invitation.
    //
    // Available since: 2.0.0 and above.
    //
    // Description: This callback will be received when the inviter is online after the inviter rejects the call invitation.
    //
    // Use cases: The inviter will receive this callback after the inviter declines the call invitation.
    //
    // Default value: After creating a ZIM instance through [create] and logging in.
    //
    // When to call /Trigger: After creating a ZIM instance through [create] and logging in.
    //
    // Restrictions: If the user is not the inviter of the call invitation or is not online, the callback will not be received.
    //
    // Related APIs:[callReject].
    // @param zim ZIM instance.
    // @param info Information about rejected call invitations.
    // @param callID The callID of the rejected call invitation.

    virtual void onCallInvitationRejected(ZIM * /*zim*/,
                                          const ZIMCallInvitationRejectedInfo & /*info*/,
                                          const std::string & /*callID*/) {}

    // @deprecated Deprecated since 2.9.0, pleace use other [onCallInvitationTimeout] instead.
    // Supported versions: 2.0.0 and above.
    //
    // Detail description: When the call invitation times out, the invitee does not respond and will receive a callback.
    //
    // Business scenario: If the invitee does not respond before the timeout period, this callback will be received.
    //
    // When to call: After creating a ZIM instance through [create].
    //
    // Note: If the user is not on the invitation list or is not online, the callback will not be received.
    //
    // Related interfaces: [callInvite], [callAccept], [callReject].
    // @param zim ZIM instance.
    // @param callID The callID of the timeout.

    virtual void onCallInvitationTimeout(ZIM * /*zim*/, const std::string & /*callID*/) {}

    virtual void onCallInvitationTimeout(ZIM * /*zim*/,
                                         const ZIMCallInvitationTimeoutInfo & /*info*/,
                                         const std::string & /*callID*/) {}

    // @deprecated Deprecated since 2.9.0, pleace use [onCallUserStateChanged] instead.
    // Notification callback of call invitation has timeouted.
    //
    // Supported versions: 2.0.0 and above.
    //
    // Detail description: When the call invitation times out, the invitee does not respond, and the inviter will receive a callback.
    //
    // Business scenario: The invitee does not respond before the timeout period, and the inviter will receive this callback.
    //
    // When to call: After creating a ZIM instance through [create].
    //
    // Note: If the user is not the inviter who initiated this call invitation or is not online, the callback will not be received.
    //
    // Related interfaces: [callInvite], [callAccept], [callReject].
    // @param zim ZIM instance.
    // @param invitees Timeout invitee ID.
    // @param callID The number of invitees that have timed out.

    virtual void onCallInviteesAnsweredTimeout(ZIM * /*zim*/,
                                               const std::vector<std::string> & /*invitees*/,
                                               const std::string & /*callID*/) {}

    virtual void onCallInvitationCreated(ZIM * /*zim*/,
                                         const ZIMCallInvitationCreatedInfo & /*info*/,
                                         const std::string & /*callID*/) {}
    /// Supported versions: 2.9.0 and above.
    ///
    /// Detail description: In an advanced call,  a participant ends the call, and all participants will receive this callback.
    ///
    /// Note: If the user is not the inviter who initiated this call invitation or is not online, the callback will not be received.
    ///
    /// Related APIs: [callEnd]
    ///
    /// @param zim ZIM instance.
    /// @param info Information carried by the event callback.
    /// @param callID Timeout invitee ID.
    virtual void onCallInvitationEnded(ZIM * /*zim*/, const ZIMCallInvitationEndedInfo & /*info*/,
                                       const std::string & /*callID*/) {}

    ///    Supported versions: 2.9.0 and above.
    ///
    ///    Detail description: Listen for calling user status changes.
    ///
    ///    When to call: When a new member is invited to a call, or a member accepts, rejects, exits, or a member response times out, all users on the current call invitation whose status is "Inviting," "Accepted," and "Received" receive the callback here. If the member is not online at the time of notification, the call is still ongoing when the login succeeds. The status changes of all members during the offline period will be sent to the user at one time.
    ///
    ///    Note: If the user is not the inviter who initiated this call invitation or is not online, the callback will not be received.
    ///
    ///    Related APIs: [callInvite], [callingInvite], [callAccept], [callReject],[callQuit].
    ///
    /// @param zim ZIM instance.
    /// @param info Information about the status change of a call member.
    /// @param Unique identifier of the call.
    virtual void onCallUserStateChanged(ZIM * /*zim*/, const ZIMCallUserStateChangeInfo & /*info*/,
                                        const std::string & /*callID*/) {}

    virtual void onBlacklistChanged(ZIM * /*zim*/, const std::vector<ZIMUserInfo> & /*userList*/,
                                    const ZIMBlacklistChangeAction & /*action*/) {}

    virtual void onFriendInfoUpdated(ZIM * /*zim*/,
                                     const std::vector<ZIMFriendInfo> & /*friendInfoList*/) {}

    virtual void onFriendListChanged(ZIM * /*zim*/,
                                     const std::vector<ZIMFriendInfo> & /*friendInfoList*/,
                                     ZIMFriendListChangeAction & /*action*/) {}

    virtual void onFriendApplicationUpdated(
        ZIM * /*zim*/,
        const std::vector<ZIMFriendApplicationInfo> & /*friendApplicationInfoList*/) {}
    virtual void onFriendApplicationListChanged(
        ZIM * /*zim*/, const std::vector<ZIMFriendApplicationInfo> & /*friendApplicationInfoList*/,
        ZIMFriendApplicationListChangeAction & /*action*/) {}
};

} // namespace zim
