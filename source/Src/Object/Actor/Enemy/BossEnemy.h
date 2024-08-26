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

	void Init();

	void Update();

	void Draw(void);

	//相手の座標
	void SetPlayerPosition(const VECTOR* playerPos);

	const VECTOR& GetBossEnemyPos(void) const;


	//プレイヤーの状態をへんこうする
	void ChangeState(std::unique_ptr<StateBase> state);

	virtual void InitAnimation(void)override;

	//相手の座標
	void SetEnemyPosition(const VECTOR* enemyPos);

	const VECTOR& GetEnemyPosition(void)const;

	void ActiveSpMove(SP_MOVE spMove);

	void InActiveSpMove(SP_MOVE spMove);

	const bool& IsActiveSpMove(SP_MOVE spMove);

	const int& GetSpMoveModelId( SP_MOVE spMove);

	const std::map<SP_MOVE, std::unique_ptr<DamageObject>>& GetSpMove()const;

	void MakeObjects(void);

	void ChangeDeathState(void);

	void PlayEffect(STATE state);


private:

	std::map<SP_MOVE, std::unique_ptr<DamageObject>> spMoves_;

	static constexpr VECTOR SCALE_BOSS_ENEMY = {10.0f,10.0f,10.0f };

	static constexpr VECTOR DEFAULT_POS = { 0.0f, -30.0f, 1600.0f };

	static constexpr int ENEMY_TYPE = 1;

	static constexpr float ENEMY_HP = 500.0f;

	static constexpr float CAPSUL_RADIUS = 300.0f;

	static constexpr float CAPSUL_TOP = 2000.0f;

	static constexpr float CAPSUL_DOWN = 200.0f;

	static constexpr VECTOR EFFECT_EXPLOSION_SCALE = { 200.0f,200.0f,200.0f };


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

