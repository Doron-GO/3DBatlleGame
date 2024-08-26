#include"../Manager/ResourceManager.h"
#include"../Object/Weapon/BeamSaber.h"
#include "../Object/Camera/Camera.h"
#include "../Scene/SceneManager.h"
#include"../Utility/AsoUtility.h"
#include"Transition/Transitor.h"
#include"../Object/Stage/Stage.h"
#include "../Input/Input.h"
#include"../../Config.h"
#include "TitleScene.h"
#include"GameScene.h"
#include<DxLib.h>

#pragma region Parameter

constexpr VECTOR  FRONT_ROBOT_POS = { 0.0f, -25.0f, 0.0f };
constexpr VECTOR  BACK_ROBOT_POS = { 0.0f, -25.0f, 5000.0f };

constexpr VECTOR  CAMERA_POS = { 100.0f, 10.0f, -130.0f };
constexpr VECTOR  CAMERA_TARGET_POS = { 100.0f, 15.0f, 0.0f };

constexpr int  TITLE_LOGO_POS_X = 775;
constexpr int  TITLE_LOGO_POS_Y = 400;
constexpr int  TITLE_LOGO_CENTER_X = 320;
constexpr int  TITLE_LOGO_CENTER_Y = 60;
constexpr double  TITLE_LOGO_SCALE = 2.5;

constexpr int  PLEASE_CROSS_BUTTON_POS_X = 800;
constexpr int  PLEASE_CROSS_BUTTON_POS_Y = 800;
constexpr int  PLEASE_CROSS_BUTTON_CENTER_X = 480;
constexpr int  PLEASE_CROSS_BUTTON_CENTER_Y = 52;
constexpr double  PLEASE_CROSS_BUTTON_SCALE = 1.2;

constexpr int  PLEASE_A_BUTTON_POS_X = 800;
constexpr int  PLEASE_A_BUTTON_POS_Y = 800;
constexpr int  PLEASE_A_BUTTON_CENTER_X = 395;
constexpr int  PLEASE_A_BUTTON_CENTER_Y = 34;
constexpr double  PLEASE_A_BUTTON_SCALE = 1.2;

constexpr int  SINGLE_MODE_POS_X = 775;
constexpr int  SINGLE_MODE_POS_Y = 400;
constexpr int  SINGLE_MODE_CENTER_X = 165;
constexpr int  SINGLE_MODE_CENTER_Y = 30;
constexpr double  SINGLE_MODE_SCALE = 2.0;

constexpr int  MULTI_PLAY_POS_X = 775;
constexpr int  MULTI_PLAY_POS_Y = 600;
constexpr int  MULTI_PLAY_CENTER_X = 165;
constexpr int  MULTI_PLAY_CENTER_Y = 52;
constexpr double  MULTI_PLAY_SCALE = 2.0;

constexpr int  TRIANGLE_POS_X = 1200;
constexpr int  TRIANGLE_POS_Y = 400;
constexpr int  TRIANGLE_CENTER_X = 280;
constexpr int  TRIANGLE_CENTER_Y = 63;
constexpr int  TRIANGLE_OFFSET = 200;
constexpr double  TRIANGLE_SCALE = 1.2;

#pragma endregion

TitleScene::TitleScene(SceneManager& manager, Transitor& transit, Input& input) :Scene(manager,  transit,input),
resMng_(ResourceManager::GetInstance()), camera_(std::make_unique<Camera>()),stage_(std::make_unique<Stage>())
{
	sceneTransitor_.Start();
	selectNum_=0;
	startFlag_ = false;
	InitImage();
	InitModel();
	InitCamera();

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

}

TitleScene::~TitleScene()
{

}

void TitleScene::Update()
{
	input_.Update();
	if (startFlag_&&SelectDecide())
	{
		return;
	}
	if (!startFlag_ && ButtonPush())
	{
		startFlag_ = true;
	}
	sceneTransitor_.Update();
}

void TitleScene::Draw()
{
	ClearDrawScreen();
	camera_->SetBeforeDraw();
	stage_->Draw();
	MV1DrawModel(frontTransform_.modelId);
	MV1DrawModel(backTransform_.modelId);
	for (auto& beamSaber : beamSabers_)
	{
		beamSaber->Draw();
	}
	DrawPleaseButton();
	if (!startFlag_)
	{
		DrawRotaGraph2(TITLE_LOGO_POS_X, TITLE_LOGO_POS_Y,
			TITLE_LOGO_CENTER_X, TITLE_LOGO_CENTER_Y, TITLE_LOGO_SCALE, 0.0, imgH_[IMG_H::TITLE_LOGO], true, false);
	}
	else
	{
		DrawModeSelect();
	}

	sceneTransitor_.Draw();
}

void TitleScene::ChangeGameScene(void)
{
	sceneManager_.ChangeScene(std::make_shared<GameScene>(sceneManager_, selectNum_,sceneTransitor_,input_));
}

void TitleScene::DrawPleaseButton(void)
{
	JOYPAD_TYPE type = input_.GetJPadType();
	if (JOYPAD_TYPE::DUAL_SHOCK_4 == type || JOYPAD_TYPE::DUAL_SENSE == type)
	{
		DrawRotaGraph2(PLEASE_CROSS_BUTTON_POS_X, PLEASE_CROSS_BUTTON_POS_Y,
			PLEASE_CROSS_BUTTON_CENTER_X, PLEASE_CROSS_BUTTON_CENTER_Y,
			PLEASE_CROSS_BUTTON_SCALE, 0.0, imgH_[IMG_H::PLEASE_CROSS], true, false);
	}
	else
	{
		DrawRotaGraph2(PLEASE_A_BUTTON_POS_X, PLEASE_CROSS_BUTTON_POS_Y,
			PLEASE_A_BUTTON_CENTER_X, PLEASE_A_BUTTON_CENTER_Y,
			PLEASE_A_BUTTON_SCALE, 0.0, imgH_[IMG_H::PLEASE_A], true, false);
	}
}

void TitleScene::DrawModeSelect(void)
{
	//シングルモード選択画像
	DrawRotaGraph2(SINGLE_MODE_POS_X, SINGLE_MODE_POS_Y,
		SINGLE_MODE_CENTER_X, SINGLE_MODE_CENTER_Y, SINGLE_MODE_SCALE, 0.0, imgH_[IMG_H::SINGLE_MODE], true, false);

	DrawRotaGraph2(MULTI_PLAY_POS_X, MULTI_PLAY_POS_Y,
		MULTI_PLAY_CENTER_X, MULTI_PLAY_CENTER_Y, MULTI_PLAY_SCALE, 0.0, imgH_[IMG_H::MULTI_PLAY], true, false);

	DrawRotaGraph2(TRIANGLE_POS_X, TRIANGLE_POS_Y + (TRIANGLE_OFFSET * selectNum_),
		TRIANGLE_CENTER_X, TRIANGLE_CENTER_Y, TRIANGLE_SCALE, 0.0, imgH_[IMG_H::TRIANGLE], true, false);
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
	//その他ならプレステコンでいう×ボタンの場所のボタン
	else
	{	
	}
	return false;

}

bool TitleScene::SelectCursor(void)
{
	if (input_.IsTriggerd("up") && selectNum_ > static_cast<int>(GAME_MODE::SINGLE))
	{
		selectNum_--;
	}
	else if (input_.IsTriggerd("down") && selectNum_ < static_cast<int>(GAME_MODE::MULTI))
	{
		selectNum_++;
	}
	else
	{
	}
	return ButtonPush();
}

bool TitleScene::SelectDecide(void)
{
	if (SelectCursor())
	{
		ChangeGameScene();
		return true;
	}
	return false;
}

void TitleScene::InitImage(void)
{
	imgH_.emplace(IMG_H::TITLE_LOGO, resMng_.Load(ResourceManager::SRC::TIRLE_LOGO_IMAGE).handleId_);
	imgH_.emplace(IMG_H::PLEASE_A, resMng_.Load(ResourceManager::SRC::PLEASE_A).handleId_);
	imgH_.emplace(IMG_H::PLEASE_CROSS, resMng_.Load(ResourceManager::SRC::PLEASE_CROSS).handleId_);
	imgH_.emplace(IMG_H::SINGLE_MODE, resMng_.Load(ResourceManager::SRC::SINGLE_PLAY_LOGO).handleId_);
	imgH_.emplace(IMG_H::MULTI_PLAY, resMng_.Load(ResourceManager::SRC::MULTI_PLAY_LOGO).handleId_);
	imgH_.emplace(IMG_H::TRIANGLE, resMng_.Load(ResourceManager::SRC::TRIANGLE).handleId_);
}

void TitleScene::InitModel(void)
{
	//手前ロボットの初期設定
	frontTransform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER));
	frontTransform_.scl = AsoUtility::VECTOR_ONE;
	frontTransform_.pos = FRONT_ROBOT_POS;
	frontTransform_.quaRot = Quaternion();
	frontTransform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(160.0f), 0.0f });
	frontTransform_.Update();
	std::string model = PATH_ANIMATION_PLAYER + "Win.mv1";
	int animModel = MV1LoadModel(model.c_str());
	MV1AttachAnim(frontTransform_.modelId, 0, animModel);
	beamSabers_.emplace_back(std::make_unique<BeamSaber>(0, frontTransform_));

	//奥ロボットの初期設定
	backTransform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER));
	backTransform_.scl = AsoUtility::VECTOR_ONE;
	backTransform_.pos = BACK_ROBOT_POS;
	backTransform_.quaRot = Quaternion();
	backTransform_.Update();
	MV1AttachAnim(backTransform_.modelId, 0, animModel);
	beamSabers_.emplace_back(std::make_unique<BeamSaber>(0, backTransform_));

	for (auto& beamSaber : beamSabers_)
	{
		beamSaber->Activate();
		beamSaber->Update();
	}
}

void TitleScene::InitCamera(void)
{
	camera_->ChangeMode(Camera::MODE::FIXED_POINT);
	camera_->SetTargetPos(CAMERA_TARGET_POS);
	camera_->SetCameraPos(CAMERA_POS);
}
