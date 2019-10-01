#include <string>
#include <time.h>
#include <sstream>
#include <stdarg.h> 
#include "appLogger.h"

static AppLogger::LogLevel WRITE_LOG_LEVEL = AppLogger::eLOG_DEBUG;
static AppLogger::LogLevel FORCE_TERMINATE_LEVEL = AppLogger::eLOG_ERROR;
static std::string LOG_FILE_NAME = "application.log";

void AppLogger::ChangeLogLevel(AppLogger::LogLevel lv) {
	WRITE_LOG_LEVEL = lv;
}

void AppLogger::ChangeForceTerminateLevel(AppLogger::LogLevel lv) {
	FORCE_TERMINATE_LEVEL = lv;
}

void AppLogger::ChangeLogFileName(std::string fname) {
	LOG_FILE_NAME = fname;
}

void AppLogger::CleanupLogFile() {
	remove(LOG_FILE_NAME.c_str());
}

void AppLogger::Log(const char* fname, int lineNo, LogLevel logLevel, const char* format, ...) {
	if( logLevel >= WRITE_LOG_LEVEL ) {
		FILE* fp;
		fopen_s(&fp, LOG_FILE_NAME.c_str(), "a");
		if( fp ) {
			// format: [INFO] year-month-day hour:min:sec fname:lineNo message
			std::stringstream output;

			// output log level
			output << "[";
			switch( logLevel ) {
			case eLOG_DEBUG:
				output << "DEBUG";
				break;
			case eLOG_INFO:
				output << "INFO";
				break;
			case eLOG_WARN:
				output << "WARN";
				break;
			case eLOG_ERROR:
				output << "ERROR";
				break;
			}
			output << "] ";

			// output date
			time_t now = time(NULL);
			struct tm t;
			localtime_s(&t, &now);
			output << ( t.tm_year + 1900 ) << "-" << ( t.tm_mon + 1 ) << "-" << t.tm_mday << " ";
			output << t.tm_hour << ":" << t.tm_min << ":" << t.tm_sec << " ";

			// output file name and line number
			output << fname << ":" << lineNo << " ";

			// output message
			va_list args;
			va_start(args, format);
			int len = _vscprintf(format, args) + 1;
			char* buf = new char[len];
			vsprintf_s(buf, len, format, args);
			output << buf << "\n";
			delete[] buf;

			fputs(output.str().c_str(), fp);
			fclose(fp);
			if( logLevel >= FORCE_TERMINATE_LEVEL )
				exit(1);
		}
	}
}
