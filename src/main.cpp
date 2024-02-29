#define ZIM_MAIN_CONFIG
#include <CLI/CLI.hpp>
#include <ZIM.h>
#include <iostream>
#include <string>
// #include "zim.h"
#include "main.h"

int appid = ;
std::string appsign = ;

zim::ZIM *zim_ = nullptr;
std::thread thread_;
std::string sender;
std::string receiver;
int qps = 1;
int execution_time = 300; //default is 300s
bool stopFlag = false;
int debug = 0;
std::string logpath = "/root/ZIMLogs";
int logsize = 0;

int main(int argc, char **argv)
{
	std::cout << "Running..." << std::endl;

	CLI::App app("zimcli");
	app.add_option("--sender", sender, "sender's userID");
	app.add_option("--receiver", receiver, "receiver's userID");
	app.add_option("--qps", qps, "qps, 1~10. Default is 1.")->default_val(1);

	app.add_option("--debug", debug, "debug or not. Default is 0.")->default_val(0);
	app.add_option("--logpath", logpath, "logpath. Default is /root/ZIMLogs")->default_val("/root/ZIMLogs");
	app.add_option("--logsize", logsize, "logsize. Default is 5242880(5M)")->default_val(0);
	app.add_option("--execution-time", execution_time,
		       "The execution time of this command line programs. 0~900s. Default is 300s.")
		->default_val(300);
	CLI11_PARSE(app, argc, argv);

	if (qps > 10) {
		qps = 10;
	}
	if (execution_time > 900) {
		execution_time = 900;
	}

	logpath = logpath + "/" + sender;
	std::string cachePath = "/root/ZIMCaches/" + sender;

	std::cout << "sender: " << sender << std::endl;
	std::cout << "receiver: " << receiver << std::endl;
	std::cout << "execution_time: " << execution_time << std::endl;
	std::cout << "logpath: " << logpath << std::endl;
	std::cout << "cachepath: " << cachePath << std::endl;
	std::cout << "logsize: " << logsize << std::endl;
	std::cout << "ZIM version: " << zim::ZIM::getVersion() << std::endl;

	if (sender.empty() || receiver.empty()) {
		std::cout << "sender, receiver are required." << std::endl;
		return 1;
	}

	// log path
	zim::ZIMLogConfig log_config;
	if (!logpath.empty()) {
		log_config.logPath = logpath;
	}
	if (logsize != 0) {
		log_config.logSize = logsize;
	}
	zim::ZIM::setLogConfig(log_config);
	zim::ZIMCacheConfig cache_config;
	cache_config.cachePath = cachePath;
	zim::ZIM::setCacheConfig(cache_config);

	// create zim
	std::cout << "Create ZIM..." << std::endl;
	zim::ZIMAppConfig app_config;
	app_config.appID = appid;
	app_config.appSign = appsign;
	zim_ = zim::ZIM::create(app_config);

	// login
	std::cout << "Login ZIM..." << std::endl;
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
	int ms = 5 + 1000 / qps;
	while (!stopFlag) {

		zim::ZIMMessageSendConfig sendConfig;

		auto message = std::make_shared<zim::ZIMTextMessage>("hello world!");
		// auto notification = std::make_shared<zim::ZIMMessageSendNotification>(
		// 	[=](const std::shared_ptr<zim::ZIMMessage> &message) {

		// 	});

		zim_->sendMessage(std::static_pointer_cast<zim::ZIMMessage>(message), receiver,
				  zim::ZIMConversationType::ZIM_CONVERSATION_TYPE_PEER, sendConfig, nullptr,
				  [=](const std::shared_ptr<zim::ZIMMessage> &message, const zim::ZIMError &errorInfo) {
					  if (debug != 0) {
						  std::cout << "[callback][sendMessage] code:" << errorInfo.code
							    << ",message:" << errorInfo.message << std::endl;
					  }
				  });

		std::this_thread::sleep_for(std::chrono::milliseconds(ms));
	}
}