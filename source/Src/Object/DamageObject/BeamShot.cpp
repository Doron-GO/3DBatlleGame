#include<DxLib.h>
#include<string>
#include"../../Utility/AsoUtility.h"
#include"../Weapon/BeamRifle.h"
#include"../Time/DeltaTime.h"
#include "BeamShot.h"

#pragma region MyRegion

//弾の回転
constexpr VECTOR DEFAULT_ROT = { 0.0f,180.0f,0.0f };
//弾の大きさ 
constexpr VECTOR DEFAULT_SCALE = { 1.0f,1.0f,1.0f };
//弾の座標
constexpr VECTOR DEFAULT_POS = { 0.0f,0.0f,0.0f };

//ビームエフェクトのオフセット
constexpr VECTOR EFECT_BEAM_OFFSET = { 0.0f,0.0f,0.0f };
//ビームエフェクトの大きさ
constexpr VECTOR EFECT_BEAM_SCALE = { 1.0f,1.0f,1.0f };
//ビームエフェクトの回転
constexpr VECTOR EFECT_BEAM_ROT = { 0.0f,0.0f,0.0f };

//ヒットエフェクトのオフセット
constexpr VECTOR EFECT_HIT_OFFSET = { 0.0f,0.0f,0.0f };
//ヒットエフェクトの大きさ
constexpr VECTOR EFECT_HIT_SCALE = { 1.0f,1.0f,1.0f };
//ヒットエフェクトの回転
constexpr VECTOR EFECT_HIT_ROT = { 0.0f,0.0f,0.0f };

//敵の初期座標
constexpr VECTOR EFECT_BEAM_POS = { 0.0f,0.0f,0.0f };
//敵への初期ベクトル
constexpr VECTOR DEFAULT_HORMING_VEC = { 0.0f,0.0f,0.0f };

//通常ダメージ
constexpr float DAMAGE_NORMAL = 50.0f;
//大ダメージ
constexpr float DAMAGE_BIG = 200.0f;

//初期クールタイム
constexpr float DEFAULT_COOLTIME = 240.0f;

//クールタイムを0にする
constexpr float COOLTIME_ZERO = 0.0f;

//ビームが発射可能になるまでの時間
constexpr float LIMIT_BEAM_COUNT = 320.0f;
//ビームの再発射可能時間を計測するデルタタイムにかける定数
constexpr float BEAM_COUNT_RATE = 60.0f;

//生存時間を0にする
constexpr float DEATH_TIME_ZERO = 0.0f;
//ビームの消滅を計測するデルタタイムにかける定数
constexpr float DEATH_COUNT_RATE = 60.0f;
//ビームがアクティブでいられる時間
constexpr float LIMIT_DEATH_COUNT_ = 140.0f;

//ホーミングしていられる時間
constexpr float LIMIT_HORMING_COUNT = 120.0f;

//弾が追いかける敵の座標を少し上にずらすための補正値
constexpr float OFFSET_ENEMY = 100.0f;

//弾が追いかけるボスの座標を少し上にずらすための補正値
constexpr float OFFSET_BOSS_ENEMY = 1500.0f;


#pragma endregion

BeamShot::BeamShot(int playerType, const bool& isHorming, int num, int playMode) :DamageObject(playerType),isHorming_(isHorming)
{
	//モデルの読み込み
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::BEAM_COLLISION));
	//プレイヤータイプの設定
	playerType_=playerType;
	//大きさ
	transform_.scl = DEFAULT_SCALE ;
	//座標
	transform_.pos = DEFAULT_POS;
	//回転
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ DEFAULT_ROT.x, AsoUtility::Deg2RadF(DEFAULT_ROT.y), DEFAULT_ROT.z });
	//非有効化
	isActive_ = false;
	//待機状態の更新処理にする
	update_ = std::bind(&BeamShot::WaitUpdate, this);
	//敵座標の初期化
	enemyPos_ = EFECT_BEAM_POS ;
	//ホーミングベクトルの初期化
	hormingVec_ = DEFAULT_HORMING_VEC;
	//エフェクトマネージャーの生成
	effectManager_ = std::make_unique<EffectManager>(transform_);
	//エフェクトの追加
	effectManager_->Add(static_cast<int>(BEAM_STATE::BEAM),
		EFECT_BEAM_SCALE,
		EFECT_BEAM_OFFSET,
		EFECT_BEAM_ROT,
		true,
		false,
		resMng_.Load(ResourceManager::SRC::BEAM).handleId_);

	effectManager_->Add(static_cast<int>(BEAM_STATE::HIT),
		HIT_EFFECT_SCARE,
		EFECT_HIT_OFFSET,
		EFECT_BEAM_ROT,
		false,
		false,
		resMng_.Load(ResourceManager::SRC::HIT).handleId_);
	//通常ダメージ量
	damage_ = DAMAGE_NORMAL;
	//大ダメージ量
	bigDamage_ = DAMAGE_BIG;
	//クールタイムの設定
	coolTime_ = DEFAULT_COOLTIME;
	//生存時間カウントの初期化
	deathCount_ = DEATH_TIME_ZERO;

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
	//
	update_();
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
	isActive_ = true;
	//ビーム非有効化までの時間の設定
	deathCount_ = LIMIT_DEATH_COUNT_;
	//ビームエフェクトの再生
	effectManager_->Play(static_cast<int>(BEAM_STATE::BEAM),true);
	//敵へのホーミング処理
	//Horming();
	//ホーミング状態なら
	if (isHorming_)
	{	//ホーミングアップデートにする
		update_ = std::bind(&BeamShot::HormingUpdate, this);
	}
	else
	{	//ノーマルアップデートにする
		update_ = std::bind(&BeamShot::NormalUpdate, this);
	}
	transform_.Update();	
	//当たり判定の更新
	MV1RefreshCollInfo(transform_.modelId);

}

void BeamShot::InActivate(void)
{
	//非有効化
	isActive_ = false;
	//弾の生存可能時間をリセット
	deathCount_ = DEATH_TIME_ZERO;
	//ビームエフェクトの再生を止める
	effectManager_->Stop(static_cast<int>(BEAM_STATE::BEAM));
	//ホーミング時間のリセット
	hormingCount_ = COOLTIME_ZERO;
	//待機状態に変更
	update_ = std::bind(&BeamShot::WaitUpdate, this);
}

void BeamShot::Hit(void)
{
	//非有効化
	isActive_ = false;
	//弾の生存可能時間をリセット
	deathCount_ = DEATH_TIME_ZERO;
	//ヒットエフェクトの再生
	effectManager_->Play(static_cast<int>(BEAM_STATE::HIT));
	//ビームエフェクトの再生を止める
	effectManager_->Stop(static_cast<int>(BEAM_STATE::BEAM));
	//
	update_ = std::bind(&BeamShot::WaitUpdate, this);
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
	//このアップデートでは何もしない
}

void BeamShot::NormalUpdate(void)
{
	//移動処理
	transform_.pos = VAdd(transform_.pos, VScale(transform_.GetForward(), SPEED * deltaTime_));
	//当たり判定の更新
	MV1RefreshCollInfo(transform_.modelId);
	//生存可能時間計測
	TimeToDeath();
}

void BeamShot::HormingUpdate(void)
{	
	//ホーミング処理
	Horming();
	//移動処理
	transform_.pos = VAdd(transform_.pos, VScale(transform_.GetForward(), SPEED * deltaTime_ ));
	//当たり判定の更新
	MV1RefreshCollInfo(transform_.modelId);
	//生存可能時間計測
	TimeToDeath();
	//ホーミング可能時間計測
	hormingCount_ += deltaTime_ * DEATH_COUNT_RATE;
	//ホーミング可能時間を超えたら
	if (hormingCount_> LIMIT_HORMING_COUNT)
	{
		//ホーミング処理のなしのアップデート処理に変更
		update_ = std::bind(&BeamShot::NormalUpdate, this);
	}
}

void BeamShot::TimeToDeath(void)
{
	//生存可能時間計測
	deathCount_ -= deltaTime_ * DEATH_COUNT_RATE;
	//生存可能時間が0になったら
	if (deathCount_ < DEATH_TIME_ZERO )
	{
		deathCount_ = DEATH_TIME_ZERO;
		//非有効化
		InActivate();
	}
}

void BeamShot::Horming(void)
{
	//敵座標の更新
	VECTOR enemyPos = { enemyPos_.x,enemyPos_.y + offsetEnemy_,enemyPos_.z };
	//ベクトルを作り、正規化
	hormingVec_ = VNorm(VSub(enemyPos, transform_.pos));
	//そのベクトルの向きに方向を変える
	transform_.quaRot = Quaternion::LookRotation(hormingVec_);
}

void BeamShot::CoolTimeCount(void)
{
	//クールタイムが規定を超えてなければ加算する
	if (!(coolTime_> LIMIT_BEAM_COUNT))
	{
		coolTime_ += deltaTime_* BEAM_COUNT_RATE;
		//一定を超えたら最大の値に固定する
		if (coolTime_ > LIMIT_BEAM_COUNT)
		{
			coolTime_ = LIMIT_BEAM_COUNT;
		}
	}
	//クールタイムが規定を超えていれば最大の値に固定する
	else
	{
		coolTime_ = LIMIT_BEAM_COUNT;
	}
}

void BeamShot::InActivateHorming()
{
	update_ = std::bind(&BeamShot::NormalUpdate, this);

}
