#include<DxLib.h>
#include<string>
#include"../../Utility/AsoUtility.h"
#include"../Weapon/BeamRifle.h"
#include"../Time/DeltaTime.h"
#include "BeamShot.h"


BeamShot::BeamShot(int playerType, const bool& isHorming, int num, int playMode) :DamageObject(playerType),isHorming_(isHorming)
{
	testShotNUm_ = num;
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::BEAM_COLLISION));
	playerNum_=playerType;
	transform_.scl = { 1.0f,1.0f,1.0f };
	transform_.pos = { 0.0f,0.0f,0.0f };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	activeFlag_ = false;
	_update = &BeamShot::WaitUpdate;

	enemyPos_ = { 0.0f,0.0f,0.0f };
	hormingVec_ = { 0.0f,0.0f,0.0f };
	effectManager_ = std::make_unique<EffectManager>(transform_);
	effectManager_->Add(static_cast<int>(BEAM_STATE::BEAM), transform_.scl,true,resMng_.Load(ResourceManager::SRC::BEAM).handleId_);
	effectManager_->Add(static_cast<int>(BEAM_STATE::HIT), HIT_EFFECT_SCARE,false,resMng_.Load(ResourceManager::SRC::HIT).handleId_);
	damage_ = 50.0f;
	bigDamage_ = 200.0f;
	coolTime_ = 240.0f;

	if (static_cast<PLAY_MODE>(playMode) == PLAY_MODE::SINGLE_MODE)
	{
		offsetEnemy_ = OFFSET_BOSS_ENEMY;
	}
	else
	{
		offsetEnemy_ = OFFSET_ENEMY;

	}
}

void BeamShot::Draw(void)
{	
	std::string str;
	if (activeFlag_)
	{
		str = "true";
	}
	else
	{
		str = "false";
	}
}

void BeamShot::Update(VECTOR enemyPos)
{
	deltaTime_ = DeltaTime::GetInstsnce().GetDeltaTime();
	enemyPos_ = enemyPos;
	(this->*_update)();
	effectManager_->Update();
	transform_.Update();
	CoolTimeCount();
}

void BeamShot::Activate(void)
{
	activeFlag_ = true;
	MV1RefreshCollInfo(transform_.modelId);
	deathCount_ = DEATH_COUNT_TIME;
	effectManager_->Play(static_cast<int>(BEAM_STATE::BEAM));
	Horming();
	if (isHorming_)
	{
		_update = &BeamShot::HormingUpdate;
	}
	else
	{
		_update = &BeamShot::NormalUpdate;
	}
	transform_.Update();

}

void BeamShot::InActivate(void)
{
	activeFlag_ = false;
	effectManager_->Stop(static_cast<int>(BEAM_STATE::BEAM));
	hormingCount_ = 0.0f;
	_update = &BeamShot::WaitUpdate;
}

void BeamShot::Hit(void)
{
	activeFlag_ = false;
	deathCount_ = 0.0f;
	effectManager_->Play(static_cast<int>(BEAM_STATE::HIT));
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
	transform_.pos = VAdd(transform_.pos, VScale(transform_.GetForward(), SPEED * (deltatime * 100.0f)));
	MV1RefreshCollInfo(transform_.modelId);
	TimeToDeath();
}

void BeamShot::HormingUpdate(void)
{	
	float deltatime = DeltaTime::GetInstsnce().GetDeltaTime();
	Horming();
	transform_.pos = VAdd(transform_.pos, VScale(transform_.GetForward(), SPEED * (deltatime * 100.0f)));
	MV1RefreshCollInfo(transform_.modelId);
	TimeToDeath();
	hormingCount_ += deltatime * DEATH_COUNT_RATE;
	if (hormingCount_> HORMING_COUNT_TIME)
	{
		_update = &BeamShot::NormalUpdate;
	}
}

void BeamShot::TimeToDeath(void)
{
	float deltatime = DeltaTime::GetInstsnce().GetDeltaTime();
	deathCount_ -= deltatime* DEATH_COUNT_RATE;
	if (deathCount_<0)
	{
		deathCount_ = 0;
		InActivate();
	}
}

void BeamShot::Horming(void)
{
	VECTOR enemyPos = { enemyPos_.x,enemyPos_.y + offsetEnemy_,enemyPos_.z };
	hormingVec_ = VNorm(VSub(enemyPos, transform_.pos));
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
