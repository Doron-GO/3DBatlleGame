#include"BoostDashState.h"
#include "BoostState.h"
#include"FallState.h"
#include"JumpState.h"
#include"IdleState.h"
#include"MoveState.h"
#include"../../Actor/Player/Player.h"

BoostState::BoostState(Player& player):player_(player)
{
	time_ = 0.0f;
	player_.GetMoveDir();
	player_.SetMaxMoveSpeed(player_.BOOST_MOVE_SPEED);
	player_.SetMoveSpeed(player_.BOOST_MOVE_SPEED);
	player_.ResetShotFlame();
	player_.pState_ = Player::STATE::BOOST;
	player_.PlayAnim(static_cast<int>(Player::STATE::BOOST), false, true,true);
	player_.BoostQuaRot();
	player_.ConsumeBoostGauge(player_.BOOST_RATE);
	player_.RechargeBoostCountReset();
	player_.JumpStop();
	player_.GravityZero();
}

void BoostState::Update()
{
	//ブースト時間が終わっているか判定
	if (time_> MAX_BOOST_TIME)
	{
		//ジャンプボタンを押していたらジャンプ状態に移行
		if (player_.GetInput().IsPrassed("jump"))
		{
			player_.ChangeState(std::make_unique<BoostDashState>(player_));
			return;
		}
		//スティックを触っていたらムーブ状態に移行
		if (!(player_.GetInput().isStickTilted(Input::STICK_LR::L)))
		{
			player_.ChangeState(std::make_unique<MoveState>(player_));
			return;
		}
		//スティックを触ってなければアイドル状態に移行
		else
		{
			player_.ChangeState(std::make_unique<IdleState>(player_));
			return;
		}
	}
	//ブースト行動関数
	player_.Boost();
	//上下の移動を止める
	player_.JumpStop();
	//射撃
	player_.Shot();
	//ブースト時間加算
	time_+=(player_.GetDeltaTime()*60.0f);
}
