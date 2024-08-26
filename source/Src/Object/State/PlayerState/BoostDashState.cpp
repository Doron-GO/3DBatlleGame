#include "BoostDashState.h"
#include"FallState.h"
#include"IdleState.h"
#include"MoveState.h"
#include"../../Actor/Player/Player.h"

BoostDashState::BoostDashState(Player& player) : player_(player)
{
	player_.pState_ = Player::STATE::BOOST_DASH;
	player_.PlayAnim(static_cast<int>(Player::STATE::BOOST), false, true, true);
	player_.SetMaxMoveSpeed(player_.BOOST_DASH_MOVE_SPEED);
	player_.SetMoveSpeed(player_.BOOST_DASH_MOVE_SPEED);
}

void BoostDashState::Update()
{
	player_.GetMoveDirBoostDash();

	float delta= player_.GetDeltaTime();
	player_.ConsumeBoostGauge(player_.BOOST_RATE * delta);
	if ( player_.IsBoostGaugeSufficient(player_.MIN_JUMP_BOOST)&&player_.GetInput().IsPrassed("jump") )
	{
		player_.StopRechargeBoost();
		player_.MoveBoodtDash();
	}
	else if((player_.GetInput().isStickTilted(Input::STICK_LR::L)))
	{
		player_.BoostDashEnd();
		player_.ChangeState(std::make_unique<MoveState>(player_));
		return;
	}
	else
	{
		player_.BoostDashEnd();
		player_.PlayAnim(static_cast<int>(Player::STATE::IDLE), false, true);
		player_.ChangeState(std::make_unique<IdleState>(player_));
		return;
	}
}
