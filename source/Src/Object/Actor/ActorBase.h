#pragma once
#include"../Collision/CollisionCapsule.h"

#include<memory>

class RobotAnimeController;
class ResourceManager;

class ActorBase
{
public:

	ActorBase();

	//モデルの全身のアニメーションの再生を行う
	virtual void PlayAnim(int state, bool priority, bool isLoop, bool isStop = false, float endStep = -1.0f, float startStep = 0.0f);

	//モデルの上半身のアニメーションの再生を行う
	virtual void PlayUpperAnim(int state, bool priority, bool isLoop, bool isStop = false, float endStep = -1.0f, float startStep = 0.0f);

	//モデルの下半身のアニメーションの再生を行う
	virtual void PlayLowerAnim(int state, bool priority, bool isLoop, bool isStop = false, float endStep = -1.0f, float startStep = 0.0f);

	const bool IsAnimEnded(void)const;

	const float& GetDeltaTime(void)const;

	const float& GetPlayerHP(void)const;

	//引数の数値文HPを減らす
	void Damage(float damage);

	CollisionCapsule& GetCapsule(void)const;


protected:

	// シングルトン参照
	ResourceManager& resMng_;

	//アニメーション管理
	std::unique_ptr<RobotAnimeController> roboAnimeController_;

	//カプセル状の当たり判定用
	std::unique_ptr<CollisionCapsule> capsule_;


	virtual void InitAnimation(void);

	//デルタタイム
	float deltaTime_;

	//プレイヤーHP
	float playerHp_;

};

