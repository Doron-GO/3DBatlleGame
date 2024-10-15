#include "BoostDashState.h"
#include"FallState.h"
#include"IdleState.h"
#include"MoveState.h"
#include"../../Actor/Player/Player.h"

BoostDashState::BoostDashState(Player& player) : player_(player)
{
	//ステートをBOOST_DASHにする
	player_.actorState_ = Player::STATE::BOOST_DASH;
	//アニメーションをBOOSTにする
	player_.PlayAnim(static_cast<int>(Player::STATE::BOOST), false, true, true);
	//最高速を設定
	player_.SetMaxMoveSpeed(player_.BOOST_DASH_MOVE_SPEED);
	//移動速度を設定
	player_.SetMoveSpeed(player_.BOOST_DASH_MOVE_SPEED);
}

void BoostDashState::Update(void)
{
	//スティックの倒れている方向を取得
	player_.GetMoveDirBoostDash();
	
	//ブーストゲージを減らす
	float delta= player_.GetDeltaTime();
	player_.ConsumeBoostGauge(player_.BOOST_RATE * delta);

	//ブーストゲージが一定以上あり、ジャンプボタンが押されていたら
	if ( player_.IsBoostGaugeSufficient(player_.MIN_JUMP_BOOST)&&player_.GetInput().IsPrassed("jump") )
	{
		//ブーストゲージの回復を止める
		player_.StopRechargeBoost();
		//ブーストダッシュ処理
		player_.MoveBoodtDash();
	}
	//ジャンプボタンが押されておらず、スティックが倒されていたら
	else if((player_.GetInput().isStickTilted(Input::STICK_LR::L)))
	{
		//ブーストダッシュ終了処理
		player_.BoostDashEnd();
		//ムーブ状態に移行
		player_.ChangeState(std::make_unique<MoveState>(player_));
		return;
	}
	else
	{
		//ブーストダッシュ終了処理
		player_.BoostDashEnd();
		//アニメーションをIDLEにする
		player_.PlayAnim(static_cast<int>(Player::STATE::IDLE), false, true);
		//アイドル状態に移行
		player_.ChangeState(std::make_unique<IdleState>(player_));
		return;
	}
}
