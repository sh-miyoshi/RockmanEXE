#include "include.h"
#include "selecter.h"
#include "drawCharacter.h"

Selecter::Selecter():color(WHITE), data(), pointer(0) {
}

Selecter::~Selecter() {
}

void Selecter::SetData(CPoint<int> firstPos, std::vector<std::string> data, int color) {
	this->firstPos = firstPos;
	this->color = color;

	for( auto d : data ) {
		SelectData sd;
		sd.msg = d;
		sd.allowSelect = true;
		this->data.push_back(sd);
	}
}

void Selecter::Draw() {
	for( unsigned int i = 0; i < data.size(); i++ ) {
		CPoint<int> pos(firstPos.x + 20, firstPos.y + i * 25);
		if( data[i].allowSelect ) {
			DrawCharacter::GetInst()->DrawString(pos.x, pos.y, data[i].msg, color);
		} else {
			DrawCharacter::GetInst()->DrawString(pos.x, pos.y, data[i].msg, GetColor(128, 128, 128));
		}
	}

	// カーソル描画
	const int s = 2;
	int tx = firstPos.x;
	int ty = firstPos.y + pointer * 25;
	DrawTriangle(tx, ty + s, tx + 18 - s * 2, ty + 10, tx, ty + 20 - s, color, TRUE);
}

int Selecter::Process() {
	if( CKey::GetInst()->CheckKey(eKEY_ENTER) == 1 ) {
		if( !data[pointer].allowSelect )// 選択できない要素なら
			return -1;
		else
			return pointer;
	} else if( CKey::GetInst()->CheckKey(eKEY_UP) == 1 && pointer > 0 ) {
		pointer--;
	} else if( CKey::GetInst()->CheckKey(eKEY_DOWN) == 1 && pointer < ( int ) data.size() - 1 ) {
		pointer++;
	}
	return -1;
}

void Selecter::SetUnSelectable(int index) {
	if( 0 <= index && index < ( int ) data.size() ) {
		data[index].allowSelect = false;
	} else {
		AppLogger::Warn("SetUnSlectable: 範囲外の部分を選択しようとしました. select: %d, size: %d", index, data.size());
	}
}

void Selecter::SetPointer(int pointer) {
	if( 0 <= pointer && pointer < ( int ) data.size() ) {
		this->pointer = pointer;
	} else {
		AppLogger::Warn("SetUnSlectable: 範囲外の部分を選択しようとしました. select: %d, size: %d", pointer, data.size());
	}
}
