#include<DxLib.h>
#include"../../Utility/AsoUtility.h"
#include "BeamSaber.h"

BeamSaber::BeamSaber(int playerType, Transform& player):WeaponBase(playerType, player)
{
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::BEAMSABER));
	transform_.pos = MV1GetFramePosition(playerTransform_.modelId, ATTACH_RIGHT_HAND_FRAME);
	transform_.scl = { 0.02f,0.02f,0.02f };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler(AsoUtility::Deg2RadF(0.0f),0.0f, AsoUtility::Deg2RadF(-90.0f));
	transform_.Update();
	saber_ = std::make_unique<Saber>(playerType,transform_);

}

void BeamSaber::Update(void)
{
	SyncPosition();
	saber_->Update(transform_.pos);
}

void BeamSaber::Draw(void)
{
	if (activeFlag_)
	{
		MV1DrawModel(transform_.modelId);
	}	
}

Saber& BeamSaber::GetSaber(void) const
{
	return *saber_;
}

