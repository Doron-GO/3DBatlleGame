#include"../../Utility/AsoUtility.h"
#include "BeamSaber.h"
#include<DxLib.h>


#pragma region Parameter

constexpr VECTOR BEAM_SABER_SCALE = { 0.02f,0.02f,0.02f };

constexpr VECTOR BEAM_SABER_LOCAL_ROT = { 0.0f,0.0f,-90.0f };


#pragma endregion




BeamSaber::BeamSaber(int playerType, Transform& player):WeaponBase(playerType, player)
{
	//モデルの読み込み
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::BEAMSABER));
	//座標
	transform_.pos = MV1GetFramePosition(playerTransform_.modelId, RIGHT_HAND_FRAME);
	//大きさ
	transform_.scl = { 0.02f,0.02f,0.02f };
	//回転
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler(BEAM_SABER_LOCAL_ROT.x,
			BEAM_SABER_LOCAL_ROT.y,
			AsoUtility::Deg2RadF(BEAM_SABER_LOCAL_ROT.z));
	transform_.Update();
	//当たり判定クラスの生成
	saber_ = std::make_unique<Saber>(playerType,transform_);

}

void BeamSaber::Update(void)
{
	//座標回転の同期
	SyncPosition();
	//当たり判定クラスの更新
	saber_->Update(transform_.pos);
}

void BeamSaber::Draw(void)
{
	//有効化されていたら描画する
	if (isActive_)
	{
		MV1DrawModel(transform_.modelId);
	}	
}

Saber& BeamSaber::GetSaber(void) const
{
	//当たり判定クラスSaberを取得する
	return *saber_;
}

