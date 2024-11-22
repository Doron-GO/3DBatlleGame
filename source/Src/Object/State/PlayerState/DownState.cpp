#include "DownState.h"
#include "IdleState.h"
#include"../../Actor/Player/Player.h"

DownState::DownState(Player& player):player_(player)
{
	//ステートをDOWN状態に
	player_.actorState_ = Player::STATE::DOWN;
	//アニメーションをDOWにする
	player_.PlayAnim(static_cast<int>(Player::STATE::DOWN), true, false,false,-1.0f,0.01f);
	//ジャンプ力を0にする
	player_.JumpStop();
	//移動量0をにする
	player_.MoveStop();
	//無敵時間を設定
	player_.SetSafeTime(Player::BIG_SAFE_TIME);
	//ビーサーベルを非有効化
	player_.GetBeamSaber().GetSaber().InActivate();
	player_.GetBeamSaber().InActivate();
	//ビームライフルを非有効化
	player_.GetBeamRifle().Activate();
	player_.StopEffect(Player::EFFECT_TYPE::JET_BACK_LEFT);
	player_.StopEffect(Player::EFFECT_TYPE::JET_BACK_RIGHT);

}

void DownState::Update(void)
{
	//アニメーション再生が終わっていたら
	if (player_.IsAnimEnded())
	{
		//状態をIDLEにする
		player_.actorState_ = Player::STATE::IDLE;
		//アニメーションをIDLEにする
		player_.PlayAnim(static_cast<int>(Player::STATE::IDLE), false, true);
		//アイドル状態に移行
		player_.ChangeState(std::make_unique<IdleState>(player_));
		return;
	}

}
