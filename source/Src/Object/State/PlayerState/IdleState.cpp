#include "IdleState.h"
#include "MoveState.h"
#include"../../Actor/Player/Player.h"
#include"JumpState.h"
#include"BoostState.h"

IdleState::IdleState(Player& player):player_(player)
{
	//�X�e�[�g��IDLE��Ԃɂ���
	player_.actorState_ = Player::STATE::IDLE;
	//�W�����v�͂��ɂ���
	player_.JumpStop();

}

void IdleState::Update(void)
{
	//���n�d�����I�����Ă��āA�i���U���d�����I�����Ă�����
	if (player_.LandingStunEnded()&&player_.CombatStunEnded())
	{
		//�ړ��֐�
		player_.Move();
		//�ˌ��֐�
		player_.Shot();

		//�X�e�B�b�N��G���Ă���΃��[�u��ԂɈڍs
		if (player_.GetInput().isStickTilted(Input::STICK_LR::L))
		{
			player_.ChangeState(std::make_unique<MoveState>(player_));
			return;
		}
		//�i���{�^���������Ă�����i����ԂɈڍs
		else if (player_.GetInput().IsTriggerd("combat"))
		{
			player_.Combat();
			return;
		}
		//�u�[�X�g�{�^���������Ă��āA�u�[�X�g�Q�[�W�����ȏ゠��΃u�[�X�g��ԂɈڍs
		else if (player_.GetInput().IsDoublePrassed("jump") && player_.IsBoostGaugeSufficient(player_.MIN_BOOST))
		{
			player_.ChangeState(std::make_unique<BoostState>(player_));
			return;
		}
		//�W�����v�{�^���������Ă��āA�u�[�X�g�Q�[�W�����ȏ゠��΃W�����v��ԂɈڍs
		else if (player_.GetInput().IsHold("jump") && player_.IsBoostGaugeSufficient(player_.MIN_JUMP_BOOST))
		{
			player_.PlayAnim(static_cast<int>(Player::STATE::JUMP), true, false, true);
			player_.ChangeState(std::make_unique<JumpState>(player_));
			return;
		}
		else
		{
		}
	}
	//�X�e�B�b�N�̌����Ă���������擾
	player_.GetMoveDir();
	//�d�͂����Z
	player_.CalcGravity();
	//�㉺���g�Ƃ��A�C�h���A�j���[�V�������Đ�
	player_.PlayAnim(static_cast<int>(Player::STATE::IDLE), false, true);
}

