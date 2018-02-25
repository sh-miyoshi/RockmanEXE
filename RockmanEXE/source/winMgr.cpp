#include "include.h"
#include "winMgr.h"

namespace{
	const int DEFAULT_MESSAGE_SPEED = 2;// debug(ê›íËÇ≈Ç´ÇÈÇÊÇ§Ç…)
};

void WinData::DrawScrollMark(){
}

WinData::WinData(int x, int y, int colNum, int rowNum, std::string msg)
	:x(x), y(y), colNum(colNum), rowNum(rowNum), msg(msg), isAlive(true)
	, msgSpeed(DEFAULT_MESSAGE_SPEED), nowRow(0){
}

void WinData::Draw()
{
}

void WinData::SetFaceImage(std::string fname, CPoint<int> size, CPoint<int> num)
{
}

void WinData::DeleteFaceImage(){
	for( unsigned i = 0; i < imgFace.size(); i++ )
		DeleteGraph(imgFace[i]);
	imgFace.clear();
}

WinMgr::WinMgr()
{
}

WinMgr::~WinMgr()
{
}

int WinMgr::Create(int x, int y, int colNum, int rowNum, std::string msg)
{
	return 0;
}

void WinMgr::Delete(int &no){
	if( 0 <= no&&no < ( int ) data.size() ){
		data[no]->Kill();
		if( no == active ){
			active = -1;
			for( unsigned i = data.size() - 1; i >= 0; i-- ){
				if( data[i]->IsAlive() ){
					active = ( int ) i;
					break;
				}
			}
		}
	}
	no = -1;
}

bool WinMgr::SetActive(int active)
{
	return false;
}

void WinMgr::Draw()
{
}
