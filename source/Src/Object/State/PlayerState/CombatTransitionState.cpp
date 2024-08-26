#include "CombatTransitionState.h"
#include "CombatState.h"
#include "IdleState.h"
#include "BoostState.h"
#include"../../Actor/Player/Player.h"


CombatTransitionState::CombatTransitionState(Player& player,VECTOR & playerPos, VECTOR& movePow, Quaternion& quarot, float offset):
	player_(player),playerPos_(playerPos),movePow_(movePow),quarot_(quarot)
{
	player_.pState_ = Player::STATE::COMBAT_RUN;

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

	//ジャンプ力をなくす
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
	player_.SetMaxMoveSpeed(player_.MOVE_SPEED);
	player_.SetMoveSpeed(0.0f);

	//相手との距離をとる
	Distance();
}

void CombatTransitionState::Update()
{
	player_.PlayAnim(static_cast<int>(Player::STATE::COMBAT_RUN), false, true,true);
	Distance();
	if (distance_<=200.0f)
	{
		player_.ChangeState(std::make_unique<CombatState>(player_));
		return;
	}
	if(player_.GetInput().IsTriggerd("combat"))
	{
		player_.ChangeState(std::make_unique<CombatState>(player_));
		return;
	}
	else if(player_.GetInput().IsDoublePrassed("jump") && player_.IsBoostGaugeSufficient(player_.MIN_BOOST))
	{
		player_.GetBeamSaber().InActivate();
		player_.GetBeamRifle().Activate();
		player_.ChangeState(std::make_unique<BoostState>(player_));
		return;
	}
	else
	{
	}


}

void CombatTransitionState::CombatDash(void)
{

}

float CombatTransitionState::Distance(void)
{
	dashVec_ = VSub(enemyPos_, playerPos_);
	distance_ = sqrtf(dashVec_.x * dashVec_.x + dashVec_.y * dashVec_.y + dashVec_.z * dashVec_.z);
	return distance_;
}
