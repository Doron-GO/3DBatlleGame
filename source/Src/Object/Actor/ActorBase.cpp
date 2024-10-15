#include"../../Manager/RobotAnimeController.h"
#include"../../Manager/ResourceManager.h"
#include "ActorBase.h"

ActorBase::ActorBase() :resMng_(ResourceManager::GetInstance())
{
	actorState_ = STATE::IDLE;
	actorType_ = ACTOR_TYPE::NON;
	boostFuel_ = 0.0f;
	deltaTime_ = 0.0f;
	hp_ = 0.0f;
	numberofBullets_ = 0;
	enemyDistance_ = 0.0f;
	enemyPos_ = nullptr;
	enemyHp_ = nullptr;
	enemyState_ = nullptr;
	isWin_ = false;
}

void ActorBase::PlayAnim(int state, bool priority, bool isLoop, bool isStop, float endStep, float startStep)
{
	robotAnimeController_->UpperBodyPlay(state, priority, isLoop, isStop, endStep, startStep);
	robotAnimeController_->LowerBodyPlay(state, priority, isLoop, isStop, endStep, startStep);
}

void ActorBase::PlayUpperAnim(int state, bool priority, bool isLoop, bool isStop, float endStep, float startStep)
{
	robotAnimeController_->UpperBodyPlay(state, priority, isLoop, isStop, endStep, startStep);
}

void ActorBase::PlayLowerAnim(int state, bool priority, bool isLoop, bool isStop, float endStep, float startStep)
{
	robotAnimeController_->LowerBodyPlay(state, priority, isLoop, isStop, endStep, startStep);
}

const bool ActorBase::IsAnimEnded(void) const
{
	return robotAnimeController_->IsEnd();
}

const float& ActorBase::GetHP(void) const
{
	return hp_;
}

void ActorBase::Damage(float damage)
{
	hp_ -= damage;
}

const Transform& ActorBase::GetTransform(void) const
{
	return transform_;
}

CollisionCapsule& ActorBase::GetCapsule(void) const
{
	return *capsule_;
}

const ActorBase::STATE& ActorBase::GetState(void)
{
	return actorState_;
}

const int& ActorBase::GetNumnberOfBullets(void)
{
	return numberofBullets_;
}

const float& ActorBase::GetBoostFuel(void) const
{
	return boostFuel_;
}

const float& ActorBase::GetEnemyDistance(void) const
{
	return enemyDistance_;
}

const bool& ActorBase::IsWin(void) const
{
	return isWin_;
}

void ActorBase::SetEnemyPosition(const VECTOR* enemyPos)
{
	enemyPos_ = enemyPos;
}

void ActorBase::SetEnemyHp(const float* enemyHp)
{
	enemyHp_ = enemyHp;
}

void ActorBase::SetEnemyState(const STATE* enemyState)
{
	enemyState_ = enemyState;
}

const ActorBase::ACTOR_TYPE& ActorBase::GetActorType(void)
{
	return actorType_;
}

void ActorBase::InitAnimation(void)
{

}
