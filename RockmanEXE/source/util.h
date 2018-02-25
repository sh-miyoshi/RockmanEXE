#pragma once

#include <stdlib.h>
#include <string>
#include <sstream>
#include <windows.h>
#include <vector>
#include <algorithm>
#include <list>

// cndがfalseの時失敗(ファイルオープン用)
void CheckFileOpen(bool cnd, std::string position, std::string fileName);
void CheckFileOpen(const FILE *fp, std::string position, std::string fileName);

int Split(std::string *buf, int max, std::string data, std::string splitStr);
bool FileExist(std::string fname);

std::string ToString(const char *format, ...);
template<typename _T> inline std::string ToString(_T x){
	std::stringstream ss;
	ss << x;
	return ss.str();
}

template<typename _T> void Shuffle(_T *data, int size){
	// ランダム番目と交換
	for( int i = 0; i < size; i++ ){
		int j = rand() % size;
		_T temp = data[i];
		data[i] = data[j];
		data[j] = temp;
	}
}

int GetStringByte(char str);

std::string ChangeViewCount(unsigned long long count);

template<typename _T> bool find(const std::vector<_T> &data,_T val){
	return ( data.end() != std::find(data.begin(), data.end(), val) );
}

template<typename _T> bool find(const std::list<_T> &data, _T val){
	return ( data.end() != std::find(data.begin(), data.end(), val) );
}

template<typename _T> std::list<_T> merge(std::list<_T> a,std::list<_T> b){
	std::list<_T> ret=a;
	for( std::list<_T>::iterator it = b.begin(); it != b.end(); it++ )
		ret.push_back(*it);
	return ret;
}
