#include "CombatState.h"
#include "IdleState.h"
#include "FallState.h"
#include"../../Actor/Player/Player.h"

//�i���d������
static constexpr float COMBAT_STAN_TIME = 20.0f;

CombatState::CombatState(Player& player):player_(player)
{
	//�X�e�[�g��COMBAT�ɂ���
	player_.pState_ = Player::STATE::COMBAT;
	//�A�j���[�V������COMBAT�ɂ���
	player_.PlayAnim(static_cast<int>(Player::STATE::COMBAT), true, true, false,-1.0f,54.0f);
	//�ړ��ʂ�0�ɂ���
	player_.MoveStop();
	//�r�[���T�[�x����L����
	player_.GetBeamSaber().GetSaber().Activate();
	//�ō�����ݒ�
	player_.SetMaxMoveSpeed(player_.MOVE_SPEED);
	//�ړ��ʂ�0�ɂ���
	player_.SetMoveSpeed(0.0f);

}

void CombatState::Update()
{
	//�A�j���[�V�����Đ����I����Ă�����
	if (player_.IsAnimEnded())
	{
		//�A�j���[�V������IDLE�ɂ���
		player_.PlayAnim(static_cast<int>(Player::STATE::IDLE), false, true);
		//�r�[���T�[�x�����L����
		player_.GetBeamSaber().GetSaber().InActivate();
		player_.GetBeamSaber().InActivate();
		//�r�[�����C�t����L����
		player_.GetBeamRifle().Activate();
		//�d�����Ԃ�ݒ�
		player_.SetCombatStan(COMBAT_STAN_TIME);
		//�A�C�h����ԂɈڍs
		player_.ChangeState(std::make_unique<IdleState>(player_));
		return;
	}

}
