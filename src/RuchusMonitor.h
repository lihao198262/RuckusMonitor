/*
 * RuchusMonitor.h
 *
 *  Created on: 2016-6-2
 *      Author: root
 */

#ifndef RUCHUSMONITOR_H_
#define RUCHUSMONITOR_H_

#include <Poco/Util/ServerApplication.h>
#include "RuchusLogMonitor.h"
#include "RuckusDeviceInfoSender.h"

class RuchusMonitorService: public Poco::Util::ServerApplication {

public:
	RuchusMonitorService();
	virtual ~RuchusMonitorService();

protected:
	void defineOptions(Poco::Util::OptionSet& options);
	void initialize(Application & self);
	void uninitialize();

	int main(const ArgVec& args);

private:
	RuchusLogMonitor mLogMonitor;
	RuckusDeviceInfoSender mDeviceInfoSender;
};

#endif /* RUCHUSMONITOR_H_ */
