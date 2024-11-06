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
	player_.SetMaxMoveSpeed(player_.MOVE_SPEED_BOOST_DASH);
	//移動速度を設定
	player_.SetMoveSpeed(player_.MOVE_SPEED_BOOST_DASH);
	//バーニアエフェクトを起動
	player_.PlayEffect(Player::EFFECT_TYPE::JET_BACK_LEFT);
	//バーニアエフェクトを起動
	player_.PlayEffect(Player::EFFECT_TYPE::JET_BACK_RIGHT);
}

void BoostDashState::Update(void)
{
	//スティックの倒れている方向を取得
	player_.GetMoveDirBoostDash();
	
	//ブーストゲージを減らす
	float delta= player_.GetDeltaTime();
	player_.ConsumeBoostGauge(player_.BOOST_RATE * delta);

	//ブーストゲージが一定以上あり、ジャンプボタンが押されていたら
	if ( player_.IsBoostGaugeSufficient(player_.MIN_JUMP_COST)&&player_.GetInput().IsPrassed("jump") )
	{
		//ブーストゲージの回復を止める
		player_.StopRechargeBoost();
		//ブーストダッシュ処理
		player_.MoveBoostDash();
	}
	//ジャンプボタンが押されておらず、スティックが倒されていたら
	else if((player_.GetInput().isStickTilted(Input::STICK_LR::L)))
	{
		player_.StopEffect(Player::EFFECT_TYPE::JET_BACK_LEFT);
		player_.StopEffect(Player::EFFECT_TYPE::JET_BACK_RIGHT);
		//ブーストダッシュ終了処理
		player_.BoostDashEnd();
		//ムーブ状態に移行
		player_.ChangeState(std::make_unique<MoveState>(player_));
		return;
	}
	else
	{
		player_.StopEffect(Player::EFFECT_TYPE::JET_BACK_LEFT);
		player_.StopEffect(Player::EFFECT_TYPE::JET_BACK_RIGHT);
		//ブーストダッシュ終了処理
		player_.BoostDashEnd();
		//アニメーションをIDLEにする
		player_.PlayAnim(static_cast<int>(Player::STATE::IDLE), false, true);
		//アイドル状態に移行
		player_.ChangeState(std::make_unique<IdleState>(player_));
		return;
	}
}
