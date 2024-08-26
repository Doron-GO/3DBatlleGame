#include "JumpState.h"
#include"../../Actor/Player/Player.h"
#include"../../../Utility/AsoUtility.h"
#include"FallState.h"
#include"MoveState.h"
JumpState::JumpState(Player& player):player_(player)
{
	player_.pState_ = Player::STATE::JUMP;
	player_.SetMaxMoveSpeed(player_.MAX_JUMP_MOVE_SPEED);
	player_.SetGravityPow(0.0f);
	player_.ConsumeBoostGauge(Player::JUMP_FAST_RATE);
}

void JumpState::Update()
{
	player_.GetMoveDir();
	player_.Shot();
	player_.Move();
	if (player_.GetInput().IsPrassed("jump") && player_.IsBoostGaugeSufficient(player_.MIN_JUMP_BOOST))
	{
		player_.Jump();
	}
	else 
	{
		player_.ChangeState(std::make_unique<FallState>(player_));
		return;
	}	
	if (player_.GetInput().IsTriggerd("combat"))
	{
		player_.pState_ = Player::STATE::COMBAT_RUN;
		player_.Combat();
		return;
	}
}

