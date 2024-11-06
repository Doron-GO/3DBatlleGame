#include "Stage.h"
#include<DxLib.h>
#include"../../Manager/ResourceManager.h"

//バリアのエフェクトタイプ
constexpr int EFFECT_TYPE = 0;

//ステージの座標
constexpr VECTOR STAGE_POS = { 0.0f, -2400.0f,0.0f };

//ステージの大きさ
constexpr VECTOR STAGE_SCALE = { 250.0f, 250.0f,250.0f };

//バリアーの大きさ
constexpr VECTOR BARRIER_SCALE = { 3500.0f,3500.0f,3500.0f };

//再生速度
constexpr float BARRIER_SPEED = 0.4f;

Stage::Stage():resMng_(ResourceManager::GetInstance())
{
	//モデルの読み込み
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::STAGE));
	//大きさ
	transform_.scl = STAGE_SCALE;
	//座標
	transform_.pos = STAGE_POS;
	//回転
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f,0.0f, 0.0f });
	transform_.Update();
	//コリジョンの作成
	transform_.MakeCollider(Collider::TYPE::STAGE);

	//エフェクトマネージャーの生成
	effectManager_ = std::make_unique<EffectManager>(transform_);
	//エフェクト
	effectManager_->Add(EFFECT_TYPE,
		{ BARRIER_SCALE.x,BARRIER_SCALE.y,BARRIER_SCALE.z },
		{ 0.0f,900.0f,0.0f },
		{ 0.0f,0.0f,0.0f },
		true,
		false,
		resMng_.Load(ResourceManager::SRC::BARRIOR_DOME).handleId_);
	//バリアのエフェクトの再生
	effectManager_->Play(EFFECT_TYPE);
	//バリアの再生速度を設定
	effectManager_->SetSpeed(EFFECT_TYPE, BARRIER_SPEED);
}

Stage::~Stage(void)
{
}

void Stage::Draw(void)
{
	//描画
	MV1DrawModel(transform_.modelId);
	//エフェクト再生
	effectManager_->Update();
}

Collider* Stage::GetCollider(void)
{
	return transform_.collider;
}
