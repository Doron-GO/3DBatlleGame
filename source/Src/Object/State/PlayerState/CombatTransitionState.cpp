#include "CombatTransitionState.h"
#include "CombatState.h"
#include "IdleState.h"
#include "BoostState.h"
#include"../../Actor/Player/Player.h"


CombatTransitionState::CombatTransitionState(Player& player,VECTOR & playerPos, VECTOR& movePow, Quaternion& quarot, float offset):
	player_(player),playerPos_(playerPos),movePow_(movePow),quarot_(quarot)
{
	player_.actorState_ = Player::STATE::COMBAT_RUN;

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

	//�W�����v�͂�0�ɂ���
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
	player_.SetMaxMoveSpeed(player_.DEFAULT_MOVE_SPEED);
	player_.SetMoveSpeed(0.0f);

	//����Ƃ̋������Ƃ�
	Distance();

	//�o�[�j�A�G�t�F�N�g���N��
	player_.PlayEffect(Player::EFFECT_TYPE::JET_BACK_LEFT, true);
	//�o�[�j�A�G�t�F�N�g���N��
	player_.PlayEffect(Player::EFFECT_TYPE::JET_BACK_RIGHT, true);

}

void CombatTransitionState::Update(void)
{
	//�A�j���[�V������COMBAT_RUN�ɂ���
	player_.PlayAnim(static_cast<int>(Player::STATE::COMBAT_RUN), false, true,true);
	//����Ƃ̋������v��
	Distance();
	//���������ȉ��Ȃ�
	if (distance_<=200.0f)
	{
		//�o�[�j�A�G�t�F�N�g���~�߂�
		player_.StopEffect(Player::EFFECT_TYPE::JET_BACK_LEFT);
		player_.StopEffect(Player::EFFECT_TYPE::JET_BACK_RIGHT);
		//�ߐڊi����ԂɈڍs
		player_.ChangeState(std::make_unique<CombatState>(player_));
		return;
	}
	//�ߐڍU���{�^����������Ă�����
	if(player_.GetInput().IsTriggerd("combat"))
	{
		//�o�[�j�A�G�t�F�N�g���~�߂�
		player_.StopEffect(Player::EFFECT_TYPE::JET_BACK_LEFT);
		player_.StopEffect(Player::EFFECT_TYPE::JET_BACK_RIGHT);
		//�ߐڊi����ԂɈڍs
		player_.ChangeState(std::make_unique<CombatState>(player_));
		return;
	}
	//�W�����v�{�^������񉟂���āA�u�[�X�g�Q�[�W�����ȏ゠������
	else if(player_.GetInput().IsDoublePrassed("jump") && player_.IsBoostGaugeSufficient(player_.MIN_BOOST_COST))
	{
		//�o�[�j�A�G�t�F�N�g���~�߂�
		player_.StopEffect(Player::EFFECT_TYPE::JET_BACK_LEFT);
		player_.StopEffect(Player::EFFECT_TYPE::JET_BACK_RIGHT);
		//�r�[���T�[�x�����L����
		player_.GetBeamSaber().InActivate();
		player_.GetBeamRifle().Activate();
		//�u�[�X�g��ԂɈڍs
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
