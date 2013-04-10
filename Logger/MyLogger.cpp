#include "stdafx.h"
#include "MyLogger.h"

CMyLogger::~CMyLogger()
{
}

void CMyLogger::setLogLevel(int level)
{
	_logLevel = level;
	_isLogLevelSet = true;

	//turn on log
	if (!_rfApp && _logLevel != OFF_LOG_LEVEL && _logFileName.size() != 0){
		setLogPath(_logFileName.c_str());
	}
	if (_rfApp && _logLevel == OFF_LOG_LEVEL)
	{
		_rfApp = NULL;
	}
}

void CMyLogger::setLogPath(const std::wstring& wstrName) 
{
	if (!_rfApp && _logLevel != OFF_LOG_LEVEL && wstrName.size() != 0)
	{
		_rfApp = SharedAppenderPtr(new RollingFileAppender(wstrName, maxLogFileSize, maxLogFileCount));
		tstring p = L"%D{%Y-%m-%d %H:%M:%S,%q} [%i/%t] %5p - %m (%l)\r\n";
		std::auto_ptr<Layout> pPatternLayout(new PatternLayout (p));
		_rfApp->setLayout(pPatternLayout);
	}

	_logFileName = wstrName;
}

void CMyLogger::setConsoleOutput()
{
	_conApp = SharedAppenderPtr(new ConsoleAppender());
	std::auto_ptr<Layout> pTTCCLayout(new TTCCLayout());
	_conApp->setLayout(pTTCCLayout);
}

void CMyLogger::write(const wchar_t* evt, int level, const char* fname, int line, int process /* = 0 */, int thread /* = 0 */)
{
	int processId = process;
	if (0 == processId)
	{
		processId = GetCurrentProcessId();
	}
	int threadId = thread;
	if (0 == threadId)
	{
		threadId = GetCurrentThreadId();
	}
	InternalLoggingEvent le(_name, level, evt, fname, line, processId, threadId);

	if(_rfApp.get() != NULL)
		_rfApp->doAppend(le);
	if(_conApp.get() != NULL)
		_conApp->doAppend(le);
}

int CMyLogger::toLoggerLevel(int level) {
	switch(level)
	{
	case OM_LOG_OFF:
		return OFF_LOG_LEVEL;
	case OM_LOG_FATAL:
		return FATAL_LOG_LEVEL;
	case OM_LOG_ERROR:
		return ERROR_LOG_LEVEL;
	case OM_LOG_WARN:
		return WARN_LOG_LEVEL;
	case OM_LOG_INFO:
		return INFO_LOG_LEVEL;
	case OM_LOG_DEBUG:
		return DEBUG_LOG_LEVEL;
	case OM_LOG_TRACE:
		return TRACE_LOG_LEVEL;
	default:
		return OFF_LOG_LEVEL;
	}
}


CMyLogger gLogger(std::wstring(L"ORDERMANAGER"));

