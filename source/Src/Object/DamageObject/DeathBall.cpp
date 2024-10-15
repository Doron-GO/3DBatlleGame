#include "DeathBall.h"
#include"../Time/DeltaTime.h"

DeathBall::DeathBall(int playerType, const VECTOR& pos) :DamageObject(playerType), playerPos_(pos)
{
	//モデルの読み込み
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::BEAM_COLLISION));
	//大きさ
	transform_.scl = { 150.0f,150.0f,150.0f };
	//座標
	transform_.pos = { 0.0f,0.0f,0.0f };
	//回転
	transform_.quaRot = Quaternion();
	//非有効化状態にする
	activeFlag_ = false;
	//ダメージ量の設定
	damage_ = 500.0f;
	//更新処理の状態
	update_ = std::bind(&DeathBall::WaitUpdate, this);
	//エフェクトマネージャーの生成
	effectManager_ = std::make_unique<EffectManager>(transform_);
	//エフェクトの追加
	effectManager_->Add(static_cast<int>(BALL_STATE::BALL), { 6000.0f,6000.0f,6000.0f }, true, resMng_.Load(ResourceManager::SRC::DEATH_BALL).handleId_);
}

DeathBall::~DeathBall()
{

}

void DeathBall::Draw(void)
{
}

void DeathBall::Update(VECTOR enemyPos)
{
	update_();
	//エフェクトの更新
	effectManager_->Update();
	//トランスフォームの更新
	transform_.Update();
}

void DeathBall::Activate(void)
{
	//座標をプレイヤーの頭上に設定
	SetPos(VECTOR{ playerPos_.x, POS_DETHBALL_Y, playerPos_.z });

	//有効化する
	activeFlag_ = true;
	//当たり判定の更新
	MV1RefreshCollInfo(transform_.modelId);
	//エフェクトの再生
	effectManager_->Play(static_cast<int>(BALL_STATE::BALL));
	//アップデートをNormalUpdateに変更
	update_ = std::bind(&DeathBall::NormalUpdate, this);


}

void DeathBall::InActivate(void)
{
	//非有効化
	activeFlag_ = false;
	//当たり判定の更新
	MV1RefreshCollInfo(transform_.modelId);
	//エフェクトの再生を停止
	effectManager_->Stop(static_cast<int>(BALL_STATE::BALL));
	//アップデートをNormalUpdateに変更
	update_ = std::bind(&DeathBall::WaitUpdate, this);

}

const int& DeathBall::GetModelId(void) const
{
	return transform_.modelId;
}

void DeathBall::WaitUpdate(void)
{
}

void DeathBall::NormalUpdate(void)
{	
	//当たり判定の更新
	MV1RefreshCollInfo(transform_.modelId);
	//移動処理
	float deltatime = DeltaTime::GetInstsnce().GetDeltaTime();
	transform_.pos.y -= 500.0f * deltatime;
	//地面の半分まで埋まったら、非有効化
	if (transform_.pos.y<=-300.0f)
	{
		InActivate();
	}
}
