#include <math.h>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../../Utility/AsoUtility.h"
#include "../Common/Transform.h"
#include "Camera.h"

// FIXED_POINT : �J�����̏������W
constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };

// FOLLOW : �J�����ʒu(�Ǐ]�ΏۂƂ̑��΍��W)
constexpr VECTOR RELATIVE_CAMERA_POS_FOLLOW = { 800.0f, 600.0f, 800.0f };

// FOLLOW : �J�����ʒu(�Ǐ]�ΏۂƂ̑��΍��W)
constexpr float SYNC_PLATER_POS_OFFSET = 300.0f;

// FOLLOW : �����_(�Ǐ]�ΏۂƂ̑��΍��W)
constexpr VECTOR RELATIVE_TARGET_POS = { 0.0f, 100.0f, 500.0f };

// �J������X��]����x�p
constexpr float LIMIT_X_UP_RAD = 40.0f * (DX_PI_F / 180.0f);
constexpr float LIMIT_X_DW_RAD = 15.0f * (DX_PI_F / 180.0f);

// �Ǐ]�ΏۂƂ̒������W
constexpr VECTOR RELATIVE_TRANSFORM_POS = { 0.0f, 20.0f, 0.0f };

// �J������]�X�s�[�h
constexpr float SPEED = 2.0f;

Camera::Camera()
{
	angles_ = VECTOR();
	cameraUp_ = {0.0f,100.0f,-500.0f};
	mode_ = MODE::NONE;
	cameraPos_ = AsoUtility::VECTOR_ZERO;
	targetPos_ = AsoUtility::VECTOR_ZERO;
	// �J�����̏����
	cameraUp_ = { 0.0f,1.0f,0.0f };
	syncTransform_ = nullptr;
}

Camera::~Camera()
{
}

void Camera::Init()
{
	//�����ݒ�
	ChangeMode(MODE::FOLLOW);
}

void Camera::SetBeforeDraw(void)
{
	// �N���b�v������ݒ肷��(SetDrawScreen�Ń��Z�b�g�����)
	SetCameraNearFar(10.0f, 45000.0f);

	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		SetBeforeDrawFixedPoint();
		break;
	case Camera::MODE::FOLLOW:
		SetBeforeDrawFollow();
		break;
	}
	// �J�����̐ݒ�
	SetCameraPositionAndTargetAndUpVec(
		cameraPos_,
		targetPos_,
		cameraUp_
	);
	Effekseer_Sync3DSetting();
}

void Camera::SetBeforeDrawFixedPoint(void)
{
	//�Œ�J�����Ȃ̂ŉ������Ȃ�
}

void Camera::SetBeforeDrawFollow(void)
{
	SyncEnemyTransform();
}

void Camera::SetTransform(const Transform* transform)
{
	syncTransform_ = transform;

	cameraPos_ = VAdd(syncTransform_->pos, RELATIVE_CAMERA_POS_FOLLOW);

}

Quaternion Camera::GetQuaRot(void) const
{
	return quaRot_;
}

Quaternion Camera::GetQuaRotOutX(void) const
{
	return quaRotOutX_;
}

void Camera::SetTargetPos(VECTOR pos)
{
	targetPos_ = pos;
}

void Camera::SetCameraPos(VECTOR pos)
{
	cameraPos_ = pos;
}

void Camera::ChangeMode(MODE mode)
{

	SetDefault();

	mode_ = mode;
	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		break;
	case Camera::MODE::FOLLOW:
		break;
	}

}

void Camera::SetDefault(void)
{

	// �J�����̏����ݒ�
	cameraPos_ = DEFAULT_CAMERA_POS;
	targetPos_ = AsoUtility::VECTOR_ZERO;

	angles_.x = AsoUtility::Deg2RadF(10.0f);
	angles_.y = AsoUtility::Deg2RadF(90.0f);
	angles_.z = 0.0f;

	quaRot_ = Quaternion();

	cameraUp_ = AsoUtility::DIR_U;

}

void Camera::SyncTransform(void)
{
	//auto& gIns = lpGravityMng;
	// ������̈ʒu
	VECTOR pos = syncTransform_->pos;

	//// �d�͂̕�������ɏ]��
	Quaternion gRot = { 1.0f,0.0f,-1.0f,0.0f };

	//// ���ʂ���ݒ肳�ꂽY�����A��]������
	quaRotOutX_ = gRot.Mult(Quaternion::AngleAxis(angles_.y, AsoUtility::AXIS_Y));

	//// ���ʂ���ݒ肳�ꂽX�����A��]������
	quaRot_ = quaRotOutX_.Mult(Quaternion::AngleAxis(angles_.x, AsoUtility::AXIS_X));

	VECTOR localPos;

	// �����_(�ʏ�d�͂ł����Ƃ����Y�l��Ǐ]�ΏۂƓ����ɂ���)
	localPos = quaRotOutX_.PosAxis(RELATIVE_TARGET_POS);
	targetPos_ = VAdd(pos, localPos);

	// �J�����ʒu
	localPos = quaRot_.PosAxis(RELATIVE_CAMERA_POS_FOLLOW);
	cameraPos_ = VAdd(pos, localPos);
	
	// �J�����̏����
	cameraUp_ = {0.0f,1.0f,0.0f};
}

void Camera::SyncEnemyTransform(void)
{
	// ������̈ʒu
	VECTOR syncPos = syncTransform_->pos;

	syncPos.y += SYNC_PLATER_POS_OFFSET;

	//����Ɍ������ăx�N�g�������
	VECTOR enemyVec_ = VNorm(VSub(targetPos_, syncPos));
	Quaternion gRot = { 1.0f,0.0f,-1.0f,0.0f };
	
	//���΍��W��ݒ�
	VECTOR localPos = { -enemyVec_.x * RELATIVE_CAMERA_POS_FOLLOW.x,
						-enemyVec_.y* RELATIVE_CAMERA_POS_FOLLOW.y ,
						-enemyVec_.z * RELATIVE_CAMERA_POS_FOLLOW.z };

	cameraPos_ = VAdd(syncPos, localPos);

	// y���W�𔲂����Ǐ]�Ώۍ��W
	VECTOR syncPosOutY= { syncPos.x,0.0f,syncPos.z };
	
	// y���W�𔲂����J�������W
	VECTOR posOutY = { cameraPos_.x,0.0f, cameraPos_.z };

	//X��]�𔲂����J������]
	quaRotOutX_ = gRot.LookRotation(VSub(syncPosOutY, posOutY));

}
