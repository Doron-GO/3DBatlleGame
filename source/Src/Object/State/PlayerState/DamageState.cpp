#include "DamageState.h"
#include"../../Actor/Player/Player.h"
#include "IdleState.h"

DamageState::DamageState(Player& player) :player_(player)
{
	//ダメージアニメーションを再生
	player_.PlayAnim(static_cast<int>(Player::STATE::DAMAGE), true, true);
	//現在のステート情報を保存
	player_.actorState_ = Player::STATE::DAMAGE;
	//前後左右移動をストップ
	player_.MoveStop();
	//上下の移動をストップ
	player_.JumpStop();
	count_ = 0.0f;
	//無敵時間を設定
	player_.SetSafeTime(Player::SMALL_SAFE_TIME);
	//ビームサーベルの当たり判定を非アクティブにする
	player_.GetBeamSaber().GetSaber().InActivate();
	//ビームサーベルの描画を非アクティブ状態にする
	player_.GetBeamSaber().InActivate();
	//ビームライフルをアクティブ状態にする
	player_.GetBeamRifle().Activate();
	player_.StopEffect(Player::EFFECT_TYPE::JET_BACK_LEFT);
	player_.StopEffect(Player::EFFECT_TYPE::JET_BACK_RIGHT);

}

void DamageState::Update(void)
{
	//現在再生中アニメーションが一ループ再生完了したら
	if (player_.IsAnimEnded())
	{
		player_.actorState_ = Player::STATE::IDLE;
		//アイドルアニメーションを再生
		player_.PlayAnim(static_cast<int>(Player::STATE::IDLE), false, true);
		//アイドル状態に移行
		player_.ChangeState(std::make_unique<IdleState>(player_));
		return;
	}

}
