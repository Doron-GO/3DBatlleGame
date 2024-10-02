#include"BoostDashState.h"
#include "BoostState.h"
#include"FallState.h"
#include"JumpState.h"
#include"IdleState.h"
#include"MoveState.h"
#include"../../Actor/Player/Player.h"

//ブースト中の硬直時間
static constexpr float MAX_BOOST_TIME = 20.0f;

BoostState::BoostState(Player& player):player_(player)
{
	//ブースト時間を初期化
	boostTime_ = 0.0f;
	//スティックの倒れている方向を取得
	player_.GetMoveDir();
	//最高速を設定
	player_.SetMaxMoveSpeed(player_.BOOST_MOVE_SPEED);
	//移動速度を設定
	player_.SetMoveSpeed(player_.BOOST_MOVE_SPEED);
	//射撃クールタイムをリセット
	player_.ResetShotFlame();
	//ステートをBOOSTにする
	player_.pState_ = Player::STATE::BOOST;
	//アニメーションをBOOSTにする
	player_.PlayAnim(static_cast<int>(Player::STATE::BOOST), false, true,true);
	//ブーストをする方向を決定する
	player_.BoostQuaRot();
	//ブーストゲージを減らす
	player_.ConsumeBoostGauge(player_.BOOST_RATE);
	//ブーストゲージ回復開始計測をリセットする
	player_.RechargeBoostCountReset();
	//ジャンプ力を0にする
	player_.JumpStop();
	//重力を0にする
	player_.GravityZero();
}

void BoostState::Update()
{
	//ブースト時間が終わっているか判定
	if (boostTime_> MAX_BOOST_TIME)
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
	boostTime_+=(player_.GetDeltaTime()*60.0f);
}
