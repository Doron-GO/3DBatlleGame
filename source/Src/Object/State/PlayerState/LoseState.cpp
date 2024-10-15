#include "LoseState.h"
#include"../../Actor/Player/Player.h"
LoseState::LoseState(Player& player):player_(player)
{
	//アニメーションをDOWNに設定
	player_.PlayAnim(static_cast<int>(Player::STATE::DOWN), true, false);
	//爆発エフェクトを起動
	player_.PlayEffect(Player::STATE::LOSE);
	//ジャンプ力を0にする
	player_.JumpStop();
	//移動量を0にする
	player_.MoveStop();
}

void LoseState::Update(void)
{
	//アニメーションをDOWNに設定
	player_.PlayAnim(static_cast<int>(Player::STATE::DOWN), false, false);

}
