#include "DamageState.h"
#include"../../Actor/Player/Player.h"
#include "IdleState.h"

DamageState::DamageState(Player& player) :player_(player)
{
	//�_���[�W�A�j���[�V�������Đ�
	player_.PlayAnim(static_cast<int>(Player::STATE::DAMAGE), true, true);
	//���݂̃X�e�[�g����ۑ�
	player_.actorState_ = Player::STATE::DAMAGE;
	//�O�㍶�E�ړ����X�g�b�v
	player_.MoveStop();
	//�㉺�̈ړ����X�g�b�v
	player_.JumpStop();
	count_ = 0.0f;
	//���G���Ԃ�ݒ�
	player_.SetSafeTime(Player::SMALL_SAFE_TIME);
	//�r�[���T�[�x���̓����蔻����A�N�e�B�u�ɂ���
	player_.GetBeamSaber().GetSaber().InActivate();
	//�r�[���T�[�x���̕`����A�N�e�B�u��Ԃɂ���
	player_.GetBeamSaber().InActivate();
	//�r�[�����C�t�����A�N�e�B�u��Ԃɂ���
	player_.GetBeamRifle().Activate();
	player_.StopEffect(Player::EFFECT_TYPE::JET_BACK_LEFT);
	player_.StopEffect(Player::EFFECT_TYPE::JET_BACK_RIGHT);

}

void DamageState::Update(void)
{
	//���ݍĐ����A�j���[�V�������ꃋ�[�v�Đ�����������
	if (player_.IsAnimEnded())
	{
		player_.actorState_ = Player::STATE::IDLE;
		//�A�C�h���A�j���[�V�������Đ�
		player_.PlayAnim(static_cast<int>(Player::STATE::IDLE), false, true);
		//�A�C�h����ԂɈڍs
		player_.ChangeState(std::make_unique<IdleState>(player_));
		return;
	}

}
