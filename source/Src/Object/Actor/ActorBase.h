#pragma once
#include"../Collision/CollisionCapsule.h"
#include<memory>

class RobotAnimeController;
class ResourceManager;

class ActorBase
{
public:

	enum class STATE
	{
		NONE,
		RUN,
		JUMP,
		DAMAGE,
		DOWN,
		SHOT,
		IDLE,
		BOOST,
		BOOST_DASH,
		FALL,
		COMBAT,
		COMBAT_RUN,
		WIN,
		LOSE,

	};

	enum class ACTOR_TYPE
	{
		PLAYER_1,
		PLAYER_2,
		BOSS,
		NON
	};


	ActorBase();

	//モデルの全身のアニメーションの再生を行う
	virtual void PlayAnim(int state, bool priority, bool isLoop, bool isStop = false, float endStep = -1.0f, float startStep = 0.0f);

	//モデルの上半身のアニメーションの再生を行う
	virtual void PlayUpperAnim(int state, bool priority, bool isLoop, bool isStop = false, float endStep = -1.0f, float startStep = 0.0f);

	//モデルの下半身のアニメーションの再生を行う
	virtual void PlayLowerAnim(int state, bool priority, bool isLoop, bool isStop = false, float endStep = -1.0f, float startStep = 0.0f);

	//アニメーション再生が終わっていたら
	const bool IsAnimEnded(void)const;

	//j自分のHPを取得
	const float& GetHP(void)const;

	//引数の数値文HPを減らす
	void Damage(float damage);

	//トランスフォーム
	const Transform& GetTransform(void)const;

	//カプセルを取得
	CollisionCapsule& GetCapsule(void)const;

	virtual const STATE& GetState(void);

	//ビームライフルの弾数
	const int& GetNumnberOfBullets(void);

	//ブーストゲージ
	const float& GetBoostFuel(void) const;

	//敵との距離
	const float& GetEnemyDistance(void) const;

	//勝利したかどうかを判定
	const bool& IsWin(void) const;

	//敵の座標を設定
	 void SetEnemyPosition(const VECTOR* enemyPos);

	 //敵のHPを設定
	 void SetEnemyHp(const float* enemyHp);

	 //敵のステートを設定
	 void SetEnemyState(const STATE* enemyState);

	 //アクタータイプを取得
	 const ACTOR_TYPE& GetActorType(void);

protected:

	// シングルトン参照
	ResourceManager& resMng_;

	//トランスフォーム
	Transform transform_;

	//アニメーション管理
	std::unique_ptr<RobotAnimeController> robotAnimeController_;

	//カプセル状の当たり判定用
	std::unique_ptr<CollisionCapsule> capsule_;

	//敵の座標
	const VECTOR *enemyPos_;

	//敵HP
	const float* enemyHp_;

	//敵のステート情報
	const STATE* enemyState_;

	//デルタタイム
	float deltaTime_;

	//自分HP
	float hp_;

	//ブースト残量
	float boostFuel_;

	//敵との距離
	float enemyDistance_;

	//敵との距離
	int numberofBullets_;

	//勝利したかどうか判定
	bool isWin_;

	//ステート
	STATE actorState_;

	//
	ACTOR_TYPE actorType_;

	//アニメーションの設定
	virtual void InitAnimation(void);

};
