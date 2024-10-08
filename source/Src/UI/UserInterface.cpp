#include<DxLib.h>
#include "../Manager/ResourceManager.h"
#include "UserInterface.h"
#include"../../Config.h"

#pragma region Parameter

//�V���O�����[�h���̏����܂��͔s�k�摜�̍��W
static constexpr VECTOR SINGLEMODE_IMG_POS = { 800.0f,400.0f ,0.0f };

static constexpr VECTOR MULTI_MODE_IMG_POS = { 400.0f,400.0f ,0.0f };

//�G�^�[�Q�b�g�摜�̍��W�����p
static constexpr float TARGET_OFFSET_Y = 100.0f;

//�G�^�[�Q�b�g�摜�̍��W�����p
static constexpr float ENEMY_HP_GAUGE_OFFSET_Y = 400.0f;

 //�u�[�X�g�Q�[�W�摜�̍��W
static constexpr VECTOR BOOST_GAUGE_IMG_POS_MULTI= { 400.0f,850.0f,0.0f };
static constexpr VECTOR BOOST_GAUGE_IMG_POS_SINGLE = { 1100.0f,850.0f,0.0f };
static constexpr double BOOST_GAUGE_CASE_SCALE_SINGLE= 2.5;
static constexpr double BOOST_GAUGE_CASE_SCALE_MULTI= 2.0;
static constexpr int BOOST_GAUGE_OFFSET_X_SINGLE = 5;
static constexpr int BOOST_GAUGE_OFFSET_Y_SINGLE = 40;
static constexpr int BOOST_GAUGE_OFFSET_X_MULTI = 4;
static constexpr int BOOST_GAUGE_OFFSET_Y_MULTI = 30;

 //HP�Q�[�W�\���̐��l
static constexpr VECTOR HP_GAUGE_POS_MULTI = {25.0f,800.0f,0.0f};
 static constexpr VECTOR HP_GAUGE_POS_SINGLE = {4.0f,750.0f,0.0f};


 //�c�e���\���̐��l
 static constexpr VECTOR NUMBER_OF_BULLETS_POS_SINGLE = {1300.0f,650.0f,0.0f};
 static constexpr VECTOR NUMBER_OF_BULLETS_POS_MULTI = {600.0f,700.0f,0.0f};
 static constexpr int NUMBER_OF_BULLETS_CENTER_X= 140;
 static constexpr int NUMBER_OF_BULLETS_CENTER_Y=100;
 static constexpr int NUMBER_OF_BULLETS_OFFSET = 10;
 static constexpr double NUMBER_OF_BULLETS_SCALE_MULTI = 0.4;
 static constexpr double NUMBER_OF_BULLETS_SCALE_SINGLE= 0.6;

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

 //�ΐ�J�n���uREADY�v�摜�\���b��
 static constexpr float READY_TIME_MAX = 1.5f;

 //��ʕ����p�̐��̑���
 static constexpr int  LINE_THICKNES = 1000;

 //�v���C���[�P�̃^�C�v
 static constexpr int PLAYER_TYPE = 0;

 //�v���C���[�h(�V���O���v���C)
 static constexpr int PLAY_MODE = 0;

 //�v���C���[�P��PAD�i���o�[
 static constexpr int PAD_NUM = 1;

 //�GHP
 static constexpr int ENEMY_HP = 1000;
 //�{�XHP
 static constexpr int BOSS_HP = 500;

#pragma endregion


UserInterface::UserInterface(ResourceManager& resMng, const VECTOR& enemyPos,const float& distance,
	const float& boostGauge,const float& hpGauge, const float& enemyHp,const bool& IsWin_,const int& numnberofBullets,
	int playMode, int playerType):
	resMng_(resMng), enemyPos_(enemyPos), enemyDistance_(distance), 
	boostGauge_(boostGauge), hpGauge_(hpGauge),enemyHpGauge_(enemyHp), isWin_(IsWin_), numnberofBullets_(numnberofBullets)
{
	//�v���C���[�h��ۑ�
	playMode_ = playMode;
	
	//�R���g���[���[�̃^�C�v���i�[(PS��XBOX)
	joyPadType_ = static_cast<JOYPAD_TYPE>(GetJoypadType(PAD_NUM));
	
	//�ΐ�I�����̃��[�h�I���̏�����(�����J�[�\��r�̓^�C�g���֖߂�)
	rematchMode_ = 0;
	
	//�Q�[���X�^�[�g���̃J�E���g�̏�����
	startCount_ = 0.0f;

	//�摜�̓ǂݍ���
	InitImage();

	//�v���C���[�^�C�v���v���C���[1�̎�����
	if (playerType == PLAYER_TYPE)
	{
		//����UI�̏�����
		InitCommonUi();
	}

}

UserInterface::~UserInterface()
{
}

void UserInterface::Draw(bool& gameSet)
{
	//�܂����s���t���Ă��Ȃ���Ε\��
	if (!gameSet)
	{
		//�u�[�X�g�Q�[�W�\��
		DrawBoostGauge();
		//HP�Q�[�W�\��
		DrawHPGauge();
		//�Ə��̕\��
		DrawTarget();
		//�c�i���̕\��
		DrawNumnberOfBullets();
	}
	else
	{
		//������������\��
		VictoryOrDefeat();
	}
}

void UserInterface::DrawCommonUI(const float& startCount, const bool& isGameSet, const float& rematchMode)
{
	//�Q�[���X�^�[�g��UI
	DrawGameUIStart(startCount);
	//�Q�[���I����UI
	DrawUIGameSet(isGameSet, rematchMode);
}

void UserInterface::InitImage(void)
{
	//�摜�̓ǂݍ���
	uiImgH_.emplace(IMG_TYPE::WIN, resMng_.Load(ResourceManager::SRC::WIN).handleId_);
	uiImgH_.emplace(IMG_TYPE::LOSE, resMng_.Load(ResourceManager::SRC::LOSE).handleId_);
	uiImgH_.emplace(IMG_TYPE::TARGERT, resMng_.Load(ResourceManager::SRC::TARGET).handleId_);
	uiImgH_.emplace(IMG_TYPE::TARGET_RED, resMng_.Load(ResourceManager::SRC::TARGET_RED).handleId_);
	uiImgH_.emplace(IMG_TYPE::RIFLE_IMAGE, resMng_.Load(ResourceManager::SRC::RIFLE_IMAGE).handleId_);
	uiImgH_.emplace(IMG_TYPE::BOOST_GAUGE, resMng_.Load(ResourceManager::SRC::BOOSTGAUGE_IMAGE).handleId_);
	uiImgH_.emplace(IMG_TYPE::ENEMY_HP_GAUGE, resMng_.Load(ResourceManager::SRC::ENEMY_HP_GAUGE).handleId_);
	uiImgH_.emplace(IMG_TYPE::BOOST_GAUGE_CASE, resMng_.Load(ResourceManager::SRC::BOOSTGAUGE_CASE_IMAGE).handleId_);

	//�V���O���v���C���[�h���̏�����
	if (playMode_ == PLAY_MODE)
	{
		//HP�Q�[�W�̉摜�̍��W
		hpGaugePos_ = HP_GAUGE_POS_SINGLE;

		//���s�摜�̍��W
		victoryOrDefeatPos_ = SINGLEMODE_IMG_POS;

		//�u�[�X�g�Q�[�W�摜�̍��W
		boostGaugePos_ = BOOST_GAUGE_IMG_POS_SINGLE;

		//�u�[�X�g�Q�[�W�g�摜�̑傫��
		boostGaugeCaseScale_ = BOOST_GAUGE_CASE_SCALE_SINGLE;

		//�u�[�X�g�Q�[�W�摜�ʒu�����p���l
		boostGaugeOffsetX_ = BOOST_GAUGE_OFFSET_X_SINGLE;
		boostGaugeOffsetY_ = BOOST_GAUGE_OFFSET_Y_SINGLE;

		//�c�e�\���摜���W
		numberOfBulletsPos_ = NUMBER_OF_BULLETS_POS_SINGLE;

		//�c�e�\���摜�̑傫��
		numberOfBulletsScale_ = NUMBER_OF_BULLETS_SCALE_SINGLE;

		//HP�\���̃t�H���g�ݒ�
		fontHp_ = CreateFontToHandle(NULL, 250, 3);

		//�c�e�\���̃t�H���g�ݒ�
		fontBullets_ = CreateFontToHandle(NULL, 150, 3);

	}
	else//�ΐ탂�[�h���̏�����
	{
		//HP�Q�[�W�̉摜�̍��W
		hpGaugePos_ = HP_GAUGE_POS_MULTI;

		//���s�摜�̍��W
		victoryOrDefeatPos_ = MULTI_MODE_IMG_POS;

		//�u�[�X�g�Q�[�W�摜�̍��W
		boostGaugePos_ = BOOST_GAUGE_IMG_POS_MULTI;
		
		//�u�[�X�g�Q�[�W�g�摜�̑傫��
		boostGaugeCaseScale_ = BOOST_GAUGE_CASE_SCALE_MULTI;

		//�u�[�X�g�Q�[�W�摜�ʒu�����p���l
		boostGaugeOffsetX_ = BOOST_GAUGE_OFFSET_X_MULTI;
		boostGaugeOffsetY_ = BOOST_GAUGE_OFFSET_Y_MULTI;

		//�c�e�\���摜���W
		numberOfBulletsPos_ = NUMBER_OF_BULLETS_POS_MULTI;

		//�c�e�\���摜�̑傫��
		numberOfBulletsScale_ = NUMBER_OF_BULLETS_SCALE_MULTI;

		//HP�\���̃t�H���g�ݒ�
		fontHp_ = CreateFontToHandle(NULL, 150, 3);
		
		//�c�e�\���̃t�H���g�ݒ�
		fontBullets_ = CreateFontToHandle(NULL, 100, 3);
	}
}

void UserInterface::InitCommonUi(void)
{
	//�摜�ǂݍ���
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
	//�Q�[�W�c�ʔ����ȏ�͗ΐF
	if (x>50)
	{
		color = GetColor(0, 0, 255);
	}
	//�Q�[�W�c��25%�ȏ�50%�����Ȃ物�F
	else if (x>25)
	{
		color = GetColor(255, 255, 0);
	}
	//�Q�[�W�c��25%�ȉ��Ȃ�ԐF
	else
	{
		color = GetColor(255, 0, 0);
	}
	//�u�[�X�g�Q�[�W�̕\��
	DrawFillBox(PosX, PosY, PosX+ (x * boostGaugeOffsetX_), PosY + boostGaugeOffsetY_, color);
	//�u�[�X�g�Q�[�W�g�̕\��
	DrawRotaGraph2(PosX, PosY, 0, 0, boostGaugeCaseScale_, 0.0, uiImgH_[IMG_TYPE::BOOST_GAUGE_CASE], true, false);
}

void UserInterface::DrawNumnberOfBullets(void)
{
	int PosX = static_cast<int>(numberOfBulletsPos_.x);
	int PosY = static_cast<int>(numberOfBulletsPos_.y);
	int geuge = static_cast<int>(numnberofBullets_);

	//�c�e�\��(����)
	DrawFormatStringToHandle(PosX, PosY - NUMBER_OF_BULLETS_OFFSET, 0xffffff, fontBullets_, "%d", static_cast<int>(numnberofBullets_));
	//�c�e�\��(����)
	DrawRotaGraph2(PosX, PosY, NUMBER_OF_BULLETS_CENTER_X, NUMBER_OF_BULLETS_CENTER_Y, 
		numberOfBulletsScale_, 0.0, uiImgH_[IMG_TYPE::RIFLE_IMAGE], true, false);

}

void UserInterface::DrawHPGauge(void)
{	
	int PosX = static_cast<int>(hpGaugePos_.x);
	int PosY = static_cast<int>(hpGaugePos_.y);
	//HP�̕\��(����)
	DrawFormatStringToHandle(PosX, PosY, 0xff0000, fontHp_, "%d", static_cast<int>(hpGauge_));
}

void UserInterface::VictoryOrDefeat(void)
{
	int posX = static_cast<int>(victoryOrDefeatPos_.x);
	int posY = static_cast<int>(victoryOrDefeatPos_.y);
	if (isWin_)
	{	//�����摜�\��
		DrawRotaGraph2(posX, posY, 140, 100, 2.0, 0.0, uiImgH_[IMG_TYPE::WIN], true, false);
	}
	else
	{	//�����摜�\��
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

	//�����ɂ���ĐF��ς���
	if (WITHIN_RANGE > (enemyDistance_))
	{	//�Ə��\��(���F)
		DrawRotaGraph(X, Y,  0.2f, 0.0f, uiImgH_[IMG_TYPE::TARGERT], true);
	}
	else
	{	//�Ə��\��(�ԐF)
		DrawRotaGraph(X, Y, 0.2f, 0.0f, uiImgH_[IMG_TYPE::TARGET_RED], true);
	}
	//�G��HP�Q�[�W�\��
	DrawEnemyHp();
}

void UserInterface::DrawEnemyHp(void)
{
	int scale = 0;
	//�v���C���[�h�ɂ���đ傫����ς���
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

	//�GHP�Q�[�W�g�̕\��
	DrawRectGraph(X, Y, 0, 0, gaugeCase, 15, uiImgH_[IMG_TYPE::BOOST_GAUGE_CASE], true);
	//�GHP�Q�[�W�̕\��
	DrawRectGraph(X, Y, 0, 0, gauge, 15, uiImgH_[IMG_TYPE::ENEMY_HP_GAUGE], true);
}

void UserInterface::DrawGameUIStart(const float& startCount)
{
	//�Q�[���X�^�[�g��͕`������Ȃ�
	if (startCount >= START_TIME_MAX)
	{
		return;
	}
	if (startCount < READY_TIME_MAX)
	{	//���f�B�摜�\��		
		DrawRotaGraph2(READY_POS_X, READY_POS_Y,
			READY_CENTER_X, READY_CENTER_Y, READY_SCALE, 0.0, uiImgH_[IMG_TYPE::READY], true, false);
	}
	else
	{	//�t�@�C�g�摜�\��
		DrawRotaGraph2(FIGHT_POS_X, FIGHT_POS_Y,
			FIGHT_CENTER_X, FIGHT_CENTER_Y, FIGHT_SCALE, 0.0, uiImgH_[IMG_TYPE::FIGHT], true, false);
	}
}

void UserInterface::DrawUIGameSet(const bool& isGameSet, const int& rematchMode)
{
	//���s�����܂��Ă��Ȃ���Ζ߂�
	if (!isGameSet)
	{
		return;
	}

	//�^�C�g���֖߂�摜�\��
	DrawRotaGraph2(BACK_TO_TITLE_POS_X, BACK_TO_TITLE_POS_Y,
		BACK_TO_TITLE_CENTER_X, BACK_TO_TITLE_CENTER_Y, BACK_TO_TITLE_SCALE, 0.0, uiImgH_[IMG_TYPE::BACK_TO_TITLE], true, false);
	//�������摜�\��
	DrawRotaGraph2(ONE_MOR_FIGHT_POS_X, ONE_MOR_FIGHT_POS_Y,
		ONE_MOR_FIGHT_CENTER_X, ONE_MOR_FIGHT_CENTER_Y, ONE_MOR_FIGHT_SCALE, 0.0, uiImgH_[IMG_TYPE::ONE_MOR_FIGHT], true, false);

	//�ڑ����ꂽ�R���g���[����Ps�Ȃ�~�{�^���\�L
	if (JOYPAD_TYPE:: DUAL_SHOCK_4 == joyPadType_ || JOYPAD_TYPE::DUAL_SENSE == joyPadType_)
	{

		DrawRotaGraph2(PLEASE_CROSS_BUTTON_POS_X, PLEASE_CROSS_BUTTON_POS_Y,
			PLEASE_CROSS_BUTTON_CENTER_X, PLEASE_CROSS_BUTTON_CENTER_Y, PLEASE_CROSS_BUTTON_SCALE, 0.0, uiImgH_[IMG_TYPE::PLEASE_CROSS], true, false);
	}//����ȊO�Ȃ�A�{�^���\�L
	else
	{
		DrawRotaGraph2(PLEASE_A_BUTTON_POS_X, PLEASE_A_BUTTON_POS_Y,
			PLEASE_A_BUTTON_CENTER_X, PLEASE_A_BUTTON_CENTER_Y, PLEASE_A_BUTTON_SCALE, 0.0, uiImgH_[IMG_TYPE::PLEASE_A], true, false);
	}
	//�J�[�\���̕\��
	DrawRotaGraph2(TRIANGLE_POS_X, TRIANGLE_POS_Y + (TRIANGLE_OFFSET * rematchMode),
		TRIANGLE_CENTER_X, TRIANGLE_CENTER_Y, TRIANGLE_SCALE, 0.0, uiImgH_[IMG_TYPE::TRIANGLE], true, false);
}
