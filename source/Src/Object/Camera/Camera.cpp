#include <math.h>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../../Utility/AsoUtility.h"
#include "../Comon/Transform.h"
#include "Camera.h"

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
	SetCameraNearFar(10.0f, 30000.0f);

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

void Camera::DrawDebug()
{

	//追従視点
	DrawSphere3D(targetPos_, 20.0f, 10, 0x00ff00, 0x00ff00, true);
	// 同期先の位置
	VECTOR syncPos = syncTransform_->pos;

	//相手に向かってベクトルを作る
	auto enemyVec_ = VSub(enemyPos_, syncPos);

	//自分の前方ベクトルと相手に向かってのベクトル間の回転量をとる
	auto  goalQuaRot_ = Quaternion::FromToRotation(VSub(syncPos, cameraPos_), enemyVec_);

	//自分から敵への角度
	auto angleY = AsoUtility::Rad2DegF(goalQuaRot_.ToEuler().y);
	auto angleX = AsoUtility::Rad2DegF(goalQuaRot_.ToEuler().x);

	DrawFormatString(0, 530, 0xffffff, "angleY:%f", angleY);
	DrawFormatString(0, 550, 0xffffff, "angleX:%f", angleX);

	DrawLine3D(syncPos, targetPos_, 0xffffff);
	DrawSphere3D(syncTransform_->pos, 20.0f, 20.0f, 0xffffff, 0xff0000, true);

	//相手に向かってベクトルを作る
	auto enemyVec= VSub(targetPos_, cameraPos_);
	enemyVec = VNorm(enemyVec_);
	//enemyVec_ = { -enemyVec_.x,enemyVec_.y*400.0f,-enemyVec_.z*800.0f};
	enemyVec = { -enemyVec.x,-(enemyVec.y *30.0f),-enemyVec.z };

	DrawFormatString(0, 570, 0xffffff, "enemyVec:%f,%f,%f", enemyVec.x, enemyVec.y, enemyVec.z);
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
