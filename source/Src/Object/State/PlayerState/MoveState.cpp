#include "MoveState.h"
#include"../../Actor/Player/Player.h"
#include"../../../Utility/AsoUtility.h"
#include "IdleState.h"
#include "JumpState.h"
#include "BoostState.h"

MoveState::MoveState(Player& player):player_(player)
{
	player_.pState_ = Player::STATE::RUN;
	player_.PlayAnim(static_cast<int>(Player::STATE::RUN), false, true);
	player_.SetMaxMoveSpeed(player_.MOVE_SPEED);
}

void MoveState::Update()
{
	player_.GetMoveDir();
	player_.PlayAnim(static_cast<int>(Player::STATE::RUN), false, true);
	if (player_.CombatStunEnded())
	{
		player_.Move();
		player_.Shot();
		if (!(player_.GetInput().isStickTilted(Input::STICK_LR::L)))
		{
			player_.PlayAnim(static_cast<int>(Player::STATE::IDLE), false, true);
			player_.ChangeState(std::make_unique<IdleState>(player_));
			return;
		}
		else if (player_.GetInput().IsTriggerd("combat"))
		{
			player_.Combat();
			return;
		}
		else if (player_.GetInput().IsDoublePrassed("jump")&& player_.IsBoostGaugeSufficient(player_.MIN_BOOST))
		{
			player_.ChangeState(std::make_unique<BoostState>(player_));
			return;
		}
		else if (player_.GetInput().IsHold("jump") && player_.IsBoostGaugeSufficient(player_.MIN_JUMP_BOOST))
		{
			player_.PlayAnim(static_cast<int>(Player::STATE::JUMP), false, false, true);
			player_.ChangeState(std::make_unique<JumpState>(player_));
			return;
		}
		else
		{

		}
	}
}


