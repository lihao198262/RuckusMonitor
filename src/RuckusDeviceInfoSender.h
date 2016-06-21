/*
 * RuckusDeviceInfoSender.h
 *
 *  Created on: 2016-6-21
 *      Author: root
 */

#ifndef RUCKUSDEVICEINFOSENDER_H_
#define RUCKUSDEVICEINFOSENDER_H_

#include <Poco/Timer.h>
#include <Poco/SharedPtr.h>

class RuckusDeviceInfoSender {
public:
	RuckusDeviceInfoSender();
	virtual ~RuckusDeviceInfoSender();

	void initial();

	void onTimer(Poco::Timer& timer);

private:
	Poco::SharedPtr<Poco::Timer> timer;
};

#endif /* RUCKUSDEVICEINFOSENDER_H_ */
