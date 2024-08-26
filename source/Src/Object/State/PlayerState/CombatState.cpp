#include "CombatState.h"
#include "IdleState.h"
#include "FallState.h"
#include"../../Actor/Player/Player.h"


CombatState::CombatState(Player& player):player_(player)
{
	player_.pState_ = Player::STATE::COMBAT1;
	player_.PlayAnim(static_cast<int>(Player::STATE::COMBAT1), true, true, false,-1.0f,54.0f);
	player_.MoveStop();
	player_.GetBeamSaber().GetSaber().Activate();
	player_.SetMaxMoveSpeed(player_.MOVE_SPEED);
	player_.SetMoveSpeed(0.0f);

}

void CombatState::Update()
{
	if (player_.IsAnimEnded())
	{
		player_.PlayAnim(static_cast<int>(Player::STATE::IDLE), false, true);
		player_.GetBeamSaber().GetSaber().InActivate();
		player_.GetBeamSaber().InActivate();
		player_.GetBeamRifle().Activate();
		player_.SetCombatStan(COMBAT_STAN_TIME);
		player_.ChangeState(std::make_unique<IdleState>(player_));
		return;
	}

}
