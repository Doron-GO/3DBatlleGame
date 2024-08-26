#include "DownState.h"
#include "IdleState.h"
#include"../../Actor/Player/Player.h"

DownState::DownState(Player& player):player_(player)
{
	player_.pState_ = Player::STATE::DOWN;
	player_.PlayAnim(static_cast<int>(Player::STATE::DOWN), true, false,false,-1.0f,0.01f);
	player_.JumpStop();
	player_.MoveStop();
	player_.SetSafeTime(Player::BIG_SAFE_TIME);
	player_.GetBeamSaber().GetSaber().InActivate();
	player_.GetBeamSaber().InActivate();
	player_.GetBeamRifle().Activate();

}

void DownState::Update()
{
	if (player_.IsAnimEnded())
	{
		player_.pState_ = Player::STATE::IDLE;
		player_.PlayAnim(static_cast<int>(Player::STATE::IDLE), false, true);
		player_.ChangeState(std::make_unique<IdleState>(player_));
		return;
	}

}
