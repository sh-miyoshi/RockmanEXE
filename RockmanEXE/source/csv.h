#pragma once

#include <string>
#include <fstream>

class CSV{
public:
	static const int COLUMN_MAX=256;
	enum mode{
		MODE_READ,
		MODE_WRITE
	};
private:
	std::string _m_header;
	bool _m_is_header;
	std::ifstream _m_read_data;
	std::ofstream _m_write_data;
public:
	CSV():_m_is_header(false){}
	CSV(const char *file_name,CSV::mode mode,bool is_header=true){
		open(file_name,mode,is_header);
	}
	CSV(const std::string &file_name,CSV::mode mode,bool is_header=true){
		open(file_name,mode,is_header);
	}
	~CSV(){}

	bool open(const char *file_name,CSV::mode mode,bool is_header=true);
	bool open(const std::string &file_name,CSV::mode mode,bool is_header=true){
		return open(file_name.c_str(),mode,is_header);
	}
	bool close();

	bool isOpen()const{return _m_read_data||_m_write_data;}

	std::string getHeader()const{return _m_header;}
	int getData(std::string *data,int max_size=COLUMN_MAX);
	std::string getLine();
	// ,pushData(string or array);// debug(–¢ŽÀ‘•)
};