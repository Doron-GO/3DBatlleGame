#include<EffekseerForDXLib.h>
#include"../Manager/ResourceManager.h"
#include"../Object/Weapon/BeamSaber.h"
#include "../Object/Camera/Camera.h"
#include "../Scene/SceneManager.h"
#include"../Utility/AsoUtility.h"
#include"Transition/Transitor.h"
#include"../Object/Stage/Stage.h"
#include"../Object/Stage/SkyDome.h"
#include"../Manager/EffectManager.h"
#include "../Input/Input.h"
#include"../../Config.h"
#include "TitleScene.h"
#include"GameScene.h"
#include"TutorialScene.h"
#include"Scene.h"
#include<DxLib.h>

#pragma region Parameter


constexpr int TYPE_ROBOT_FRONT = 0;
constexpr int TYPE_ROBOT_BACK = 1;

//モデルの座標
constexpr VECTOR FRONT_ROBOT_POS = { 0.0f, -25.0f, 0.0f };
constexpr VECTOR BACK_ROBOT_POS = { 0.0f, -450.0f, 5000.0f };

//カメラ座標
constexpr VECTOR CAMERA_POS = { 100.0f, 20.0f, -130.0f };
constexpr VECTOR CAMERA_TARGET_POS = { 100.0f, 15.0f, 0.0f };

//タイトルロゴ画像関係の数値(座標、中心座標、大きさ)
constexpr int TITLE_LOGO_POS_X = 775;
constexpr int TITLE_LOGO_POS_Y = 400;
constexpr int TITLE_LOGO_CENTER_X = 320;
constexpr int TITLE_LOGO_CENTER_Y = 60;
constexpr double TITLE_LOGO_SCALE = 2.5;

//×ボタンを押してね画像の数値(座標、中心座標、大きさ)
constexpr int PLEASE_CROSS_BUTTON_POS_X = 800;
constexpr int PLEASE_CROSS_BUTTON_POS_Y = 800;
constexpr int PLEASE_CROSS_BUTTON_CENTER_X = 480;
constexpr int PLEASE_CROSS_BUTTON_CENTER_Y = 52;
constexpr double PLEASE_CROSS_BUTTON_SCALE = 1.2;

//Aボタンを押してね画像の数値(座標、中心座標、大きさ)
constexpr int PLEASE_A_BUTTON_POS_X = 800;
constexpr int PLEASE_A_BUTTON_POS_Y = 800;
constexpr int PLEASE_A_BUTTON_CENTER_X = 395;
constexpr int PLEASE_A_BUTTON_CENTER_Y = 34;
constexpr double PLEASE_A_BUTTON_SCALE = 1.2;

//モード選択画像の数値(座標、中心座標、大きさ)
//一人用
constexpr int SINGLE_MODE_POS_X = 775;
constexpr int SINGLE_MODE_POS_Y = 400;
constexpr int SINGLE_MODE_CENTER_X = 165;
constexpr int SINGLE_MODE_CENTER_Y = 30;
constexpr double SINGLE_MODE_SCALE = 2.0;
//二人用
constexpr int MULTI_PLAY_POS_X = 775;
constexpr int MULTI_PLAY_POS_Y = 600;
constexpr int MULTI_PLAY_CENTER_X = 165;
constexpr int MULTI_PLAY_CENTER_Y = 52;
constexpr double MULTI_PLAY_SCALE = 2.0;

//チュートリアル選択画像数値
constexpr int TUTORIAL_MODE_POS_X = 775;
constexpr int TUTORIAL_MODE_POS_Y = 600;
constexpr int TUTORIAL_MODE_CENTER_X = 280;
constexpr int TUTORIAL_MODE_CENTER_Y = 30;
constexpr double TUTORIAL_MODE_SCALE = 2.0;

//ゲームにすすむ選択画像数値
constexpr int START_GAME_MODE_POS_X = 775;
constexpr int START_GAME_MODE_POS_Y = 400;
constexpr int START_GAME_MODE_CENTER_X = 230;
constexpr int START_GAME_MODE_CENTER_Y = 30;
constexpr double START_GAME_MODE_SCALE = 2.0;



//選択カーソル画像の数値(座標、中心座標、大きさ)
constexpr int TRIANGLE_POS_X = 1200;
constexpr int TRIANGLE_POS_Y = 400;
constexpr int TRIANGLE_CENTER_X = 280;
constexpr int TRIANGLE_CENTER_Y = 63;
constexpr int TRIANGLE_OFFSET_MODE_SELECT = 200;
constexpr int TRIANGLE_OFFSET_MODE_TUTORIAL_X= 150;
constexpr int TRIANGLE_OFFSET_MODE_TUTORIAL_Y= 150;
constexpr int TRIANGLE_POS_TUTORIAL_Y = 420;
constexpr double TRIANGLE_SCALE = 1.2;

const int EFFECT_TYPE_LIGHTNING = 0;
const int EFFECT_TYPE_ATOMOSPHERE = 1;

//手前ロボットサーベルエフェクト
//スケール
constexpr VECTOR EFFECT_LIGHTNING_FRONT_SCALE = { 5.0f,5.0f,10.0f };
//オフセット値
constexpr VECTOR EFFECT_LIGHTNING_FRONT_OFFSET = { 30.0f, 10.0f, 20.0f };
//回転
constexpr VECTOR EFFECT_LIGHTNING_FRONT_ROT = { 0.0f, 0.0f, 0.0f };

//奥ロボットサーベルエフェクト
//スケール
constexpr VECTOR EFFECT_LIGHTNING_BACK_SCALE = { 10.0f,15.0f,10.0f };
//オフセット値
constexpr VECTOR EFFECT_LIGHTNING_BACK_OFFSET = { -50.0f, 50.0f, 0.0f };
//回転
constexpr VECTOR EFFECT_LIGHTNING_BACK_ROT = { 0.0f, 0.0f, 0.0f };

//舞う破片エフェクト
constexpr VECTOR EFFECT_ATOMOSPHERE_SCALE = { 280.0f,280.0f,280.0f };
//
constexpr VECTOR EFFECT_ATOMOSPHERE_OFFSET = { 0.0f, -100.0f, 100.0f };

constexpr VECTOR EFFECT_ATOMOSPHERE_ROT = { 0.0f, 0.0f, 0.0f };



#pragma endregion

TitleScene::TitleScene(SceneManager& manager, Transitor& transit, Input& input) :Scene(manager,  transit,input),
resMng_(ResourceManager::GetInstance()), camera_(std::make_unique<Camera>()),stage_(std::make_unique<Stage>()),
skyDome_(std::make_unique<SkyDome>())

{
	//モード選択の初期カーソル位置を一人用に設定
	playMode_=0;
	//チュートリアル選択カーソル
	tutorialMode_ =0;
	//チュートリアル選択が終わっているかを判定
	isTutorialSelect_ =false;
	//ゲームモード選択が終わっているかを判定
	isGameModeSelect_ =false;
	//ボタンを押してねの状態にする(押されたら選択画面へ)
	isTitleStart_ = false;

	//最初のアップデートを初期化
	funcUpdate_ = std::bind(&TitleScene::TitleUpdate,this);

	//最初の描画を初期化
	funcDraw_ = std::bind(&TitleScene::DrawTitleLogo,this);
	
	//画像の読み込み
	InitImage();
	//モデルの読み込み
	InitModel();
	//カメラの初期設定
	InitCamera();
	//エフェクトの初期化
	InitEffect();
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

	sceneTransitor_.Start();

}

TitleScene::~TitleScene(void)
{

}

void TitleScene::Update(void)
{
	input_.Update();
	//ゲームモードの選択がされていたら、中に入らない
	if (isTutorialSelect_)
	{
		return;
	}

	funcUpdate_();

	if (!isTutorialSelect_)
	{
		skyDome_->Update();
		//エフェクトマネージャーの更新
		for (auto& effectManager: effectManagers_)
		{
			effectManager.second->Update();
		}
		//エフェクシアのアップデート
		UpdateEffekseer3D();
		sceneTransitor_.Update();
	}
}

void TitleScene::Draw(void)
{
	ClearDrawScreen();
	//カメラの更新
	camera_->SetBeforeDraw();
	//
	skyDome_->Draw();
	//ステージの描画
	stage_->Draw();
	//手前モデルの描画
	MV1DrawModel(frontTransform_.modelId);
	//奥モデルの描画
	MV1DrawModel(backTransform_.modelId);
	//ビームサーベルの描画
	for (auto& beamSaber : beamSabers_)
	{
		beamSaber->Draw();
	}

	//エフェクシアの描画
	DrawEffekseer3D();
	//ボタンを押してね画像描画
	DrawPleaseButton();

	funcDraw_();

	if (!isTutorialSelect_)
	{
		sceneTransitor_.Draw();
	}
}

void TitleScene::ChangeGameScene(void)
{
	//ゲームシーンへ移行
	sceneManager_.ChangeScene(std::make_shared<GameScene>(sceneManager_, playMode_, sceneTransitor_, input_));
}

void TitleScene::ChangeTutorialScene(void)
{
	//ゲームシーンへ移行
	sceneManager_.ChangeScene(std::make_shared<TutorialScene>(sceneManager_, playMode_, sceneTransitor_, input_));
}

void TitleScene::DrawPleaseButton(void)
{
	JOYPAD_TYPE type = input_.GetJPadType();
	//PSコントローラーなら×ボタン表記
	if (JOYPAD_TYPE::DUAL_SHOCK_4 == type || JOYPAD_TYPE::DUAL_SENSE == type)
	{
		DrawRotaGraph2(PLEASE_CROSS_BUTTON_POS_X, PLEASE_CROSS_BUTTON_POS_Y,
			PLEASE_CROSS_BUTTON_CENTER_X, PLEASE_CROSS_BUTTON_CENTER_Y,
			PLEASE_CROSS_BUTTON_SCALE, 0.0, imgType_[IMG_TYPE::PLEASE_CROSS], true, false);
	}
	else//XBOXコントローラーならAボタン表記
	{
		DrawRotaGraph2(PLEASE_A_BUTTON_POS_X, PLEASE_CROSS_BUTTON_POS_Y,
			PLEASE_A_BUTTON_CENTER_X, PLEASE_A_BUTTON_CENTER_Y,
			PLEASE_A_BUTTON_SCALE, 0.0, imgType_[IMG_TYPE::PLEASE_A], true, false);
	}
}

bool TitleScene::ButtonPush(void)
{

	JOYPAD_TYPE type = input_.GetJPadType();
	//プレステコンなら×ボタン
	if (JOYPAD_TYPE::DUAL_SHOCK_4 == type || JOYPAD_TYPE::DUAL_SENSE == type)
	{
		if (input_.IsTriggerd("b"))
		{
			return true;
		}
	}
	//XBOXならAボタン
	else if (JOYPAD_TYPE::XBOX_360 == type || JOYPAD_TYPE::XBOX_ONE == type)
	{
		if (input_.IsTriggerd("jump"))
		{
			return true;
		}
	}
	else
	{	
		if (input_.IsTriggerd("b"))
		{
			return true;
		}
	}
	return false;

}

bool TitleScene::SelectCursorGameMode(void)
{
	//現在接続されているコントローラの数
	int joyPadNum = GetJoypadNum();

	//上ボタンか十字キー上を押したら、カーソルを上に動かす
	if (input_.IsTriggerd("up") && playMode_ > static_cast<int>(GAME_MODE::SINGLE))
	{
		playMode_--;
	}
	//下ボタンか十字キー下を押したら、カーソルを上に動かす
	else if (joyPadNum>=2&&
			input_.IsTriggerd("down") &&
			playMode_ < static_cast<int>(GAME_MODE::MULTI)
			)
	{
		playMode_++;
	}
	return ButtonPush();
}

bool TitleScene::SelectCursorTutorialMode(void)
{

	//上ボタンか十字キー上を押したら、カーソルを上に動かす
	if (input_.IsTriggerd("up") && 
		tutorialMode_>static_cast<int>(TUTORIAL_MODE::NON))
	{
		tutorialMode_--;
	}
	//下ボタンか十字キー下を押したら、カーソルを上に動かす
	else if (	input_.IsTriggerd("down") && 
		tutorialMode_<static_cast<int>(TUTORIAL_MODE::TUTORIAL)
		)
	{
		tutorialMode_++;
	}
	return ButtonPush();

}

void TitleScene::SelectTutorialMode(void)
{

	//決定ボタンが押されたら選択済み状態にする
	if (SelectCursorTutorialMode())
	{
		//チュートリアルを飛ばすが選択されていたらゲームシーンに移行
		if (tutorialMode_ == static_cast<int>(TUTORIAL_MODE::NON))
		{
			effectManagers_[TYPE_ROBOT_FRONT]->Stop(EFFECT_TYPE_ATOMOSPHERE);
			isTutorialSelect_ = true;
			ChangeGameScene();
		}
		//チュートリアルを見るが選択されていたらチュートリアルシーンに移行
		else if(tutorialMode_ == static_cast<int>(TUTORIAL_MODE::TUTORIAL))
		{
			effectManagers_[TYPE_ROBOT_FRONT]->Stop(EFFECT_TYPE_ATOMOSPHERE);
			isTutorialSelect_ = true;
			ChangeTutorialScene();
		}
	}
}

void TitleScene::InitImage(void)
{
	//画像読み込み
	imgType_.emplace(IMG_TYPE::TITLE_LOGO, resMng_.Load(ResourceManager::SRC::TIRLE_LOGO_IMAGE).handleId_);
	imgType_.emplace(IMG_TYPE::PLEASE_A, resMng_.Load(ResourceManager::SRC::PLEASE_A).handleId_);
	imgType_.emplace(IMG_TYPE::PLEASE_CROSS, resMng_.Load(ResourceManager::SRC::PLEASE_CROSS).handleId_);
	imgType_.emplace(IMG_TYPE::SINGLE_MODE, resMng_.Load(ResourceManager::SRC::SINGLE_PLAY_LOGO).handleId_);
	imgType_.emplace(IMG_TYPE::MULTI_PLAY, resMng_.Load(ResourceManager::SRC::MULTI_PLAY_LOGO).handleId_);
	imgType_.emplace(IMG_TYPE::TRIANGLE, resMng_.Load(ResourceManager::SRC::TRIANGLE).handleId_);
	imgType_.emplace(IMG_TYPE::GAME_START, resMng_.Load(ResourceManager::SRC::GAME_START).handleId_);
	imgType_.emplace(IMG_TYPE::TUTORIAL, resMng_.Load(ResourceManager::SRC::WATCH_TUTORIAL).handleId_);
}

void TitleScene::InitModel(void)
{
	//手前ロボットの初期設定
	//モデルの読み込み
	frontTransform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::ROBOT));
	//モデルの大きさ
	frontTransform_.scl = AsoUtility::VECTOR_ONE;
	//モデルの座標
	frontTransform_.pos = FRONT_ROBOT_POS;
	//モデルの回転
	frontTransform_.quaRot = Quaternion();
	//モデルのローカル回転
	frontTransform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(160.0f), 0.0f });
	//モデルの設定を更新
	frontTransform_.Update();
	//モデルのポーズを読み込み、設定
	std::string model = PATH_ANIMATION_PLAYER + "Win.mv1";
	int animModel = MV1LoadModel(model.c_str());
	MV1AttachAnim(frontTransform_.modelId, 0, animModel);
	//ビームサーベルの生成
	beamSabers_.emplace_back(std::make_unique<BeamSaber>(0, frontTransform_));

	//奥ロボットの初期設定
	//モデルの読み込み
	backTransform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::ROBOT_CASVAL));
	//モデルの大きさ
	backTransform_.scl = AsoUtility::VECTOR_ONE;
	//モデルの座標
	backTransform_.pos = BACK_ROBOT_POS;
	//モデルの回転
	backTransform_.quaRot = Quaternion();
	//モデルの設定を更新
	backTransform_.Update();
	//モデルのポーズを読み込み、設定
	MV1AttachAnim(backTransform_.modelId, 0, animModel);
	//ビームサーベルの生成
	beamSabers_.emplace_back(std::make_unique<BeamSaber>(0, backTransform_));

	//ビームサーベルをロボットの手に同期させる
	for (auto& beamSaber : beamSabers_)
	{
		beamSaber->Activate();
		beamSaber->Update();
	}

}

void TitleScene::InitCamera(void)
{
	//カメラ設定
	camera_->ChangeMode(Camera::MODE::FIXED_POINT);
	camera_->SetTargetPos(CAMERA_TARGET_POS);
	camera_->SetCameraPos(CAMERA_POS);
}

void TitleScene::InitEffect(void)
{

	effectManagers_.emplace(
					TYPE_ROBOT_FRONT,
					std::make_unique <EffectManager>(beamSabers_[TYPE_ROBOT_FRONT]->GetTransform()) );

	effectManagers_[TYPE_ROBOT_FRONT]->Add(EFFECT_TYPE_LIGHTNING,
		EFFECT_LIGHTNING_FRONT_SCALE,
		EFFECT_LIGHTNING_FRONT_OFFSET,	
		EFFECT_LIGHTNING_FRONT_ROT,
		true,
		true,
		resMng_.Load(ResourceManager::SRC::LIGHTNING).handleId_
	);

	effectManagers_[TYPE_ROBOT_FRONT]->Add(EFFECT_TYPE_ATOMOSPHERE,
		EFFECT_ATOMOSPHERE_SCALE,
		EFFECT_ATOMOSPHERE_OFFSET,
		EFFECT_ATOMOSPHERE_ROT,
		true,
		true,
		resMng_.Load(ResourceManager::SRC::ATOMOSPHERE).handleId_
	);

	//サーベルのエフェクト
	effectManagers_[TYPE_ROBOT_FRONT]->Play(EFFECT_TYPE_LIGHTNING,true);
	//霧のようなエフェクト
	effectManagers_[TYPE_ROBOT_FRONT]->Play(EFFECT_TYPE_ATOMOSPHERE);



	effectManagers_.emplace(
		TYPE_ROBOT_BACK,
		std::make_unique <EffectManager>(beamSabers_[TYPE_ROBOT_BACK]->GetTransform()));

	effectManagers_[TYPE_ROBOT_BACK]->Add(EFFECT_TYPE_LIGHTNING,
		EFFECT_LIGHTNING_BACK_SCALE,
		EFFECT_LIGHTNING_BACK_OFFSET,
		EFFECT_LIGHTNING_BACK_ROT,
		true,
		false,
		resMng_.Load(ResourceManager::SRC::LIGHTNING).handleId_
	);
	//サーベルのエフェクト
	effectManagers_[TYPE_ROBOT_BACK]->Play(EFFECT_TYPE_LIGHTNING,true);


}

void TitleScene::TitleUpdate(void)
{
	//ボタンが押されたらタイトル画面からゲームモードセレクト画面に変える
	if (ButtonPush() && !isTitleStart_)
	{
		funcUpdate_ = std::bind(&TitleScene::SelectGameMode, this);
		funcDraw_ = std::bind(&TitleScene::DrawSelectGameMode, this);

		isTitleStart_ = true;
	}
}

void TitleScene::UpdateSelectGameMode(void)
{
	SelectGameMode();
}

void TitleScene::UpdateSelectTutorial(void)
{
	SelectTutorialMode();
}

void TitleScene::DrawTitleLogo(void)
{
	DrawRotaGraph2(TITLE_LOGO_POS_X, TITLE_LOGO_POS_Y,
		TITLE_LOGO_CENTER_X, TITLE_LOGO_CENTER_Y,
		TITLE_LOGO_SCALE, 0.0, imgType_[IMG_TYPE::TITLE_LOGO], true, false);

}

void TitleScene::DrawSelectGameMode(void)
{
	//現在接続されているコントローラの数
	int joyPadNum = GetJoypadNum();

	//シングルモード選択画像
	DrawRotaGraph2(SINGLE_MODE_POS_X, SINGLE_MODE_POS_Y,
		SINGLE_MODE_CENTER_X, SINGLE_MODE_CENTER_Y, SINGLE_MODE_SCALE, 0.0, imgType_[IMG_TYPE::SINGLE_MODE], true, false);

	if (joyPadNum >= 2)
	{
		//対戦モード選択画像
		DrawRotaGraph2(MULTI_PLAY_POS_X, MULTI_PLAY_POS_Y,
			MULTI_PLAY_CENTER_X, MULTI_PLAY_CENTER_Y, MULTI_PLAY_SCALE, 0.0, imgType_[IMG_TYPE::MULTI_PLAY], true, false);
	}

	//カーソル選択画像
	DrawRotaGraph2(TRIANGLE_POS_X, TRIANGLE_POS_Y + (TRIANGLE_OFFSET_MODE_SELECT * playMode_),
		TRIANGLE_CENTER_X, TRIANGLE_CENTER_Y, TRIANGLE_SCALE, 0.0, imgType_[IMG_TYPE::TRIANGLE], true, false);
}

void TitleScene::DrawSelectTutorial(void)
{
	int SCX = static_cast<int>(SCREEN_SIZE.x);
	int SCY = static_cast<int>(SCREEN_SIZE.y);

	//シングルモード選択画像
	DrawRotaGraph2(START_GAME_MODE_POS_X, START_GAME_MODE_POS_Y,
		START_GAME_MODE_CENTER_X,START_GAME_MODE_CENTER_Y ,
		START_GAME_MODE_SCALE, 0.0, imgType_[IMG_TYPE::GAME_START], true, false);

	//対戦モード選択画像
	DrawRotaGraph2(TUTORIAL_MODE_POS_X, TUTORIAL_MODE_POS_Y,
		TUTORIAL_MODE_CENTER_X, TUTORIAL_MODE_CENTER_Y,
		TUTORIAL_MODE_SCALE, 0.0, imgType_[IMG_TYPE::TUTORIAL], true, false);

	//カーソル選択画像
	DrawRotaGraph2(TRIANGLE_POS_X + TRIANGLE_OFFSET_MODE_TUTORIAL_X, TRIANGLE_POS_TUTORIAL_Y + (TRIANGLE_OFFSET_MODE_SELECT * tutorialMode_),
		TRIANGLE_CENTER_X, TRIANGLE_CENTER_Y, TRIANGLE_SCALE, 0.0, imgType_[IMG_TYPE::TRIANGLE], true, false);

}

void TitleScene::SelectGameMode(void)
{
	//決定ボタンが押されたら選択済み状態にする
	if (SelectCursorGameMode())
	{
		isGameModeSelect_ = true;
		funcUpdate_ = std::bind(&TitleScene::UpdateSelectTutorial, this);
		funcDraw_ = std::bind(&TitleScene::DrawSelectTutorial, this);
	}
}