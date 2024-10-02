#include "WinnerState.h"
#include"../../Actor/Player/Player.h"

WinnerState::WinnerState(Player& player):player_(player)
{
	//アニメーションを勝利ポーズに設定
	player_.PlayAnim(static_cast<int>(Player::STATE::WIN), true, false);
	//ジャンプ力を消す
	player_.JumpStop();
	//移動量を消す
	player_.MoveStop();

}

void WinnerState::Update()
{
	player_.PlayAnim(static_cast<int>(Player::STATE::WIN), true, false);
}
