#include <stdarg.h>
#include "util.h"
#include "fps.h"
#include "appLogger.h"

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
	std::string res;

	va_list args;
	va_start(args, format);
	int len = _vscprintf(format, args) + 1;
	char* buf = new char[len];
	vsprintf_s(buf, len, format, args);
	res = buf;
	delete[] buf;
	return res;
}

int GetStringByte(char str){
	unsigned char bstr = ( unsigned char ) str;
	// SJISSpΆ»θ
	if( ( 0x81 <= bstr&&bstr <= 0x9f ) || ( 0xE0 <= bstr&&bstr <= 0xff ) ){
		return 2;// Sp
	} else
		return 1;// Όp
}

std::string ConvertToViewCount(unsigned long long count){
	int ms = ( int ) ( ( float ) ( count%Fps::FPS ) / Fps::FPS ) * 100;// ~b(3)
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

