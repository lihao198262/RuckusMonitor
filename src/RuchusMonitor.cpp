//============================================================================
// Name        : RuchusMonitor.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include <iostream>
#include "RuchusMonitor.h"
#include "SmartacFileChannelFactory.h"
#include "SmartacFormatterFactory.h"

RuchusMonitorService::RuchusMonitorService() {

}

RuchusMonitorService::~RuchusMonitorService() {

}

void RuchusMonitorService::defineOptions(Poco::Util::OptionSet& options) {
	ServerApplication::defineOptions(options);
	options.addOption(Poco::Util::Option("configdir", "c", "config dir", true,
			"configdir", false).binding("configdir"));
}

void RuchusMonitorService::initialize(Application & self) {
	Poco::LoggingFactory::defaultFactory().registerFormatterClass(
			"SmartacPatternFormatter", new SmartacFormatterFactory());
	Poco::LoggingFactory::defaultFactory().registerChannelClass(
			"SmartacFileChannel", new SmartacFileChannelFactory());

	Application::instance().logger().information(
			"RuchusMonitorService::initialize");

	loadConfiguration(config().getString("configdir") + "/RuchusMonitor.ini");
	Poco::Util::ServerApplication::initialize(self);
	Application::instance().logger().information(
			"RuchusMonitorService::loadConfiguration ok");

	Poco::ThreadPool::defaultPool().start(mLogMonitor);
}

void RuchusMonitorService::uninitialize() {
	mLogMonitor.stop();

	Poco::ThreadPool::defaultPool().joinAll();

	Application::instance().logger().information(
			"RuchusMonitorService::uninitialize");
}

int RuchusMonitorService::main(const ArgVec& args) {
	waitForTerminationRequest();
	return Application::EXIT_OK;
}

POCO_SERVER_MAIN(RuchusMonitorService);
