#pragma once
#include"../../Comon/Transform.h"
#include"../ActorBase.h"

class ResourceManager;
class DeathBall;
class RobotAnimeController;
class StateBase;
class EffectManager;
class DamageObject;

class BossEnemy:
	public ActorBase
{
public:

	enum class STATE
	{
		NONE,
		WAIT,
		SHOOTING,
		DOWN,
		DEAD

	};

	enum class SP_MOVE
	{
		NONE,
		DEATH_BALL
	};

	BossEnemy();
	~BossEnemy();

	//アップデート
	void Update();

	//描画
	void Draw(void);

	//相手の座標
	void SetPlayerPosition(const VECTOR* playerPos);

	//座標を取得
	const VECTOR& GetBossEnemyPos(void) const;

	//プレイヤーの状態をへんこうする
	void ChangeState(std::unique_ptr<StateBase> state);

	//アニメーションの初期化
	virtual void InitAnimation(void)override;

	//相手の座標を設定
	void SetEnemyPosition(const VECTOR* enemyPos);

	//攻撃を行う
	void ActiveSpMove(SP_MOVE spMove);

	//攻撃が終わっているかどうかを判定
	const bool& IsActiveSpMove(SP_MOVE spMove);

	//攻撃の配列を取得
	const std::map<SP_MOVE, std::unique_ptr<DamageObject>>& GetSpMove()const;

	//攻撃オブジェクトの生成
	void MakeSpMoveObjects(void);

	//死亡状態に移行
	void ChangeDeathState(void);

	//エフェクトの再生
	void PlayEffect(STATE state);

private:

	//攻撃オブジェクト配列
	std::map<SP_MOVE, std::unique_ptr<DamageObject>> spMoves_;

	//座標、回転、大きさ等の情報を持つ
	Transform transform_;

	//エフェクト再生
	std::unique_ptr<EffectManager> effectManager_;

	//状態の管理
	std::unique_ptr<StateBase> state_;

	//状態の管理
	std::unique_ptr<DeathBall> deathBall_;

	//プレイヤーの座標
	const VECTOR* playerPos_;

};

