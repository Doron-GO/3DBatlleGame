#include<EffekseerForDXLib.h>
#include"../Manager/ResourceManager.h"
#include"../Object/Time/DeltaTime.h"
#include"../Manager/ActorManager.h"
#include"../Object/Camera/Camera.h"
#include "../Scene/SceneManager.h"
#include"../Object/Stage/Stage.h"
#include"Transition/Transitor.h"
#include"../UI/UserInterface.h"
#include"../Object/Time/DeltaTime.h"
#include"../Input/Input.h"
#include "TitleScene.h"
#include "GameScene.h"
#include"../../Config.h"

#pragma region Parameter

//シングルプレイモード
static constexpr int SINGLE_PLAY_MODE = 0;

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

//画面分割用の線の太さ
static constexpr int  LINE_THICKNES = 1000;

#pragma endregion


GameScene::GameScene(SceneManager& manager, int playMode, Transitor& transit, Input& input):Scene(manager, transit,input), 
resMng_(ResourceManager::GetInstance())
{
	sceneTransitor_.Start();
	
	//タイトルシーンで決まったゲームモードを格納
	playMode_ = playMode;

	//再戦モード選択の初期カーソルは「タイトルに戻る」に設定
	rematchMode_ = 0;

	//ゲームスタート計測変数を初期化
	startCount_ = 0.0f;

	//プレイヤー１のパッドの種類を取得
	joyPadType_ = input_.GetJPadType();

	//プレイヤー１と２の画面を作る
	int SCX = static_cast<int>(SCREEN_SIZE.x);
	int SCY = static_cast<int>(SCREEN_SIZE.y);
	cameraScreens_.emplace_back(MakeScreen(SCX /2, SCY, true));
	cameraScreens_.emplace_back(MakeScreen(SCX /2, SCY, true));

	//プレイヤー１と２の両方の画面を描画するスクリーンを作る
	integrationScreen_= MakeScreen(SCX, SCY, true);

	//プレイヤー１と２の画面の座標
	screenPos_.emplace(PLAYER_NUM::P_1, VECTOR(0, 0));
	screenPos_.emplace(PLAYER_NUM::P_2, VECTOR(SCREEN_SIZE.x / 2, 0));

	//プレイヤーとボスを管理するマネージャ―の生成
	actorManager_ = std::make_unique<ActorManager>(playMode);

	//ステージの生成
	stage_ = std::make_unique<Stage>();

	//ステージのコリジョンを追加
	actorManager_->AddClliders(stage_->GetCollider());

	if (playMode == SINGLE_PLAY_MODE)
	{
		draw_ = &GameScene::DrawSIngleMode;
	}
	else
	{
		draw_ = &GameScene::DrawBattleMode;
	}

	//画像の読み込み
	InitImage();
	
	//一回アップデートをしておく
	actorManager_->Update();

	// Zバッファを有効にする
	SetUseZBuffer3D(true);

	// Zバッファへの書き込みを有効にする
	SetWriteZBuffer3D(true);

	// バックカリングを有効にする
	SetUseBackCulling(true);
	// ライトの設定
	ChangeLightTypeDir({ 0.3f, -0.7f, 0.8f });

	// 背景色設定
	SetBackgroundColor(0, 139, 139);

	// フォグ設定
	SetFogEnable(true);
	SetFogColor(5, 5, 5);
	SetFogStartEnd(10000.0f, 20000.0f);

}

GameScene::~GameScene(void)
{
}

void GameScene::Update(void)
{
	deltaTime_ = DeltaTime::GetInstsnce().GetDeltaTime();
	input_.Update();
	if (IsGameStart())
	{		
		actorManager_->Update();
	}		
	if (SelectCursor())
	{
		return;
	}
	sceneTransitor_.Update();
}

void GameScene::Draw(void)
{	
	(this->*draw_)();
	sceneTransitor_.Draw();
}

void GameScene::InitImage(void)
{
	//画像の読み込み
	imgType_.emplace(IMG_TYPE::BACK_TO_TITLE, resMng_.Load(ResourceManager::SRC::BACK_TO_TITLE).handleId_);
	imgType_.emplace(IMG_TYPE::ONE_MOR_FIGHT, resMng_.Load(ResourceManager::SRC::ONE_MOR_FIGHT).handleId_);
	imgType_.emplace(IMG_TYPE::PLEASE_A, resMng_.Load(ResourceManager::SRC::PLEASE_A).handleId_);
	imgType_.emplace(IMG_TYPE::PLEASE_CROSS, resMng_.Load(ResourceManager::SRC::PLEASE_CROSS).handleId_);
	imgType_.emplace(IMG_TYPE::TRIANGLE, resMng_.Load(ResourceManager::SRC::TRIANGLE).handleId_);
	imgType_.emplace(IMG_TYPE::READY, resMng_.Load(ResourceManager::SRC::READY_IMAGE).handleId_);
	imgType_.emplace(IMG_TYPE::FIGHT, resMng_.Load(ResourceManager::SRC::FIGHT_IMAGE).handleId_);
}

void GameScene::DrawSIngleMode(void)
{
	ClearDrawScreen();
	actorManager_->DrawCamera(SINGLE_PLAY_MODE);
	actorManager_->Draw();
	stage_->Draw();
	actorManager_->DrawUI(SINGLE_PLAY_MODE);
	UpdateEffekseer3D();
	DrawEffekseer3D();
	//共通UI描画(ゲームスタートなど)
	actorManager_->DrawCommonUI(startCount_, IsGameSet(), rematchMode_);
}
void GameScene::DrawBattleMode(void)
{
	int maxIdx = static_cast<int>(PLAYER_NUM::MAX);
	for (int idx = 0; idx < maxIdx; idx++)
	{
		SetDrawScreen(cameraScreens_[idx]);
		// 画面を初期化
		ClearDrawScreen();
		actorManager_->DrawCamera(idx);
		actorManager_->Draw();
		stage_->Draw();
		actorManager_->DrawUI(idx);
		UpdateEffekseer3D();
		DrawEffekseer3D();
	}
	SetDrawScreen(integrationScreen_);

	for (int idx = 0; idx < maxIdx; idx++)
	{
		//透過処理は後で見直す
		PLAYER_NUM key = static_cast<PLAYER_NUM>(idx);
		DrawGraph(
			static_cast<int>(screenPos_[key].x),
			static_cast<int>(screenPos_[key].y),
			cameraScreens_[idx], false);
	}
	//二分割されている画面の真ん中に線を引く
	DrawLine(
		static_cast<int>(screenPos_[PLAYER_NUM::P_2].x), 
		0, 
		static_cast<int>(screenPos_[PLAYER_NUM::P_2].x), LINE_THICKNES, 0x000000, true);

	SetDrawScreen(DX_SCREEN_BACK);

	//プレイヤーごとに分割されたスクリーンをまとめて描画
	DrawGraph(0, 0, integrationScreen_, true);

	//共通UI描画(ゲームスタートなど)
	actorManager_->DrawCommonUI(startCount_,IsGameSet(),rematchMode_);

}
void GameScene::ChangeGameScene(void)
{
	//ゲームシーンに移行
	sceneManager_.ChangeScene(std::make_shared<GameScene>(sceneManager_, playMode_, sceneTransitor_, input_));
}

void GameScene::ChangeTitleScene(void)
{
	//タイトルシーンに移行
	sceneManager_.ChangeScene(std::make_shared<TitleScene>(sceneManager_, sceneTransitor_,input_));
}

bool GameScene::IsGameStart(void)
{
	if (startCount_ < START_TIME_MAX)
	{
		startCount_ += deltaTime_;
	}
	else
	{
		startCount_ = START_TIME_MAX;
		return true;
	}
	return false;
}

bool GameScene::IsGameSet(void)
{
	return actorManager_->IsDeadAnyPlayer();
}

bool GameScene::SelectCursor(void)
{
	if (!actorManager_->IsDeadAnyPlayer())
	{
		return false;
	}

	if (input_.IsTriggerd("up") && rematchMode_> static_cast<int>(REMATCH_MODE::BACK_TO_TITLE))
	{
		rematchMode_--;
	}
	else if (input_.IsTriggerd("down") && rematchMode_ < static_cast<int>(REMATCH_MODE::ONE_MORE_FIGHT))
	{
		rematchMode_++;
	}
	else
	{
	}
	return SelectDecide();
}

bool GameScene::SelectDecide(void)
{	//PSコンの使用時に×ボタンが押された時
	if (JOYPAD_TYPE::DUAL_SHOCK_4 == joyPadType_ ||
		JOYPAD_TYPE::DUAL_SENSE == joyPadType_ &&
		input_.IsTriggerd("b"))
	{
		TitleOrGame();
		return true;
	}
	//XBOXならAボタン
	else if (JOYPAD_TYPE::XBOX_360 == joyPadType_ ||
		JOYPAD_TYPE::XBOX_ONE == joyPadType_ &&
		input_.IsTriggerd("jump"))
	{
		TitleOrGame();
		return true;
	}
	//その他ならプレステコンでいう×ボタンの場所のボタン
	else
	{
	}
	return false;
}

void GameScene::TitleOrGame(void)
{
	if (rematchMode_ == static_cast<int>(REMATCH_MODE::BACK_TO_TITLE))
	{
		ChangeTitleScene();
	}
	else if (rematchMode_ == static_cast<int>(REMATCH_MODE::ONE_MORE_FIGHT))
	{
		ChangeGameScene();
	}
	else
	{
		ChangeTitleScene();
	}
}




