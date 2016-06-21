/*
 * RuckusDeviceInfoSender.cpp
 *
 *  Created on: 2016-6-21
 *      Author: root
 */

#include "RuckusDeviceInfoSender.h"
#include "RuchusMonitor.h"
#include <iostream>
#include <Poco/LocalDateTime.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/FileStream.h>

RuckusDeviceInfoSender::RuckusDeviceInfoSender() {
}

RuckusDeviceInfoSender::~RuckusDeviceInfoSender() {
}

void RuckusDeviceInfoSender::initial() {
	std::string config_time =
			Poco::Util::Application::instance().config().getString(
					"ac_ap_upload_time");
	timer = new Poco::Timer(0, 60000);
	timer->start(Poco::TimerCallback<RuckusDeviceInfoSender>(*this,
			&RuckusDeviceInfoSender::onTimer));
}

void RuckusDeviceInfoSender::onTimer(Poco::Timer& timer) {
	try {
		Poco::LocalDateTime dt;

		std::string now = Poco::DateTimeFormatter::format(dt, "%H:%M");

		if (now.compare(Poco::Util::Application::instance().config().getString(
				"ac_ap_upload_time")) == 0) {
			Poco::FileStream fs(
					Poco::Util::Application::instance().config().getString(
							"ac_ap_file"), std::ios::in);

			Poco::SharedPtr<char> temp = new char[1024];
			Poco::Net::DatagramSocket s;
			Poco::SharedPtr<Poco::Net::SocketAddress>
					address =
							new Poco::Net::SocketAddress(
									Poco::Util::Application::instance().config().getString(
											"dest_ip"),
									Poco::Util::Application::instance().config().getUInt(
											"dest_port"));
			while (fs.getline(temp, 1024)) {
				std::string line = temp.get();

				std::string sendline =
						Poco::format("%s|%s", line,
								Poco::DateTimeFormatter::format(dt,
										"%Y/%m/%d %H:%M:%S"));
				Poco::Util::Application::instance().logger().information(
						"RuckusDeviceInfoSender:" + sendline);
				s.sendTo(sendline.c_str(), sendline.length(), *address);

			}
		}
	} catch (Poco::Exception &e) {
		Poco::Util::Application::instance().logger().error(
				std::string("RuckusDeviceInfoSender:") + e.what());
	}
}
