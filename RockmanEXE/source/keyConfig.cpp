#include <fstream>
#include "include.h"
#include "keyConfig.h"

const std::string CKey::CONFIG_FILE_NAME = "data/text/config.dat";
const std::string CKey::CODE_TABLE_FILE_NAME = "data/text/codeTable.txt";

CKey::CKey(){
	// コード表の読み込み
	std::ifstream ifs(CODE_TABLE_FILE_NAME);
	if( !ifs ) {
		AppLogger::Error("Key Config File[%s]が開けませんでした", CODE_TABLE_FILE_NAME);
	}
	std::string buf;
	for( int i = 0; i < 256 && !std::getline(ifs, buf); i++ )
		codeTable[i] = buf;

	// データの読み込み
	ifs.close();
	ifs.open(CONFIG_FILE_NAME, std::ios::binary);
	if( ifs ){
		for( int i = 0; i < eKEY_MAX; i++ )
			ifs.read(( char * ) &keyInfo[i], sizeof(int));
	} else{
		// デフォルト値を設定
		keyInfo[eKEY_ENTER] = KEY_INPUT_Z;
		keyInfo[eKEY_CANCEL] = KEY_INPUT_X;
		keyInfo[eKEY_RIGHT] = KEY_INPUT_RIGHT;
		keyInfo[eKEY_LEFT] = KEY_INPUT_LEFT;
		keyInfo[eKEY_DOWN] = KEY_INPUT_DOWN;
		keyInfo[eKEY_UP] = KEY_INPUT_UP;
		keyInfo[eKEY_MENU] = KEY_INPUT_M;
		keyInfo[eKEY_START] = KEY_INPUT_ESCAPE;
		keyInfo[eKEY_DEV_L] = KEY_INPUT_A;
		keyInfo[eKEY_DEV_R] = KEY_INPUT_S;
	}
}

CKey::~CKey(){
}

void CKey::Update(){
	char temp[256];
	GetHitKeyStateAll(temp);
	for( int i = 0; i < 256; i++ ){
		if( temp[i] == 1 ) tempKey[i]++;
		else tempKey[i] = 0;
	}

	for( int i = 0; i < eKEY_MAX; i++ )
		stateKey[i] = tempKey[keyInfo[i]];
}

int CKey::GetNowInput(){
	for( int i = 0; i < 256; i++ )
		if( tempKey[i] == 1 )
			return i;
	return -1;
}

void CKey::Change(int handle, int code){
	if( handle == eKEY_START )// スタートキーは変更できない
		return;
	// もしそのコードが既に使われていたら変更しない
	for( int i = 0; i < eKEY_MAX; i++ )
		if( keyInfo[i] == code )
			return;
	keyInfo[handle] = code;
}

void CKey::SaveKeySetting(){
	// データの保存
	std::ofstream ofs(CONFIG_FILE_NAME, std::ios::binary);
	if( ofs )
		for( int i = 0; i < eKEY_MAX; i++ )
			ofs.write(( char * ) &keyInfo[i], sizeof(int));
}
