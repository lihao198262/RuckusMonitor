/*
 * RuchusLogMonitor.cpp
 *
 *  Created on: 2016-6-2
 *      Author: root
 */

#include "RuchusLogMonitor.h"
#include <sys/inotify.h>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <Poco/FileStream.h>
#include <iostream>
#include <Poco/DateTime.h>
#include <Poco/DateTimeParser.h>
#include <Poco/DateTimeFormat.h>
#include <Poco/DateTimeFormatter.h>
#include <errno.h>

RuchusLogMonitor::RuchusLogMonitor() :
	fd(-1), wd(-1) {
}

RuchusLogMonitor::~RuchusLogMonitor() {
}

void RuchusLogMonitor::test() {
	std::string a = "Jun 14 12:06:32 localhost syslog: eventd_to_syslog():User[48:d7:05:d5:b5:27] joins WLAN[ruckus-access] from AP[R700-BJOffice@84:18:3a:09:f8:f0]";
	Poco::SharedPtr<Poco::RegularExpression> re = new Poco::RegularExpression("^[Feb|Jun]+[\\s]+[\\d]+[\\s]+[\\d]+:[\\d]+:[\\d]+");
	Poco::RegularExpression::MatchVec m;

	int b = re->match(a, 0, m);

	std::cout << b << std::endl;
	std::cout << m[0].length << std::endl;
	std::cout << m[0].offset << std::endl;

}

void RuchusLogMonitor::stop() {
	ios.stop();
}

void RuchusLogMonitor::run() {
	try {
		re = new Poco::RegularExpression(Poco::Util::Application::instance().config().getString("fliter_regex"));
	} catch (Poco::Exception &e) {
		Poco::Util::Application::instance().logger().error("log monitor config regex error %s", std::string(e.what()));
		return;
	}

	try {
		reDateTime = new Poco::RegularExpression(Poco::Util::Application::instance().config().getString("date_regex"));
	} catch (Poco::Exception &e) {
		Poco::Util::Application::instance().logger().error("log monitor config date_regex error %s", std::string(e.what()));
		return;
	}

	try {
		reUserMac = new Poco::RegularExpression(Poco::Util::Application::instance().config().getString("user_mac_regex"));
	} catch (Poco::Exception &e) {
		Poco::Util::Application::instance().logger().error("log monitor config user_mac_regex error %s", std::string(e.what()));
		return;
	}

	try {
		reAPMac = new Poco::RegularExpression(Poco::Util::Application::instance().config().getString("ap_mac_regex"));
	} catch (Poco::Exception &e) {
		Poco::Util::Application::instance().logger().error("log monitor config ap_mac_regex error %s", std::string(e.what()));
		return;
	}

	try {
		reMac = new Poco::RegularExpression(Poco::Util::Application::instance().config().getString("mac_regex"));
	} catch (Poco::Exception &e) {
		Poco::Util::Application::instance().logger().error("log monitor config mac_regex error %s", std::string(e.what()));
		return;
	}

	try {
		address = new Poco::Net::SocketAddress(Poco::Util::Application::instance().config().getString("dest_ip"), Poco::Util::Application::instance().config().getUInt("dest_port"));
	} catch (Poco::Exception &e) {
		Poco::Util::Application::instance().logger().error("log monitor config regex error %s", std::string(e.what()));
		return;
	}

	fd = inotify_init();

	if (fd == -1) {
		Poco::Util::Application::instance().logger().error("log monitor init fail %s", std::string(strerror(errno)));
		return;
	}

	Poco::Util::Application::instance().logger().information("log monitor init ok %d", fd);

	stream_descriptor = new boost::asio::posix::stream_descriptor(ios, fd);

	stream_descriptor->async_read_some(boost::asio::buffer(databuf, 128), boost::bind(&RuchusLogMonitor::handler, this));

	while (initial() == false)
		sleep(1);

	int n = ios.run();
	Poco::Util::Application::instance().logger().information("log monitor run close after handle %d", n);
}

void RuchusLogMonitor::handler() {
	stream_descriptor->async_read_some(boost::asio::buffer(databuf, 128), boost::bind(&RuchusLogMonitor::handler, this));
	struct inotify_event * p = (struct inotify_event *) databuf;

	int cur_event_cookie = p->cookie;

	unsigned long flags;

	flags = p->mask & ~(IN_ALL_EVENTS | IN_UNMOUNT | IN_Q_OVERFLOW | IN_IGNORED);

	switch (p->mask & (IN_ALL_EVENTS | IN_UNMOUNT | IN_Q_OVERFLOW | IN_IGNORED)) {
	case IN_ACCESS:
		Poco::Util::Application::instance().logger().information("IN_ACCESS");
		break;
	case IN_ATTRIB:
		Poco::Util::Application::instance().logger().information("IN_ATTRIB");
		break;
	case IN_CLOSE_WRITE:
		Poco::Util::Application::instance().logger().information("IN_CLOSE_WRITE");
		sender();
		break;
	case IN_CLOSE_NOWRITE:
		Poco::Util::Application::instance().logger().information("IN_CLOSE_NOWRITE");
		break;
	case IN_CREATE:
		Poco::Util::Application::instance().logger().information("IN_CREATE");
		break;
	case IN_DELETE:
		Poco::Util::Application::instance().logger().information("IN_DELETE");
		break;
	case IN_DELETE_SELF:
		Poco::Util::Application::instance().logger().information("IN_DELETE_SELF");
		break;
	case IN_MODIFY:
		sender();
		break;
	case IN_MOVE_SELF:
		while (initial() == false)
			sleep(1);
		break;
	case IN_MOVED_FROM:
		Poco::Util::Application::instance().logger().information("IN_MOVED_FROM Cookie=%d\n", cur_event_cookie);
		break;
	case IN_MOVED_TO:
		Poco::Util::Application::instance().logger().information("IN_MOVED_TO Cookie=%d\n", cur_event_cookie);
		break;
	case IN_OPEN:
		Poco::Util::Application::instance().logger().information("IN_OPEN");
		break;
	case IN_DONT_FOLLOW:
		Poco::Util::Application::instance().logger().information("IN_DONT_FOLLOW");
		break;
	case IN_MASK_ADD:
		Poco::Util::Application::instance().logger().information("IN_MASK_ADD");
		break;
	case IN_ONESHOT:
		Poco::Util::Application::instance().logger().information("IN_ONESHOT");
		break;
	case IN_ONLYDIR:
		Poco::Util::Application::instance().logger().information("IN_ONLYDIR");
		break;
	case IN_IGNORED:
		Poco::Util::Application::instance().logger().information("IN_IGNORED");
		break;
	case IN_ISDIR:
		Poco::Util::Application::instance().logger().information("IN_ISDIR");
		break;
	case IN_Q_OVERFLOW:
		Poco::Util::Application::instance().logger().information("IN_Q_OVERFLOW");
		break;
	case IN_UNMOUNT:
		Poco::Util::Application::instance().logger().information("IN_UNMOUNT");
		break;
	default:
		Poco::Util::Application::instance().logger().information("UNKNOWN EVENT ");
		break;
	}
	/* If any flags were set other than IN_ISDIR, report the flags */
	if (flags & (~IN_ISDIR)) {
		flags = p->mask;
		Poco::Util::Application::instance().logger().information("Flags=%lX\n", flags);
	}
}

void RuchusLogMonitor::sender() {
	try {
		Poco::FileStream fs(moniter_file->path(), std::ios::in);
		fs.seekg(begin_size);
		Poco::SharedPtr<char> temp = new char[1024];

		while (fs.getline(temp, 1024)) {
			std::string line = temp.get();
			Poco::Util::Application::instance().logger().debug("log monitor get line:%s", line);

			bool match = re->match(line);
			Poco::Util::Application::instance().logger().debug("log monitor match %b", match);

			if (!match)
				continue;

			Poco::RegularExpression::MatchVec m;

			int b = reDateTime->match(line, 0, m);

			if (b == 0) {
				Poco::Util::Application::instance().logger().error("log monitor find datatime error in line:%s", line);
				continue;
			}

			int tzd;
			Poco::DateTime dt;
			int year = dt.year();

			bool flag = Poco::DateTimeParser::tryParse("%b %e %H:%M:%S", line.substr(m[0].offset, m[0].length), dt, tzd);

			if (!flag) {
				Poco::Util::Application::instance().logger().error("log monitor datetime paser error:%s--->%s", line.substr(m[0].offset, m[0].length), line);
				continue;
			}

			Poco::RegularExpression::Match m3;
			b = reUserMac->match(line, 0, m3);
			if (b == 0) {
				Poco::Util::Application::instance().logger().error("log monitor user mac paser error--->%s", line);
				continue;
			}

			Poco::RegularExpression::Match m4;
			b = reAPMac->match(line, 0, m4);
			if (b == 0) {
				Poco::Util::Application::instance().logger().error("log monitor ap mac paser error--->%s", line);
				continue;
			}

			std::string sUserTemp = line.substr(m3.offset, m3.length);

			Poco::RegularExpression::Match m5;
			b = reMac->match(sUserTemp, 0, m5);

			if (b == 0) {
				Poco::Util::Application::instance().logger().error("log monitor user mac paser error--->%s", line);
				continue;
			}

			std::string sAPTemp = line.substr(m4.offset, m4.length);

			Poco::RegularExpression::Match m6;
			b = reMac->match(sAPTemp, 0, m6);

			if (b == 0) {
				Poco::Util::Application::instance().logger().error("log monitor ap mac paser error--->%s", line);
				continue;
			}

			std::string sendline = Poco::format("108|%d/%s|%s|%s", year, Poco::DateTimeFormatter::format(dt, "%n/%e %H:%M:%S"), sUserTemp.substr(m5.offset, m5.length), sAPTemp.substr(m6.offset,
					m6.length));

			int send_len = s.sendTo(sendline.c_str(), sendline.length(), *address);
			Poco::Util::Application::instance().logger().information("log monitor send->%s  %d", sendline, send_len);
		}

		begin_size = moniter_file->getSize();

	} catch (Poco::Exception &e) {
		Poco::Util::Application::instance().logger().error("log monitor send error %s:%s:%d", std::string(e.what()), e.displayText(), e.code());
	}
}

bool RuchusLogMonitor::initial() {

	if (wd != -1) {
		inotify_rm_watch(fd, wd);
		wd = -1;
	}

	std::string temp;
	try {
		temp = Poco::Util::Application::instance().config().getString("monitor_file");
	} catch (Poco::Exception &e) {
		Poco::Util::Application::instance().logger().error("log monitor get config error %s", std::string(e.what()));
		return false;
	}

	Poco::Util::Application::instance().logger().information("log minitor file %s", temp);

	const char * p = temp.c_str();

	wd = inotify_add_watch(fd, p, IN_ALL_EVENTS);

	if (wd == -1) {
		Poco::Util::Application::instance().logger().error("log monitor add watch fail %s", std::string(strerror(errno)));
		return false;
	}

	Poco::Util::Application::instance().logger().information("log monitor add watch id %d", wd);

	try {
		moniter_file = new Poco::File(temp);
		begin_size = moniter_file->getSize();
	} catch (Poco::Exception &e) {
		Poco::Util::Application::instance().logger().error("log monitor error %s", std::string(e.what()));
		return false;
	}

	Poco::Util::Application::instance().logger().information("log monitor file size %lu", begin_size);
	return true;
}

