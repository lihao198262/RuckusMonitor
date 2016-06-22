/*
 * RuchusLogMonitor.h
 *
 *  Created on: 2016-6-2
 *      Author: root
 */

#ifndef RUCHUSLOGMONITOR_H_
#define RUCHUSLOGMONITOR_H_

#include <Poco/Runnable.h>
#include <Poco/Util/ServerApplication.h>
#include <boost/asio.hpp>
#include <Poco/SharedPtr.h>
#include <Poco/File.h>
#include <Poco/RegularExpression.h>
#include <Poco/Net/DatagramSocket.h>

class RuchusLogMonitor: public Poco::Runnable {
public:
	RuchusLogMonitor();
	virtual ~RuchusLogMonitor();

	void run();

	void test();

	void stop();

protected:
	virtual void handler();

	void sender();

	bool initial();

private:
	boost::asio::io_service ios;

	int fd;

	int wd;

	Poco::SharedPtr<boost::asio::posix::stream_descriptor> stream_descriptor;

	char databuf[128];

	Poco::SharedPtr<Poco::File> moniter_file;

	Poco::File::FileSize begin_size;

	Poco::SharedPtr<Poco::RegularExpression> re;

	Poco::SharedPtr<Poco::RegularExpression> reDateTime;

	Poco::SharedPtr<Poco::RegularExpression> reUserMac;

	Poco::SharedPtr<Poco::RegularExpression> reAPMac;

	Poco::SharedPtr<Poco::RegularExpression> reMac;

	Poco::Net::DatagramSocket s;

	Poco::SharedPtr<Poco::Net::SocketAddress> address;

};

#endif /* RUCHUSLOGMONITOR_H_ */
