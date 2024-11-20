#include<DxLib.h>
#include "../Manager/ResourceManager.h"
#include "UserInterface.h"
#include"../../Config.h"

#pragma region Parameter

//シングルモード時の勝利または敗北画像の座標
constexpr VECTOR SINGLEMODE_IMG_POS = { 800.0f,400.0f ,0.0f };

constexpr VECTOR MULTI_MODE_IMG_POS = { 400.0f,400.0f ,0.0f };

//敵ターゲット画像の座標調整用
constexpr float TARGET_OFFSET_Y = 100.0f;

//敵ターゲット画像の座標調整用
constexpr float ENEMY_HP_GAUGE_OFFSET_Y = 400.0f;

 //ブーストゲージ画像の座標
constexpr VECTOR BOOST_GAUGE_IMG_POS_MULTI= { 400.0f,850.0f,0.0f };
constexpr VECTOR BOOST_GAUGE_IMG_POS_SINGLE = { 1100.0f,850.0f,0.0f };
//大きさ
constexpr double BOOST_GAUGE_CASE_SCALE_SINGLE= 2.5;
constexpr double BOOST_GAUGE_CASE_SCALE_MULTI= 2.0;
//オフセット
constexpr int BOOST_GAUGE_OFFSET_X_SINGLE = 5;
constexpr int BOOST_GAUGE_OFFSET_Y_SINGLE = 40;
constexpr int BOOST_GAUGE_OFFSET_X_MULTI = 4;
constexpr int BOOST_GAUGE_OFFSET_Y_MULTI = 30;

//HP表示の数値
constexpr VECTOR HP_VALUE_POS_MULTI = {170.0f,850.0f,0.0f};
constexpr VECTOR HP_VALUE_POS_SINGLE = {250.0f,850.0f,0.0f};
//HP表示の大きさ
constexpr float HP_VALUE_SCALE_SINGLE = 2.0f;
constexpr float HP_VALUE_SCALE_MULTI = 1.7f;
//HP表示の大きさ
constexpr int HP_VALUE_OFFSET_SINGLE = 120;
constexpr int HP_VALUE_OFFSET_MULTI = 100;

//HP背面画像表示の数値
constexpr VECTOR HP_BACK_POS_MULTI = { 170.0f,850.0f,0.0f };
constexpr VECTOR HP_BACK_POS_SINGLE = { 250.0f,850.0f,0.0f };
//HP背面画像の大きさ
constexpr float HP_BACK_SCALE_SINGLE = 1.2f;
constexpr float HP_BACK_SCALE_MULTI = 1.0f;
//HP背面画像中心の数値
constexpr int HP_BACK_CENTER_X = 160;
constexpr int HP_BACK_CENTER_Y = 80;

//残弾表示の数値
constexpr VECTOR BULLET_POS_MULTI = { 600.0f,690.0f,0.0f };
constexpr VECTOR  BULLET_POS_SINGLE = { 1300.0f,690.0f,0.0f };
//残弾表示の大きさ
constexpr float BULLET_SCALE_SINGLE = 1.0f;
constexpr float BULLET_SCALE_MULTI = 0.7f;
//残弾表示の大きさ
constexpr float  BULLET_OFFSET_SINGLE = 120;
constexpr float  BULLET_OFFSET_MULTI = 100;
//残弾表示の数値
constexpr int BULLET_CENTER_X = 40;
constexpr int BULLET_CENTER_Y = 40;
constexpr int BULLET_OFFSET = 10;

//ライフル表示の数値
constexpr VECTOR RIFLE_POS_MULTI = { 670.0f,670.0f,0.0f };
constexpr VECTOR  RIFLE_POS_SINGLE = { 1420.0f,660.0f,0.0f };
//ライフル表示の大きさ
constexpr float RIFLE_SCALE_SINGLE = 0.6f;
constexpr float RIFLE_SCALE_MULTI = 0.4f;
//ライフル表示の大きさ
constexpr float  RIFLE_OFFSET = 10;
//ライフル表示の数値
constexpr int RIFLE_CENTER_X = 320;
constexpr int RIFLE_CENTER_Y =160;

 //タイトルに戻る画像
constexpr VECTOR BACK_TO_TITLE_POS = { 800.0f,500.0f,0.0f };
constexpr int BACK_TO_TITLE_CENTER_X = 250;
constexpr int BACK_TO_TITLE_CENTER_Y = 50;
constexpr double BACK_TO_TITLE_SCALE = 1.5;

 //もう一戦画像
constexpr VECTOR ONE_MOR_FIGHT_POS = { 800.0f,700.0f };
constexpr int ONE_MOR_FIGHT_CENTER_X = 273;
constexpr int ONE_MOR_FIGHT_CENTER_Y = 54;
constexpr double ONE_MOR_FIGHT_SCALE = 1.5;

 //×ボタンを押してくれ画像
constexpr VECTOR PLEASE_CROSS_BUTTON_POS = { 800.0f,900.0f };
constexpr int PLEASE_CROSS_BUTTON_CENTER_X = 480;
constexpr int PLEASE_CROSS_BUTTON_CENTER_Y = 52;
constexpr double PLEASE_CROSS_BUTTON_SCALE = 1.2;

 //Aボタンを押してくれ画像
constexpr VECTOR PLEASE_A_BUTTON_POS = { 800.0f,800.0f };
constexpr int PLEASE_A_BUTTON_CENTER_X = 395;
constexpr int PLEASE_A_BUTTON_CENTER_Y = 34;
constexpr double  PLEASE_A_BUTTON_SCALE = 1.2;

 //セレクト画像の数値
constexpr VECTOR TRIANGLE_POS = { 1200.0f,500.0f };
constexpr int  TRIANGLE_POS_X = 1200;
constexpr int  TRIANGLE_POS_Y = 500;
constexpr int  TRIANGLE_CENTER_X = 280;
constexpr int  TRIANGLE_CENTER_Y = 63;
constexpr int  TRIANGLE_OFFSET = 200;
constexpr double TRIANGLE_SCALE = 1.2;

 //ゲーム開始時に出るREADYの画像の数値
constexpr int READY_POS_X = 800;
constexpr int  READY_POS_Y = 400;
constexpr int  READY_CENTER_X = 117;
constexpr int  READY_CENTER_Y = 44;
constexpr int  READY_OFFSET = 200;
constexpr double  READY_SCALE = 3.0;

 //ゲーム開始時に出るFIGHTの画像の数値
constexpr int FIGHT_POS_X = 800;
constexpr int  FIGHT_POS_Y = 400;
constexpr int  FIGHT_CENTER_X = 120;
constexpr int  FIGHT_CENTER_Y = 49;
constexpr int  FIGHT_OFFSET = 200;
constexpr double  FIGHT_SCALE = 4.5;

 //ゲームスタート開始秒数
constexpr float START_TIME_MAX = 3.0f;

 //対戦開始時「READY」画像表示秒数
constexpr float READY_TIME_MAX = 1.5f;

 //画面分割用の線の太さ
constexpr int  LINE_THICKNES = 1000;

 //プレイヤー１のタイプ
constexpr int PLAYER_TYPE = 0;

 //プレイモード(シングルプレイ)
constexpr int PLAY_MODE = 0;

 //プレイヤー１のPADナンバー
constexpr int PAD_NUM = 1;

 //敵HP
constexpr int ENEMY_HP = 1000;
 //ボスHP
constexpr int BOSS_HP = 500;

#pragma endregion


UserInterface::UserInterface(
	int playMode,
	int playerType,
	const float& distance,
	const int& bulletNum,
	const float& boostGauge,
	const float& hpGauge,
	const bool& isWin,
	const VECTOR& enemyPos,
	const float& enemyHp) :
	resMng_(ResourceManager::GetInstance()), enemyPos_(enemyPos), enemyDistance_(distance),
	boostGauge_(boostGauge), hpGauge_(hpGauge),enemyHpGauge_(enemyHp), isWin_(isWin), numnberofBullets_(&bulletNum)
{
	//プレイモードを保存
	playMode_ = playMode;
	
	//コントローラーのタイプを格納(PSかXBOX)
	joyPadType_ = static_cast<JOYPAD_TYPE>(GetJoypadType(PAD_NUM));
	
	//対戦終了時のモード選択の初期化(初期カーソルrはタイトルへ戻る)
	rematchMode_ = 0;
	
	//ゲームスタート時のカウントの初期化
	startCount_ = 0.0f;

	//画像の読み込み
	InitImage();

	//プレイヤータイプがプレイヤー1の時だけ
	if (playerType == PLAYER_TYPE)
	{
		//共通UIの初期化
		InitCommonUI();
	}

}

UserInterface::~UserInterface()
{
}

void UserInterface::Draw(bool& gameSet)
{
	//まだ勝敗が付いていなければ表示
	if (!gameSet)
	{
		//ブーストゲージ表示
		DrawBoostGauge();
		//HPゲージ表示
		DrawHPGauge();
		//照準の表示
		DrawTarget();
		//残段数の表示
		DrawNumnberOfBullets();
	}
	else
	{
		//決着がついたら表示
		VictoryOrDefeat();
	}
}

void UserInterface::DrawCommonUI(const float& startCount, const bool& isGameSet, const int& rematchMode)
{
	//ゲームスタート時UI
	DrawGameUIStart(startCount);
	//ゲーム終了時UI
	DrawUIGameSet(isGameSet, rematchMode);
}

void UserInterface::InitImage(void)
{
	//画像の読み込み
	uiImgH_.emplace(IMG_TYPE::WIN, resMng_.Load(ResourceManager::SRC::WIN).handleId_);
	uiImgH_.emplace(IMG_TYPE::LOSE, resMng_.Load(ResourceManager::SRC::LOSE).handleId_);
	uiImgH_.emplace(IMG_TYPE::TARGERT, resMng_.Load(ResourceManager::SRC::TARGET).handleId_);
	uiImgH_.emplace(IMG_TYPE::TARGET_RED, resMng_.Load(ResourceManager::SRC::TARGET_RED).handleId_);
	uiImgH_.emplace(IMG_TYPE::RIFLE_IMAGE, resMng_.Load(ResourceManager::SRC::RIFLE_IMAGE).handleId_);
	uiImgH_.emplace(IMG_TYPE::BOOST_GAUGE, resMng_.Load(ResourceManager::SRC::BOOSTGAUGE_IMAGE).handleId_);
	uiImgH_.emplace(IMG_TYPE::ENEMY_HP_GAUGE, resMng_.Load(ResourceManager::SRC::ENEMY_HP_GAUGE).handleId_);
	uiImgH_.emplace(IMG_TYPE::BOOST_GAUGE_CASE, resMng_.Load(ResourceManager::SRC::BOOSTGAUGE_CASE_IMAGE).handleId_);
	uiImgH_.emplace(IMG_TYPE::HP_BACK, resMng_.Load(ResourceManager::SRC::HP_BACK).handleId_);
	numberImg_ = resMng_.Load(ResourceManager::SRC::NUMBER).handleIds_;

	//タイトルへ戻る画像の情報
	imgInfo_.emplace(IMG_TYPE::BACK_TO_TITLE,
		Info(BACK_TO_TITLE_POS,
			BACK_TO_TITLE_SCALE,
			BACK_TO_TITLE_CENTER_X,
			BACK_TO_TITLE_CENTER_Y));
	//もう一戦画像の情報
	imgInfo_.emplace(IMG_TYPE::ONE_MOR_FIGHT,
		Info(ONE_MOR_FIGHT_POS,
			ONE_MOR_FIGHT_SCALE,
			ONE_MOR_FIGHT_CENTER_X,
			ONE_MOR_FIGHT_CENTER_Y));
	//選択画像の情報
	imgInfo_.emplace(IMG_TYPE::TRIANGLE,
		Info(TRIANGLE_POS,
			TRIANGLE_SCALE,
			TRIANGLE_CENTER_X,
			TRIANGLE_CENTER_Y,
			TRIANGLE_OFFSET));

	//HPゲージ画像の情報
	imgInfo_.emplace(IMG_TYPE::PLEASE_A,
		Info(PLEASE_A_BUTTON_POS,
			PLEASE_CROSS_BUTTON_SCALE,
			PLEASE_A_BUTTON_CENTER_X,
			PLEASE_A_BUTTON_CENTER_Y,
			PLEASE_A_BUTTON_SCALE));
	
	imgInfo_.emplace(IMG_TYPE::PLEASE_CROSS,
		Info(PLEASE_CROSS_BUTTON_POS,
			PLEASE_CROSS_BUTTON_SCALE,
			PLEASE_CROSS_BUTTON_CENTER_X,
			PLEASE_CROSS_BUTTON_CENTER_Y,
			PLEASE_CROSS_BUTTON_SCALE));

	//シングルプレイモード時の初期化
	if (playMode_ == PLAY_MODE)
	{
		//HPゲージ画像の情報
		imgInfo_.emplace(IMG_TYPE::HP_BACK,
			Info(HP_BACK_POS_SINGLE,
				HP_BACK_SCALE_SINGLE,
				HP_BACK_CENTER_X,
				HP_BACK_CENTER_Y));
		//残弾数表示の情報
		imgInfo_.emplace(IMG_TYPE::BULLET_NUM,
			Info(BULLET_POS_SINGLE,
				BULLET_SCALE_SINGLE,
				BULLET_CENTER_X,
				BULLET_CENTER_Y, 
				BULLET_OFFSET_SINGLE));
		//ライフル画像情報
		imgInfo_.emplace(IMG_TYPE::RIFLE_IMAGE,
			Info(RIFLE_POS_SINGLE,
				RIFLE_SCALE_SINGLE,
				RIFLE_CENTER_X,
				RIFLE_CENTER_Y));
		//ブートゲージ画像情報
		imgInfo_.emplace(IMG_TYPE::BOOST_GAUGE,
			Info(BOOST_GAUGE_IMG_POS_SINGLE,
				BOOST_GAUGE_CASE_SCALE_SINGLE,
				BOOST_GAUGE_OFFSET_X_SINGLE,
				BOOST_GAUGE_OFFSET_Y_SINGLE));

		//ブートゲージ画像情報
		imgInfo_.emplace(IMG_TYPE::WIN,
			Info(SINGLEMODE_IMG_POS));

		//HP表示のオフセット値
		hpValueOffset_ = HP_VALUE_OFFSET_SINGLE;
		//HP表示の大きさ
		hpValueScale_ = HP_VALUE_SCALE_SINGLE;

	}
	else//対戦モード時の初期化
	{

		//HPゲージ画像の情報
		imgInfo_.emplace(IMG_TYPE::HP_BACK,
			Info(HP_VALUE_POS_MULTI,
				HP_BACK_SCALE_MULTI,
				HP_BACK_CENTER_X,
				HP_BACK_CENTER_Y));
		//残弾数表示情報
		imgInfo_.emplace(IMG_TYPE::BULLET_NUM,
			Info(BULLET_POS_MULTI,
				BULLET_SCALE_MULTI,
				BULLET_CENTER_X,
				BULLET_CENTER_Y,
				BULLET_OFFSET_MULTI));
		//ライフル画像
		imgInfo_.emplace(IMG_TYPE::RIFLE_IMAGE,
			Info(RIFLE_POS_MULTI,
				RIFLE_SCALE_MULTI,
				RIFLE_CENTER_X,
				RIFLE_CENTER_Y));
		//ブートゲージ画像情報
		imgInfo_.emplace(IMG_TYPE::BOOST_GAUGE, Info(BOOST_GAUGE_IMG_POS_MULTI,
			BOOST_GAUGE_CASE_SCALE_MULTI,
			BOOST_GAUGE_OFFSET_X_MULTI,
			BOOST_GAUGE_OFFSET_Y_MULTI));

		//ブートゲージ画像情報
		imgInfo_.emplace(IMG_TYPE::WIN, Info(MULTI_MODE_IMG_POS));

		//HP表示のオフセット
		hpValueOffset_ = HP_VALUE_OFFSET_MULTI;
		//HP表示の大きさ
		hpValueScale_ = HP_VALUE_SCALE_MULTI;

	}
}

void UserInterface::InitCommonUI(void)
{
	//画像読み込み
	uiImgH_.emplace(IMG_TYPE::BACK_TO_TITLE, resMng_.Load(ResourceManager::SRC::BACK_TO_TITLE).handleId_);
	uiImgH_.emplace(IMG_TYPE::ONE_MOR_FIGHT, resMng_.Load(ResourceManager::SRC::ONE_MOR_FIGHT).handleId_);
	uiImgH_.emplace(IMG_TYPE::PLEASE_A, resMng_.Load(ResourceManager::SRC::PLEASE_A).handleId_);
	uiImgH_.emplace(IMG_TYPE::PLEASE_CROSS, resMng_.Load(ResourceManager::SRC::PLEASE_CROSS).handleId_);
	uiImgH_.emplace(IMG_TYPE::TRIANGLE, resMng_.Load(ResourceManager::SRC::TRIANGLE).handleId_);
	uiImgH_.emplace(IMG_TYPE::READY, resMng_.Load(ResourceManager::SRC::READY_IMAGE).handleId_);
	uiImgH_.emplace(IMG_TYPE::FIGHT, resMng_.Load(ResourceManager::SRC::FIGHT_IMAGE).handleId_);
}

void UserInterface::DrawBoostGauge(void)
{
	int x = static_cast<int>(boostGauge_);
	int PosX = static_cast<int>(imgInfo_[IMG_TYPE::BOOST_GAUGE].pos_.x);
	int PosY = static_cast<int>(imgInfo_[IMG_TYPE::BOOST_GAUGE].pos_.y);
	int color = 0;
	//ゲージ残量半分以上は緑色
	if (x>50)
	{
		color = GetColor(0, 0, 255);
	}
	//ゲージ残量25%以上50%未満なら黄色
	else if (x>25)
	{
		color = GetColor(255, 255, 0);
	}
	//ゲージ残量25%以下なら赤色
	else
	{
		color = GetColor(255, 0, 0);
	}
	//ブーストゲージの表示
	DrawFillBox(PosX, PosY, PosX+ (x * imgInfo_[IMG_TYPE::BOOST_GAUGE].centerX_), PosY + imgInfo_[IMG_TYPE::BOOST_GAUGE].centerY_, color);
	//ブーストゲージ枠の表示
	DrawRotaGraph2(PosX, PosY, 0, 0, imgInfo_[IMG_TYPE::BOOST_GAUGE].scale_, 0.0, uiImgH_[IMG_TYPE::BOOST_GAUGE_CASE], true, false);
}

void UserInterface::DrawNumnberOfBullets(void)
{
	int PosX = static_cast<int>(imgInfo_[IMG_TYPE::BULLET_NUM].pos_.x);
	int PosY = static_cast<int>(imgInfo_[IMG_TYPE::BULLET_NUM].pos_.y);
	int riflePosX = static_cast<int>(imgInfo_[IMG_TYPE::RIFLE_IMAGE].pos_.x);
	int riflePosY = static_cast<int>(imgInfo_[IMG_TYPE::RIFLE_IMAGE].pos_.y);

	//残弾表示(武器)
	DrawRotaGraph2(riflePosX, riflePosY, imgInfo_[IMG_TYPE::RIFLE_IMAGE].centerX_, imgInfo_[IMG_TYPE::RIFLE_IMAGE].centerY_,
		imgInfo_[IMG_TYPE::RIFLE_IMAGE].scale_, 0.0, uiImgH_[IMG_TYPE::RIFLE_IMAGE], true, false);

	//残弾表示(数字)
	DrawRotaGraph2(PosX ,PosY,
					imgInfo_[IMG_TYPE::BULLET_NUM].centerX_,imgInfo_[IMG_TYPE::BULLET_NUM].centerX_,
		imgInfo_[IMG_TYPE::BULLET_NUM].scale_, 0.0, numberImg_[*numnberofBullets_], true, false);

}

void UserInterface::DrawHPGauge(void)
{
	int PosX = static_cast<int>(imgInfo_[IMG_TYPE::HP_BACK].pos_.x);
	int PosY = static_cast<int>(imgInfo_[IMG_TYPE::HP_BACK].pos_.y );
	int hp = static_cast<int>(hpGauge_);

	//百の桁の数字
	int singleDigit = hp/ 100;
	//十の桁の数字
	int twoDigit = hp;
	twoDigit = (twoDigit % 100 ) / 10;
	//十の桁の数字
	int threeDigit = hp;
	threeDigit = (threeDigit % 100)%10;

	//HP背面画像
	DrawRotaGraph2(PosX, PosY, imgInfo_[IMG_TYPE::HP_BACK].centerX_, imgInfo_[IMG_TYPE::HP_BACK].centerY_,
		imgInfo_[IMG_TYPE::HP_BACK].scale_, 0.0, uiImgH_[IMG_TYPE::HP_BACK], true, false);


	//HP表示(百の桁)
	DrawRotaGraph2(PosX - hpValueOffset_, PosY, 40, 40,
		hpValueScale_, 0.0, numberImg_[singleDigit], true, false);

	//HP表示(十の桁)
	DrawRotaGraph2(PosX, PosY, 40, 40,
		hpValueScale_, 0.0, numberImg_[twoDigit], true, false);

	//HP表示(一の桁)
	DrawRotaGraph2(PosX + hpValueOffset_, PosY, 40, 40,
		hpValueScale_, 0.0, numberImg_[threeDigit], true, false);

}

void UserInterface::VictoryOrDefeat(void)
{
	int posX = static_cast<int>(imgInfo_[IMG_TYPE::WIN].pos_.x);
	int posY = static_cast<int>(imgInfo_[IMG_TYPE::WIN].pos_.y);
	if (isWin_)
	{	//勝利画像表示
		DrawRotaGraph2(posX, posY, 140, 100, 2.0, 0.0, uiImgH_[IMG_TYPE::WIN], true, false);
	}
	else
	{	//勝利画像表示
		DrawRotaGraph2(posX, posY, 140, 100, 2.0, 0.0, uiImgH_[IMG_TYPE::LOSE],  true, false);
	}
}

void UserInterface::DrawTarget(void)
{
	VECTOR pos = (enemyPos_);
	VECTOR TargetPos = pos;
	TargetPos.y += TARGET_OFFSET_Y;
	TargetPos = ConvWorldPosToScreenPos(TargetPos);
	int X = static_cast<int>(TargetPos.x);
	int Y = static_cast<int>(TargetPos.y);

	//距離によって色を変える
	if (WITHIN_RANGE > (enemyDistance_))
	{	//照準表示(白色)
		DrawRotaGraph(X, Y,  0.2f, 0.0f, uiImgH_[IMG_TYPE::TARGERT], true);
	}
	else
	{	//照準表示(赤色)
		DrawRotaGraph(X, Y, 0.2f, 0.0f, uiImgH_[IMG_TYPE::TARGET_RED], true);
	}
	//敵のHPゲージ表示
	DrawEnemyHp();
}

void UserInterface::DrawEnemyHp(void)
{
	int scale = 0;
	//プレイモードによって大きさを変える
	if (playMode_== PLAY_MODE)
	{
		scale = 50;
	}
	else
	{
		scale = 10;
	}
	VECTOR enemyHpPos = (enemyPos_);
	enemyHpPos.y += ENEMY_HP_GAUGE_OFFSET_Y;
	enemyHpPos = ConvWorldPosToScreenPos(enemyHpPos);
	int X = static_cast<int>(enemyHpPos.x);
	int Y = static_cast<int>(enemyHpPos.y);
	int gauge = static_cast<int>((enemyHpGauge_ * 2) / scale);
	int gaugeCase = static_cast<int>((ENEMY_HP * 2) / scale);

	//敵HPゲージ枠の表示
	DrawRectGraph(X, Y, 0, 0, gaugeCase, 15, uiImgH_[IMG_TYPE::BOOST_GAUGE_CASE], true);
	//敵HPゲージの表示
	DrawRectGraph(X, Y, 0, 0, gauge, 15, uiImgH_[IMG_TYPE::ENEMY_HP_GAUGE], true);
}

void UserInterface::DrawGameUIStart(const float& startCount)
{
	//ゲームスタート後は描画をしない
	if (startCount >= START_TIME_MAX)
	{
		return;
	}
	if (startCount < READY_TIME_MAX)
	{	//レディ画像表示		
		DrawRotaGraph2(READY_POS_X, READY_POS_Y,
			READY_CENTER_X, READY_CENTER_Y, READY_SCALE, 0.0, uiImgH_[IMG_TYPE::READY], true, false);
	}
	else
	{	//ファイト画像表示
		DrawRotaGraph2(FIGHT_POS_X, FIGHT_POS_Y,
			FIGHT_CENTER_X, FIGHT_CENTER_Y, FIGHT_SCALE, 0.0, uiImgH_[IMG_TYPE::FIGHT], true, false);
	}
}

void UserInterface::DrawUIGameSet(const bool& isGameSet, const int& rematchMode)
{
	//勝敗が決まっていなければ戻る
	if (!isGameSet)
	{
		return;
	}

	int titleX = static_cast<int>(imgInfo_[IMG_TYPE::BACK_TO_TITLE].pos_.x);
	int titleY = static_cast<int>(imgInfo_[IMG_TYPE::BACK_TO_TITLE].pos_.y);
	int fightX = static_cast<int>(imgInfo_[IMG_TYPE::ONE_MOR_FIGHT].pos_.x);
	int fightY = static_cast<int>(imgInfo_[IMG_TYPE::ONE_MOR_FIGHT].pos_.y);


	//タイトルへ戻る画像表示
	DrawRotaGraph2(titleX, titleY,
		imgInfo_[IMG_TYPE::BACK_TO_TITLE].centerX_,
		imgInfo_[IMG_TYPE::BACK_TO_TITLE].centerY_,
		imgInfo_[IMG_TYPE::BACK_TO_TITLE].scale_, 0.0, uiImgH_[IMG_TYPE::BACK_TO_TITLE], true, false);
	//もう一戦画像表示
	DrawRotaGraph2(fightX, fightY,
		imgInfo_[IMG_TYPE::ONE_MOR_FIGHT].centerX_,
		imgInfo_[IMG_TYPE::ONE_MOR_FIGHT].centerY_,
		imgInfo_[IMG_TYPE::ONE_MOR_FIGHT].scale_, 0.0, uiImgH_[IMG_TYPE::ONE_MOR_FIGHT], true, false);

	//接続されたコントローラがPsなら×ボタン表記
	if (JOYPAD_TYPE:: DUAL_SHOCK_4 == joyPadType_ || JOYPAD_TYPE::DUAL_SENSE == joyPadType_)
	{

		DrawRotaGraph2(imgInfo_[IMG_TYPE::PLEASE_CROSS].pos_.x, imgInfo_[IMG_TYPE::PLEASE_CROSS].pos_.y,
			imgInfo_[IMG_TYPE::PLEASE_CROSS].centerX_, imgInfo_[IMG_TYPE::PLEASE_CROSS].centerY_,
			imgInfo_[IMG_TYPE::PLEASE_CROSS].scale_, 0.0, uiImgH_[IMG_TYPE::PLEASE_CROSS], true, false);
	}//それ以外ならAボタン表記
	else
	{
		DrawRotaGraph2(imgInfo_[IMG_TYPE::PLEASE_A].pos_.x, imgInfo_[IMG_TYPE::PLEASE_A].pos_.y,
			imgInfo_[IMG_TYPE::PLEASE_A].centerX_, imgInfo_[IMG_TYPE::PLEASE_A].centerY_,
			imgInfo_[IMG_TYPE::PLEASE_A].scale_, 0.0, uiImgH_[IMG_TYPE::PLEASE_A], true, false);
	}
	//カーソルの表示
	DrawRotaGraph2(imgInfo_[IMG_TYPE::TRIANGLE].pos_.x, imgInfo_[IMG_TYPE::TRIANGLE].pos_.y + (imgInfo_[IMG_TYPE::TRIANGLE].offset_ * rematchMode),
		imgInfo_[IMG_TYPE::TRIANGLE].centerX_, imgInfo_[IMG_TYPE::TRIANGLE].centerY_, imgInfo_[IMG_TYPE::TRIANGLE].scale_, 0.0, uiImgH_[IMG_TYPE::TRIANGLE], true, false);
}
