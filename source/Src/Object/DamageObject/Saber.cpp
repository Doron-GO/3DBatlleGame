#include "Saber.h"

Saber::Saber(int playerType, Transform& transform) :DamageObject(playerType), transform_(transform)
{
	damage_ = 250.0f;
	effectManager_ = std::make_unique<EffectManager>(transform_);
	effectManager_->Add(static_cast<int>(SABER_STATE::SLASH), HIT_EFFECT_SCARE, false, resMng_.Load(ResourceManager::SRC::SLASH).handleId_);
	effectManager_->Add(static_cast<int>(SABER_STATE::HIT), HIT_EFFECT_SCARE, false, resMng_.Load(ResourceManager::SRC::HIT).handleId_);
}

void Saber::Update(VECTOR enemyPos)
{
	MV1RefreshCollInfo(transform_.modelId);
	effectManager_->Update();
}

void Saber::Draw(void)
{

}

void Saber::Hit(void)
{
	activeFlag_ = false;
	effectManager_->Play(static_cast<int>(SABER_STATE::HIT));
	//effectManager_->Play(static_cast<int>(SABER_STATE::SLASH));

}

const int& Saber::GetModelId(void) const
{
	return transform_.modelId;
}


