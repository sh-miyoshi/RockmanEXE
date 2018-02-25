#include <algorithm>
#include "csv.h"
#include "util.h"

bool CSV::open(const char *file_name,CSV::mode mode,bool is_header){
	switch(mode){
	case MODE_READ:
		_m_read_data.close();
		_m_read_data.open(file_name);
		_m_is_header=is_header;
		if(_m_read_data){
			if(is_header)
				std::getline(_m_read_data,_m_header);
			return true;
		}
	case MODE_WRITE:
		_m_write_data.close();
		_m_write_data.open(file_name);
		_m_is_header=false;
		if(_m_write_data)
			return true;
	}
	return false;
}

bool CSV::close(){
	if(_m_read_data){
		_m_read_data.close();
		return true;
	}else if(_m_write_data){
		_m_write_data.close();
		return true;
	}else
		return false;
}

int CSV::getData(std::string *data,int max_size){
	std::string temp;
	if(_m_read_data){
		std::getline(_m_read_data,temp);
		for(int i=0;i<max_size;i++)
			data[i].clear();
		if(temp.empty())
			return 0;
		else
			return Split(data,COLUMN_MAX,temp,",");
	}
	return 0;
}

std::string CSV::getLine(){
	std::string temp;
	if(_m_read_data)
		std::getline(_m_read_data,temp);
	return temp;
}
