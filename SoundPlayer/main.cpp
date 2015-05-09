#include "DxLib.h"
#include <string>
using namespace std;

enum {
	SELECT,
	PLAY,
};

class SoundPlayer {
public:
	SoundPlayer( );
	~SoundPlayer( );
	int checkPhase( );
	void select( );
	void play( );
private:
	string _s_name[ 5 ];
	int _sound[ 5 ];
	int _sele_sound[ 2 ];
	int _phase;
	int _sel;
	int _back_count;
	int _sele_count;
	string _r_name;
	string _l_name;
	bool _p_flag;
	bool _r_flag;
	bool _l_flag;
};

SoundPlayer::SoundPlayer( ) {
	_sel = 0;
	_phase = SELECT;
	_p_flag = false;
	_r_flag = false;
	_l_flag = false;
	_back_count = 0;
	_sele_count = 0;
	_s_name[ 0 ] = "sample1";
	_s_name[ 1 ] = "sample2";
	_s_name[ 2 ] = "sample3";
	_s_name[ 3 ] = "engine";
	_s_name[ 4 ] = "garasu";
	_sound[ 0 ] = LoadSoundMem( "resource/sounds/tyuntyun.mp3" );
	_sound[ 1 ] = LoadSoundMem( "resource/sounds/lion1.mp3" );
	_sound[ 2 ] = LoadSoundMem( "resource/sounds/elephant1.mp3" );
	_sound[ 3 ] = LoadSoundMem( "resource/sounds/car-engine1.mp3" );
	_sound[ 4 ] = LoadSoundMem( "resource/sounds/garasu.mp3" );
} 

SoundPlayer::~SoundPlayer( ) {

}

///////////////////////////////////////////////////////////////////////////////
//現在の場面確認用関数
//
int SoundPlayer::checkPhase( ) {
	return _phase;
}

///////////////////////////////////////////////////////////////////////////////
//メニュー画面
//
void SoundPlayer::select( ) {
	//初期化処理
	int Cr = GetColor( 255, 255, 255 );
	SetFontSize( 32 );
	string select[ 5 ] = {
		"  ",
		"  ",
		"  ",
		"  ",
		"  ",
	};

	//ポインタの移動処理
	if ( CheckHitKey( KEY_INPUT_UP ) && !_p_flag ) {
		_p_flag = true;
		_sel--;
		if ( _sel < 0 ) {
			_sel = 0;
		}
	} 
	if ( CheckHitKey( KEY_INPUT_DOWN ) && !_p_flag ) {
		_p_flag = true;
		_sel++;
		if ( _sel > 4 ) {
			_sel = 4;
		}
	}
	select[ _sel ] = "→";
	if ( !CheckHitKey( KEY_INPUT_UP ) && !CheckHitKey( KEY_INPUT_DOWN ) ) { 
		_p_flag = false;
	}

	//画面描画
	DrawString( 250, 240 - 32, "SELECT SOUND", Cr );
	for ( int i = 0; i < 5; i++ ) {
		string disp = select[ i ] + _s_name[ i ]; 
		DrawString( 300, 240 + 16 + ( 32 * i ) , disp.c_str( ), Cr );
	}

	//Soundの選択処理
	if ( ( GetMouseInput() & MOUSE_INPUT_LEFT ) != 0 && _l_flag == false ) {
		_sele_sound[ 0 ] = _sound[ _sel ];
		_l_name = _s_name[ _sel ];
		_l_flag = true;
	}
	if ( ( GetMouseInput() & MOUSE_INPUT_RIGHT ) != 0 && _r_flag == false ) {
		_sele_sound[ 1 ] = _sound[ _sel ];
		_r_name = _s_name[ _sel ];
		_r_flag = true;
	}
	if ( _r_flag == true && _l_flag == true ) {
		_phase = PLAY;
	}
	_sele_count = 0;
}

///////////////////////////////////////////////////////////////////////////////
//プレイ画面
//
void SoundPlayer::play( ) {
	int Cr = GetColor( 255, 255, 255 );
	SetFontSize( 64 );
	string disp1 = "NOW_PLAY_" + _r_name;
	string disp2 = "         " + _l_name;
	DrawString( 360, 340, disp1.c_str( ), Cr );
	DrawString( 360, 410, disp2.c_str( ), Cr );


	//押されている
	if(( GetMouseInput() & MOUSE_INPUT_LEFT ) != 0 ) {
		PlaySoundMem( _sele_sound[ 0 ], DX_PLAYTYPE_NORMAL );			
	}
	if ( ( GetMouseInput() & MOUSE_INPUT_RIGHT ) != 0 ) {
		PlaySoundMem( _sele_sound[ 1 ], DX_PLAYTYPE_NORMAL );
	}
 	//押されていない
	if ( CheckHitKey( KEY_INPUT_BACK ) ) {
		if ( _back_count > 100 ) {
			for ( int i = 0; i < 2; i++ ) {
				_sele_sound[ i ] = 0;
			}
			_phase = SELECT;
		}
		_back_count++;
		return;
	}		
	_back_count = 0;
}

///////////////////////////////////////////////////////////////////////////////
//メインループ
//
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
						LPSTR lpCmdLine, int nCmdShow )
{
	ChangeWindowMode( TRUE ) ;
	int screen_size_x;
	int screen_size_y;
	GetDefaultState( &screen_size_x, &screen_size_y, NULL );
	SetGraphMode( screen_size_x, screen_size_y, 32 );
	if( DxLib_Init() == -1 )		// ＤＸライブラリ初期化処理
	{
		return -1 ;			// エラーが起きたら直ちに終了
	}
	SoundPlayer obj;
	
	SetDrawScreen( DX_SCREEN_BACK );
	
	//メインループ
	while( true ) {
		ClearDrawScreen( );
		switch ( obj.checkPhase( ) ) {
			case SELECT:
				obj.select( );
				break;
			case PLAY:
				obj.play( );
				break;
		}
		ScreenFlip( );
		if ( CheckHitKey( KEY_INPUT_ESCAPE ) ) {
			break;
		}
	}

	DxLib_End() ;	// ＤＸライブラリ使用の終了
	return 0 ;		// ソフトの終了 
}