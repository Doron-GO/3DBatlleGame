#include<EffekseerForDXLib.h>
#include"../Manager/ResourceManager.h"
#include"../Object/Time/DeltaTime.h"
#include"../Manager/ActorManager.h"
#include"../Object/Camera/Camera.h"
#include "../Scene/SceneManager.h"
#include"../Object/Actor/Enemy/BossEnemy.h"
#include"../Object/Actor/Player/Player.h"
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

//�V���O���v���C���[�h
constexpr int SINGLE_PLAY_MODE = 0;

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
constexpr int PLEASE_CROSS_BUTTON_POS_X = 800;
constexpr int PLEASE_CROSS_BUTTON_POS_Y = 900;
constexpr int PLEASE_CROSS_BUTTON_CENTER_X = 480;
constexpr int PLEASE_CROSS_BUTTON_CENTER_Y = 52;
constexpr double PLEASE_CROSS_BUTTON_SCALE = 1.2;

//A�{�^���������Ă���摜
constexpr int PLEASE_A_BUTTON_POS_X = 800;
constexpr int PLEASE_A_BUTTON_POS_Y = 800;
constexpr int PLEASE_A_BUTTON_CENTER_X = 395;
constexpr int PLEASE_A_BUTTON_CENTER_Y = 34;
constexpr double PLEASE_A_BUTTON_SCALE = 1.2;

//�Z���N�g�摜�̐��l
constexpr int TRIANGLE_POS_X = 1200;
constexpr int TRIANGLE_POS_Y = 500;
constexpr int TRIANGLE_CENTER_X = 280;
constexpr int TRIANGLE_CENTER_Y = 63;
constexpr int TRIANGLE_OFFSET = 200;
constexpr double TRIANGLE_SCALE = 1.2;

//�Q�[���J�n���ɏo��READY�̉摜�̐��l
constexpr int READY_POS_X = 800;
constexpr int READY_POS_Y = 400;
constexpr int READY_CENTER_X = 117;
constexpr int READY_CENTER_Y = 44;
constexpr int READY_OFFSET = 200;
constexpr double READY_SCALE = 3.0;

//�Q�[���J�n���ɏo��FIGHT�̉摜�̐��l
constexpr int FIGHT_POS_X = 800;
constexpr int FIGHT_POS_Y = 400;
constexpr int FIGHT_CENTER_X = 120;
constexpr int FIGHT_CENTER_Y = 49;
constexpr int FIGHT_OFFSET = 200;
constexpr double FIGHT_SCALE = 4.5;

//�Q�[���X�^�[�g�J�n�b��
constexpr float START_TIME_MAX = 3.0f;

//��ʕ����p�̐��̑���
constexpr int LINE_THICKNES = 1000;

constexpr int SINGLE_PLAY = 0;


#pragma endregion


GameScene::GameScene(SceneManager& manager, int playMode, Transitor& transit, Input& input):Scene(manager, transit,input), 
resMng_(ResourceManager::GetInstance())
{

	// �g�����W�b�V�����̊J�n
	sceneTransitor_.Start();
	
	//�^�C�g���V�[���Ō��܂����Q�[�����[�h���i�[
	playMode_ = playMode;

	//�Đ탂�[�h�I���̏����J�[�\���́u�^�C�g���ɖ߂�v�ɐݒ�
	rematchMode_ = 0;

	//�Q�[���X�^�[�g�v���ϐ���������
	startCount_ = 0.0f;

	//�v���C���[�P�̃p�b�h�̎�ނ��擾
	joyPadType_ = input_.GetJPadType();

	//�v���C���[�P�ƂQ�̉�ʂ̍��W
	screenPos_.emplace(PLAYER_NUM::P_1, VECTOR(0, 0));
	screenPos_.emplace(PLAYER_NUM::P_2, VECTOR(SCREEN_SIZE.x / 2, 0));

	//�X�e�[�W�̐���
	stage_ = std::make_unique<Stage>();

	//�v���C���[�ƃ{�X���Ǘ�����}�l�[�W���\�̐���
	actorManager_ = std::make_unique<ActorManager>(playMode);

	//�X�e�[�W�̃R���W������ǉ�
	actorManager_->AddColliders(stage_->GetCollider());

	//�J�����X�V���K�v�Ȃ��߁A���A�b�v�f�[�g�����Ă���
	actorManager_->Update();

	//UI�̏�����
	InitUI2();

	// 3D�ݒ�
	Init3DSetting();

	//�摜�̓ǂݍ���
	InitImage();

	//�`��X�N���[���ƕ`��X�e�b�v
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

void GameScene::InitRender(void)
{
	//�v���C���[�P�ƂQ�̗����̉�ʂ�`�悷��X�N���[�������
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
		//�v���C���[�P�ƂQ�̉�ʂ����
		cameraScreens_.emplace_back(MakeScreen(SCX / 2, SCY, true));
		cameraScreens_.emplace_back(MakeScreen(SCX / 2, SCY, true));
		//draw_ = &GameScene::DrawBattleMode;
		funcDraw_ = std::bind(&GameScene::DrawBattleMode, this);
	}
}

void GameScene::InitUI(void)
{
	//actorManager���������Ă����̂��āA�ǂ��̂��킩��Ȃ�

	//�v���C���[1
	int player1 = static_cast<int>(ActorBase::ACTOR_TYPE::PLAYER_1);

	 auto& players=  actorManager_->GetPlayers();
	 
	if (playMode_ == SINGLE_PLAY)
	{
		auto& bossEnemy = actorManager_->GetBossEnemy();
		//�v���C���[1��UI�𐶐�
		CreateUserInterface(*players[player1], *bossEnemy);
	}
	else
	{
		//�v���C���[2
		int player2 = static_cast<int>(ActorBase::ACTOR_TYPE::PLAYER_2);

		//�v���C���[�P�ƃv���C���[�Q��UI�𐶐�
		CreateUserInterface(*players[player1], *players[player2]);
		CreateUserInterface(*players[player2], *players[player1]);
	}
}

void GameScene::InitUI2(void)
{
	userInterfaces_ = actorManager_->MoveUI();
}

void GameScene::CreateUserInterface( ActorBase& player, ActorBase& target)
{
	//�v���C���[�^�C�v
	 int playerType = static_cast<int>(player.GetActorType());

	//UI�𐶐�
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
	bool& isDeadAnyPlayer = actorManager_->IsDeadAnyPlayer();

	ClearDrawScreen();
	actorManager_->DrawCamera(SINGLE_PLAY_MODE);
	actorManager_->Draw();
	stage_->Draw();
	//actorManager_->DrawUI(SINGLE_PLAY_MODE);
	userInterfaces_[SINGLE_PLAY]->Draw(isDeadAnyPlayer);
	UpdateEffekseer3D();
	DrawEffekseer3D();
	//����UI�`��(�Q�[���X�^�[�g�Ȃ�)
	//actorManager_->DrawCommonUI(startCount_, IsGameSet(), rematchMode_);
	userInterfaces_[SINGLE_PLAY]->DrawCommonUI(startCount_, isDeadAnyPlayer, rematchMode_);

}
void GameScene::DrawBattleMode(void)
{
	int maxIdx = static_cast<int>(PLAYER_NUM::MAX);
	bool& isDeadAnyPlayer = actorManager_->IsDeadAnyPlayer();
	for (int idx = 0; idx < maxIdx; idx++)
	{
		SetDrawScreen(cameraScreens_[idx]);
		// ��ʂ�������
		ClearDrawScreen();
		actorManager_->DrawCamera(idx);
		actorManager_->Draw();
		stage_->Draw();
		//actorManager_->DrawUI(idx);
		userInterfaces_[idx]->Draw(isDeadAnyPlayer);
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
//	actorManager_->DrawCommonUI(startCount_,IsGameSet(),rematchMode_);
	int player1 = static_cast<int>(ActorBase::ACTOR_TYPE::PLAYER_1);

	userInterfaces_[player1]->DrawCommonUI(startCount_, isDeadAnyPlayer, rematchMode_);


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
	return SelectDecide();
}

bool GameScene::SelectDecide(void)
{	
	//PS�R���̎g�p���Ɂ~�{�^���������ꂽ��
	if (JOYPAD_TYPE::DUAL_SHOCK_4 == joyPadType_ ||
		JOYPAD_TYPE::DUAL_SENSE == joyPadType_ &&
		input_.IsTriggerd("b"))
	{
		SelectTitleOrGame();
		return true;
	}
	//XBOX�Ȃ�A�{�^��
	else if (JOYPAD_TYPE::XBOX_360 == joyPadType_ ||
		JOYPAD_TYPE::XBOX_ONE == joyPadType_ &&
		input_.IsTriggerd("jump"))
	{
		SelectTitleOrGame();
		return true;
	}
	//���̑��Ȃ�v���X�e�R���ł����~�{�^���̏ꏊ�̃{�^��
	else
	{
	}
	return false;
}

void GameScene::SelectTitleOrGame(void)
{
	//�J�[�\�����^�C�g���Ȃ�^�C�g���V�[���Ɉڍs
	if (rematchMode_ == static_cast<int>(REMATCH_MODE::BACK_TO_TITLE))
	{
		ChangeTitleScene();
	}
	//�J�[�\�����^�C�g���Ȃ�������
	else if (rematchMode_ == static_cast<int>(REMATCH_MODE::ONE_MORE_FIGHT))
	{
		ChangeGameScene();
	}
	else
	{
		ChangeTitleScene();
	}
}
