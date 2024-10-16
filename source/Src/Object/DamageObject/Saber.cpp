#include "Saber.h"

Saber::Saber(int playerType, Transform& transform) :DamageObject(playerType), transform_(transform)
{
	//ダメージ量を設定
	damage_ = 250.0f;
	//エフェクトマネージャーを生成
	effectManager_ = std::make_unique<EffectManager>(transform_);
	//エフェクトの追加
	effectManager_->Add(static_cast<int>(SABER_STATE::HIT), HIT_EFFECT_SCARE, false, resMng_.Load(ResourceManager::SRC::HIT).handleId_);
}

void Saber::Update(VECTOR enemyPos)
{
	//当たり判定の更新
	MV1RefreshCollInfo(transform_.modelId);
	//エフェクトの更新
	effectManager_->Update();
}

void Saber::Draw(void)
{

}

void Saber::Hit(void)
{
	//非有効化する
	isActive_ = false;
	//エフェクトの再生
	effectManager_->Play(static_cast<int>(SABER_STATE::HIT));
}

const int& Saber::GetModelId(void) const
{
	return transform_.modelId;
}


