#include<DxLib.h>
#include "../Manager/ResourceManager.h"
#include "UserInterface.h"
#include"../../Config.h"

#pragma region Parameter

//�V���O�����[�h���̏����܂��͔s�k�摜�̍��W
constexpr VECTOR SINGLEMODE_IMG_POS = { 800.0f,400.0f ,0.0f };

constexpr VECTOR MULTI_MODE_IMG_POS = { 400.0f,400.0f ,0.0f };

//�G�^�[�Q�b�g�摜�̍��W�����p
constexpr float TARGET_OFFSET_Y = 100.0f;

//�G�^�[�Q�b�g�摜�̍��W�����p
constexpr float ENEMY_HP_GAUGE_OFFSET_Y = 400.0f;

 //�u�[�X�g�Q�[�W�摜�̍��W
constexpr VECTOR BOOST_GAUGE_IMG_POS_MULTI= { 400.0f,850.0f,0.0f };
constexpr VECTOR BOOST_GAUGE_IMG_POS_SINGLE = { 1100.0f,850.0f,0.0f };
//�傫��
constexpr double BOOST_GAUGE_CASE_SCALE_SINGLE= 2.5;
constexpr double BOOST_GAUGE_CASE_SCALE_MULTI= 2.0;
//�I�t�Z�b�g
constexpr int BOOST_GAUGE_OFFSET_X_SINGLE = 5;
constexpr int BOOST_GAUGE_OFFSET_Y_SINGLE = 40;
constexpr int BOOST_GAUGE_OFFSET_X_MULTI = 4;
constexpr int BOOST_GAUGE_OFFSET_Y_MULTI = 30;

//HP�\���̐��l
constexpr VECTOR HP_VALUE_POS_MULTI = {170.0f,850.0f,0.0f};
constexpr VECTOR HP_VALUE_POS_SINGLE = {250.0f,850.0f,0.0f};
//HP�\���̑傫��
constexpr float HP_VALUE_SCALE_SINGLE = 2.0f;
constexpr float HP_VALUE_SCALE_MULTI = 1.7f;
//HP�\���̑傫��
constexpr int HP_VALUE_OFFSET_SINGLE = 120;
constexpr int HP_VALUE_OFFSET_MULTI = 100;

//HP�w�ʉ摜�\���̐��l
constexpr VECTOR HP_BACK_POS_MULTI = { 170.0f,850.0f,0.0f };
constexpr VECTOR HP_BACK_POS_SINGLE = { 250.0f,850.0f,0.0f };
//HP�w�ʉ摜�̑傫��
constexpr float HP_BACK_SCALE_SINGLE = 1.2f;
constexpr float HP_BACK_SCALE_MULTI = 1.0f;
//HP�w�ʉ摜���S�̐��l
constexpr int HP_BACK_CENTER_X = 160;
constexpr int HP_BACK_CENTER_Y = 80;

//�c�e�\���̐��l
constexpr VECTOR BULLET_POS_MULTI = { 600.0f,690.0f,0.0f };
constexpr VECTOR  BULLET_POS_SINGLE = { 1300.0f,690.0f,0.0f };
//�c�e�\���̑傫��
constexpr float BULLET_SCALE_SINGLE = 1.0f;
constexpr float BULLET_SCALE_MULTI = 0.7f;
//�c�e�\���̑傫��
constexpr float  BULLET_OFFSET_SINGLE = 120;
constexpr float  BULLET_OFFSET_MULTI = 100;
//�c�e�\���̐��l
constexpr int BULLET_CENTER_X = 40;
constexpr int BULLET_CENTER_Y = 40;
constexpr int BULLET_OFFSET = 10;

//���C�t���\���̐��l
constexpr VECTOR RIFLE_POS_MULTI = { 670.0f,670.0f,0.0f };
constexpr VECTOR  RIFLE_POS_SINGLE = { 1420.0f,660.0f,0.0f };
//���C�t���\���̑傫��
constexpr float RIFLE_SCALE_SINGLE = 0.6f;
constexpr float RIFLE_SCALE_MULTI = 0.4f;
//���C�t���\���̑傫��
constexpr float  RIFLE_OFFSET = 10;
//���C�t���\���̐��l
constexpr int RIFLE_CENTER_X = 320;
constexpr int RIFLE_CENTER_Y =160;

 //�^�C�g���ɖ߂�摜
constexpr VECTOR BACK_TO_TITLE_POS = { 800.0f,500.0f,0.0f };
constexpr int BACK_TO_TITLE_CENTER_X = 250;
constexpr int BACK_TO_TITLE_CENTER_Y = 50;
constexpr double BACK_TO_TITLE_SCALE = 1.5;

 //�������摜
constexpr VECTOR ONE_MOR_FIGHT_POS = { 800.0f,700.0f };
constexpr int ONE_MOR_FIGHT_CENTER_X = 273;
constexpr int ONE_MOR_FIGHT_CENTER_Y = 54;
constexpr double ONE_MOR_FIGHT_SCALE = 1.5;

 //�~�{�^���������Ă���摜
constexpr VECTOR PLEASE_CROSS_BUTTON_POS = { 800.0f,900.0f };
constexpr int PLEASE_CROSS_BUTTON_CENTER_X = 480;
constexpr int PLEASE_CROSS_BUTTON_CENTER_Y = 52;
constexpr double PLEASE_CROSS_BUTTON_SCALE = 1.2;

 //A�{�^���������Ă���摜
constexpr VECTOR PLEASE_A_BUTTON_POS = { 800.0f,800.0f };
constexpr int PLEASE_A_BUTTON_CENTER_X = 395;
constexpr int PLEASE_A_BUTTON_CENTER_Y = 34;
constexpr double  PLEASE_A_BUTTON_SCALE = 1.2;

 //�Z���N�g�摜�̐��l
constexpr VECTOR TRIANGLE_POS = { 1200.0f,500.0f };
constexpr int  TRIANGLE_POS_X = 1200;
constexpr int  TRIANGLE_POS_Y = 500;
constexpr int  TRIANGLE_CENTER_X = 280;
constexpr int  TRIANGLE_CENTER_Y = 63;
constexpr int  TRIANGLE_OFFSET = 200;
constexpr double TRIANGLE_SCALE = 1.2;

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

 //�Q�[���X�^�[�g�J�n�b��
constexpr float START_TIME_MAX = 3.0f;

 //�ΐ�J�n���uREADY�v�摜�\���b��
constexpr float READY_TIME_MAX = 1.5f;

 //��ʕ����p�̐��̑���
constexpr int  LINE_THICKNES = 1000;

 //�v���C���[�P�̃^�C�v
constexpr int PLAYER_TYPE = 0;

 //�v���C���[�h(�V���O���v���C)
constexpr int PLAY_MODE = 0;

 //�v���C���[�P��PAD�i���o�[
constexpr int PAD_NUM = 1;

 //�GHP
constexpr int ENEMY_HP = 1000;
 //�{�XHP
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
		InitCommonUI();
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

void UserInterface::DrawCommonUI(const float& startCount, const bool& isGameSet, const int& rematchMode)
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
	uiImgH_.emplace(IMG_TYPE::HP_BACK, resMng_.Load(ResourceManager::SRC::HP_BACK).handleId_);
	numberImg_ = resMng_.Load(ResourceManager::SRC::NUMBER).handleIds_;

	//�^�C�g���֖߂�摜�̏��
	imgInfo_.emplace(IMG_TYPE::BACK_TO_TITLE,
		Info(BACK_TO_TITLE_POS,
			BACK_TO_TITLE_SCALE,
			BACK_TO_TITLE_CENTER_X,
			BACK_TO_TITLE_CENTER_Y));
	//�������摜�̏��
	imgInfo_.emplace(IMG_TYPE::ONE_MOR_FIGHT,
		Info(ONE_MOR_FIGHT_POS,
			ONE_MOR_FIGHT_SCALE,
			ONE_MOR_FIGHT_CENTER_X,
			ONE_MOR_FIGHT_CENTER_Y));
	//�I���摜�̏��
	imgInfo_.emplace(IMG_TYPE::TRIANGLE,
		Info(TRIANGLE_POS,
			TRIANGLE_SCALE,
			TRIANGLE_CENTER_X,
			TRIANGLE_CENTER_Y,
			TRIANGLE_OFFSET));

	//HP�Q�[�W�摜�̏��
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

	//�V���O���v���C���[�h���̏�����
	if (playMode_ == PLAY_MODE)
	{
		//HP�Q�[�W�摜�̏��
		imgInfo_.emplace(IMG_TYPE::HP_BACK,
			Info(HP_BACK_POS_SINGLE,
				HP_BACK_SCALE_SINGLE,
				HP_BACK_CENTER_X,
				HP_BACK_CENTER_Y));
		//�c�e���\���̏��
		imgInfo_.emplace(IMG_TYPE::BULLET_NUM,
			Info(BULLET_POS_SINGLE,
				BULLET_SCALE_SINGLE,
				BULLET_CENTER_X,
				BULLET_CENTER_Y, 
				BULLET_OFFSET_SINGLE));
		//���C�t���摜���
		imgInfo_.emplace(IMG_TYPE::RIFLE_IMAGE,
			Info(RIFLE_POS_SINGLE,
				RIFLE_SCALE_SINGLE,
				RIFLE_CENTER_X,
				RIFLE_CENTER_Y));
		//�u�[�g�Q�[�W�摜���
		imgInfo_.emplace(IMG_TYPE::BOOST_GAUGE,
			Info(BOOST_GAUGE_IMG_POS_SINGLE,
				BOOST_GAUGE_CASE_SCALE_SINGLE,
				BOOST_GAUGE_OFFSET_X_SINGLE,
				BOOST_GAUGE_OFFSET_Y_SINGLE));

		//�u�[�g�Q�[�W�摜���
		imgInfo_.emplace(IMG_TYPE::WIN,
			Info(SINGLEMODE_IMG_POS));

		//HP�\���̃I�t�Z�b�g�l
		hpValueOffset_ = HP_VALUE_OFFSET_SINGLE;
		//HP�\���̑傫��
		hpValueScale_ = HP_VALUE_SCALE_SINGLE;

	}
	else//�ΐ탂�[�h���̏�����
	{

		//HP�Q�[�W�摜�̏��
		imgInfo_.emplace(IMG_TYPE::HP_BACK,
			Info(HP_VALUE_POS_MULTI,
				HP_BACK_SCALE_MULTI,
				HP_BACK_CENTER_X,
				HP_BACK_CENTER_Y));
		//�c�e���\�����
		imgInfo_.emplace(IMG_TYPE::BULLET_NUM,
			Info(BULLET_POS_MULTI,
				BULLET_SCALE_MULTI,
				BULLET_CENTER_X,
				BULLET_CENTER_Y,
				BULLET_OFFSET_MULTI));
		//���C�t���摜
		imgInfo_.emplace(IMG_TYPE::RIFLE_IMAGE,
			Info(RIFLE_POS_MULTI,
				RIFLE_SCALE_MULTI,
				RIFLE_CENTER_X,
				RIFLE_CENTER_Y));
		//�u�[�g�Q�[�W�摜���
		imgInfo_.emplace(IMG_TYPE::BOOST_GAUGE, Info(BOOST_GAUGE_IMG_POS_MULTI,
			BOOST_GAUGE_CASE_SCALE_MULTI,
			BOOST_GAUGE_OFFSET_X_MULTI,
			BOOST_GAUGE_OFFSET_Y_MULTI));

		//�u�[�g�Q�[�W�摜���
		imgInfo_.emplace(IMG_TYPE::WIN, Info(MULTI_MODE_IMG_POS));

		//HP�\���̃I�t�Z�b�g
		hpValueOffset_ = HP_VALUE_OFFSET_MULTI;
		//HP�\���̑傫��
		hpValueScale_ = HP_VALUE_SCALE_MULTI;

	}
}

void UserInterface::InitCommonUI(void)
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
	int PosX = static_cast<int>(imgInfo_[IMG_TYPE::BOOST_GAUGE].pos_.x);
	int PosY = static_cast<int>(imgInfo_[IMG_TYPE::BOOST_GAUGE].pos_.y);
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
	DrawFillBox(PosX, PosY, PosX+ (x * imgInfo_[IMG_TYPE::BOOST_GAUGE].centerX_), PosY + imgInfo_[IMG_TYPE::BOOST_GAUGE].centerY_, color);
	//�u�[�X�g�Q�[�W�g�̕\��
	DrawRotaGraph2(PosX, PosY, 0, 0, imgInfo_[IMG_TYPE::BOOST_GAUGE].scale_, 0.0, uiImgH_[IMG_TYPE::BOOST_GAUGE_CASE], true, false);
}

void UserInterface::DrawNumnberOfBullets(void)
{
	int PosX = static_cast<int>(imgInfo_[IMG_TYPE::BULLET_NUM].pos_.x);
	int PosY = static_cast<int>(imgInfo_[IMG_TYPE::BULLET_NUM].pos_.y);
	int riflePosX = static_cast<int>(imgInfo_[IMG_TYPE::RIFLE_IMAGE].pos_.x);
	int riflePosY = static_cast<int>(imgInfo_[IMG_TYPE::RIFLE_IMAGE].pos_.y);

	//�c�e�\��(����)
	DrawRotaGraph2(riflePosX, riflePosY, imgInfo_[IMG_TYPE::RIFLE_IMAGE].centerX_, imgInfo_[IMG_TYPE::RIFLE_IMAGE].centerY_,
		imgInfo_[IMG_TYPE::RIFLE_IMAGE].scale_, 0.0, uiImgH_[IMG_TYPE::RIFLE_IMAGE], true, false);

	//�c�e�\��(����)
	DrawRotaGraph2(PosX ,PosY,
					imgInfo_[IMG_TYPE::BULLET_NUM].centerX_,imgInfo_[IMG_TYPE::BULLET_NUM].centerX_,
		imgInfo_[IMG_TYPE::BULLET_NUM].scale_, 0.0, numberImg_[*numnberofBullets_], true, false);

}

void UserInterface::DrawHPGauge(void)
{
	int PosX = static_cast<int>(imgInfo_[IMG_TYPE::HP_BACK].pos_.x);
	int PosY = static_cast<int>(imgInfo_[IMG_TYPE::HP_BACK].pos_.y );
	int hp = static_cast<int>(hpGauge_);

	//�S�̌��̐���
	int singleDigit = hp/ 100;
	//�\�̌��̐���
	int twoDigit = hp;
	twoDigit = (twoDigit % 100 ) / 10;
	//�\�̌��̐���
	int threeDigit = hp;
	threeDigit = (threeDigit % 100)%10;

	//HP�w�ʉ摜
	DrawRotaGraph2(PosX, PosY, imgInfo_[IMG_TYPE::HP_BACK].centerX_, imgInfo_[IMG_TYPE::HP_BACK].centerY_,
		imgInfo_[IMG_TYPE::HP_BACK].scale_, 0.0, uiImgH_[IMG_TYPE::HP_BACK], true, false);


	//HP�\��(�S�̌�)
	DrawRotaGraph2(PosX - hpValueOffset_, PosY, 40, 40,
		hpValueScale_, 0.0, numberImg_[singleDigit], true, false);

	//HP�\��(�\�̌�)
	DrawRotaGraph2(PosX, PosY, 40, 40,
		hpValueScale_, 0.0, numberImg_[twoDigit], true, false);

	//HP�\��(��̌�)
	DrawRotaGraph2(PosX + hpValueOffset_, PosY, 40, 40,
		hpValueScale_, 0.0, numberImg_[threeDigit], true, false);

}

void UserInterface::VictoryOrDefeat(void)
{
	int posX = static_cast<int>(imgInfo_[IMG_TYPE::WIN].pos_.x);
	int posY = static_cast<int>(imgInfo_[IMG_TYPE::WIN].pos_.y);
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

	int titleX = static_cast<int>(imgInfo_[IMG_TYPE::BACK_TO_TITLE].pos_.x);
	int titleY = static_cast<int>(imgInfo_[IMG_TYPE::BACK_TO_TITLE].pos_.y);
	int fightX = static_cast<int>(imgInfo_[IMG_TYPE::ONE_MOR_FIGHT].pos_.x);
	int fightY = static_cast<int>(imgInfo_[IMG_TYPE::ONE_MOR_FIGHT].pos_.y);


	//�^�C�g���֖߂�摜�\��
	DrawRotaGraph2(titleX, titleY,
		imgInfo_[IMG_TYPE::BACK_TO_TITLE].centerX_,
		imgInfo_[IMG_TYPE::BACK_TO_TITLE].centerY_,
		imgInfo_[IMG_TYPE::BACK_TO_TITLE].scale_, 0.0, uiImgH_[IMG_TYPE::BACK_TO_TITLE], true, false);
	//�������摜�\��
	DrawRotaGraph2(fightX, fightY,
		imgInfo_[IMG_TYPE::ONE_MOR_FIGHT].centerX_,
		imgInfo_[IMG_TYPE::ONE_MOR_FIGHT].centerY_,
		imgInfo_[IMG_TYPE::ONE_MOR_FIGHT].scale_, 0.0, uiImgH_[IMG_TYPE::ONE_MOR_FIGHT], true, false);

	//�ڑ����ꂽ�R���g���[����Ps�Ȃ�~�{�^���\�L
	if (JOYPAD_TYPE:: DUAL_SHOCK_4 == joyPadType_ || JOYPAD_TYPE::DUAL_SENSE == joyPadType_)
	{

		DrawRotaGraph2(imgInfo_[IMG_TYPE::PLEASE_CROSS].pos_.x, imgInfo_[IMG_TYPE::PLEASE_CROSS].pos_.y,
			imgInfo_[IMG_TYPE::PLEASE_CROSS].centerX_, imgInfo_[IMG_TYPE::PLEASE_CROSS].centerY_,
			imgInfo_[IMG_TYPE::PLEASE_CROSS].scale_, 0.0, uiImgH_[IMG_TYPE::PLEASE_CROSS], true, false);
	}//����ȊO�Ȃ�A�{�^���\�L
	else
	{
		DrawRotaGraph2(imgInfo_[IMG_TYPE::PLEASE_A].pos_.x, imgInfo_[IMG_TYPE::PLEASE_A].pos_.y,
			imgInfo_[IMG_TYPE::PLEASE_A].centerX_, imgInfo_[IMG_TYPE::PLEASE_A].centerY_,
			imgInfo_[IMG_TYPE::PLEASE_A].scale_, 0.0, uiImgH_[IMG_TYPE::PLEASE_A], true, false);
	}
	//�J�[�\���̕\��
	DrawRotaGraph2(imgInfo_[IMG_TYPE::TRIANGLE].pos_.x, imgInfo_[IMG_TYPE::TRIANGLE].pos_.y + (imgInfo_[IMG_TYPE::TRIANGLE].offset_ * rematchMode),
		imgInfo_[IMG_TYPE::TRIANGLE].centerX_, imgInfo_[IMG_TYPE::TRIANGLE].centerY_, imgInfo_[IMG_TYPE::TRIANGLE].scale_, 0.0, uiImgH_[IMG_TYPE::TRIANGLE], true, false);
}
