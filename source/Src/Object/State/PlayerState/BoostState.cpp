#include"BoostDashState.h"
#include "BoostState.h"
#include"FallState.h"
#include"JumpState.h"
#include"IdleState.h"
#include"MoveState.h"
#include"../../Actor/Player/Player.h"

//�u�[�X�g���̍d������
static constexpr float MAX_BOOST_TIME = 20.0f;

BoostState::BoostState(Player& player):player_(player)
{
	//�u�[�X�g���Ԃ�������
	boostTime_ = 0.0f;
	//�X�e�B�b�N�̓|��Ă���������擾
	player_.GetMoveDir();
	//�ō�����ݒ�
	player_.SetMaxMoveSpeed(player_.MOVE_SPEED_BOOST);
	//�ړ����x��ݒ�
	player_.SetMoveSpeed(player_.MOVE_SPEED_BOOST);
	//�ˌ��N�[���^�C�������Z�b�g
	player_.ResetShotFlame();
	//�X�e�[�g��BOOST�ɂ���
	player_.actorState_ = Player::STATE::BOOST;
	//�A�j���[�V������BOOST�ɂ���
	player_.PlayAnim(static_cast<int>(Player::STATE::BOOST), true, true,true);
	//�u�[�X�g��������������肷��
	player_.BoostQuaRot();
	//�u�[�X�g�Q�[�W�����炷
	player_.ConsumeBoostGauge(player_.BOOST_RATE);
	//�u�[�X�g�Q�[�W�񕜊J�n�v�������Z�b�g����
	player_.RechargeBoostCountReset();
	//�W�����v�͂�0�ɂ���
	player_.JumpStop();
	//�d�͂�0�ɂ���
	player_.GravityOne();
	//�o�[�j�A�G�t�F�N�g���N��
	player_.PlayEffect(Player::EFFECT_TYPE::JET_BACK_LEFT);
	//�o�[�j�A�G�t�F�N�g���N��
	player_.PlayEffect(Player::EFFECT_TYPE::JET_BACK_RIGHT);
	player_.PlayEffect(Player::EFFECT_TYPE::BOOST);

}

void BoostState::Update(void)
{
	//�u�[�X�g���Ԃ��I����Ă��邩����
	if (boostTime_> MAX_BOOST_TIME)
	{
		player_.StopEffect(Player::EFFECT_TYPE::BOOST);
		//�W�����v�{�^���������Ă�����W�����v��ԂɈڍs
		if (player_.GetInput().IsPrassed("jump"))
		{
			player_.ChangeState(std::make_unique<BoostDashState>(player_));
			return;
		}
		//�X�e�B�b�N��G���Ă����烀�[�u��ԂɈڍs
		if (!(player_.GetInput().isStickTilted(Input::STICK_LR::L)))
		{
			player_.StopEffect(Player::EFFECT_TYPE::JET_BACK_LEFT);
			player_.StopEffect(Player::EFFECT_TYPE::JET_BACK_RIGHT);
			player_.ChangeState(std::make_unique<MoveState>(player_));
			return;
		}
		//�X�e�B�b�N��G���ĂȂ���΃A�C�h����ԂɈڍs
		else
		{
			player_.StopEffect(Player::EFFECT_TYPE::JET_BACK_LEFT);
			player_.StopEffect(Player::EFFECT_TYPE::JET_BACK_RIGHT);
			player_.ChangeState(std::make_unique<IdleState>(player_));
			return;
		}
	}

	player_.PlayAnim(static_cast<int>(Player::STATE::BOOST), true, true, true);
	//�u�[�X�g�s���֐�
	player_.Boost();
	//�㉺�̈ړ����~�߂�
	player_.JumpStop();
	//�ˌ�
	player_.Shot();
	//�u�[�X�g���ԉ��Z
	boostTime_+=(player_.GetDeltaTime()*60.0f);
}
