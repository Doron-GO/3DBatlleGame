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
	//���f���̓ǂݍ���
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::BEAMRIFLE));
	//���W
	transform_.pos = MV1GetFramePosition(player_.GetTransform().modelId, ATTACH_RIGHT_HAND_FRAME);
	//�傫��
	transform_.scl = SCALE;
	//��]
	transform_.quaRotLocal =
		Quaternion::Euler(AsoUtility::Deg2RadF(-90.0f), AsoUtility::Deg2RadF(90.0f), AsoUtility::Deg2RadF(180.0f));
	transform_.Update();
	
	//�L����
	activeFlag_ = true;

	//�c�e�����ő��
	numberofBullets_ = MAX_BULLETS;

	for (int beam = 0; beam < numberofBullets_; beam++)
	{
		beams_.emplace_back(std::make_unique<BeamShot>(playerType, player.IsHorming(),beam, playMode));
	}

	//�f���^�^�C���̏�����
	deltaTime_ = 0.0f;

	//�N�[���^�C���̏�����
	coolTime_ = RELOAD_COUNT_TIME;
}

void BeamRifle::Update(void)
{
	//�f���^�^�C���̍X�V
	deltaTime_ = DeltaTime::GetInstsnce().GetDeltaTime();
	
	//���W�A��]�̓���
	SyncPosition();
	
	//�e�̍X�V
	for (auto& beam : beams_)
	{
		beam->Update(player_.GetEnemyPos());
	}

	//�N�[���^�C���̍X�V
	CoolTimeCount();
}

void BeamRifle::Draw(void)
{
	//�L����ԂȂ烉�C�t����`��
	if (activeFlag_)
	{
		MV1DrawModel(transform_.modelId);
	}
}

void BeamRifle::Trigger(void)
{
	for (auto& beam : beams_)
	{
		//�e���L����ԂłȂ��A�c�e���c���Ă����甭�˂���
		if (beam->IsActive()||!(numberofBullets_>0))
		{
			continue;
		}		
		//�N�[���^�C����ݒ�
		coolTime_ = 0.0f;
		
		//�c�e��1���炷
		numberofBullets_--;

		//���W��ݒ�
		beam->SetPos(transform_.pos);
		
		//�e��L����
		beam->Activate();
		break;
	}
}

void BeamRifle::InActivateHorming(void)
{
	for (auto& beam : beams_)
	{
		//�L����ԂłȂ����
		if (!beam->IsActive())
		{
			continue;
		}
		//�z�[�~���O�����ŗL����
		beam->InActivateHorming();
	}
}

const std::vector<std::unique_ptr<BeamShot>>& BeamRifle::GetBeams() const
{
	//�r�[���̔z���Ԃ�
	return beams_;
}

const int& BeamRifle::GetNumnberOfBullets(void)
{
	//�c�e����Ԃ�
	return numberofBullets_;
}

void BeamRifle::NumnberOfBullets(void)
{
	//�c�e���𐔂���
	int bullets = MAX_BULLETS;
	for (auto& beam : beams_)
	{
		//�L����ԂȂ�1���炷
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

