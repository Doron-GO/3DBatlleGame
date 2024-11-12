#include<EffekseerForDXLib.h>
#include"../Manager/ResourceManager.h"
#include"../Object/Time/DeltaTime.h"
#include"../Manager/ActorManager.h"
#include"../Object/Camera/Camera.h"
#include "../Scene/SceneManager.h"
#include"../Object/Actor/Enemy/BossEnemy.h"
#include"../Object/Actor/Player/Player.h"
#include"../Object/Stage/SkyDome.h"
#include"../UI/UserInterface.h"
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
constexpr int SINGLE_PLAY_MODE = 0;

//タイトルに戻る画像
constexpr int BACK_TO_TITLE_POS_X = 800;
constexpr int BACK_TO_TITLE_POS_Y = 500;
constexpr int BACK_TO_TITLE_CENTER_X = 250;
constexpr int BACK_TO_TITLE_CENTER_Y = 50;
constexpr double BACK_TO_TITLE_SCALE = 1.5;

//もう一戦画像
constexpr int ONE_MOR_FIGHT_POS_X = 800;
constexpr int ONE_MOR_FIGHT_POS_Y = 700;
constexpr int ONE_MOR_FIGHT_CENTER_X = 273;
constexpr int ONE_MOR_FIGHT_CENTER_Y = 54;
constexpr double ONE_MOR_FIGHT_SCALE = 1.5;

//×ボタンを押してくれ画像
constexpr int PLEASE_CROSS_BUTTON_POS_X = 800;
constexpr int PLEASE_CROSS_BUTTON_POS_Y = 900;
constexpr int PLEASE_CROSS_BUTTON_CENTER_X = 480;
constexpr int PLEASE_CROSS_BUTTON_CENTER_Y = 52;
constexpr double PLEASE_CROSS_BUTTON_SCALE = 1.2;

//Aボタンを押してくれ画像
constexpr int PLEASE_A_BUTTON_POS_X = 800;
constexpr int PLEASE_A_BUTTON_POS_Y = 800;
constexpr int PLEASE_A_BUTTON_CENTER_X = 395;
constexpr int PLEASE_A_BUTTON_CENTER_Y = 34;
constexpr double PLEASE_A_BUTTON_SCALE = 1.2;

//セレクト画像の数値
constexpr int TRIANGLE_POS_X = 1200;
constexpr int TRIANGLE_POS_Y = 500;
constexpr int TRIANGLE_CENTER_X = 280;
constexpr int TRIANGLE_CENTER_Y = 63;
constexpr int TRIANGLE_OFFSET = 200;
constexpr double TRIANGLE_SCALE = 1.2;

//ゲーム開始時に出るREADYの画像の数値
constexpr int READY_POS_X = 800;
constexpr int READY_POS_Y = 400;
constexpr int READY_CENTER_X = 117;
constexpr int READY_CENTER_Y = 44;
constexpr int READY_OFFSET = 200;
constexpr double READY_SCALE = 3.0;

//ゲーム開始時に出るFIGHTの画像の数値
constexpr int FIGHT_POS_X = 800;
constexpr int FIGHT_POS_Y = 400;
constexpr int FIGHT_CENTER_X = 120;
constexpr int FIGHT_CENTER_Y = 49;
constexpr int FIGHT_OFFSET = 200;
constexpr double FIGHT_SCALE = 4.5;

//ゲームスタート開始秒数
constexpr float START_TIME_MAX = 3.0f;

//画面分割用の線の太さ
constexpr int LINE_THICKNES = 1000;

constexpr int SINGLE_PLAY = 0;


#pragma endregion


GameScene::GameScene(SceneManager& manager, int playMode, Transitor& transit, Input& input):Scene(manager, transit,input), 
resMng_(ResourceManager::GetInstance())
{

	// トランジッションの開始
	sceneTransitor_.Start();
	
	//タイトルシーンで決まったゲームモードを格納
	playMode_ = playMode;

	//再戦モード選択の初期カーソルは「タイトルに戻る」に設定
	rematchMode_ = 0;

	//ゲームスタート計測変数を初期化
	startCount_ = 0.0f;

	//プレイヤー１のパッドの種類を取得
	joyPadType_ = input_.GetJPadType();

	//プレイヤー１と２の画面の座標
	screenPos_.emplace(PLAYER_NUM::P_1, VECTOR(0, 0));
	screenPos_.emplace(PLAYER_NUM::P_2, VECTOR(SCREEN_SIZE.x / 2, 0));

	//ステージの生成
	skyDome_ = std::make_unique<SkyDome>();

	//ステージの生成
	stage_ = std::make_unique<Stage>();

	//プレイヤーとボスを管理するマネージャ―の生成
	actorManager_ = std::make_unique<ActorManager>(playMode);

	//ステージのコリジョンを追加
	actorManager_->AddColliders(stage_->GetCollider());

	//カメラ更新が必要なため、一回アップデートをしておく
	actorManager_->Update();

	//UIの初期化
	InitUI();

	// 3D設定
	Init3DSetting();

	//画像の読み込み
	InitImage();

	//描画スクリーンと描画ステップ
	InitRender();
	
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
	funcDraw_();
	sceneTransitor_.Draw();
}

void GameScene::Init3DSetting(void)
{
	// Zバッファを有効にする
	SetUseZBuffer3D(true);

	// Zバッファへの書き込みを有効にする
	SetWriteZBuffer3D(true);

	// バックカリングを有効にする
	SetUseBackCulling(true);

	// ライトの設定
	SetUseLighting(true);
	ChangeLightTypeDir({ 0.3f, -0.7f, 0.8f });

	// 背景色設定
	SetBackgroundColor(0, 139, 139);

	//要確認
	//フォグ設定
	//SetFogEnable(true);
	//SetFogColor(5, 5, 5);
	//SetFogStartEnd(10000.0f, 20000.0f);

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

void GameScene::InitRender(void)
{
	//プレイヤー１と２の両方の画面を描画するスクリーンを作る
	int SCX = static_cast<int>(SCREEN_SIZE.x);
	int SCY = static_cast<int>(SCREEN_SIZE.y);
	integrationScreen_ = MakeScreen(SCX, SCY, true);

	if (playMode_ == SINGLE_PLAY_MODE)
	{
		//draw_ = &GameScene::DrawSingleMode;
		funcDraw_ = std::bind(&GameScene::DrawSingleMode, this);
	}
	else
	{
		//プレイヤー１と２の画面を作る
		cameraScreens_.emplace_back(MakeScreen(SCX / 2, SCY, true));
		cameraScreens_.emplace_back(MakeScreen(SCX / 2, SCY, true));
		funcDraw_ = std::bind(&GameScene::DrawBattleMode, this);
	}
}

void GameScene::InitUI(void)
{
	//プレイヤー1
	int player1 = static_cast<int>(ActorBase::ACTOR_TYPE::PLAYER_1);
	//
	 auto& players=  actorManager_->GetPlayers();
	//
	if (playMode_ == SINGLE_PLAY)
	{
		auto& bossEnemy = actorManager_->GetBossEnemy();
		//プレイヤー1のUIを生成
		CreateUserInterface(*players[player1], *bossEnemy);
	}
	else
	{
		//プレイヤー2
		int player2 = static_cast<int>(ActorBase::ACTOR_TYPE::PLAYER_2);
		//プレイヤー１とプレイヤー２のUIを生成
		CreateUserInterface(*players[player1], *players[player2]);
		CreateUserInterface(*players[player2], *players[player1]);
	}
}

void GameScene::InitCollider(void)
{

}

void GameScene::CreateUserInterface(const ActorBase& player,const ActorBase& target)
{
	//プレイヤータイプ
	 int playerType = static_cast<int>(player.GetActorType());
	//UIを生成
	userInterfaces_.emplace_back(
		std::make_unique<UserInterface>(
			playMode_,
			playerType,
			player.GetEnemyDistance(),
			player.GetNumnberOfBullets(),
			player.GetBoostFuel(),
			player.GetHP(),
			player.IsWin(),
			target.GetTransform().pos,
			target.GetHP()
			)
	);
}

void GameScene::DrawSingleMode(void)
{
	//勝者が決まっているかどうか判定
	bool& isDeadAnyPlayer = actorManager_->IsDeadAnyPlayer();
	//スクリーンをクリア
	ClearDrawScreen();
	//カメラ描画
	actorManager_->DrawCamera(SINGLE_PLAY_MODE);
	//キャラクターの描画
	actorManager_->Draw();
	//ステージの描画
	stage_->Draw();
	skyDome_->Draw();
	//UIの描画
	userInterfaces_[SINGLE_PLAY]->Draw(isDeadAnyPlayer);
	//エフェクシアのアップデート
	UpdateEffekseer3D();
	//エフェクシアの描画
	DrawEffekseer3D();
	//共通UI描画(ゲームスタートなど)
	userInterfaces_[SINGLE_PLAY]->DrawCommonUI(startCount_, isDeadAnyPlayer, rematchMode_);
}

void GameScene::DrawBattleMode(void)
{
	//プレイヤーのマックス人数
	int maxIdx = static_cast<int>(PLAYER_NUM::MAX);
	//勝者が決まっているかどうか判定
	bool& isDeadAnyPlayer = actorManager_->IsDeadAnyPlayer();
	for (int idx = 0; idx < maxIdx; idx++)
	{
		//描画するスクリーンをセット
		SetDrawScreen(cameraScreens_[idx]);
		
		// 画面を初期化
		ClearDrawScreen();
		//カメラの描画
		actorManager_->DrawCamera(idx);
		//キャラクターの描画
		actorManager_->Draw();
		//ステージの描画
		stage_->Draw();
		//スカイドームの描画
		skyDome_->Draw();
		//エフェクシアのアップデート
		UpdateEffekseer3D();
		//エフェクシアの描画
		DrawEffekseer3D();
		//UIの描画
		userInterfaces_[idx]->Draw(isDeadAnyPlayer);

	}

	//二枚のスクリーンを統合した、スクリーンのセット
	SetDrawScreen(integrationScreen_);

	for (int idx = 0; idx < maxIdx; idx++)
	{
		//透過処理は後で見直す
		PLAYER_NUM key = static_cast<PLAYER_NUM>(idx);

		//各プレイヤーのスクリーンの描画
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
	int player1 = static_cast<int>(ActorBase::ACTOR_TYPE::PLAYER_1);
	userInterfaces_[player1]->DrawCommonUI(startCount_, isDeadAnyPlayer, rematchMode_);

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
	//開始時間を超えていなければ、カウントし続ける
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
	//勝敗が決まっていなければ入る
	if (!IsGameSet())
	{
		return false;
	}
	//上ボタンを押したら、カーソルを上に動かす
	if (input_.IsTriggerd("up") && rematchMode_> static_cast<int>(REMATCH_MODE::BACK_TO_TITLE))
	{
		rematchMode_--;
	}
	//下ボタンを押したら、カーソルを下に動かす
	else if (input_.IsTriggerd("down") && rematchMode_ < static_cast<int>(REMATCH_MODE::ONE_MORE_FIGHT))
	{
		rematchMode_++;
	}
	//
	return SelectDecide();
}

bool GameScene::SelectDecide(void)
{	
	//PSコンの使用時に×ボタンが押された時
	if (JOYPAD_TYPE::DUAL_SHOCK_4 == joyPadType_ ||
		JOYPAD_TYPE::DUAL_SENSE == joyPadType_ &&
		input_.IsTriggerd("b"))
	{
		SelectTitleOrGame();
		return true;
	}
	//XBOXならAボタン
	else if (JOYPAD_TYPE::XBOX_360 == joyPadType_ ||
		JOYPAD_TYPE::XBOX_ONE == joyPadType_ &&
		input_.IsTriggerd("jump"))
	{
		SelectTitleOrGame();
		return true;
	}

	return false;

}

void GameScene::SelectTitleOrGame(void)
{
	//カーソルがタイトルならタイトルシーンに移行
	if (rematchMode_ == static_cast<int>(REMATCH_MODE::BACK_TO_TITLE))
	{
		ChangeTitleScene();
	}
	//カーソルがタイトルならもう一戦
	else if (rematchMode_ == static_cast<int>(REMATCH_MODE::ONE_MORE_FIGHT))
	{
		ChangeGameScene();
	}
	else
	{
		ChangeTitleScene();
	}
}
