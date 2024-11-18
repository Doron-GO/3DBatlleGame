#include <math.h>
#include <DxLib.h>
#include "../../Object/Time/DeltaTime.h"
#include"../../Manager/ResourceManager.h"
#include "../../Utility/AsoUtility.h"
#include "IrisTransitor.h"


const float DELTATIME_RATE = 120.0f;

IrisTransitor::IrisTransitor(bool irisOut, float interval, bool isTiled, int gHandle) 
	:Transitor(interval),irisOut_(irisOut),isTiled_(isTiled),gHandle_(gHandle), 
	resMng_(ResourceManager::GetInstance())
{
	//�E�B���h�E�T�C�Y
	VECTOR screenSize = { 1600.0f,1000.0f };
	int X = static_cast<int>(screenSize.x);
	int Y = static_cast<int>(screenSize.y);

	//�}�X�N���C���[�̍쐬
	handleForMaskScreen_ = MakeScreen(X, Y, true);
	//�}�X�N�n���h��
	maskH_ = CreateMaskScreen();

	//�E�B���h�E�̑Ίp���̒���
	diagonalLength_ = hypotf(screenSize.x, screenSize.y) / 2.0f;

	//�g�����W�V�����摜
	imgFrameHandle_ = resMng_.Load(ResourceManager::SRC::TRANSITION_FRAME).handleId_;

	//�g�����W�V�����g�摜
	imgMaskHandle_ = resMng_.Load(ResourceManager::SRC::TRANSITION).handleId_;

	//�A���O���̏�����
	angle_ = 0.0f;
}

IrisTransitor::~IrisTransitor()
{
	DeleteMaskScreen();
}

void IrisTransitor::Start()
{
	VECTOR size = { 1600,1000 };
	int sizeX = static_cast<int>(size.x);
	int sizeY = static_cast<int>(size.y);

	oldRT_ = MakeScreen(sizeX, sizeY);
	newRT_ = MakeScreen(sizeX, sizeY);

	int result = GetDrawScreenGraph(0, 0, sizeX, sizeY, oldRT_);
	frame_ = 0.0f;
	//�A���O���̏�����
	angle_ = 0.0f;
}


void IrisTransitor::Update()
{
	if (frame_ < interval_) 
	{
		frame_ += DELTATIME_RATE * DeltaTime::GetInstsnce().GetDeltaTime();
		mainScreen_ = newRT_;
	}
	else if (frame_ >= interval_) 
	{
		mainScreen_ = DX_SCREEN_BACK;
	}
	SetDrawScreen(mainScreen_);
}

void IrisTransitor::Draw()
{
	if (IsEnd()) 
	{
		return;
	}
	auto rate = frame_ / interval_;
	int backRT = oldRT_;
	int maskedRT = newRT_;
	if (irisOut_) {
		backRT = newRT_;
		maskedRT = oldRT_;
		rate = 1.0f - rate;
	}
	//
	float radius = (diagonalLength_)*rate;
	SetDrawScreen(handleForMaskScreen_);
	ClearDrawScreen();

	//
	const float SPEED_POW = 2.0f;
	const float IMG_SCALE = 15.0f;
	//�ێ���Ԓ��̉摜�̑傫��
	const float IMG_SCALE_STOP = 1.4f;

	//const float FRAME_RATE_1ST_SCALING = 0.45f;
	//const float FRAME_RATE_2ST_STOP = 0.75f;
	
	//�����{�g��X�P�[��
	const float FRAME_RATE_1ST_SCALING = 0.25f;
	//�X�P�[���ێ����Ԃ̃��~�b�g
	const float FRAME_RATE_2ST_STOP = 0.55f;
	//�摜�̊g�嗦
	float xRate = 0.0f;

	if (rate < FRAME_RATE_1ST_SCALING)
	{
		// �����{
		xRate = rate / FRAME_RATE_1ST_SCALING * IMG_SCALE_STOP;
	}
	else if (rate < FRAME_RATE_2ST_STOP)
	{
		// �ێ�
		xRate = IMG_SCALE_STOP;
	}
	else
	{
		// ����{
		angle_ += AsoUtility::Deg2RadF(3.0f);
		float scaleMax = pow(rate, SPEED_POW)* IMG_SCALE;
		float scaleRate = (rate - FRAME_RATE_2ST_STOP) / (1.0f - FRAME_RATE_2ST_STOP);
		xRate = AsoUtility::Lerp(
			IMG_SCALE_STOP,
			scaleMax,
			scaleRate);
	}

	VECTOR pos = { 1600.0f / 2.0f, 1000.0f / 2.0f };

	DrawRotaGraph(pos.x, pos.y, xRate, angle_, imgMaskHandle_, true);

	//�B���֐�(���݂̃O���t�B�b�N�n���h�����}�X�N�X�N���[���ɓ]��)
	SetMaskScreenGraph(handleForMaskScreen_);
	//�`��̈�𔽓]����
	SetMaskReverseEffectFlag(true);

	SetDrawScreen(DX_SCREEN_BACK);
	SetUseMaskScreenFlag(false);
	DrawGraph(0, 0, backRT, true);
	SetUseMaskScreenFlag(true);
	DrawGraph(0, 0, maskedRT, true);
	SetUseMaskScreenFlag(false);

	DrawRotaGraph(pos.x, pos.y, xRate, angle_, imgFrameHandle_, true);

}
