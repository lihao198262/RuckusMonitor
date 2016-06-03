#pragma once

#include "SmartacFileChannel.h"
#include "Poco/LoggingFactory.h"

class SmartacFileChannelFactory : public Poco::LoggingFactory::ChannelInstantiator
{
public:
	SmartacFileChannelFactory(void);
	~SmartacFileChannelFactory(void);

	SmartacFileChannel* createInstance() const
	{
		return new SmartacFileChannel;
	}
};

