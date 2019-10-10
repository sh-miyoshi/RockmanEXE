#include "include.h"
#include "effectMgr.h"

//-------------------------------------------------------
// �N���X��`
//-------------------------------------------------------
class EffectHit:public EffectData {
	static const unsigned int IMAGE_DELAY = 1;

	unsigned int count;
	CPoint<int> drawPos;
	int image[6];
public:
	EffectHit(CPoint<int> centerPos, unsigned int rndSize, std::vector<int> image);
	~EffectHit();

	void Draw();
	bool Process();
};

//-------------------------------------------------------
// �O���[�o�����\�b�h
//-------------------------------------------------------
void EffectMgr::LoadImages() {
	const std::string position = "EffectMgr::LoadImages";
	int tmp[10]; // debug(���͍ő�l�ɍ��킹�Č��߂Ă���)

	// ���b�N�o�X�^�[�q�b�g
	std::string fname = def::SKILL_IMAGE_PATH + "���b�N�o�X�^�[_hit.png";
	LoadDivGraphWithErrorCheck(tmp, fname, position, 6, 1, 60, 50);
	for( auto i = 0; i < 6; i++ ) {
		images[eID_���b�N�o�X�^�[_HIT].push_back(tmp[i]);
	}
}

void EffectMgr::DeleteImages() {
	for( unsigned int i = 0; i < eID_MAX; i++ ) {
		for( unsigned int j = 0; j < images[i].size(); j++ ) {
			DeleteGraph(images[i][j]);
		}
	}
}

void EffectMgr::Draw() {
	for( auto e : effectList ) {
		e->Draw();
	}
}

void EffectMgr::Process() {
	for( auto it = effectList.begin(); it != effectList.end();) {
		if( ( *it )->Process() ) {
			it = effectList.erase(it);
		} else {
			it++;
		}
	}
}

void EffectMgr::Register(EffectID id, EffectArg args) {
	switch( id ) {
	case eID_���b�N�o�X�^�[_HIT:
		effectList.push_back(std::shared_ptr<EffectHit>(new EffectHit(args.pos, args.rndSize, images[eID_���b�N�o�X�^�[_HIT])));
		break;
	default:
		AppLogger::Error("Effect %d is not implemented yet", id);
		break;
	}
}

//-------------------------------------------------------
// ���b�N�o�X�^�[�q�b�g
//-------------------------------------------------------
EffectHit::EffectHit(CPoint<int> centerPos, unsigned int rndSize, std::vector<int> image)
	:count(0), image() {

	// �摜�̃Z�b�g
	if( image.size() != 6 ) {
		AppLogger::Error("���b�N�o�X�^�[�̃q�b�g�摜�̖�������v���܂���. Expect: 6, Got: %d", image.size());
	}
	for( int i = 0; i < 6; i++ ) {
		this->image[i] = image[i];
	}

	// �`��ʒu�̌���
	drawPos = centerPos;
	int rndX = rnd_generator() % ( rndSize * 2 );
	int rndY = rnd_generator() % ( rndSize * 2 );
	drawPos.x = drawPos.x - rndSize + rndX;
	drawPos.y = drawPos.y - rndSize + rndY;
}

EffectHit::~EffectHit() {
}

void EffectHit::Draw() {
	unsigned int ino = count / IMAGE_DELAY;
	if( ino > 5 ) {
		ino = 5;
	}
	DrawRotaGraph(drawPos.x, drawPos.y, 1, 0, image[ino], TRUE);
}

bool EffectHit::Process() {
	count++;
	if( count >= ( IMAGE_DELAY * 6 ) ) {
		return true;
	}
	return false;
}
