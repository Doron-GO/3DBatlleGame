#include"../../Manager/RobotAnimeController.h"
#include"../../Manager/ResourceManager.h"
#include "ActorBase.h"

ActorBase::ActorBase():resMng_(ResourceManager::GetInstance())
{
}

void ActorBase::PlayAnim(int state, bool priority, bool isLoop, bool isStop, float endStep, float startStep)
{
	roboAnimeController_->UpperBodyPlay(state, priority, isLoop, isStop, endStep, startStep);
	roboAnimeController_->LowerBodyPlay(state, priority, isLoop, isStop, endStep, startStep);
}

void ActorBase::PlayUpperAnim(int state, bool priority, bool isLoop, bool isStop, float endStep, float startStep)
{
	roboAnimeController_->UpperBodyPlay(state, priority, isLoop, isStop, endStep, startStep);
}

void ActorBase::PlayLowerAnim(int state, bool priority, bool isLoop, bool isStop, float endStep, float startStep)
{
	roboAnimeController_->LowerBodyPlay(state, priority, isLoop, isStop, endStep, startStep);
}

const bool ActorBase::IsAnimEnded(void) const
{
	return roboAnimeController_->IsEnd();
}

const float& ActorBase::GetDeltaTime(void) const
{
	return deltaTime_;
}

const float& ActorBase::GetPlayerHP(void) const
{
	return playerHp_;
}

void ActorBase::Damage(float damage)
{
	playerHp_ -= damage;
}

CollisionCapsule& ActorBase::GetCapsule(void) const
{
	return *capsule_;
}

void ActorBase::InitAnimation(void)
{

}
