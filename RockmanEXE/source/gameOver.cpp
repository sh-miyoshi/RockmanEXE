#include "include.h"
#include "gameOver.h"

GameOver::GameOver():count(0),backImage(-1) {
	std::string fname = def::IMAGE_FILE_PATH + "game_over/game_over_back.png";
	backImage = LoadGraphWithErrorCheck(fname, "Main::StateGameOver::StateGameOver");

	// block size: 4,7,8
	const int init_rand_range = ABS_BOX_RANGE_SIZE * 2 + def::FMX;
	for( int y = 0; y < 19; y++ ) {
		// rnd_x = random value in left_screen - FMX to right_screen + FMX
		int rnd_x = rand() % init_rand_range - ABS_BOX_RANGE_SIZE;
		int block_size = 4;
		int ty = y * BACK_BOX_SIZE_Y + OFFSET_UP;
		for( int i = 0; i < block_size; i++ ) {
			back_box_pos[y][0].push_back(CPoint<int>(rnd_x + i * BACK_BOX_SIZE_X, ty));
		}

		// back_box_pos[y][1-2] is calclated by back_box_pos[y][0]
		const int space = def::FMX * 6 / 7;
		for( int i = 1; i < 3; i++ ) {
			block_size = 6 + i;
			CPoint<int> t = back_box_pos[y][0][0];
			t.x += space * i;// add space
			for( int j = 0; j < block_size; j++ ) {
				t.x += BACK_BOX_SIZE_X;// add block length
				back_box_pos[y][i].push_back(t);
			}
		}
	}

	ReshapePosition();

	std::string init_pos = "Init GameOver Block Position:\n";
	int max_x = -ABS_BOX_RANGE_SIZE, min_x = def::FMX + ABS_BOX_RANGE_SIZE;
	for( int y = 0; y < 19; y++ ) {
		init_pos += "  [";
		for( int n = 0; n < 3; n++ ) {
			init_pos += " [";
			for( std::vector<CPoint<int>>::iterator it = back_box_pos[y][n].begin(); it != back_box_pos[y][n].end(); it++ ) {
				init_pos += ToString("(%d,%d),", it->x, it->y);
				if( it->x > max_x )
					max_x = it->x;
				if( it->x < min_x )
					min_x = it->x;
			}
			init_pos.erase(--init_pos.end());// 最後のカンマを削除
			init_pos += "],";
		}
		init_pos.erase(--init_pos.end());// 最後のカンマを削除
		init_pos += "]\n";
	}
	init_pos += ToString("    max x: %d, min x: %d\n", max_x, min_x);
	AppLogger::Debug(init_pos.c_str());
}

GameOver::~GameOver() {
	DeleteGraph(backImage);
}

void GameOver::Draw() {
	DrawBox(0, 0, def::FMX, def::FMY, GetColor(128, 24, 96), TRUE);
	BoxDraw();

	DrawGraph(0, 0, backImage, TRUE);
}

bool GameOver::Process() {
	// BOXを移動させる処理
	const int slide_value = 2;
	for( int y = 0; y < 19; y++ ) {
		for( int n = 0; n < 3; n++ ) {
			for( std::vector<CPoint<int>>::iterator it = back_box_pos[y][n].begin(); it != back_box_pos[y][n].end(); it++ ) {
				it->x -= slide_value;
			}
		}
	}
	ReshapePosition();

	// キーが押されたらタイトルに戻る
	if( count > 60 ) {
		if( CKey::GetInst()->CheckKey(eKEY_CANCEL) != 0 || CKey::GetInst()->CheckKey(eKEY_ENTER) != 0 ) {
			return true;
		}
	}
	count++;
	return false;
}

void GameOver::ReshapePosition() {
	for( int y = 0; y < 19; y++ ) {
		for( int n = 0; n < 3; n++ ) {
			for( std::vector<CPoint<int>>::iterator it = back_box_pos[y][n].begin(); it != back_box_pos[y][n].end(); it++ ) {
				// xが許容範囲外にあるなら戻す
				while( it->x < -ABS_BOX_RANGE_SIZE )
					it->x += ( def::FMX + ABS_BOX_RANGE_SIZE );
				while( it->x > def::FMX + ABS_BOX_RANGE_SIZE )
					it->x -= ( def::FMX + ABS_BOX_RANGE_SIZE * 2 );
			}
		}
	}
}

void GameOver::BoxDraw() {
	for( int y = 0; y < 19; y++ ) {
		for( int n = 0; n < 3; n++ ) {
			for( std::vector<CPoint<int>>::iterator it = back_box_pos[y][n].begin(); it != back_box_pos[y][n].end(); it++ ) {
				if( it->x + BACK_BOX_SIZE_X > 0 && it->x < def::FMX ) {// 描画範囲内にあるなら
					DrawBox(it->x, it->y, it->x + BACK_BOX_SIZE_X, it->y + BACK_BOX_SIZE_Y, GetColor(160, 56, 128), TRUE);
				}
			}
		}
	}
}
