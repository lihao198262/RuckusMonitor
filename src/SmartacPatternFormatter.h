#pragma once
#include "Poco/PatternFormatter.h"

class SmartacPatternFormatter :	public Poco::PatternFormatter
{
public:
	SmartacPatternFormatter(void);
	~SmartacPatternFormatter(void);

	void format(const Poco::Message& msg, std::string& text);

	void setProperty(const std::string& name, const std::string& value);

	std::string getProperty(const std::string& name) const;

protected:
	static const std::string& getPriorityName(int);

private:
	struct PatternAction
	{
		PatternAction(): key(0), length(0) 
		{
		}

		char key;
		unsigned int length;
		std::string property;
		std::string prepend;
	};

	void parsePattern();
		/// Will parse the _pattern string into the vector of PatternActions,
		/// which contains the message key, any text that needs to be written first
		/// a proprety in case of %[] and required length.

	std::vector<PatternAction> _patternActions;
	bool _localTime;
	std::string _pattern;
};

