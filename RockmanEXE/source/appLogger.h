#pragma once

namespace AppLogger {
	// log level (light to heavy,the final is nothing)
	enum LogLevel {
		eLOG_DEBUG,
		eLOG_INFO,
		eLOG_WARN,
		eLOG_ERROR,
		eLOG_NOTHING
	};

	void ChangeLogLevel(LogLevel lv);
	void ChangeForceTerminateLevel(LogLevel lv);
	void ChangeLogFileName(std::string fname);
	void CleanupLogFile();

	void Log(const char* fname, int lineNo, LogLevel logLevel, const char* format, ...);

#define Debug(format, ...) Log(__FILE__, __LINE__, AppLogger::eLOG_DEBUG, format, ##__VA_ARGS__)
#define Info(format, ...) Log(__FILE__, __LINE__, AppLogger::eLOG_INFO, format, ##__VA_ARGS__)
#define Warn(format, ...) Log(__FILE__, __LINE__, AppLogger::eLOG_WARN, format, ##__VA_ARGS__)
#define Error(format, ...) Log(__FILE__, __LINE__, AppLogger::eLOG_ERROR, format, ##__VA_ARGS__)
};
