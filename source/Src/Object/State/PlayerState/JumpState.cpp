#include "JumpState.h"
#include"../../Actor/Player/Player.h"
#include"../../../Utility/AsoUtility.h"
#include"FallState.h"
#include"MoveState.h"
JumpState::JumpState(Player& player):player_(player)
{
	//ステートをJUMPにする
	player_.actorState_ = Player::STATE::JUMP;
	//最高速度を設定
	player_.SetMaxMoveSpeed(player_.MAX_JUMP_MOVE_SPEED);
	//重力を0にする
	player_.SetGravityPow(0.0f);
	//ブーストゲージを減らす
	player_.ConsumeBoostGauge(Player::JUMP_FAST_RATE);	
	//バーニアエフェクトを起動
	player_.PlayEffect(Player::EFFECT_TYPE::JET_BACK_LEFT,true);
	//バーニアエフェクトを起動
	player_.PlayEffect(Player::EFFECT_TYPE::JET_BACK_RIGHT,true);

}

void JumpState::Update(void)
{
	//スティックの倒れている方向を取得
	player_.GetMoveDir();
	//射撃処理
	player_.Shot();
	//移動処理
	player_.Move();

	//ジャンプボタンが押されていて、ブーストゲージが一定以上あったら
	if (player_.GetInput().IsPrassed("jump") && player_.IsBoostGaugeSufficient(player_.MIN_JUMP_COST))
	{
		player_.PlayAnim(static_cast<int>(Player::STATE::JUMP), true, false);
		//ジャンプ処理
		player_.Jump();
	}
	else //でなければフォール状態に移行
	{
		player_.StopEffect(Player::EFFECT_TYPE::JET_BACK_LEFT);
		player_.StopEffect(Player::EFFECT_TYPE::JET_BACK_RIGHT);
		player_.ChangeState(std::make_unique<FallState>(player_));
		return;
	}
	//近接攻撃ボタンが押されていたら
	if (player_.GetInput().IsTriggerd("combat"))
	{
		//ステートを近接攻撃ダッシュ状態にする
		player_.actorState_ = Player::STATE::COMBAT_RUN;
		//近接攻撃処理
		player_.Combat();
		return;
	}
}

