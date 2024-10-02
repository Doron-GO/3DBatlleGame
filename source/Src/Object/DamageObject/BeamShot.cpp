#include<DxLib.h>
#include<string>
#include"../../Utility/AsoUtility.h"
#include"../Weapon/BeamRifle.h"
#include"../Time/DeltaTime.h"
#include "BeamShot.h"


BeamShot::BeamShot(int playerType, const bool& isHorming, int num, int playMode) :DamageObject(playerType),isHorming_(isHorming)
{
	//モデルの読み込み
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::BEAM_COLLISION));
	//プレイヤータイプの設定
	playerType_=playerType;
	//大きさ
	transform_.scl = { 1.0f,1.0f,1.0f };
	//座標
	transform_.pos = { 0.0f,0.0f,0.0f };
	//回転
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	//非有効化
	activeFlag_ = false;
	//待機状態の更新処理にする
	_update = &BeamShot::WaitUpdate;
	//敵座標の初期化
	enemyPos_ = { 0.0f,0.0f,0.0f };
	//ホーミングベクトルの初期化
	hormingVec_ = { 0.0f,0.0f,0.0f };
	//エフェクトマネージャーの生成
	effectManager_ = std::make_unique<EffectManager>(transform_);
	//エフェクトの追加
	effectManager_->Add(static_cast<int>(BEAM_STATE::BEAM), transform_.scl,true,resMng_.Load(ResourceManager::SRC::BEAM).handleId_);
	effectManager_->Add(static_cast<int>(BEAM_STATE::HIT), HIT_EFFECT_SCARE,false,resMng_.Load(ResourceManager::SRC::HIT).handleId_);
	//通常ダメージ量
	damage_ = 50.0f;
	//大ダメージ量
	bigDamage_ = 200.0f;
	//クールタイムの設定
	coolTime_ = 240.0f;

	//モードによってオフセット値を変える
	if (static_cast<PLAY_MODE>(playMode) == PLAY_MODE::SINGLE_MODE)
	{
		offsetEnemy_ = OFFSET_BOSS_ENEMY;
	}
	else
	{
		offsetEnemy_ = OFFSET_ENEMY;
	}
}

void BeamShot::Update(VECTOR enemyPos)
{
	//デルタタイムの更新
	deltaTime_ = DeltaTime::GetInstsnce().GetDeltaTime();
	//敵の座標の更新
	enemyPos_ = enemyPos;
	(this->*_update)();
	//エフェクトマネージャーの更新
	effectManager_->Update();
	//トランスフォームの更新
	transform_.Update();
	//クールタイムのカウント更新
	CoolTimeCount();
}

void BeamShot::Draw(void)
{
}

void BeamShot::Activate(void)
{
	//有効化
	activeFlag_ = true;
	//当たり判定の更新
	MV1RefreshCollInfo(transform_.modelId);
	//ビーム非有効化までの時間の設定
	deathCount_ = DEATH_COUNT_TIME;
	//ビームエフェクトの再生
	effectManager_->Play(static_cast<int>(BEAM_STATE::BEAM));
	//敵へのホーミング処理
	Horming();
	//ホーミング状態なら
	if (isHorming_)
	{	//ホーミングアップデートにする
		_update = &BeamShot::HormingUpdate;
	}
	else
	{	//ノーマルアップデートにする
		_update = &BeamShot::NormalUpdate;
	}
	transform_.Update();
}

void BeamShot::InActivate(void)
{
	//有効化
	activeFlag_ = false;
	//ビームエフェクトの再生を止める
	effectManager_->Stop(static_cast<int>(BEAM_STATE::BEAM));
	//ホーミング時間のリセット
	hormingCount_ = 0.0f;
	//待機状態に変更
	_update = &BeamShot::WaitUpdate;
}

void BeamShot::Hit(void)
{
	//非有効化
	activeFlag_ = false;
	//弾の生存可能時間をリセット
	deathCount_ = 0.0f;
	//ヒットエフェクトの再生
	effectManager_->Play(static_cast<int>(BEAM_STATE::HIT));
	//ビームエフェクトの再生を止める
	effectManager_->Stop(static_cast<int>(BEAM_STATE::BEAM));

	_update = &BeamShot::WaitUpdate;
}

const int& BeamShot::GetModelId(void) const
{
	return transform_.modelId;
}

const float& BeamShot::GetBigDamage(void) const
{
	return bigDamage_;
}

void BeamShot::WaitUpdate(void)
{
}

void BeamShot::NormalUpdate(void)
{
	float deltatime = DeltaTime::GetInstsnce().GetDeltaTime();
	//移動処理
	transform_.pos = VAdd(transform_.pos, VScale(transform_.GetForward(), SPEED * (deltatime * 100.0f)));
	//当たり判定の更新
	MV1RefreshCollInfo(transform_.modelId);
	//生存可能時間計測
	TimeToDeath();
}

void BeamShot::HormingUpdate(void)
{	
	float deltatime = DeltaTime::GetInstsnce().GetDeltaTime();
	//ホーミング処理
	Horming();
	//移動処理
	transform_.pos = VAdd(transform_.pos, VScale(transform_.GetForward(), SPEED * (deltatime * 100.0f)));
	//当たり判定の更新
	MV1RefreshCollInfo(transform_.modelId);
	//生存可能時間計測
	TimeToDeath();
	//ホーミング可能時間計測
	hormingCount_ += deltatime * DEATH_COUNT_RATE;
	//ホーミング可能時間を超えたら
	if (hormingCount_> HORMING_COUNT_TIME)
	{
		//ホーミング処理のなしのアップデート処理に変更
		_update = &BeamShot::NormalUpdate;
	}
}

void BeamShot::TimeToDeath(void)
{
	float deltatime = DeltaTime::GetInstsnce().GetDeltaTime();
	//生存可能時間計測
	deathCount_ -= deltatime* DEATH_COUNT_RATE;
	//生存可能時間をこえていたら
	if (deathCount_<0)
	{
		deathCount_ = 0;
		//非有効化
		InActivate();
	}
}

void BeamShot::Horming(void)
{
	VECTOR enemyPos = { enemyPos_.x,enemyPos_.y + offsetEnemy_,enemyPos_.z };
	//ベクトルを作り、正規化
	hormingVec_ = VNorm(VSub(enemyPos, transform_.pos));
	//そのベクトルの向きに方向を変える
	transform_.quaRot = Quaternion::LookRotation(hormingVec_);
}

void BeamShot::CoolTimeCount(void)
{
	//クールタイムが規定を超えてなければ加算する
	if (!(coolTime_> BEAM_COUNT_TIME))
	{
		coolTime_ += deltaTime_* BEAM_COUNT_RATE;
		//一定を超えたら最大の値に固定する
		if (coolTime_ > BEAM_COUNT_TIME)
		{
			coolTime_ = BEAM_COUNT_TIME;
		}
	}
	//クールタイムが規定を超えていれば最大の値に固定する
	else
	{
		coolTime_ = BEAM_COUNT_TIME;
	}
}

void BeamShot::InActivateHorming()
{
	_update = &BeamShot::NormalUpdate;
}
