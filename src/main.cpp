#define ZIM_MAIN_CONFIG
#include <CLI/CLI.hpp>
#include <ZIM.h>
#include <iostream>
#include <string>
// #include "zim.h"
#include "main.h"

zim::ZIM *zim_ = nullptr;
std::thread thread_;

int appid;
std::string appsign;
std::string sender;
std::string receiver;
int qps = 1;

int main(int argc, char **argv)
{

	std::cout << "Running..." << std::endl;

	CLI::App app("zimcli");
	app.add_option("--appid", appid, "appid");
	app.add_option("--appsign", appsign, "appsign");
	app.add_option("--sender", sender, "sender");
	app.add_option("--receiver", receiver, "receiver");
	app.add_option("--qps", qps, "qps");
	CLI11_PARSE(app, argc, argv);

	std::cout << "appid: " << appid << std::endl;
	std::cout << "appsign: " << appsign << std::endl;
	std::cout << "sender: " << sender << std::endl;
	std::cout << "receiver: " << receiver << std::endl;
	std::cout << "ZIM version: " << zim::ZIM::getVersion() << std::endl;

	// create zim
	zim::ZIMAppConfig app_config;
	app_config.appID = appid;
	app_config.appSign = appsign;
	zim_ = zim::ZIM::create(app_config);

	// login
	zim::ZIMUserInfo userInfo;
	userInfo.userID = sender;
	userInfo.userName = sender;
	std::cout << "waiting for login success" << std::endl;
	zim_->login(userInfo, [=](const zim::ZIMError &errorInfo) {
		std::cout << "[callback][ZIMLoggedInCallback] code:" << errorInfo.code
			  << ",message:" << errorInfo.message << std::endl;

		thread_ = std::thread(loopMessage);
	});
	while (1) {
	}

	return 0;
}

void loopMessage()
{
	int ms = 1000 / qps;
	while (1) {

		zim::ZIMMessageSendConfig sendConfig;

		auto message = std::make_shared<zim::ZIMTextMessage>("hello world!");
		auto notification = std::make_shared<zim::ZIMMessageSendNotification>(
			[=](const std::shared_ptr<zim::ZIMMessage> &message) {
				std::cout << "[callback][sendMessage] onMessageAttached" << std::endl;
			});

		zim_->sendMessage(std::static_pointer_cast<zim::ZIMMessage>(message), receiver,
				  zim::ZIMConversationType::ZIM_CONVERSATION_TYPE_PEER, sendConfig, notification,
				  [=](const std::shared_ptr<zim::ZIMMessage> &message, const zim::ZIMError &errorInfo) {
					  std::cout << "[callback][sendMessage] code:" << errorInfo.code
						    << ",message:" << errorInfo.message << std::endl;
				  });

		std::this_thread::sleep_for(std::chrono::milliseconds(ms));
	}
}