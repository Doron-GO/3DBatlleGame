#include "TutorialScene.h"
#include"../Manager/ResourceManager.h"
#include "../Scene/SceneManager.h"
#include"Transition/Transitor.h"
#include"../Input/Input.h"
#include"../../Config.h"
#include"GameScene.h"
#include<DxLib.h>

#pragma region �p�X

//UI�����摜
std::string IMAGE_UI = PATH_IMAGE_TUTORIAL + "UI.png";
//�W�����v�����摜
std::string IMAGE_JUMP = PATH_IMAGE_TUTORIAL + "Tutorial_Jump.png";
//�ˌ������摜
std::string IMAGE_SHOT = PATH_IMAGE_TUTORIAL + "Tutorial_Shot.png";
//�u�[�X�g�����摜
std::string IMAGE_BOOST = PATH_IMAGE_TUTORIAL + "Tutorial_Boost.png";
//�u�[�X�g�_�b�V�������摜
std::string IMAGE_BOOSTDASH = PATH_IMAGE_TUTORIAL + "Tutorial_BoostDash.png";
//�z�[�~���O�����摜
std::string IMAGE_HORMING = PATH_IMAGE_TUTORIAL + "Tutorial_Horming.png";
//�˒������������摜
std::string IMAGE_RANGE_IN = PATH_IMAGE_TUTORIAL + "Tutorial_InRange.png";
//�˒��O�����摜
std::string IMAGE_RANGE_OUT = PATH_IMAGE_TUTORIAL + "Tutorial_OutRange.png";
//�u�[�X�g�z�[�~���O�����摜
std::string IMAGE_HORMING_BOOST = PATH_IMAGE_TUTORIAL + "Tutorial_Boost_Horming.png";
//�u�[�X�g�_�b�V���z�[�~���O�����摜
std::string IMAGE_HORMING_BOOSTDASH = PATH_IMAGE_TUTORIAL + "Tutorial_BoostDash_Horming.png";
//�i�������摜
std::string IMAGE_COMBAT = PATH_IMAGE_TUTORIAL + "Tutorial_Combat .png";
//�i���L�����Z���摜
std::string IMAGE_COMBAT_CANCEL= PATH_IMAGE_TUTORIAL + "Tutorial_Combat_Cancel.png";
//���̏�i�������摜
std::string IMAGE_COMBAT_QUICK = PATH_IMAGE_TUTORIAL + "Tutorial_Combat_Quick.png";
//�i���ڍs�����摜
std::string IMAGE_COMBAT_TRANSITION = PATH_IMAGE_TUTORIAL + "Tutorial_CombatTransition.png";

#pragma endregion

#pragma region �p�����[�^

//�ő�̃y�[�W��
constexpr int PAGE_MAX = 14;
//�ŏ��̃y�[�W��
constexpr int PAGE_MIN = 0;

//�ő�̃J�[�\���i���o�[
constexpr int CURSOR_MAX = 1;

//�I���J�[�\���摜�̐��l(���W�A���S���W�A�傫��)
constexpr int TRIANGLE_POS_X = 1200;
constexpr int TRIANGLE_POS_Y = 300;
constexpr int TRIANGLE_CENTER_X = 280;
constexpr int TRIANGLE_CENTER_Y = 63;
constexpr int TRIANGLE_OFFSET_MODE_SELECT = 160;
constexpr double TRIANGLE_SCALE = 1.2;


#pragma endregion



TutorialScene::TutorialScene(SceneManager& manager, int playMode, Transitor& transit, Input& input) :Scene(manager, transit, input),
resMng_(ResourceManager::GetInstance())
{
	playMode_ = playMode;
	//�`���[�g���A���̏����y�[�W
	pageIdx_ = 0;
	//�摜�̏�����
	InitImage();
	//�������̏�����
	InitExplanation();
	//�g�p����t�H���g��ǂݍ���
	explanationHandle_ = CreateFontToHandle(FONT_NAME.c_str(), 70, 3);
	//�g�p����t�H���g��ǂݍ���
	nextHandle_ = CreateFontToHandle(FONT_NAME.c_str(), 130, 3);

	//�J�[�\���̏�����
	cursorIdx_ = 1;

	sceneTransitor_.Start();

}

TutorialScene::~TutorialScene()
{
}

void TutorialScene::Update(void)
{
	input_.Update();


	if (pageIdx_ < PAGE_MAX)
	{
		if (input_.IsTriggerd("right") && pageIdx_ < PAGE_MAX)
		{
			pageIdx_++;
		}
		else if (input_.IsTriggerd("left") && pageIdx_ > PAGE_MIN)
		{
			pageIdx_--;
		}
	}
	//�J�[�\�����ړ�����
	SelectCursor();
	//���̃V�[���ɍs�����𔻒f����
	if (SelectTutorialOrGame())
	{
		return;
	}

	sceneTransitor_.Update();
}

void TutorialScene::Draw(void)
{
	int SCX = static_cast<int>(SCREEN_SIZE.x);
	int SCY = static_cast<int>(SCREEN_SIZE.y);
	int FONT_Y = 610;
	
	//�w�i�`��
	DrawBox(0, 0,SCX, SCY , 0x7d7d7d, true);

	if (pageIdx_ < PAGE_MAX)
	{
		//�����摜�`��
		DrawRotaGraph2(SCX / 2, SCY / 3,
			863, 540,0.5,0.0,imgType_[static_cast<PAGE_TYPE>(pageIdx_)], true);
		//������
		DrawStringToHandle(SCY - (SCX / 2), FONT_Y,
			explanationType_[static_cast<PAGE_TYPE>(pageIdx_)].c_str(),
			GetColor(255, 255, 255), explanationHandle_);
	}
	else
	{
		DrawStringToHandle(460, 400,
			explanationType_[static_cast<PAGE_TYPE>(pageIdx_)].c_str(),
			GetColor(255, 255, 255), nextHandle_);

		//�J�[�\���I���摜
		DrawRotaGraph2(TRIANGLE_POS_X, TRIANGLE_POS_Y + (TRIANGLE_OFFSET_MODE_SELECT *cursorIdx_),
			TRIANGLE_CENTER_X, TRIANGLE_CENTER_Y, TRIANGLE_SCALE, 0.0, cursorImg_, true, false);

	}

	sceneTransitor_.Draw();
}

void TutorialScene::InitImage(void)
{

	imgType_.emplace(PAGE_TYPE::UI, LoadGraph(IMAGE_UI.c_str()));
	imgType_.emplace(PAGE_TYPE::JUMP, LoadGraph(IMAGE_JUMP.c_str()));
	imgType_.emplace(PAGE_TYPE::SHOT, LoadGraph(IMAGE_SHOT.c_str()));
	imgType_.emplace(PAGE_TYPE::RANGE_IN, LoadGraph(IMAGE_RANGE_IN.c_str()));
	imgType_.emplace(PAGE_TYPE::RANGE_OUT, LoadGraph(IMAGE_RANGE_OUT.c_str()));
	imgType_.emplace(PAGE_TYPE::HORMING, LoadGraph(IMAGE_HORMING.c_str()));
	imgType_.emplace(PAGE_TYPE::BOOST, LoadGraph(IMAGE_BOOST.c_str()));
	imgType_.emplace(PAGE_TYPE::BOOST_DASH, LoadGraph(IMAGE_BOOSTDASH.c_str()));
	imgType_.emplace(PAGE_TYPE::HORMING_BOOST, LoadGraph(IMAGE_HORMING_BOOST.c_str()));
	imgType_.emplace(PAGE_TYPE::HORMING_DASH, LoadGraph(IMAGE_HORMING_BOOSTDASH.c_str()));
	imgType_.emplace(PAGE_TYPE::COMBAT_TRANSITION, LoadGraph(IMAGE_COMBAT_TRANSITION.c_str()));
	imgType_.emplace(PAGE_TYPE::COMBAT_QUICK, LoadGraph(IMAGE_COMBAT_QUICK.c_str()));
	imgType_.emplace(PAGE_TYPE::COMBAT_CANCEL, LoadGraph(IMAGE_COMBAT_CANCEL.c_str()));
	imgType_.emplace(PAGE_TYPE::COMBAT, LoadGraph(IMAGE_COMBAT.c_str()));
	
	cursorImg_ = resMng_.Load(ResourceManager::SRC::TRIANGLE).handleId_;
}

void TutorialScene::InitExplanation(void)
{
	explanationType_.emplace(PAGE_TYPE::UI,"�@������HP�A�G��HP\n�B�^�[�Q�b�g�\��\n�C�c�e��\n�D�u�[�X�g�Q�[�W");
	explanationType_.emplace(PAGE_TYPE::JUMP,"XBOX:A�{�^�� PS�R��:���{�^��\n����������ƃu�[�X�g�Q�[�W���s����܂ŏオ�邼");
	explanationType_.emplace(PAGE_TYPE::SHOT,"XBOX:X�{�^�� PS�R��:�Z�{�^��\n����Ɍ������Ēe�𔭎�");
	explanationType_.emplace(PAGE_TYPE::RANGE_IN,"�^�[�Q�b�g�\�������Ȃ瑊���ǔ�����");
	explanationType_.emplace(PAGE_TYPE::RANGE_OUT,"�^�[�Q�b�g�\�����ԂȂ�ǔ����Ȃ�");
	explanationType_.emplace(PAGE_TYPE::HORMING,"�ǔ�����e�͒ʏ�ړ�����������Ȃ�");
	explanationType_.emplace(PAGE_TYPE::BOOST,"XBOX:A�{�^�� PS�R��:���{�^��\n��f������񉟂�\n�u�[�X�g�Q�[�W������č����ړ�����");
	explanationType_.emplace(PAGE_TYPE::BOOST_DASH,"�u�[�X�g��Ƀ{�^��������������ƃu�[�X�g�Q�[�W��\n�s����܂Ńu�[�X�g�_�b�V�����ł���");
	explanationType_.emplace(PAGE_TYPE::HORMING_BOOST,"�G�̒e�̒ǔ��̓u�[�X�g�ŐU��؂邱�Ƃ��ł���");
	explanationType_.emplace(PAGE_TYPE::HORMING_DASH,"�u�[�X�g�_�b�V���ł͓G�̒e�̒ǔ���\n�U��؂邱�Ƃ͂ł��Ȃ�");
	explanationType_.emplace(PAGE_TYPE::COMBAT_TRANSITION,"XBOX:Y�{�^�� PS�R��:���{�^��\n�i���ڍs��ԂɈڍs\n���������_�ł̑���̏ꏊ\n�Ɍ������Ĉړ�����");
	explanationType_.emplace(PAGE_TYPE::COMBAT,"����̏ꏊ�ɓ��B�����玩���Ŏa�����");
	explanationType_.emplace(PAGE_TYPE::COMBAT_QUICK,"�G�̈ʒu�ɓ��B����O�ɂ�����x�{�^����\n�����Ƃ��̏�Ŏa�����o��");
	explanationType_.emplace(PAGE_TYPE::COMBAT_CANCEL,"�i���ڍs��Ԓ���\nXBOX:A�{�^�� PS�R��:���{�^����\n�f������񉟂���\n�u�[�X�g�ŃL�����Z�����ł���");
	explanationType_.emplace(PAGE_TYPE::NEXT,"�Q�[���ɐi��\n�܂�����");

}

bool TutorialScene::ButtonPush(void)
{
	JOYPAD_TYPE type = input_.GetJPadType();
	//�v���X�e�R���Ȃ�~�{�^��
	if (JOYPAD_TYPE::DUAL_SHOCK_4 == type || JOYPAD_TYPE::DUAL_SENSE == type)
	{
		if (input_.IsTriggerd("b"))
		{
			return true;
		}
	}
	//XBOX�Ȃ�A�{�^��
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

void TutorialScene::SelectCursor(void)
{
	if (pageIdx_ < PAGE_MAX)
	{
		return;
	}
	//��{�^�����\���L�[�����������A�J�[�\������ɓ�����
	if (input_.IsTriggerd("up") && cursorIdx_ > 1)
	{
		cursorIdx_--;
	}
	//���{�^�����\���L�[������������A�J�[�\�������ɓ�����
	else if (input_.IsTriggerd("down") && cursorIdx_ < 2)
	{
		cursorIdx_++;
	}

	if (input_.IsTriggerd("up") && cursorIdx_ > 1)
	{
		cursorIdx_--;
	}

}

void TutorialScene::ChangeGameScene(void)
{
	//�Q�[���V�[���ֈڍs
	sceneManager_.ChangeScene(std::make_shared<GameScene>(sceneManager_, playMode_, sceneTransitor_, input_));
}

bool TutorialScene::SelectTutorialOrGame(void)
{	
	if (ButtonPush())
	{
		if (cursorIdx_ == CURSOR_MAX)
		{
			ChangeGameScene();
			return true;
		}
		else
		{
			pageIdx_ = 13;
			return false;
		}
	}
	return false;

}
