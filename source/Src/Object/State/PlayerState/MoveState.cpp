#include "MoveState.h"
#include"../../Actor/Player/Player.h"
#include"../../../Utility/AsoUtility.h"
#include "IdleState.h"
#include "JumpState.h"
#include "BoostState.h"

MoveState::MoveState(Player& player):player_(player)
{
	//ステートをRUNに
	player_.actorState_ = Player::STATE::RUN;
	//アニメーションをRUNにする
	player_.PlayAnim(static_cast<int>(Player::STATE::RUN), false, true);
	//マックススピードを設定
	player_.SetMaxMoveSpeed(player_.MOVE_SPEED);
}

void MoveState::Update(void)
{
	//スティックの倒れている方向を取得
	player_.GetMoveDir();
	//アニメーションをRUN状態に
	player_.PlayAnim(static_cast<int>(Player::STATE::RUN), false, true);
	//近接攻撃が終わっていたら入る
	if (player_.CombatStunEnded())
	{
		//移動処理
		player_.Move();
		//射撃処理
		player_.Shot();

		//スティック入力がなければアイドル状態に移行
		if (!(player_.GetInput().isStickTilted(Input::STICK_LR::L)))
		{
			//アニメーションをIDLEにする
			player_.PlayAnim(static_cast<int>(Player::STATE::IDLE), false, true);
			//アイドル状態に移行
			player_.ChangeState(std::make_unique<IdleState>(player_));
			return;
		}
		//近接攻撃ボタンが押されていたら
		else if (player_.GetInput().IsTriggerd("combat"))
		{
			//アタック状態に移行
			player_.Combat();
			return;
		}
		//ジャンプボタンが二回押されていて、ブーストゲージが一定以上あったら
		else if (player_.GetInput().IsDoublePrassed("jump")&& player_.IsBoostGaugeSufficient(player_.MIN_BOOST))
		{
			//ブースト状態に移行
			player_.ChangeState(std::make_unique<BoostState>(player_));
			return;
		}
		//ジャンプボタンが押されていて、ブーストゲージが一定以上あったら
		else if (player_.GetInput().IsHold("jump") && player_.IsBoostGaugeSufficient(player_.MIN_JUMP_BOOST))
		{
			//アニメーションをJUMPにする
			player_.PlayAnim(static_cast<int>(Player::STATE::JUMP), false, false, true);
			//ジャンプ状態に移行
			player_.ChangeState(std::make_unique<JumpState>(player_));
			return;
		}
		else
		{
		}
	}
}


