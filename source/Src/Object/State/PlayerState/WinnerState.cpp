#include "WinnerState.h"
#include"../../Actor/Player/Player.h"

WinnerState::WinnerState(Player& player):player_(player)
{
	player_.PlayAnim(static_cast<int>(Player::STATE::WIN), true, false);
	player_.JumpStop();
	player_.MoveStop();

}

void WinnerState::Update()
{
	player_.PlayAnim(static_cast<int>(Player::STATE::WIN), true, false);

}
