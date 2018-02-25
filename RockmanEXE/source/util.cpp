#include <stdarg.h>
#include "util.h"
#include "fps.h"
#include "appLogger.h"

void CheckFileOpen(bool cnd, std::string position, std::string fileName){
	if( !cnd ){
		std::string message;
		message += "ファイルが開けません。 ";
		message += "場所: " + position + " ";
		message += "ファイル名: " + fileName + "\n";
		AppLogger::Log(AppLogger::eLOG_ERROR, message);
	}
}

void CheckFileOpen(const FILE *fp, std::string position, std::string fileName){
	CheckFileOpen(fp != NULL, position, fileName);
}

bool FileExist(std::string fname){
	FILE *fp;
	fopen_s(&fp, fname.c_str(), "r");
	if( fp ){
		fclose(fp);
		return true;
	} else
		return false;
}

int Split(std::string *buf, int max, std::string data, std::string splitStr){
	int now = 0;
	for( unsigned i = 0; i < data.size() && now < max; i++ ){
		if( data[i] == splitStr[0] ){
			bool flag = true;
			for( unsigned j = 1; j < splitStr.size(); j++ ){
				if( data[i + j] != splitStr[j] ){
					flag = false;
					break;
				}
			}
			if( flag ){
				now++;
				i += ( unsigned int ) splitStr.size() - 1;
				continue;
			}
		}
		buf[now] += data[i];
	}
	return now + 1;
}

std::string ToString(const char *format, ...){
	char buf[256];

	va_list argptr;
	va_start(argptr, format);
	vsprintf_s(buf, format, argptr);
	va_end(argptr);

	return buf;
}

int GetStringByte(char str){
	unsigned char bstr = ( unsigned char ) str;
	// SJIS全角文字判定
	if( ( 0x81 <= bstr&&bstr <= 0x9f ) || ( 0xE0 <= bstr&&bstr <= 0xff ) ){
		return 2;// 全角
	} else
		return 1;// 半角
}

std::string ChangeViewCount(unsigned long long count){
	int ms = ( int ) ( ( float ) ( count%Fps::FPS ) / Fps::FPS ) * 100;// ミリ秒(3桁)
	count /= Fps::FPS;
	int s = count % 60;
	count -= s;
	count /= 60;
	int m = count % 60;
	count -= m;
	char buf[256];
	sprintf_s(buf, "%d:%02d:%02d.%d", ( int ) count / 60, m, s, ms);
	return buf;
}

