#include "FallState.h"
#include"IdleState.h"
#include"../../Actor/Player/Player.h"
#include"JumpState.h"
#include"BoostState.h"

FallState::FallState(Player& player):player_(player)
{
	//ステートをFALL状態にする
	player_.actorState_ = Player::STATE::FALL;
	//アニメーションをFALL状態にする
	player_.PlayAnim(static_cast<int>(Player::STATE::FALL), false, true, true);
	//最高速度を設定
	player_.SetMaxMoveSpeed(player_.FALL_MAX_MOVE_SPEED);
}

void FallState::Update(void)
{

	//ジャンプ量を徐々に減らす
	player_.JumpPowZero();
	//アニメーションをJUMPにする
	player_.PlayAnim(static_cast<int>(Player::STATE::JUMP), false, false, false);
	//移動処理
	player_.Move();
	//射撃処理
	player_.Shot();
	//重力処理
	player_.CalcGravity();

	//地面に着地したら
	if (player_.IsGround())
	{	
		//アニメーションをIDLEにする
		player_.PlayAnim(static_cast<int>(Player::STATE::IDLE), false, false);
		player_.PlayEffect(Player::EFFECT_TYPE::DUST_CLOUD);
		//アイドル状態に移行
		player_.ChangeState(std::make_unique<IdleState>(player_));
		return;
	}
	//近接攻撃ボタンをおしたら
	if (player_.GetInput().IsTriggerd("combat"))
	{
		//近接攻撃処理
		player_.Combat();
		return;
	}
	//ジャンプボタンが二回早く押されたかつ、ブーストゲージが20以上あったら
	if (player_.GetInput().IsDoublePrassed("jump") && player_.IsBoostGaugeSufficient(player_.MIN_BOOST_COST))
	{
		//ブースト状態に移行
		player_.ChangeState(std::make_unique<BoostState>(player_));
		return;
	}
	//ジャンプボタンが押されているかつ、ブーストゲージが１以上あったら
	else if (player_.GetInput().IsHold("jump") && player_.IsBoostGaugeSufficient(player_.MIN_JUMP_COST))
	{
		//アニメーションをJUMPにする
		player_.PlayAnim(static_cast<int>(Player::STATE::JUMP), true, false, true);
		//ジャンプ状態に移行
		player_.ChangeState(std::make_unique<JumpState>(player_));
		return;
	}

}
