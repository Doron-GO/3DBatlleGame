#include "BoostDashState.h"
#include"FallState.h"
#include"IdleState.h"
#include"MoveState.h"
#include"../../Actor/Player/Player.h"

BoostDashState::BoostDashState(Player& player) : player_(player)
{
	//�X�e�[�g��BOOST_DASH�ɂ���
	player_.actorState_ = Player::STATE::BOOST_DASH;
	//�A�j���[�V������BOOST�ɂ���
	player_.PlayAnim(static_cast<int>(Player::STATE::BOOST), false, true, true);
	//�ō�����ݒ�
	player_.SetMaxMoveSpeed(player_.MOVE_SPEED_BOOST_DASH);
	//�ړ����x��ݒ�
	player_.SetMoveSpeed(player_.MOVE_SPEED_BOOST_DASH);
	//�o�[�j�A�G�t�F�N�g���N��
	player_.PlayEffect(Player::EFFECT_TYPE::JET_BACK_LEFT);
	//�o�[�j�A�G�t�F�N�g���N��
	player_.PlayEffect(Player::EFFECT_TYPE::JET_BACK_RIGHT);
}

void BoostDashState::Update(void)
{
	//�X�e�B�b�N�̓|��Ă���������擾
	player_.GetMoveDirBoostDash();
	
	//�u�[�X�g�Q�[�W�����炷
	float delta= player_.GetDeltaTime();
	player_.ConsumeBoostGauge(player_.BOOST_RATE * delta);

	//�u�[�X�g�Q�[�W�����ȏ゠��A�W�����v�{�^����������Ă�����
	if ( player_.IsBoostGaugeSufficient(player_.MIN_JUMP_COST)&&player_.GetInput().IsPrassed("jump") )
	{
		//�u�[�X�g�Q�[�W�̉񕜂��~�߂�
		player_.StopRechargeBoost();
		//�u�[�X�g�_�b�V������
		player_.MoveBoostDash();
	}
	//�W�����v�{�^����������Ă��炸�A�X�e�B�b�N���|����Ă�����
	else if((player_.GetInput().isStickTilted(Input::STICK_LR::L)))
	{
		player_.StopEffect(Player::EFFECT_TYPE::JET_BACK_LEFT);
		player_.StopEffect(Player::EFFECT_TYPE::JET_BACK_RIGHT);
		//�u�[�X�g�_�b�V���I������
		player_.BoostDashEnd();
		//���[�u��ԂɈڍs
		player_.ChangeState(std::make_unique<MoveState>(player_));
		return;
	}
	else
	{
		player_.StopEffect(Player::EFFECT_TYPE::JET_BACK_LEFT);
		player_.StopEffect(Player::EFFECT_TYPE::JET_BACK_RIGHT);
		//�u�[�X�g�_�b�V���I������
		player_.BoostDashEnd();
		//�A�j���[�V������IDLE�ɂ���
		player_.PlayAnim(static_cast<int>(Player::STATE::IDLE), false, true);
		//�A�C�h����ԂɈڍs
		player_.ChangeState(std::make_unique<IdleState>(player_));
		return;
	}
}
