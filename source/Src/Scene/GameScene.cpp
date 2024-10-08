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

//�V���O���v���C���[�h
static constexpr int SINGLE_PLAY_MODE = 0;

//�^�C�g���ɖ߂�摜
static constexpr int BACK_TO_TITLE_POS_X = 800;
static constexpr int BACK_TO_TITLE_POS_Y = 500;
static constexpr int BACK_TO_TITLE_CENTER_X = 250;
static constexpr int BACK_TO_TITLE_CENTER_Y = 50;
static constexpr double BACK_TO_TITLE_SCALE = 1.5;

//�������摜
static constexpr int ONE_MOR_FIGHT_POS_X = 800;
static constexpr int ONE_MOR_FIGHT_POS_Y = 700;
static constexpr int ONE_MOR_FIGHT_CENTER_X = 273;
static constexpr int ONE_MOR_FIGHT_CENTER_Y = 54;
static constexpr double ONE_MOR_FIGHT_SCALE = 1.5;

//�~�{�^���������Ă���摜
static constexpr int  PLEASE_CROSS_BUTTON_POS_X = 800;
static constexpr int  PLEASE_CROSS_BUTTON_POS_Y = 900;
static constexpr int  PLEASE_CROSS_BUTTON_CENTER_X = 480;
static constexpr int  PLEASE_CROSS_BUTTON_CENTER_Y = 52;
static constexpr double  PLEASE_CROSS_BUTTON_SCALE = 1.2;

//A�{�^���������Ă���摜
static constexpr int  PLEASE_A_BUTTON_POS_X = 800;
static constexpr int  PLEASE_A_BUTTON_POS_Y = 800;
static constexpr int  PLEASE_A_BUTTON_CENTER_X = 395;
static constexpr int  PLEASE_A_BUTTON_CENTER_Y = 34;
static constexpr double  PLEASE_A_BUTTON_SCALE = 1.2;

//�Z���N�g�摜�̐��l
static constexpr int  TRIANGLE_POS_X = 1200;
static constexpr int  TRIANGLE_POS_Y = 500;
static constexpr int  TRIANGLE_CENTER_X = 280;
static constexpr int  TRIANGLE_CENTER_Y = 63;
static constexpr int  TRIANGLE_OFFSET = 200;
static constexpr double  TRIANGLE_SCALE = 1.2;

//�Q�[���J�n���ɏo��READY�̉摜�̐��l
static constexpr int READY_POS_X = 800;
static constexpr int  READY_POS_Y = 400;
static constexpr int  READY_CENTER_X = 117;
static constexpr int  READY_CENTER_Y = 44;
static constexpr int  READY_OFFSET = 200;
static constexpr double  READY_SCALE = 3.0;

//�Q�[���J�n���ɏo��FIGHT�̉摜�̐��l
static constexpr int FIGHT_POS_X = 800;
static constexpr int  FIGHT_POS_Y = 400;
static constexpr int  FIGHT_CENTER_X = 120;
static constexpr int  FIGHT_CENTER_Y = 49;
static constexpr int  FIGHT_OFFSET = 200;
static constexpr double  FIGHT_SCALE = 4.5;

//�Q�[���X�^�[�g�J�n�b��
static constexpr float START_TIME_MAX = 3.0f;

//��ʕ����p�̐��̑���
static constexpr int  LINE_THICKNES = 1000;

#pragma endregion


GameScene::GameScene(SceneManager& manager, int playMode, Transitor& transit, Input& input):Scene(manager, transit,input), 
resMng_(ResourceManager::GetInstance())
{
	sceneTransitor_.Start();
	
	//�^�C�g���V�[���Ō��܂����Q�[�����[�h���i�[
	playMode_ = playMode;

	//�Đ탂�[�h�I���̏����J�[�\���́u�^�C�g���ɖ߂�v�ɐݒ�
	rematchMode_ = 0;

	//�Q�[���X�^�[�g�v���ϐ���������
	startCount_ = 0.0f;

	//�v���C���[�P�̃p�b�h�̎�ނ��擾
	joyPadType_ = input_.GetJPadType();

	//�v���C���[�P�ƂQ�̉�ʂ����
	int SCX = static_cast<int>(SCREEN_SIZE.x);
	int SCY = static_cast<int>(SCREEN_SIZE.y);
	cameraScreens_.emplace_back(MakeScreen(SCX /2, SCY, true));
	cameraScreens_.emplace_back(MakeScreen(SCX /2, SCY, true));

	//�v���C���[�P�ƂQ�̗����̉�ʂ�`�悷��X�N���[�������
	integrationScreen_= MakeScreen(SCX, SCY, true);

	//�v���C���[�P�ƂQ�̉�ʂ̍��W
	screenPos_.emplace(PLAYER_NUM::P_1, VECTOR(0, 0));
	screenPos_.emplace(PLAYER_NUM::P_2, VECTOR(SCREEN_SIZE.x / 2, 0));

	//�v���C���[�ƃ{�X���Ǘ�����}�l�[�W���\�̐���
	actorManager_ = std::make_unique<ActorManager>(playMode);

	//�X�e�[�W�̐���
	stage_ = std::make_unique<Stage>();

	//�X�e�[�W�̃R���W������ǉ�
	actorManager_->AddClliders(stage_->GetCollider());

	if (playMode == SINGLE_PLAY_MODE)
	{
		draw_ = &GameScene::DrawSIngleMode;
	}
	else
	{
		draw_ = &GameScene::DrawBattleMode;
	}

	//�摜�̓ǂݍ���
	InitImage();
	
	//���A�b�v�f�[�g�����Ă���
	actorManager_->Update();

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
	//�摜�̓ǂݍ���
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
	//����UI�`��(�Q�[���X�^�[�g�Ȃ�)
	actorManager_->DrawCommonUI(startCount_, IsGameSet(), rematchMode_);
}
void GameScene::DrawBattleMode(void)
{
	int maxIdx = static_cast<int>(PLAYER_NUM::MAX);
	for (int idx = 0; idx < maxIdx; idx++)
	{
		SetDrawScreen(cameraScreens_[idx]);
		// ��ʂ�������
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
		//���ߏ����͌�Ō�����
		PLAYER_NUM key = static_cast<PLAYER_NUM>(idx);
		DrawGraph(
			static_cast<int>(screenPos_[key].x),
			static_cast<int>(screenPos_[key].y),
			cameraScreens_[idx], false);
	}
	//�񕪊�����Ă����ʂ̐^�񒆂ɐ�������
	DrawLine(
		static_cast<int>(screenPos_[PLAYER_NUM::P_2].x), 
		0, 
		static_cast<int>(screenPos_[PLAYER_NUM::P_2].x), LINE_THICKNES, 0x000000, true);

	SetDrawScreen(DX_SCREEN_BACK);

	//�v���C���[���Ƃɕ������ꂽ�X�N���[�����܂Ƃ߂ĕ`��
	DrawGraph(0, 0, integrationScreen_, true);

	//����UI�`��(�Q�[���X�^�[�g�Ȃ�)
	actorManager_->DrawCommonUI(startCount_,IsGameSet(),rematchMode_);

}
void GameScene::ChangeGameScene(void)
{
	//�Q�[���V�[���Ɉڍs
	sceneManager_.ChangeScene(std::make_shared<GameScene>(sceneManager_, playMode_, sceneTransitor_, input_));
}

void GameScene::ChangeTitleScene(void)
{
	//�^�C�g���V�[���Ɉڍs
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




