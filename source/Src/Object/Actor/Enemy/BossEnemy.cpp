#include"../../State/BossEnemyState/WaitState.h"
#include"../../State/BossEnemyState/DeadState.h"
#include"../../../Manager/ResourceManager.h"
#include"../../../Manager/EffectManager.h"
#include"../../../Utility/AsoUtility.h"
#include"../../../Manager/RobotAnimeController.h"
#include"../../../../Config.h"
#include"../../DamageObject/DeathBall.h"
#include"../../Time/DeltaTime.h"
#include "BossEnemy.h"
#include "BossEnemy.h"
#include<DxLib.h>
#pragma region Parameter

//大きさ
constexpr VECTOR SCALE_BOSS_ENEMY = { 10.0f,10.0f,10.0f };

//座標
constexpr VECTOR DEFAULT_BOSS_POS = { 0.0f, -500.0f, 1600.0f };

//ボスタイプ
constexpr int ENEMY_TYPE = 1;

//ボスHP
constexpr float ENEMY_HP = 100.0f;

//当たり判定カプセルの半径
constexpr float CAPSUL_RADIUS = 300.0f;

//カプセル上座標
constexpr float CAPSUL_TOP = 2000.0f;

//カプセル下座標
constexpr float CAPSUL_DOWN = 200.0f;

//死亡時エフェクトの大きさ
constexpr VECTOR EFFECT_EXPLOSION_SCALE = { 200.0f,200.0f,200.0f };

//攻撃エフェクトのオフセット
constexpr VECTOR EFFECT_EXPLOSION_OFFSET = { 0.0f,0.0f,0.0f };

//攻撃エフェクトの回転
constexpr VECTOR EFFECT_EXPLOSION_ROT = { 0.0f,0.0f,0.0f };

//シュートアニメーションの再生速度
constexpr float ANIM_SHOOT_SPEED = 10.0f;
//シュートアニメーションの総再生時間
constexpr float  TOTAL_TIME_ANIM_SHOOT = 750.0f;

//アイドルアニメーションの再生速度
constexpr float ANIM_IDLE_SPEED = 60.0f;
//アイドルアニメーションの総再生時間
constexpr float  TOTAL_TIME_ANIM_IDLE = 427.0f;

//死亡アニメーションの再生速度
constexpr float ANIM_DEAD_SPEED = 30.0f;
//死亡アニメーションの総再生時間
constexpr float  TOTAL_TIME_ANIM_DEAD = 174.0f;

//ダウンアニメーションの再生速度
constexpr float ANIM_DOWN_SPEED = 100.0f;
//ダウンアニメーションの総再生時間
constexpr float  TOTAL_TIME_ANIM_DOWN = 427.0f;



#pragma endregion

BossEnemy::BossEnemy(int playerType)
{

	actorType_ = static_cast<ACTOR_TYPE>(playerType);

	//モデルの読み込み
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER));
	//大きさ
	transform_.scl = SCALE_BOSS_ENEMY;
	//座標
	transform_.pos = { DEFAULT_BOSS_POS.x, DEFAULT_BOSS_POS.y, (DEFAULT_BOSS_POS.z) };
	//回転
	transform_.quaRot = Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	transform_.Update();

	//アニメーションコントローラーの生成
	robotAnimeController_ = std::make_unique <RobotAnimeController>(transform_.modelId);

	//カプセルあたらい判定クラスの生成
	capsule_ = std::make_unique<CollisionCapsule>(transform_, CAPSUL_TOP, CAPSUL_DOWN, CAPSUL_RADIUS);
	//エフェクトマネージャーの生成
	effectManager_= std::make_unique<EffectManager>(transform_);
	//アニメーションの読み込み
	InitAnimation();

	//待機状態に設定　
	ChangeState(std::make_unique<WaitState>(*this));

	//HPを設定
	hp_ = ENEMY_HP;

}

BossEnemy::~BossEnemy(void)
{
	
}


void BossEnemy::InitAnimation(void)
{
	//アニメーションの追加
	robotAnimeController_->Add(static_cast<int>(BOSS_STATE::SHOOTING),
				PATH_ANIMATION_BOSS_ENEMY + "Boss_Shooting.mv1",
				ANIM_SHOOT_SPEED,
				TOTAL_TIME_ANIM_SHOOT);
	robotAnimeController_->Add(static_cast<int>(BOSS_STATE::WAIT),
				PATH_ANIMATION_BOSS_ENEMY + "Boss_Idle.mv1",
				ANIM_IDLE_SPEED,
				TOTAL_TIME_ANIM_IDLE);
	robotAnimeController_->Add(static_cast<int>(BOSS_STATE::DEAD),
				PATH_ANIMATION_BOSS_ENEMY + "Dying.mv1",
				ANIM_DEAD_SPEED,
				TOTAL_TIME_ANIM_DEAD);
	robotAnimeController_->Add(static_cast<int>(BOSS_STATE::DOWN),
				PATH_ANIMATION_BOSS_ENEMY + "Crouch.mv1",
				ANIM_DOWN_SPEED,
				TOTAL_TIME_ANIM_DOWN);
	robotAnimeController_->Update();

	//エフェクトの追加
	effectManager_->Add(static_cast<int>(BOSS_STATE::DEAD),
		EFFECT_EXPLOSION_SCALE,
		EFFECT_EXPLOSION_OFFSET,
		EFFECT_EXPLOSION_ROT,
		false,
		false,
		resMng_.Load(ResourceManager::SRC::EXPLOSION).handleId_);

}

void BossEnemy::ActiveSpMove(SP_MOVE spMove)
{
	spMoves_[spMove]->Activate();
}

const bool& BossEnemy::IsActiveSpMove(SP_MOVE spMove) 
{
	//攻撃が有効中かどうかを返す
	return 	spMoves_[spMove]->IsActive();
}

const std::map<BossEnemy::SP_MOVE, std::unique_ptr<DamageObject>>& BossEnemy::GetSpMove() const
{
	return spMoves_;
}

void BossEnemy::MakeSpMoveObjects(void)
{
	spMoves_.emplace(SP_MOVE::DEATH_BALL, std::make_unique <DeathBall>(ENEMY_TYPE,*enemyPos_));

}

void BossEnemy::ChangeDeathState(void)
{
	ChangeState(std::make_unique<DeadState>(*this));
	return;
}

void BossEnemy::PlayEffect(BOSS_STATE state)
{
	effectManager_->Play(static_cast<int>(state));
}

const BossEnemy::STATE& BossEnemy::GetState(void)
{
	return actorState_;
}

void BossEnemy::Update(void)
{
	//デルタタイムの更新
	deltaTime_ = DeltaTime::GetInstsnce().GetDeltaTime();
	//ステートのアップデート
	state_->Update();
	//攻撃オブジェクトのアップデート
	for(auto& spMove: spMoves_)
	{
		spMove.second->Update(*enemyPos_);
	}
	//アニメーションコントローラーのアップデート
	robotAnimeController_->Update();
	//当たり判定の更新
	capsule_->Update();
	//エフェクトマネージャーの更新
	effectManager_->Update();

}

void BossEnemy::Draw(void)
{
	MV1DrawModel(transform_.modelId);
	for (auto& spMove : spMoves_)
	{
		spMove.second->Draw();
	}
}

const VECTOR& BossEnemy::GetBossEnemyPos(void) const
{
	return transform_.pos;
}

void BossEnemy::ChangeState(std::unique_ptr<StateBase> state)
{
	state_ = std::move(state);
}

