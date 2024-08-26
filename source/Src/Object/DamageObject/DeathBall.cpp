#include "DeathBall.h"
#include"../Time/DeltaTime.h"

DeathBall::DeathBall(int playerType, const VECTOR& pos) :DamageObject(playerType), playerPos_(pos)
{
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::BEAM_COLLISION));
	playerNum_ = playerType;
	transform_.scl = { 150.0f,150.0f,150.0f };
	transform_.pos = { 0.0f,0.0f,0.0f };
	transform_.quaRot = Quaternion();
	activeFlag_ = false;
	damage_ = 500.0f;
	_update = &DeathBall::WaitUpdate;
	effectManager_ = std::make_unique<EffectManager>(transform_);
	effectManager_->Add(static_cast<int>(BALL_STATE::BALL), { 6000.0f,6000.0f,6000.0f }, true, resMng_.Load(ResourceManager::SRC::DEATH_BALL).handleId_);
}

DeathBall::~DeathBall()
{

}

void DeathBall::Draw(void)
{
}

void DeathBall::Update(VECTOR enemyPos)
{
	(this->*_update)();
	effectManager_->Update();
	transform_.Update();
}

void DeathBall::Activate(void)
{
	transform_.pos = { playerPos_.x,POS_DETHBALL_Y,playerPos_.z };
	activeFlag_ = true;
	MV1RefreshCollInfo(transform_.modelId);
	effectManager_->Play(static_cast<int>(BALL_STATE::BALL));
	_update = &DeathBall::NormalUpdate;
}

void DeathBall::InActivate(void)
{
	activeFlag_ = false;
	MV1RefreshCollInfo(transform_.modelId);
	effectManager_->Stop(static_cast<int>(BALL_STATE::BALL));
	_update = &DeathBall::WaitUpdate;

}

const int& DeathBall::GetModelId(void) const
{
	return transform_.modelId;
}

void DeathBall::WaitUpdate(void)
{
}

void DeathBall::NormalUpdate(void)
{
	MV1RefreshCollInfo(transform_.modelId);
	float deltatime = DeltaTime::GetInstsnce().GetDeltaTime();
	transform_.pos.y -= 500.0f * deltatime;
	if (transform_.pos.y<=-300.0f)
	{
		InActivate();
	}
}
