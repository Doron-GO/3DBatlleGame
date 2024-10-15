#include "IdleState.h"
#include "MoveState.h"
#include"../../Actor/Player/Player.h"
#include"JumpState.h"
#include"BoostState.h"

IdleState::IdleState(Player& player):player_(player)
{
	//ステートをIDLE状態にする
	player_.actorState_ = Player::STATE::IDLE;
	//ジャンプ力をにする
	player_.JumpStop();

}

void IdleState::Update(void)
{
	//着地硬直が終了していて、格闘攻撃硬直を終了していたら
	if (player_.LandingStunEnded()&&player_.CombatStunEnded())
	{
		//移動関数
		player_.Move();
		//射撃関数
		player_.Shot();

		//スティックを触っていればムーブ状態に移行
		if (player_.GetInput().isStickTilted(Input::STICK_LR::L))
		{
			player_.ChangeState(std::make_unique<MoveState>(player_));
			return;
		}
		//格闘ボタンを押していたら格闘状態に移行
		else if (player_.GetInput().IsTriggerd("combat"))
		{
			player_.Combat();
			return;
		}
		//ブーストボタンを押していて、ブーストゲージが一定以上あればブースト状態に移行
		else if (player_.GetInput().IsDoublePrassed("jump") && player_.IsBoostGaugeSufficient(player_.MIN_BOOST))
		{
			player_.ChangeState(std::make_unique<BoostState>(player_));
			return;
		}
		//ジャンプボタンを押していて、ブーストゲージが一定以上あればジャンプ状態に移行
		else if (player_.GetInput().IsHold("jump") && player_.IsBoostGaugeSufficient(player_.MIN_JUMP_BOOST))
		{
			player_.PlayAnim(static_cast<int>(Player::STATE::JUMP), true, false, true);
			player_.ChangeState(std::make_unique<JumpState>(player_));
			return;
		}
		else
		{
		}
	}
	//スティックの向いている向きを取得
	player_.GetMoveDir();
	//重力を加算
	player_.CalcGravity();
	//上下半身ともアイドルアニメーションを再生
	player_.PlayAnim(static_cast<int>(Player::STATE::IDLE), false, true);
}

