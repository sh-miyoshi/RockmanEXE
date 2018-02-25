#include "include.h"
#include "selectMgr.h"
#include "drawCharacter.h"

namespace{
	const SelectColor DEFAULT_COLOR = {
		BLACK, // strColor
		BLACK, // pointerColor
	};
};

SelectData::SelectData(int x, int y, std::list<std::string> data)
	:x(x), y(y), data(data), color(DEFAULT_COLOR), pointer(0){
}

void SelectData::Draw(){
	int ty = y;
	int index = 0;
	for( auto d : data ){
		if( find<int>(unSelectables, index) ){// 選択できないものは色を落とす
			DrawCharacter::GetInst()->DrawString(x + 20, ty, d, GetColor(128,128,128));
		} else{
			DrawCharacter::GetInst()->DrawString(x + 20, ty, d, color.strColor);
		}
		ty += 25;
		index++;
	}

	// カーソル描画
	const int s = 2;
	ty = y + pointer * 25;
	DrawTriangle(x, ty + s, x + 18 - s * 2, ty + 10, x, ty + 20 - s, color.pointerColor, TRUE);
}

int SelectData::Process(){
	if( CKey::GetInst()->CheckKey(eKEY_ENTER) == 1 ){
		if( find<int>(unSelectables, pointer) )// 選択できない要素なら
			return -1;
		else
			return pointer;
	} else if( CKey::GetInst()->CheckKey(eKEY_UP) == 1 && pointer > 0 ){
		pointer--;
	} else if( CKey::GetInst()->CheckKey(eKEY_DOWN) == 1 && pointer < (int)data.size() - 1 ){
		pointer++;
	}
	return -1;
}

void SelectData::SetUnSelectable(int index){
	if( 0 <= index&&index < ( int ) data.size() )
		unSelectables.push_back(index);
}

void SelectData::SetPointer(int pointer){
	if( 0 <= pointer&&pointer < ( int ) data.size() )
		this->pointer = pointer;
}

SelectMgr::SelectMgr():active(-1){
}

SelectMgr::~SelectMgr(){
}

int SelectMgr::Create(int x, int y, std::list<std::string> data){
	this->data.push_back(SelectData(x, y, data));
	active = this->data.size() - 1;
	return active;
}

int SelectMgr::Create(int x, int y, const std::string * data, unsigned size){
	std::list<std::string> t;
	for( unsigned i = 0; i < size; i++ )
		t.push_back(data[i]);
	return Create(x, y, t);
}

void SelectMgr::Delete(int & no){
	if( 0 <= no&&no < ( int ) data.size() ){
		data[no].Kill();
		if( no == active ){
			active = -1;
			for( unsigned i = data.size() - 1; i >= 0; i-- ){
				if( data[i].IsAlive() ){
					active = ( int ) i;
					break;
				}
			}
		}
	}
	no = -1;
}

bool SelectMgr::SetActive(int active){
	if( 0 <= active&&active < ( int ) data.size() ){
		if( data[active].IsAlive() ){
			this->active = active;
			return true;
		}
	}
	return false;
}

void SelectMgr::Draw(){
	for( auto d : data ){
		d.Draw();
	}
}

int SelectMgr::Process(){
	if( active >= 0 ){
		return data[active].Process();
	}
	return -1;
}

void SelectMgr::SetColor(SelectColor color, int active){
	if( active == -1 )
		active = this->active;
	if( 0 <= active&&active < ( int ) data.size() ){
		data[active].SetColor(color);
	}
}

void SelectMgr::SetUnSelectable(int index, int active){
	if( active == -1 )
		active = this->active;
	if( 0 <= active&&active < ( int ) data.size() ){
		data[active].SetUnSelectable(index);
	}
}

void SelectMgr::SetPointer(int pointer, int active){
	if( active == -1 )
		active = this->active;
	if( 0 <= active&&active < ( int ) data.size() ){
		data[active].SetPointer(pointer);
	}
}
