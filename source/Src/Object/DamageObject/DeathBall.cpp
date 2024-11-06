#include "DeathBall.h"
#include"../Time/DeltaTime.h"

//出現するY座標
constexpr float POS_DETHBALL_Y = 5500.0f;

constexpr float DAMAGE = 500.0f;

constexpr float BALL_SPEED = 500.0f;

constexpr float LIMIT_POS_Y = -300.0f;

//Ballの大きさ
constexpr VECTOR BALL_SCALE = { 150.0f,150.0f,150.0f };

//Ballの座標
constexpr VECTOR BALL_POS = { 0.0f,0.0f,0.0f };


//エフェクトの大きさ
constexpr VECTOR EFFECT_BALL_SCALE = { 6000.0f,6000.0f,6000.0f };

//エフェクトのオフセット
constexpr VECTOR EFFECT_BALL_OFFSET = { 1.0f,1.0f,1.0f };

//エフェクトの回転
constexpr VECTOR EFFECT_BALL_ROT = { 1.0f,1.0f,1.0f };


DeathBall::DeathBall(int playerType, const VECTOR& pos) :DamageObject(playerType), playerPos_(pos)
{
	//モデルの読み込み
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::BEAM_COLLISION));
	//大きさ
	transform_.scl = BALL_SCALE;
	//座標
	transform_.pos = BALL_POS;
	//回転
	transform_.quaRot = Quaternion();
	//非有効化状態にする
	isActive_ = false;
	//ダメージ量の設定
	damage_ = DAMAGE;
	//更新処理の状態
	update_ = std::bind(&DeathBall::WaitUpdate, this);
	//エフェクトマネージャーの生成
	effectManager_ = std::make_unique<EffectManager>(transform_);
	//エフェクトの追加
	effectManager_->Add(static_cast<int>(BALL_STATE::BALL),
		EFFECT_BALL_SCALE,
		EFFECT_BALL_OFFSET,
		EFFECT_BALL_ROT,
		true,
		false,
		resMng_.Load(ResourceManager::SRC::DEATH_BALL).handleId_);
}

DeathBall::~DeathBall()
{

}

void DeathBall::Draw(void)
{
}

void DeathBall::Update(VECTOR enemyPos)
{
	deltaTime_ = DeltaTime::GetInstsnce().GetDeltaTime();

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
	isActive_ = true;
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
	isActive_ = false;
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
	//何もしない
}

void DeathBall::NormalUpdate(void)
{	
	//当たり判定の更新
	MV1RefreshCollInfo(transform_.modelId);
	//移動処理
	transform_.pos.y -= BALL_SPEED * deltaTime_;
	//地面の半分まで埋まったら、非有効化
	if (transform_.pos.y<= LIMIT_POS_Y)
	{
		InActivate();
	}
}
