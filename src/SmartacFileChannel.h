#pragma once
#include "Poco/LogFile.h"
#include "Poco/Channel.h"
#include "Poco/Mutex.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTime.h"
class SmartacFileChannel : public Poco::Channel
{
public:
	SmartacFileChannel(void);
	
	virtual void open();

	virtual void close();
		
	virtual void log(const Poco::Message& msg);

	void setProperty(const std::string& name, const std::string& value);

	static const std::string PROP_PATH;
	static const std::string PROP_BASENAME;
protected:
	~SmartacFileChannel(void);

private:
	std::string		_path;
	std::string		_fileBaseName;
	Poco::DateTime _createTime;
	Poco::LogFile*	_pFile;
	Poco::FastMutex _mutex;
};

