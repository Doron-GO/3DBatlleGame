#include<DxLib.h>
#include "../Manager/ResourceManager.h"
#include "UserInterface.h"
#include"../../Config.h"

#pragma region Parameter

//シングルモード時の勝利または敗北画像の座標
static constexpr VECTOR SINGLEMODE_IMG_POS = { 800.0f,400.0f ,0.0f };

static constexpr VECTOR MULTI_MODE_IMG_POS = { 400.0f,400.0f ,0.0f };

//敵ターゲット画像の座標調整用
static constexpr float TARGET_OFFSET_Y = 100.0f;

//敵ターゲット画像の座標調整用
static constexpr float ENEMY_HP_GAUGE_OFFSET_Y = 400.0f;

 //ブーストゲージ画像の座標
static constexpr VECTOR BOOST_GAUGE_IMG_POS_MULTI= { 400.0f,850.0f,0.0f };
static constexpr VECTOR BOOST_GAUGE_IMG_POS_SINGLE = { 1100.0f,850.0f,0.0f };
static constexpr double BOOST_GAUGE_CASE_SCALE_SINGLE= 2.5;
static constexpr double BOOST_GAUGE_CASE_SCALE_MULTI= 2.0;
static constexpr int BOOST_GAUGE_OFFSET_X_SINGLE = 5;
static constexpr int BOOST_GAUGE_OFFSET_Y_SINGLE = 40;
static constexpr int BOOST_GAUGE_OFFSET_X_MULTI = 4;
static constexpr int BOOST_GAUGE_OFFSET_Y_MULTI = 30;

 //HPゲージ表示の数値
static constexpr VECTOR HP_GAUGE_POS_MULTI = {25.0f,800.0f,0.0f};
 static constexpr VECTOR HP_GAUGE_POS_SINGLE = {4.0f,750.0f,0.0f};


 //残弾数表示の数値
 static constexpr VECTOR NUMBER_OF_BULLETS_POS_SINGLE = {1300.0f,650.0f,0.0f};
 static constexpr VECTOR NUMBER_OF_BULLETS_POS_MULTI = {600.0f,700.0f,0.0f};
 static constexpr int NUMBER_OF_BULLETS_CENTER_X= 140;
 static constexpr int NUMBER_OF_BULLETS_CENTER_Y=100;
 static constexpr int NUMBER_OF_BULLETS_OFFSET = 10;
 static constexpr double NUMBER_OF_BULLETS_SCALE_MULTI = 0.4;
 static constexpr double NUMBER_OF_BULLETS_SCALE_SINGLE= 0.6;

 //タイトルに戻る画像
 static constexpr int BACK_TO_TITLE_POS_X = 800;
 static constexpr int BACK_TO_TITLE_POS_Y = 500;
 static constexpr int BACK_TO_TITLE_CENTER_X = 250;
 static constexpr int BACK_TO_TITLE_CENTER_Y = 50;
 static constexpr double BACK_TO_TITLE_SCALE = 1.5;

 //もう一戦画像
 static constexpr int ONE_MOR_FIGHT_POS_X = 800;
 static constexpr int ONE_MOR_FIGHT_POS_Y = 700;
 static constexpr int ONE_MOR_FIGHT_CENTER_X = 273;
 static constexpr int ONE_MOR_FIGHT_CENTER_Y = 54;
 static constexpr double ONE_MOR_FIGHT_SCALE = 1.5;

 //×ボタンを押してくれ画像
 static constexpr int  PLEASE_CROSS_BUTTON_POS_X = 800;
 static constexpr int  PLEASE_CROSS_BUTTON_POS_Y = 900;
 static constexpr int  PLEASE_CROSS_BUTTON_CENTER_X = 480;
 static constexpr int  PLEASE_CROSS_BUTTON_CENTER_Y = 52;
 static constexpr double  PLEASE_CROSS_BUTTON_SCALE = 1.2;

 //Aボタンを押してくれ画像
 static constexpr int  PLEASE_A_BUTTON_POS_X = 800;
 static constexpr int  PLEASE_A_BUTTON_POS_Y = 800;
 static constexpr int  PLEASE_A_BUTTON_CENTER_X = 395;
 static constexpr int  PLEASE_A_BUTTON_CENTER_Y = 34;
 static constexpr double  PLEASE_A_BUTTON_SCALE = 1.2;

 //セレクト画像の数値
 static constexpr int  TRIANGLE_POS_X = 1200;
 static constexpr int  TRIANGLE_POS_Y = 500;
 static constexpr int  TRIANGLE_CENTER_X = 280;
 static constexpr int  TRIANGLE_CENTER_Y = 63;
 static constexpr int  TRIANGLE_OFFSET = 200;
 static constexpr double  TRIANGLE_SCALE = 1.2;

 //ゲーム開始時に出るREADYの画像の数値
 static constexpr int READY_POS_X = 800;
 static constexpr int  READY_POS_Y = 400;
 static constexpr int  READY_CENTER_X = 117;
 static constexpr int  READY_CENTER_Y = 44;
 static constexpr int  READY_OFFSET = 200;
 static constexpr double  READY_SCALE = 3.0;

 //ゲーム開始時に出るFIGHTの画像の数値
 static constexpr int FIGHT_POS_X = 800;
 static constexpr int  FIGHT_POS_Y = 400;
 static constexpr int  FIGHT_CENTER_X = 120;
 static constexpr int  FIGHT_CENTER_Y = 49;
 static constexpr int  FIGHT_OFFSET = 200;
 static constexpr double  FIGHT_SCALE = 4.5;

 //ゲームスタート開始秒数
 static constexpr float START_TIME_MAX = 3.0f;

 //対戦開始時「READY」画像表示秒数
 static constexpr float READY_TIME_MAX = 1.5f;

 //画面分割用の線の太さ
 static constexpr int  LINE_THICKNES = 1000;

 //プレイヤー１のタイプ
 static constexpr int PLAYER_TYPE = 0;

 //プレイモード(シングルプレイ)
 static constexpr int PLAY_MODE = 0;

 //プレイヤー１のPADナンバー
 static constexpr int PAD_NUM = 1;

 //敵HP
 static constexpr int ENEMY_HP = 1000;
 //ボスHP
 static constexpr int BOSS_HP = 500;

#pragma endregion


UserInterface::UserInterface(ResourceManager& resMng, const VECTOR& enemyPos,const float& distance,
	const float& boostGauge,const float& hpGauge, const float& enemyHp,const bool& IsWin_,const int& numnberofBullets,
	int playMode, int playerType):
	resMng_(resMng), enemyPos_(enemyPos), enemyDistance_(distance), 
	boostGauge_(boostGauge), hpGauge_(hpGauge),enemyHpGauge_(enemyHp), isWin_(IsWin_), numnberofBullets_(numnberofBullets)
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
		InitCommonUi();
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

void UserInterface::DrawCommonUI(const float& startCount, const bool& isGameSet, const float& rematchMode)
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

	//シングルプレイモード時の初期化
	if (playMode_ == PLAY_MODE)
	{
		//HPゲージの画像の座標
		hpGaugePos_ = HP_GAUGE_POS_SINGLE;

		//勝敗画像の座標
		victoryOrDefeatPos_ = SINGLEMODE_IMG_POS;

		//ブーストゲージ画像の座標
		boostGaugePos_ = BOOST_GAUGE_IMG_POS_SINGLE;

		//ブーストゲージ枠画像の大きさ
		boostGaugeCaseScale_ = BOOST_GAUGE_CASE_SCALE_SINGLE;

		//ブーストゲージ画像位置調整用数値
		boostGaugeOffsetX_ = BOOST_GAUGE_OFFSET_X_SINGLE;
		boostGaugeOffsetY_ = BOOST_GAUGE_OFFSET_Y_SINGLE;

		//残弾表示画像座標
		numberOfBulletsPos_ = NUMBER_OF_BULLETS_POS_SINGLE;

		//残弾表示画像の大きさ
		numberOfBulletsScale_ = NUMBER_OF_BULLETS_SCALE_SINGLE;

		//HP表示のフォント設定
		fontHp_ = CreateFontToHandle(NULL, 250, 3);

		//残弾表示のフォント設定
		fontBullets_ = CreateFontToHandle(NULL, 150, 3);

	}
	else//対戦モード時の初期化
	{
		//HPゲージの画像の座標
		hpGaugePos_ = HP_GAUGE_POS_MULTI;

		//勝敗画像の座標
		victoryOrDefeatPos_ = MULTI_MODE_IMG_POS;

		//ブーストゲージ画像の座標
		boostGaugePos_ = BOOST_GAUGE_IMG_POS_MULTI;
		
		//ブーストゲージ枠画像の大きさ
		boostGaugeCaseScale_ = BOOST_GAUGE_CASE_SCALE_MULTI;

		//ブーストゲージ画像位置調整用数値
		boostGaugeOffsetX_ = BOOST_GAUGE_OFFSET_X_MULTI;
		boostGaugeOffsetY_ = BOOST_GAUGE_OFFSET_Y_MULTI;

		//残弾表示画像座標
		numberOfBulletsPos_ = NUMBER_OF_BULLETS_POS_MULTI;

		//残弾表示画像の大きさ
		numberOfBulletsScale_ = NUMBER_OF_BULLETS_SCALE_MULTI;

		//HP表示のフォント設定
		fontHp_ = CreateFontToHandle(NULL, 150, 3);
		
		//残弾表示のフォント設定
		fontBullets_ = CreateFontToHandle(NULL, 100, 3);
	}
}

void UserInterface::InitCommonUi(void)
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
	int PosX = static_cast<int>(boostGaugePos_.x);
	int PosY = static_cast<int>(boostGaugePos_.y);
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
	DrawFillBox(PosX, PosY, PosX+ (x * boostGaugeOffsetX_), PosY + boostGaugeOffsetY_, color);
	//ブーストゲージ枠の表示
	DrawRotaGraph2(PosX, PosY, 0, 0, boostGaugeCaseScale_, 0.0, uiImgH_[IMG_TYPE::BOOST_GAUGE_CASE], true, false);
}

void UserInterface::DrawNumnberOfBullets(void)
{
	int PosX = static_cast<int>(numberOfBulletsPos_.x);
	int PosY = static_cast<int>(numberOfBulletsPos_.y);
	int geuge = static_cast<int>(numnberofBullets_);

	//残弾表示(数字)
	DrawFormatStringToHandle(PosX, PosY - NUMBER_OF_BULLETS_OFFSET, 0xffffff, fontBullets_, "%d", static_cast<int>(numnberofBullets_));
	//残弾表示(武器)
	DrawRotaGraph2(PosX, PosY, NUMBER_OF_BULLETS_CENTER_X, NUMBER_OF_BULLETS_CENTER_Y, 
		numberOfBulletsScale_, 0.0, uiImgH_[IMG_TYPE::RIFLE_IMAGE], true, false);

}

void UserInterface::DrawHPGauge(void)
{	
	int PosX = static_cast<int>(hpGaugePos_.x);
	int PosY = static_cast<int>(hpGaugePos_.y);
	//HPの表示(数字)
	DrawFormatStringToHandle(PosX, PosY, 0xff0000, fontHp_, "%d", static_cast<int>(hpGauge_));
}

void UserInterface::VictoryOrDefeat(void)
{
	int posX = static_cast<int>(victoryOrDefeatPos_.x);
	int posY = static_cast<int>(victoryOrDefeatPos_.y);
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

	//タイトルへ戻る画像表示
	DrawRotaGraph2(BACK_TO_TITLE_POS_X, BACK_TO_TITLE_POS_Y,
		BACK_TO_TITLE_CENTER_X, BACK_TO_TITLE_CENTER_Y, BACK_TO_TITLE_SCALE, 0.0, uiImgH_[IMG_TYPE::BACK_TO_TITLE], true, false);
	//もう一戦画像表示
	DrawRotaGraph2(ONE_MOR_FIGHT_POS_X, ONE_MOR_FIGHT_POS_Y,
		ONE_MOR_FIGHT_CENTER_X, ONE_MOR_FIGHT_CENTER_Y, ONE_MOR_FIGHT_SCALE, 0.0, uiImgH_[IMG_TYPE::ONE_MOR_FIGHT], true, false);

	//接続されたコントローラがPsなら×ボタン表記
	if (JOYPAD_TYPE:: DUAL_SHOCK_4 == joyPadType_ || JOYPAD_TYPE::DUAL_SENSE == joyPadType_)
	{

		DrawRotaGraph2(PLEASE_CROSS_BUTTON_POS_X, PLEASE_CROSS_BUTTON_POS_Y,
			PLEASE_CROSS_BUTTON_CENTER_X, PLEASE_CROSS_BUTTON_CENTER_Y, PLEASE_CROSS_BUTTON_SCALE, 0.0, uiImgH_[IMG_TYPE::PLEASE_CROSS], true, false);
	}//それ以外ならAボタン表記
	else
	{
		DrawRotaGraph2(PLEASE_A_BUTTON_POS_X, PLEASE_A_BUTTON_POS_Y,
			PLEASE_A_BUTTON_CENTER_X, PLEASE_A_BUTTON_CENTER_Y, PLEASE_A_BUTTON_SCALE, 0.0, uiImgH_[IMG_TYPE::PLEASE_A], true, false);
	}
	//カーソルの表示
	DrawRotaGraph2(TRIANGLE_POS_X, TRIANGLE_POS_Y + (TRIANGLE_OFFSET * rematchMode),
		TRIANGLE_CENTER_X, TRIANGLE_CENTER_Y, TRIANGLE_SCALE, 0.0, uiImgH_[IMG_TYPE::TRIANGLE], true, false);
}
