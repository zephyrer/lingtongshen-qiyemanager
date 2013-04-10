#pragma once

#include <log4cplus/logger.h>    
#include <log4cplus/fileappender.h>    
#include <log4cplus/consoleappender.h>    
#include <log4cplus/layout.h>

using namespace log4cplus;
using namespace log4cplus::spi;

#define maxLogFileSize		1024*1024*10
#define maxLogFileCount		10

#define LogFileName			L"debug\\ordermanager.log"

typedef enum {
	OM_LOG_OFF                    = 0,
	OM_LOG_TRACE                  = 1,
	OM_LOG_DEBUG                  = 2,
	OM_LOG_INFO                   = 3,
	OM_LOG_WARN                   = 4,
	OM_LOG_ERROR                  = 5,
	OM_LOG_FATAL                  = 6,
	OM_LOG_ALL                    = OM_LOG_TRACE
} OM_LOG_LEVEL;

class CMyLogger
{
public:
	CMyLogger(std::wstring& wstrName):
	  _name(wstrName), _logLevel(OFF_LOG_LEVEL), _isLogLevelSet(false) {}
	~CMyLogger();

	void setLogLevel(int level);

	int getLogLevel(){return _logLevel;}

    bool isLogLevelSet(){return _isLogLevelSet;}

	bool hasAppender() {return _rfApp || _conApp;}

	void setLogPath(const std::wstring& wstrName);

	void setConsoleOutput();

	void write(const wchar_t* evt, int level, const char* fname, int line, int process = 0, int thread = 0);

	static int CMyLogger::toLoggerLevel(int level);

private:
	std::wstring _name;
	std::wstring _logFileName;
	int _logLevel;
    bool _isLogLevelSet;

	SharedAppenderPtr _rfApp;
	SharedAppenderPtr _conApp;
};

extern CMyLogger gLogger;


// Logging macros
#define _LOG_MACRO(logEvent, logLevel, fileName, lineNum, processId, threadId) {			\
	if (logLevel >= gLogger.getLogLevel()) {					\
	log4cplus::tostringstream logBuf;						\
	logBuf << logEvent;										\
	gLogger.write(logBuf.str().c_str(), logLevel, fileName, lineNum, processId, threadId);	\
	}															\
}

#define S_LOG_MACRO(logEvent, logLevel) _LOG_MACRO(logEvent, logLevel, __FILE__, __LINE__, 0, 0)

#define S_LOG_TRACE(logEvent)	S_LOG_MACRO(logEvent, TRACE_LOG_LEVEL)
#define S_LOG_DEBUG(logEvent)	S_LOG_MACRO(logEvent, DEBUG_LOG_LEVEL)
#define S_LOG_INFO(logEvent)	S_LOG_MACRO(logEvent, INFO_LOG_LEVEL)
#define S_LOG_WARN(logEvent)	S_LOG_MACRO(logEvent, WARN_LOG_LEVEL)
#define S_LOG_ERROR(logEvent)	S_LOG_MACRO(logEvent, ERROR_LOG_LEVEL)
#define S_LOG_FATAL(logEvent)	S_LOG_MACRO(logEvent, FATAL_LOG_LEVEL)

// Trace function entrances and exits
class S_TraceFunction {
public:
	S_TraceFunction(const char* func):_func(func) {
		S_LOG_TRACE("Entering:>>> " <<  _func.c_str());
	}
	~S_TraceFunction() {
		S_LOG_TRACE("Leaving:<<< " <<  _func.c_str());
	}

private:
	std::string _func;
};

#define S_TRACE_FUNCTION S_TraceFunction trace(__FUNCTION__);
