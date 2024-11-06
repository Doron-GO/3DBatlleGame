#include <math.h>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../../Utility/AsoUtility.h"
#include "../Common/Transform.h"
#include "Camera.h"

// FIXED_POINT : カメラの初期座標
constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };

// FOLLOW : カメラ位置(追従対象との相対座標)
constexpr VECTOR RELATIVE_CAMERA_POS_FOLLOW = { 800.0f, 600.0f, 800.0f };

// FOLLOW : カメラ位置(追従対象との相対座標)
constexpr float SYNC_PLATER_POS_OFFSET = 300.0f;

// FOLLOW : 注視点(追従対象との相対座標)
constexpr VECTOR RELATIVE_TARGET_POS = { 0.0f, 100.0f, 500.0f };

// カメラのX回転上限度角
constexpr float LIMIT_X_UP_RAD = 40.0f * (DX_PI_F / 180.0f);
constexpr float LIMIT_X_DW_RAD = 15.0f * (DX_PI_F / 180.0f);

// 追従対象との調整座標
constexpr VECTOR RELATIVE_TRANSFORM_POS = { 0.0f, 20.0f, 0.0f };

// カメラ回転スピード
constexpr float SPEED = 2.0f;

Camera::Camera()
{
	angles_ = VECTOR();
	cameraUp_ = {0.0f,100.0f,-500.0f};
	mode_ = MODE::NONE;
	cameraPos_ = AsoUtility::VECTOR_ZERO;
	targetPos_ = AsoUtility::VECTOR_ZERO;
	// カメラの上方向
	cameraUp_ = { 0.0f,1.0f,0.0f };
	syncTransform_ = nullptr;
}

Camera::~Camera()
{
}

void Camera::Init()
{
	//初期設定
	ChangeMode(MODE::FOLLOW);
}

void Camera::SetBeforeDraw(void)
{
	// クリップ距離を設定する(SetDrawScreenでリセットされる)
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
	// カメラの設定
	SetCameraPositionAndTargetAndUpVec(
		cameraPos_,
		targetPos_,
		cameraUp_
	);
	Effekseer_Sync3DSetting();
}

void Camera::SetBeforeDrawFixedPoint(void)
{
	//固定カメラなので何もしない
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

	// カメラの初期設定
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
	// 同期先の位置
	VECTOR pos = syncTransform_->pos;

	//// 重力の方向制御に従う
	Quaternion gRot = { 1.0f,0.0f,-1.0f,0.0f };

	//// 正面から設定されたY軸分、回転させる
	quaRotOutX_ = gRot.Mult(Quaternion::AngleAxis(angles_.y, AsoUtility::AXIS_Y));

	//// 正面から設定されたX軸分、回転させる
	quaRot_ = quaRotOutX_.Mult(Quaternion::AngleAxis(angles_.x, AsoUtility::AXIS_X));

	VECTOR localPos;

	// 注視点(通常重力でいうところのY値を追従対象と同じにする)
	localPos = quaRotOutX_.PosAxis(RELATIVE_TARGET_POS);
	targetPos_ = VAdd(pos, localPos);

	// カメラ位置
	localPos = quaRot_.PosAxis(RELATIVE_CAMERA_POS_FOLLOW);
	cameraPos_ = VAdd(pos, localPos);
	
	// カメラの上方向
	cameraUp_ = {0.0f,1.0f,0.0f};
}

void Camera::SyncEnemyTransform(void)
{
	// 同期先の位置
	VECTOR syncPos = syncTransform_->pos;

	syncPos.y += SYNC_PLATER_POS_OFFSET;

	//相手に向かってベクトルを作る
	VECTOR enemyVec_ = VNorm(VSub(targetPos_, syncPos));
	Quaternion gRot = { 1.0f,0.0f,-1.0f,0.0f };
	
	//相対座標を設定
	VECTOR localPos = { -enemyVec_.x * RELATIVE_CAMERA_POS_FOLLOW.x,
						-enemyVec_.y* RELATIVE_CAMERA_POS_FOLLOW.y ,
						-enemyVec_.z * RELATIVE_CAMERA_POS_FOLLOW.z };

	cameraPos_ = VAdd(syncPos, localPos);

	// y座標を抜いた追従対象座標
	VECTOR syncPosOutY= { syncPos.x,0.0f,syncPos.z };
	
	// y座標を抜いたカメラ座標
	VECTOR posOutY = { cameraPos_.x,0.0f, cameraPos_.z };

	//X回転を抜いたカメラ回転
	quaRotOutX_ = gRot.LookRotation(VSub(syncPosOutY, posOutY));

}
