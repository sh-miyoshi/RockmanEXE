#pragma once

#include <string>

/*
 * TODO(new logger example)
 * namespace AppLogger{
 *     void Log(const char *fname, const char *function, int lineNo, const char *type, const char *message){
 *             std::cout << fname << " " << function << " " << lineNo << " " << type << " " << message << std::endl;
 *                 }
 *
 *                     #define Info(msg) Log(__FILE__, __func__, __LINE__, "[INFO]", msg)
 *                     };
*/

class AppLogger{
public:
	// log level (light to heavy,the final is nothing)
	enum LogLevel{
		eLOG_DEBUG,
		eLOG_WARN,
		eLOG_ERROR,
		eLOG_NOTHING
	};
private:
	static LogLevel WRITE_LOG_LEVEL;
	static LogLevel FORCE_TERMINATE_LEVEL;
	static std::string LOG_FILE_NAME;
public:
	static void ChangeLogLevel(LogLevel lv);
	static void ChangeForceTerminateLevel(LogLevel lv);
	static void ChangeLogFileName(std::string fname);

	static bool Log(LogLevel logLevel, std::string message);
	static bool Debug(std::string message);
	static bool Warn(std::string message);
	static bool Error(std::string message);

	static void CleanupLogFile();
};
