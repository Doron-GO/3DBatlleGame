#include "BeamRifle.h"
#include<DxLib.h>
#include"../../Utility/AsoUtility.h"
#include"../../Object/Actor/Player/Player.h"
#include"../Time/DeltaTime.h"

#pragma region Parameter
//�r�[�������ˉ\�ɂȂ�܂ł̎���
 constexpr float RELOAD_COUNT_TIME = 100.0f;

//�r�[���̍Ĕ��ˉ\���Ԃ��v������f���^�^�C���ɂ�����萔
 constexpr float RELOAD_COUNT_RATE = 60.0f;

//���e��
 constexpr int MAX_BULLETS = 5;

//�r�[�����C�t���̑傫��
 constexpr VECTOR SCALE = { 0.07f,0.07f,0.07f };

#pragma endregion


BeamRifle::BeamRifle(int playerType, int playMode, Player& player):player_(player),WeaponBase(playerType, player.GetTransform())

{
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::BEAMRIFLE));

	transform_.pos = MV1GetFramePosition(player_.GetTransform().modelId, ATTACH_RIGHT_HAND_FRAME);
	transform_.scl = SCALE;
	transform_.quaRotLocal =
		Quaternion::Euler(AsoUtility::Deg2RadF(-90.0f), AsoUtility::Deg2RadF(90.0f), AsoUtility::Deg2RadF(180.0f));
	transform_.Update();

	MATRIX mat = MGetIdent();
	Quaternion quaLocal =transform_.quaRotLocal;
	mat = MMult(quaLocal.ToMatrix(),mat); 
	MV1SetMatrix(transform_.modelId, MMult(transform_.matScl, mat));
	activeFlag_ = true;
	numberofBullets_ = MAX_BULLETS;
	for (int beam = 0; beam < numberofBullets_; beam++)
	{
		beams_.emplace_back(std::make_unique<BeamShot>(playerType, player.IsHorming(),beam, playMode));
	}
	deltaTime_ = 0.0f;
	coolTime_ = RELOAD_COUNT_TIME;
}

void BeamRifle::Update(void)
{
	deltaTime_ = DeltaTime::GetInstsnce().GetDeltaTime();
	SyncPosition();
	for (auto& beam : beams_)
	{
		beam->Update(player_.GetEnemyPos());
	}
	CoolTimeCount();
}

void BeamRifle::Draw(void)
{
	if (activeFlag_)
	{
		MV1DrawModel(transform_.modelId);
	}
	for (auto& beam : beams_)
	{
		beam->Draw();
	}
}

void BeamRifle::Trigger(void)
{
	for (auto& beam : beams_)
	{
		if (beam->IsActive()||!(numberofBullets_>0))
		{
			continue;
		}
		coolTime_ = 0.0f;
		numberofBullets_--;
		beam->SetPos(transform_.pos);
		beam->Activate();
		break;
	}
}

void BeamRifle::InActivateHorming(void)
{
	for (auto& beam : beams_)
	{
		if (!beam->IsActive())
		{
			continue;
		}
		beam->InActivateHorming();
	}
}

const std::vector<std::unique_ptr<BeamShot>>& BeamRifle::GetBeams() const
{
	return beams_;
}

const int& BeamRifle::GetNumnberOfBullets(void)
{
	return numberofBullets_;
}

void BeamRifle::NumnberOfBullets(void)
{

	int bullets = 5;
	for (auto& beam : beams_)
	{
		if (beam->IsActive() )
		{
			bullets--;
			continue;
		}
	}
	numberofBullets_ = bullets ;
}

void BeamRifle::CoolTimeCount(void)
{
	//�N�[���^�C�����K��𒴂��ĂȂ���Ή��Z����
	if (!(coolTime_ > RELOAD_COUNT_TIME))
	{
		coolTime_ += deltaTime_ * RELOAD_COUNT_RATE;
		//���𒴂�����ő�̒l�ɌŒ肷��
		if (coolTime_ > RELOAD_COUNT_TIME)
		{
			coolTime_ = RELOAD_COUNT_TIME;
		}
	}
	//�N�[���^�C�����K��𒴂��Ă���΍ő�̒l�ɌŒ肷��
	else
	{
		coolTime_ = RELOAD_COUNT_TIME;
	}
	Reload();
}

void BeamRifle::Reload(void)
{
	//�����[�h�J�n���Ԃ܂œ��B������ꔭ���U���ăJ�E���g��0�ɖ߂�
	if (coolTime_ == RELOAD_COUNT_TIME)
	{
		numberofBullets_++;
		coolTime_ = 0.0f;
		//�e��5�ȏ�ɂȂ�����5�ɌŒ肷��
		if (numberofBullets_>= MAX_BULLETS)
		{
			numberofBullets_ = MAX_BULLETS;
		}
	}
}

