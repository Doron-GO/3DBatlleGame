#include "WeaponBase.h"
#include"../Actor/Player/Player.h"

WeaponBase::WeaponBase(int playerType, const Transform& playerTransform):resMng_(ResourceManager::GetInstance()),
playerTransform_(playerTransform), playerType_(playerType)
{
	activeFlag_ = false;
}

void WeaponBase::Activate(void)
{
	//有効化
	activeFlag_ = true;
}

void WeaponBase::InActivate(void)
{
	//非有効化
	activeFlag_ = false;
}

void WeaponBase::SyncPosition(void)
{
	//右手の行列
	MATRIX rightHandMat = MV1GetFrameLocalWorldMatrix(playerTransform_.modelId, ATTACH_RIGHT_HAND_FRAME);
	//武器のローカル回転
	Quaternion quaLocal = transform_.quaRotLocal;
	//武器のローカル座標
	MATRIX offset = MGetTranslate(VECTOR{5.0f,43.0f,-3.0f });
	//武器のローカル回転とローカル座標を右手の行列と合成
	rightHandMat = MMult(quaLocal.ToMatrix(), MMult(offset, rightHandMat));
	//武器のモデルの行列を設定
	MV1SetMatrix(transform_.modelId, MMult(transform_.matScl, rightHandMat));
	transform_.pos = MV1GetFramePosition(playerTransform_.modelId, 96);
}
