#include "MoveState.h"
#include"../../Actor/Player/Player.h"
#include"../../../Utility/AsoUtility.h"
#include "IdleState.h"
#include "JumpState.h"
#include "BoostState.h"

MoveState::MoveState(Player& player):player_(player)
{
	//�X�e�[�g��RUN��
	player_.actorState_ = Player::STATE::RUN;
	//�A�j���[�V������RUN�ɂ���
	player_.PlayAnim(static_cast<int>(Player::STATE::RUN), false, true);
	//�}�b�N�X�X�s�[�h��ݒ�
	player_.SetMaxMoveSpeed(player_.MOVE_SPEED);
}

void MoveState::Update(void)
{
	//�X�e�B�b�N�̓|��Ă���������擾
	player_.GetMoveDir();
	//�A�j���[�V������RUN��Ԃ�
	player_.PlayAnim(static_cast<int>(Player::STATE::RUN), false, true);
	//�ߐڍU�����I����Ă��������
	if (player_.CombatStunEnded())
	{
		//�ړ�����
		player_.Move();
		//�ˌ�����
		player_.Shot();

		//�X�e�B�b�N���͂��Ȃ���΃A�C�h����ԂɈڍs
		if (!(player_.GetInput().isStickTilted(Input::STICK_LR::L)))
		{
			//�A�j���[�V������IDLE�ɂ���
			player_.PlayAnim(static_cast<int>(Player::STATE::IDLE), false, true);
			//�A�C�h����ԂɈڍs
			player_.ChangeState(std::make_unique<IdleState>(player_));
			return;
		}
		//�ߐڍU���{�^����������Ă�����
		else if (player_.GetInput().IsTriggerd("combat"))
		{
			//�A�^�b�N��ԂɈڍs
			player_.Combat();
			return;
		}
		//�W�����v�{�^������񉟂���Ă��āA�u�[�X�g�Q�[�W�����ȏ゠������
		else if (player_.GetInput().IsDoublePrassed("jump")&& player_.IsBoostGaugeSufficient(player_.MIN_BOOST))
		{
			//�u�[�X�g��ԂɈڍs
			player_.ChangeState(std::make_unique<BoostState>(player_));
			return;
		}
		//�W�����v�{�^����������Ă��āA�u�[�X�g�Q�[�W�����ȏ゠������
		else if (player_.GetInput().IsHold("jump") && player_.IsBoostGaugeSufficient(player_.MIN_JUMP_BOOST))
		{
			//�A�j���[�V������JUMP�ɂ���
			player_.PlayAnim(static_cast<int>(Player::STATE::JUMP), false, false, true);
			//�W�����v��ԂɈڍs
			player_.ChangeState(std::make_unique<JumpState>(player_));
			return;
		}
		else
		{
		}
	}
}


