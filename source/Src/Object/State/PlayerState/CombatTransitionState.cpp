#include "CombatTransitionState.h"
#include "CombatState.h"
#include "IdleState.h"
#include "BoostState.h"
#include"../../Actor/Player/Player.h"


CombatTransitionState::CombatTransitionState(Player& player,VECTOR & playerPos, VECTOR& movePow, Quaternion& quarot, float offset):
	player_(player),playerPos_(playerPos),movePow_(movePow),quarot_(quarot)
{
	player_.actorState_ = Player::STATE::COMBAT_RUN;

	//敵の座標を取得
	enemyPos_ = player_.GetEnemyPos();

	//向かう座標の位置を補正する
	enemyPos_.y += offset;

	//敵へのベクトルを作る
	dashVec_ = VSub(enemyPos_, playerPos_);
	
	//正規化
	VECTOR dashNorm = VNorm(dashVec_);

	//移動量を出す
	movePow_ = VScale(dashNorm, SPEED*(player_.GetDeltaTime()*RATE));

	//ジャンプ力を0にする
	player_.JumpStop();

	//モデルの向きを相手の方へ回転させる
	Quaternion XoutQuaRot= Quaternion::LookRotation(dashNorm);
	XoutQuaRot.x = 0.0f;
	XoutQuaRot.z = 0.0f;
	player_.quaRot_ = XoutQuaRot;

	//ビームライフルを非アクティブ化
	player_.GetBeamRifle().InActivate();

	//ビームサーベルをアクティブ化
	player_.GetBeamSaber().Activate();

	//移動スピードとMax最大スピードを設定
	player_.SetMaxMoveSpeed(player_.DEFAULT_MOVE_SPEED);
	player_.SetMoveSpeed(0.0f);

	//相手との距離をとる
	Distance();

	//バーニアエフェクトを起動
	player_.PlayEffect(Player::EFFECT_TYPE::JET_BACK_LEFT, true);
	//バーニアエフェクトを起動
	player_.PlayEffect(Player::EFFECT_TYPE::JET_BACK_RIGHT, true);

}

void CombatTransitionState::Update(void)
{
	//アニメーションをCOMBAT_RUNにする
	player_.PlayAnim(static_cast<int>(Player::STATE::COMBAT_RUN), false, true,true);
	//相手との距離を計測
	Distance();
	//距離が一定以下なら
	if (distance_<=200.0f)
	{
		//バーニアエフェクトを止める
		player_.StopEffect(Player::EFFECT_TYPE::JET_BACK_LEFT);
		player_.StopEffect(Player::EFFECT_TYPE::JET_BACK_RIGHT);
		//近接格闘状態に移行
		player_.ChangeState(std::make_unique<CombatState>(player_));
		return;
	}
	//近接攻撃ボタンが押されていたら
	if(player_.GetInput().IsTriggerd("combat"))
	{
		//バーニアエフェクトを止める
		player_.StopEffect(Player::EFFECT_TYPE::JET_BACK_LEFT);
		player_.StopEffect(Player::EFFECT_TYPE::JET_BACK_RIGHT);
		//近接格闘状態に移行
		player_.ChangeState(std::make_unique<CombatState>(player_));
		return;
	}
	//ジャンプボタンが二回押されて、ブーストゲージが一定以上あったら
	else if(player_.GetInput().IsDoublePrassed("jump") && player_.IsBoostGaugeSufficient(player_.MIN_BOOST_COST))
	{
		//バーニアエフェクトを止める
		player_.StopEffect(Player::EFFECT_TYPE::JET_BACK_LEFT);
		player_.StopEffect(Player::EFFECT_TYPE::JET_BACK_RIGHT);
		//ビームサーベルを非有効化
		player_.GetBeamSaber().InActivate();
		player_.GetBeamRifle().Activate();
		//ブースト状態に移行
		player_.ChangeState(std::make_unique<BoostState>(player_));
		return;
	}
	else
	{
	}
}

float CombatTransitionState::Distance(void)
{
	dashVec_ = VSub(enemyPos_, playerPos_);
	distance_ = sqrtf(dashVec_.x * dashVec_.x + dashVec_.y * dashVec_.y + dashVec_.z * dashVec_.z);
	return distance_;
}
