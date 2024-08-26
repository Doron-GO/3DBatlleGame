#include<EffekseerForDXLib.h>
#include"../Manager/ResourceManager.h"
#include"../Object/Time/TimeCount.h"
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

#pragma region Parameter

//�^�C�g���ɖ߂�摜
constexpr int BACK_TO_TITLE_POS_X = 800;
constexpr int BACK_TO_TITLE_POS_Y = 500;
constexpr int BACK_TO_TITLE_CENTER_X = 250;
constexpr int BACK_TO_TITLE_CENTER_Y = 50;
constexpr double BACK_TO_TITLE_SCALE = 1.5;

//�������摜
constexpr int ONE_MOR_FIGHT_POS_X = 800;
constexpr int ONE_MOR_FIGHT_POS_Y = 700;
constexpr int ONE_MOR_FIGHT_CENTER_X = 273;
constexpr int ONE_MOR_FIGHT_CENTER_Y = 54;
constexpr double ONE_MOR_FIGHT_SCALE = 1.5;

//�~�{�^���������Ă���摜
constexpr int  PLEASE_CROSS_BUTTON_POS_X = 800;
constexpr int  PLEASE_CROSS_BUTTON_POS_Y = 900;
constexpr int  PLEASE_CROSS_BUTTON_CENTER_X = 480;
constexpr int  PLEASE_CROSS_BUTTON_CENTER_Y = 52;
constexpr double  PLEASE_CROSS_BUTTON_SCALE = 1.2;

//A�{�^���������Ă���摜
constexpr int  PLEASE_A_BUTTON_POS_X = 800;
constexpr int  PLEASE_A_BUTTON_POS_Y = 800;
constexpr int  PLEASE_A_BUTTON_CENTER_X = 395;
constexpr int  PLEASE_A_BUTTON_CENTER_Y = 34;
constexpr double  PLEASE_A_BUTTON_SCALE = 1.2;

//�Z���N�g�摜�̐��l
constexpr int  TRIANGLE_POS_X = 1200;
constexpr int  TRIANGLE_POS_Y = 500;
constexpr int  TRIANGLE_CENTER_X = 280;
constexpr int  TRIANGLE_CENTER_Y = 63;
constexpr int  TRIANGLE_OFFSET = 200;
constexpr double  TRIANGLE_SCALE = 1.2;

//�Q�[���J�n���ɏo��READY�̉摜�̐��l
constexpr int READY_POS_X = 800;
constexpr int  READY_POS_Y = 400;
constexpr int  READY_CENTER_X = 117;
constexpr int  READY_CENTER_Y = 44;
constexpr int  READY_OFFSET = 200;
constexpr double  READY_SCALE = 3.0;

//�Q�[���J�n���ɏo��FIGHT�̉摜�̐��l
constexpr int FIGHT_POS_X = 800;
constexpr int  FIGHT_POS_Y = 400;
constexpr int  FIGHT_CENTER_X = 120;
constexpr int  FIGHT_CENTER_Y = 49;
constexpr int  FIGHT_OFFSET = 200;
constexpr double  FIGHT_SCALE = 4.5;

constexpr float START_TIME = 3.0f;

constexpr float READY_TIME = 1.5f;

#pragma endregion


GameScene::GameScene(SceneManager& manager, int playMode, Transitor& transit, Input& input):Scene(manager, transit,input), 
resMng_(ResourceManager::GetInstance())
{
	sceneTransitor_.Start();
	playMode_ = playMode;
	imgH_.emplace(IMG_H::BACK_TO_TITLE, resMng_.Load(ResourceManager::SRC::BACK_TO_TITLE).handleId_);
	imgH_.emplace(IMG_H::ONE_MOR_FIGHT, resMng_.Load(ResourceManager::SRC::ONE_MOR_FIGHT).handleId_);
	imgH_.emplace(IMG_H::PLEASE_A, resMng_.Load(ResourceManager::SRC::PLEASE_A).handleId_);
	imgH_.emplace(IMG_H::PLEASE_CROSS, resMng_.Load(ResourceManager::SRC::PLEASE_CROSS).handleId_);
	imgH_.emplace(IMG_H::TRIANGLE, resMng_.Load(ResourceManager::SRC::TRIANGLE).handleId_);
	imgH_.emplace(IMG_H::READY, resMng_.Load(ResourceManager::SRC::READY_IMAGE).handleId_);
	imgH_.emplace(IMG_H::FIGHT, resMng_.Load(ResourceManager::SRC::FIGHT_IMAGE).handleId_);

	selectNum_ = 0;

	startCount_ = 0.0f;

	//�v���C���[�P�̃p�b�h�̎�ނ��擾
	joyPadType_ = input_.GetJPadType();

	int SCX = static_cast<int>(sceneManager_.SCREEN_SIZE_X);
	int SCY = static_cast<int>(sceneManager_.SCREEN_SIZE_Y);
	
	//�v���C���[�P�ƂQ�̉�ʂ����
	cameraScreens_.emplace_back(MakeScreen(SCX /2, SCY, true));
	cameraScreens_.emplace_back(MakeScreen(SCX /2, SCY, true));

	//�v���C���[�P�ƂQ�̗����̉�ʂ�`�悷��X�N���[�������
	integrationScreen_= MakeScreen(SCX, SCY, true);

	//�v���C���[�P�ƂQ�̉�ʂ̍��W
	screenPos_.emplace_back(VECTOR(0, 0));
	screenPos_.emplace_back(VECTOR(sceneManager_.SCREEN_SIZE_X / 2, 0));

	//�v���C���[�ƃ{�X���Ǘ�����}�l�[�W���\�̐���
	ActorManager_ = std::make_unique<ActorManager>(playMode);

	//�X�e�[�W�̐���
	stage_ = std::make_unique<Stage>();

	//�X�e�[�W�̃R���W������ǉ�
	ActorManager_->AddClliders(stage_->GetCollider());

	if (playMode == SINGLE_PLEY_MODE)
	{
		_draw = &GameScene::DrawSIngleMode;
	}
	else
	{
		_draw = &GameScene::DrawBattleMode;
	}
	//���A�b�v�f�[�g�����Ă���
	ActorManager_->Update();

	// Z�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(true);

	// Z�o�b�t�@�ւ̏������݂�L���ɂ���
	SetWriteZBuffer3D(true);

	// �o�b�N�J�����O��L���ɂ���
	SetUseBackCulling(true);
	// ���C�g�̐ݒ�
	ChangeLightTypeDir({ 0.3f, -0.7f, 0.8f });

	// �w�i�F�ݒ�
	SetBackgroundColor(0, 139, 139);

	// �t�H�O�ݒ�
	SetFogEnable(true);
	SetFogColor(5, 5, 5);
	SetFogStartEnd(10000.0f, 20000.0f);

}

GameScene::~GameScene()
{
}

void GameScene::Update()
{
	deltaTime_ = DeltaTime::GetInstsnce().GetDeltaTime();
	input_.Update();
	if (IsStart())
	{		
		ActorManager_->Update();
	}		
	if (SelectCursor())
	{
		return;
	}
	sceneTransitor_.Update();
}

void GameScene::Draw()
{	
	(this->*_draw)();
	sceneTransitor_.Draw();
}
void GameScene::DrawSIngleMode(void)
{
	ClearDrawScreen();
	ActorManager_->DrawCamera(SINGLE_PLEY_MODE);
	ActorManager_->Draw();
	stage_->Draw();
	ActorManager_->DrawUI(SINGLE_PLEY_MODE);
	UpdateEffekseer3D();
	DrawEffekseer3D();
	GameUI();
}
void GameScene::DrawBattleMode(void)
{
	for (int idx = 0; idx < 2; idx++)
	{
		SetDrawScreen(cameraScreens_[idx]);
		// ��ʂ�������
		ClearDrawScreen();
		ActorManager_->DrawCamera(idx);
		ActorManager_->Draw();
		stage_->Draw();
		ActorManager_->DrawUI(idx);
		//ActorManager_->DrawDebug(idx);
		//ActorManager_->DrawAnimeDebug(idx);
		UpdateEffekseer3D();
		DrawEffekseer3D();
	}
	SetDrawScreen(integrationScreen_);
	for (int idx = 0; idx <= 1; idx++)
	{
		DrawGraph(static_cast<int>(screenPos_[idx].x),
			static_cast<int>(screenPos_[idx].y),
			cameraScreens_[idx], false);//���ߏ����͌�Ō�����
	}
	//�񕪊�����Ă����ʂ̐^�񒆂ɐ�������
	DrawLine(static_cast<int>(screenPos_[1].x), 0, static_cast<int>(screenPos_[1].x), 1000, 0x000000, true);
	SetDrawScreen(DX_SCREEN_BACK);
	DrawGraph(0, 0, integrationScreen_, true);
	GameUI();
}
void GameScene::ChangeGameScene(void)
{
	sceneManager_.ChangeScene(std::make_shared<GameScene>(sceneManager_, playMode_, sceneTransitor_, input_));
}

void GameScene::ChangeTitleScene()
{
	sceneManager_.ChangeScene(std::make_shared<TitleScene>(sceneManager_, sceneTransitor_,input_));
}

bool GameScene::IsStart(void)
{
	if (startCount_ < START_TIME)
	{
		startCount_ += deltaTime_;
	}
	else
	{
		startCount_ = START_TIME;
		return true;
	}
	return false;
}

void GameScene::GameUI(void)
{	
	GameSetUI();
	StartUI();
}

void GameScene::StartUI(void)
{
	if ( startCount_ < START_TIME)
	{
		if (startCount_ < READY_TIME)
		{
			DrawRotaGraph2(READY_POS_X, READY_POS_Y,
				READY_CENTER_X, READY_CENTER_Y, READY_SCALE, 0.0, imgH_[IMG_H::READY], true, false);
		}
		else
		{
			DrawRotaGraph2(FIGHT_POS_X, FIGHT_POS_Y,
				FIGHT_CENTER_X, FIGHT_CENTER_Y, FIGHT_SCALE, 0.0, imgH_[IMG_H::FIGHT], true, false);
		}
	}
}

void GameScene::GameSetUI(void)
{
	if (!ActorManager_->IsGameSet())
	{
		return;
	}

	DrawRotaGraph2(BACK_TO_TITLE_POS_X, BACK_TO_TITLE_POS_Y,
		BACK_TO_TITLE_CENTER_X, BACK_TO_TITLE_CENTER_Y, BACK_TO_TITLE_SCALE, 0.0, imgH_[IMG_H::BACK_TO_TITLE], true, false);

	DrawRotaGraph2(ONE_MOR_FIGHT_POS_X, ONE_MOR_FIGHT_POS_Y,
		ONE_MOR_FIGHT_CENTER_X, ONE_MOR_FIGHT_CENTER_Y, ONE_MOR_FIGHT_SCALE, 0.0, imgH_[IMG_H::ONE_MOR_FIGHT], true, false);

	//�ڑ����ꂽ�R���g���[����Ps�Ȃ�~�{�^���\�L
	if (JOYPAD_TYPE::DUAL_SHOCK_4 == joyPadType_ || JOYPAD_TYPE::DUAL_SENSE == joyPadType_)
	{
		DrawRotaGraph2(PLEASE_CROSS_BUTTON_POS_X, PLEASE_CROSS_BUTTON_POS_Y,
			PLEASE_CROSS_BUTTON_CENTER_X, PLEASE_CROSS_BUTTON_CENTER_Y, PLEASE_CROSS_BUTTON_SCALE, 0.0, imgH_[IMG_H::PLEASE_CROSS], true, false);
	}//����ȊO�Ȃ�X�{�b�N�X�\�L
	else
	{
		DrawRotaGraph2(PLEASE_A_BUTTON_POS_X, PLEASE_A_BUTTON_POS_Y,
			PLEASE_A_BUTTON_CENTER_X, PLEASE_A_BUTTON_CENTER_Y, PLEASE_A_BUTTON_SCALE, 0.0, imgH_[IMG_H::PLEASE_A], true, false);
	}
	DrawRotaGraph2(TRIANGLE_POS_X, TRIANGLE_POS_Y + (TRIANGLE_OFFSET * selectNum_ ),
		TRIANGLE_CENTER_X, TRIANGLE_CENTER_Y, TRIANGLE_SCALE, 0.0, imgH_[IMG_H::TRIANGLE], true, false);

}

bool GameScene::SelectCursor(void)
{
	if (!ActorManager_->IsGameSet())
	{
		return false;
	}

	if (input_.IsTriggerd("up") && selectNum_> static_cast<int>(SELECT::BACK_TO_TITLE))
	{
		selectNum_--;
	}
	else if (input_.IsTriggerd("down") && selectNum_ < static_cast<int>(SELECT::ONE_MORE_FIGHT))
	{
		selectNum_++;
	}
	else
	{
	}
	return SelectDecide();
}

bool GameScene::SelectDecide(void)
{	//PS�R���̎g�p���Ɂ~�{�^���������ꂽ��
	if (JOYPAD_TYPE::DUAL_SHOCK_4 == joyPadType_ ||
		JOYPAD_TYPE::DUAL_SENSE == joyPadType_ &&
		input_.IsTriggerd("b"))
	{
		TitleOrGame();
		return true;
	}
	//XBOX�Ȃ�A�{�^��
	else if (JOYPAD_TYPE::XBOX_360 == joyPadType_ ||
		JOYPAD_TYPE::XBOX_ONE == joyPadType_ &&
		input_.IsTriggerd("jump"))
	{
		TitleOrGame();
		return true;
	}
	//���̑��Ȃ�v���X�e�R���ł����~�{�^���̏ꏊ�̃{�^��
	else
	{
	}
	return false;
}

void GameScene::TitleOrGame(void)
{
	if (selectNum_ == static_cast<int>(SELECT::BACK_TO_TITLE))
	{
		ChangeTitleScene();
	}
	else if (selectNum_ == static_cast<int>(SELECT::ONE_MORE_FIGHT))
	{
		ChangeGameScene();
	}
	else
	{
		ChangeTitleScene();
	}
}




