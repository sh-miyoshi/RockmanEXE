#include "include.h"
#include "utilDxLib.h"

int LoadGraphWithErrorCheck(std::string fileName, std::string position){
	int handle = LoadGraph(fileName.c_str());
	if( handle == -1 ){
		std::string message;
		message += "LoadGraph�G���[�����B ";
		message += "�ꏊ: " + position + " ";
		message += "�t�@�C����: " + fileName + "\n";
		AppLogger::Error(message.c_str());
	}
	return handle;
}

void LoadDivGraphWithErrorCheck(int * handle, std::string fileName, std::string position, int xnum, int ynum, int xsize, int ysize){
	int ret = LoadDivGraph(fileName.c_str(), xnum*ynum, xnum, ynum, xsize, ysize, handle);
	if( ret == -1 ){
		std::string message;
		message += "LoadDivGraph�G���[�����B ";
		message += "�ꏊ: " + position + " ";
		message += "�t�@�C����: " + fileName + "\n";
		AppLogger::Error(message.c_str());
	}
}

int LoadSoundMemWithErrorCheck(std::string fileName, std::string position){
	int handle = LoadSoundMem(fileName.c_str());
	if( handle == -1 ){
		std::string message;
		message += "LoadSoundMem�G���[�����B ";
		message += "�ꏊ: " + position + " ";
		message += "�t�@�C����: " + fileName + "\n";
		AppLogger::Error(message.c_str());
	}
	return handle;
}
