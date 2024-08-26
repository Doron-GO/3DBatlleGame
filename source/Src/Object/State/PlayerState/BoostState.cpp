#include"BoostDashState.h"
#include "BoostState.h"
#include"FallState.h"
#include"JumpState.h"
#include"IdleState.h"
#include"MoveState.h"
#include"../../Actor/Player/Player.h"

BoostState::BoostState(Player& player):player_(player)
{
	time_ = 0.0f;
	player_.GetMoveDir();
	player_.SetMaxMoveSpeed(player_.BOOST_MOVE_SPEED);
	player_.SetMoveSpeed(player_.BOOST_MOVE_SPEED);
	player_.ResetShotFlame();
	player_.pState_ = Player::STATE::BOOST;
	player_.PlayAnim(static_cast<int>(Player::STATE::BOOST), false, true,true);
	player_.BoostQuaRot();
	player_.ConsumeBoostGauge(player_.BOOST_RATE);
	player_.RechargeBoostCountReset();
	player_.JumpStop();
	player_.GravityZero();
}

void BoostState::Update()
{
	//�u�[�X�g���Ԃ��I����Ă��邩����
	if (time_> MAX_BOOST_TIME)
	{
		//�W�����v�{�^���������Ă�����W�����v��ԂɈڍs
		if (player_.GetInput().IsPrassed("jump"))
		{
			player_.ChangeState(std::make_unique<BoostDashState>(player_));
			return;
		}
		//�X�e�B�b�N��G���Ă����烀�[�u��ԂɈڍs
		if (!(player_.GetInput().isStickTilted(Input::STICK_LR::L)))
		{
			player_.ChangeState(std::make_unique<MoveState>(player_));
			return;
		}
		//�X�e�B�b�N��G���ĂȂ���΃A�C�h����ԂɈڍs
		else
		{
			player_.ChangeState(std::make_unique<IdleState>(player_));
			return;
		}
	}
	//�u�[�X�g�s���֐�
	player_.Boost();
	//�㉺�̈ړ����~�߂�
	player_.JumpStop();
	//�ˌ�
	player_.Shot();
	//�u�[�X�g���ԉ��Z
	time_+=(player_.GetDeltaTime()*60.0f);
}
