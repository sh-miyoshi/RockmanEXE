#include "include.h"
#include "battleObject.h"
#include "battleField.h"

//---------------------------------------------
// 外部関数
//---------------------------------------------
BattleObject::BattleObject(int hp, CPoint<int> pos, def::charType damagedType,def::charType myCharType)
	:hp(hp), hpMax(hp), damagedType(damagedType), pos(pos), delFlag(false)
	, actCount(0), atachedAnimNo(-1), myCharType(myCharType)
{
	for( int i = 0; i < ANIM_PTN_MAX; i++ ) {
		anim[i].imageNum = -1;
		for( int j = 0; j < AnimData::EACH_ANIM_MAX; j++ ){
			anim[i].image[j] = -1;
		}
		anim[i].imageDelay = 1;
		anim[i].animProc = AnimProcess::Create();
	}

	// アニメーション定義なしでも呼ばれる可能性があるやつは0を指定
	anim[ANIM_DAMAGE].imageNum = 0;

	// オブジェクト固有のIDを設定
	objectID = IDManager::CreateUniqueID();
}

BattleObject::BattleObject(const BattleObject &data)
	:hp(data.hp), hpMax(data.hpMax), damagedType(data.damagedType)
	, pos(data.pos), delFlag(data.delFlag), actCount(data.actCount)
	, atachedAnimNo(data.atachedAnimNo)
{
	for( int i = 0; i < ANIM_PTN_MAX; i++ ){
		anim[i] = data.anim[i];
	}
}

void BattleObject::StopAnim(){
	if( atachedAnimNo != -1 ){
		anim[atachedAnimNo].animProc->End();// 後始末
		atachedAnimNo = -1;// アニメーションのデタッチ
	}
}

void BattleObject::Draw(){
	int x = BattleField::PANEL_SIZE.x*pos.x + BattleField::PANEL_SIZE.x / 2;
	int y = BattleField::BATTLE_PANEL_TOP + BattleField::PANEL_SIZE.y*pos.y - 10;// ふちの分だけ上げる
	DrawAnim(x, y);
}

void BattleObject::DrawAnim(int x, int y){
	int ino = -1;

	if( atachedAnimNo != -1 ){
		int n = actCount / anim[atachedAnimNo].imageDelay;
		if( n >= anim[atachedAnimNo].imageNum )
			n = anim[atachedAnimNo].imageNum - 1;
		ino = anim[atachedAnimNo].image[n];
	} else// 動いてないときはMOVEの0を再生
		ino = anim[ANIM_MOVE].image[0];

	if( ino == -1 )// イメージがなければ
		ino = anim[ANIM_MOVE].image[0];// 静止画像を表示

	DrawRotaGraph(x + drawPosOffset.x, y + drawPosOffset.y, 1, 0, ino, TRUE);

	if( atachedAnimNo != -1 ) {
		ASSERT(anim[atachedAnimNo].imageNum != -1,"BattleObject::DrawAnim the image is not registerd attach no = "+ToString<int>(atachedAnimNo));
		anim[atachedAnimNo].animProc->Draw();
	}
}

void BattleObject::Process(){
}

bool BattleObject::DamageProc(std::list<DamageData>& damageList){
	if( hp <= 0 ){
		return false;
	}

	for( std::list<DamageData>::iterator it = damageList.begin(); it != damageList.end(); it++ ){
		bool damageFlag = ( it->myCharType&damagedType ) != 0;// damagedType=myCharTypeなら攻撃を受ける

		if( damageFlag&&it->pos == pos && !it->forceDelFlag ){// もし敵のダメージが自分の位置にあるなら
			it->forceDelFlag = true;// ダメージが連続で多段ヒットしないように削除
			hp -= it->damage;
			if( hp<0 )
				hp = 0;
			else if( hp>hpMax )// ダメージがマイナス(回復)の場合を考慮して
				hp = hpMax;
			if( it->damage>0 ){
				if( it->makeMuteki ){// 無敵化攻撃
					DamageMutekiFunc();
					return true;// のけぞり処理
				}
			}
		}
	}
	return false;
}

void BattleObject::DeleteAnim(){
	for( int i = 0; i<ANIM_PTN_MAX; i++ ) {
		anim[i].imageNum = -1;
		for( int j = 0; j<AnimData::EACH_ANIM_MAX; j++ ) {
			if( anim[i].image[j] != -1 ) {
				DeleteGraph(anim[i].image[j]);
				anim[i].image[j] = -1;
			}
		}
	}
}

void BattleObject::SetHP(int hp){
	if( 0 <= hp&&hp < this->hpMax )
		this->hp = hp;
	else{
		std::string message = "不適切なHPがセットされようとしました。";
		message += ToString(" HP/HPMAX: %d/%d ", hp, hpMax);
		message += " 場所: BattleObject::SetHP";
		AppLogger::Warn(message);
	}
}

void BattleObject::AddHP(int addVal){
	hp += addVal;
	if( hp < 0 )
		hp = 0;
	else if( hp > hpMax )
		hp = hpMax;
}

//---------------------------------------------
// 内部関数
//---------------------------------------------
void BattleObject::SetAnim(int animNo, bool animSetMaxFlag){
	ASSERT(0 <= animNo&&animNo<ANIM_PTN_MAX,ToString("BattleObject::SetAnim wrong animNo (%d)",animNo));
	ASSERT(anim[animNo].imageNum != -1, "BattleObject::SetAnim the image is not registerd anim no = " + ToString<int>(animNo));

	actCount = 0;
	atachedAnimNo = animNo;

	if( animSetMaxFlag )
		this->animNum = anim[animNo].imageNum;
	else
		this->animNum = AnimData::EACH_ANIM_MAX - 1;
}

bool BattleObject::AnimProcess(){
	if( atachedAnimNo != -1 ) {
		ASSERT(anim[atachedAnimNo].imageNum != -1, "BattleObject::AnimProcess the image is not registerd attach no = " + ToString<int>(atachedAnimNo));

		if( actCount == 0 )// アニメーションをアタッチした瞬間なら
			anim[atachedAnimNo].animProc->Begin();
		anim[atachedAnimNo].animProc->Process();
		actCount++;

		if( actCount<( animNum*anim[atachedAnimNo].imageDelay ) - 1 ) {
			return false;
		} else {
			anim[atachedAnimNo].animProc->End();// 後始末
			atachedAnimNo = -1;// アニメーションのデタッチ
			return true;// 行動できる
		}
	}
	return true;// 行動できる
}

AnimData::AnimData(const AnimData &data)
	:imageNum(data.imageNum), imageDelay(data.imageDelay), animProc(data.animProc)
{
	for( int i = 0; i < EACH_ANIM_MAX; i++ ){
		image[i] = data.image[i];
	}
}
