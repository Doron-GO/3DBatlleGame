#include "LoseState.h"
#include"../../Actor/Player/Player.h"
LoseState::LoseState(Player& player):player_(player)
{
	player_.PlayAnim(static_cast<int>(Player::STATE::DOWN), true, false);
	player_.PlayEffect(Player::STATE::LOSE);
	player_.JumpStop();
	player_.MoveStop();
}

void LoseState::Update()
{
	player_.PlayAnim(static_cast<int>(Player::STATE::DOWN), false, false);

}
