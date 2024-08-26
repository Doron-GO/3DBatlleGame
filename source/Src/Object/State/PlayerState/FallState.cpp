#include "FallState.h"
#include"IdleState.h"
#include"../../Actor/Player/Player.h"
#include"JumpState.h"
#include"BoostState.h"

FallState::FallState(Player& player):player_(player)
{
	player_.pState_ = Player::STATE::FALL;
	player_.PlayAnim(static_cast<int>(Player::STATE::FALL), false, true, true);
	player_.SetMaxMoveSpeed(player_.FALL_MAX_MOVE_SPEED);
}

void FallState::Update()
{
	//�n�ʂɒ��n������
	if (player_.IsGround())
	{	
		player_.PlayAnim(static_cast<int>(Player::STATE::IDLE), false, false);
		player_.ChangeState(std::make_unique<IdleState>(player_));
		return;
	}
	if (player_.GetInput().IsTriggerd("combat"))
	{
		player_.Combat();
		return;
	}
	//�W�����v�{�^������񑁂������ꂽ���A�u�[�X�g�Q�[�W��20�ȏ゠������
	if (player_.GetInput().IsDoublePrassed("jump") && player_.IsBoostGaugeSufficient(player_.MIN_BOOST))
	{
		player_.ChangeState(std::make_unique<BoostState>(player_));
		return;
	}
	//�W�����v�{�^����������Ă��邩�A�u�[�X�g�Q�[�W���P�ȏ゠������
	else if (player_.GetInput().IsHold("jump") && player_.IsBoostGaugeSufficient(player_.MIN_JUMP_BOOST))
	{
		player_.PlayAnim(static_cast<int>(Player::STATE::JUMP), false, false, true);
		player_.ChangeState(std::make_unique<JumpState>(player_));
		return;
	}
	else
	{
	}
	player_.JumpPowZero();
	player_.PlayAnim(static_cast<int>(Player::STATE::JUMP), false, false);
	player_.Move();
	player_.Shot();
	player_.CalcGravity();

}
