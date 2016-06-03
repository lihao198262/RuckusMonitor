
#include "SmartacFileChannel.h"

#include <iostream>
#include <Poco/Message.h>
#include <Poco/Path.h>
#include <Poco/File.h>
#include <Poco/DateTime.h>
#include <Poco/Timezone.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/NumberFormatter.h>

const std::string SmartacFileChannel::PROP_PATH         = "path";
const std::string SmartacFileChannel::PROP_BASENAME		= "basename";

SmartacFileChannel::SmartacFileChannel(void) : _pFile(0)
{
}

SmartacFileChannel::~SmartacFileChannel(void)
{
	try
	{
		close();
	}
	catch (...)
	{
		poco_unexpected();
	}
}

using namespace std;

void SmartacFileChannel::open() {
	Poco::FastMutex::ScopedLock lock(_mutex);

	Poco::Timestamp now;

	now += Poco::Timezone::utcOffset()*Poco::Timestamp::resolution();
	now += Poco::Timezone::dst()*Poco::Timestamp::resolution();
	Poco::DateTime dateTime = now;

	if (!_pFile || dateTime.day() != _createTime.day())	{
		try {

			Poco::Path path(_path);
			Poco::File file(path);
			if(file.exists() == false) {
				file.createDirectories();
			}
			std::string daystr = Poco::DateTimeFormatter::format(dateTime, "%Y-%m-%d");
			Poco::Path pathfilename = path;
			std::string filename = _fileBaseName + "-" + daystr + ".log";
			pathfilename.append(filename);

			int count = 0;
			while(true) {
				Poco::File filetemp(pathfilename);
				if(filetemp.exists()) {
					filename = _fileBaseName + "-" + daystr + "-" + Poco::NumberFormatter::formatHex(count++, 2, false) + ".log";
					pathfilename = path;
					pathfilename.append(filename);
				} else {
					break;
				}
			}

			_pFile = new Poco::LogFile(pathfilename.toString());
			_createTime = dateTime;
		} catch(Poco::Exception &e) {
			std::cerr << e.displayText() << std::endl;
		}
	}
}

void SmartacFileChannel::close() {
	Poco::FastMutex::ScopedLock lock(_mutex);
	delete _pFile;
	_pFile = 0;
}

void SmartacFileChannel::log(const Poco::Message& msg) {
	open();
	Poco::FastMutex::ScopedLock lock(_mutex);

	_pFile->write(msg.getText(), true);
}

void SmartacFileChannel::setProperty(const std::string& name, const std::string& value) {
	if (name == PROP_PATH)
		_path = value;
	else if(name == PROP_BASENAME) 
		_fileBaseName = value;
}
