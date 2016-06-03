#pragma once
#include "Poco/LoggingFactory.h"
#include "SmartacPatternFormatter.h"

class SmartacFormatterFactory :	public Poco::LoggingFactory::FormatterFactory
{
public:
	SmartacFormatterFactory(void);
	~SmartacFormatterFactory(void);
	SmartacPatternFormatter* createInstance() const
	{
		return new SmartacPatternFormatter;
	}
};

