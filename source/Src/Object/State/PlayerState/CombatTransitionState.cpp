#include "CombatTransitionState.h"
#include "CombatState.h"
#include "IdleState.h"
#include "BoostState.h"
#include"../../Actor/Player/Player.h"


CombatTransitionState::CombatTransitionState(Player& player,VECTOR & playerPos, VECTOR& movePow, Quaternion& quarot, float offset):
	player_(player),playerPos_(playerPos),movePow_(movePow),quarot_(quarot)
{
	player_.pState_ = Player::STATE::COMBAT_RUN;

	//�G�̍��W���擾
	enemyPos_ = player_.GetEnemyPos();

	//���������W�̈ʒu��␳����
	enemyPos_.y += offset;

	//�G�ւ̃x�N�g�������
	dashVec_ = VSub(enemyPos_, playerPos_);
	
	//���K��
	VECTOR dashNorm = VNorm(dashVec_);

	//�ړ��ʂ��o��
	movePow_ = VScale(dashNorm, SPEED*(player_.GetDeltaTime()*RATE));

	//�W�����v�͂��Ȃ���
	player_.JumpStop();

	//���f���̌����𑊎�̕��։�]������
	Quaternion XoutQuaRot= Quaternion::LookRotation(dashNorm);
	XoutQuaRot.x = 0.0f;
	XoutQuaRot.z = 0.0f;
	player_.quaRot_ = XoutQuaRot;

	//�r�[�����C�t�����A�N�e�B�u��
	player_.GetBeamRifle().InActivate();

	//�r�[���T�[�x�����A�N�e�B�u��
	player_.GetBeamSaber().Activate();

	//�ړ��X�s�[�h��Max�ő�X�s�[�h��ݒ�
	player_.SetMaxMoveSpeed(player_.MOVE_SPEED);
	player_.SetMoveSpeed(0.0f);

	//����Ƃ̋������Ƃ�
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
