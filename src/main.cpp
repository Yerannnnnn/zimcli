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
int execution_time = 300; //default is 300s
bool stopFlag = false;

int main(int argc, char **argv)
{
	std::cout << "Running..." << std::endl;

	CLI::App app("zimcli");
	app.add_option("--appid", appid, "appid");
	app.add_option("--appsign", appsign, "appsign");
	app.add_option("--sender", sender, "sender's userID");
	app.add_option("--receiver", receiver, "receiver's userID");
	app.add_option("--qps", qps, "qps, max is 10")->default_val(1);
	app.add_option("--execution-time", execution_time, "The execution time of this command line programs.")
		->default_val(300);
	CLI11_PARSE(app, argc, argv);

	if (qps > 10) {
		qps = 10;
	}

	std::cout << "appid: " << appid << std::endl;
	std::cout << "appsign: " << appsign << std::endl;
	std::cout << "sender: " << sender << std::endl;
	std::cout << "receiver: " << receiver << std::endl;
	std::cout << "execution_time: " << execution_time << std::endl;
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

	std::this_thread::sleep_for(std::chrono::seconds(execution_time));
	stopFlag = true;
	if (thread_.joinable()) {
		thread_.join();
	}
	return 0;
}

void loopMessage()
{
	int ms = 1000 / qps;
	while (!stopFlag) {

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