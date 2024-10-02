#include "CombatState.h"
#include "IdleState.h"
#include "FallState.h"
#include"../../Actor/Player/Player.h"

//格闘硬直時間
static constexpr float COMBAT_STAN_TIME = 20.0f;

CombatState::CombatState(Player& player):player_(player)
{
	//ステートをCOMBATにする
	player_.pState_ = Player::STATE::COMBAT;
	//アニメーションをCOMBATにする
	player_.PlayAnim(static_cast<int>(Player::STATE::COMBAT), true, true, false,-1.0f,54.0f);
	//移動量を0にする
	player_.MoveStop();
	//ビームサーベルを有効化
	player_.GetBeamSaber().GetSaber().Activate();
	//最高速を設定
	player_.SetMaxMoveSpeed(player_.MOVE_SPEED);
	//移動量を0にする
	player_.SetMoveSpeed(0.0f);

}

void CombatState::Update()
{
	//アニメーション再生が終わっていたら
	if (player_.IsAnimEnded())
	{
		//アニメーションをIDLEにする
		player_.PlayAnim(static_cast<int>(Player::STATE::IDLE), false, true);
		//ビームサーベルを非有効化
		player_.GetBeamSaber().GetSaber().InActivate();
		player_.GetBeamSaber().InActivate();
		//ビームライフルを有効化
		player_.GetBeamRifle().Activate();
		//硬直時間を設定
		player_.SetCombatStan(COMBAT_STAN_TIME);
		//アイドル状態に移行
		player_.ChangeState(std::make_unique<IdleState>(player_));
		return;
	}

}
