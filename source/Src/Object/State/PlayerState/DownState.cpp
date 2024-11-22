#include "DownState.h"
#include "IdleState.h"
#include"../../Actor/Player/Player.h"

DownState::DownState(Player& player):player_(player)
{
	//�X�e�[�g��DOWN��Ԃ�
	player_.actorState_ = Player::STATE::DOWN;
	//�A�j���[�V������DOW�ɂ���
	player_.PlayAnim(static_cast<int>(Player::STATE::DOWN), true, false,false,-1.0f,0.01f);
	//�W�����v�͂�0�ɂ���
	player_.JumpStop();
	//�ړ���0���ɂ���
	player_.MoveStop();
	//���G���Ԃ�ݒ�
	player_.SetSafeTime(Player::BIG_SAFE_TIME);
	//�r�[�T�[�x�����L����
	player_.GetBeamSaber().GetSaber().InActivate();
	player_.GetBeamSaber().InActivate();
	//�r�[�����C�t�����L����
	player_.GetBeamRifle().Activate();
	player_.StopEffect(Player::EFFECT_TYPE::JET_BACK_LEFT);
	player_.StopEffect(Player::EFFECT_TYPE::JET_BACK_RIGHT);

}

void DownState::Update(void)
{
	//�A�j���[�V�����Đ����I����Ă�����
	if (player_.IsAnimEnded())
	{
		//��Ԃ�IDLE�ɂ���
		player_.actorState_ = Player::STATE::IDLE;
		//�A�j���[�V������IDLE�ɂ���
		player_.PlayAnim(static_cast<int>(Player::STATE::IDLE), false, true);
		//�A�C�h����ԂɈڍs
		player_.ChangeState(std::make_unique<IdleState>(player_));
		return;
	}

}
