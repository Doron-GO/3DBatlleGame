#include "JumpState.h"
#include"../../Actor/Player/Player.h"
#include"../../../Utility/AsoUtility.h"
#include"FallState.h"
#include"MoveState.h"
JumpState::JumpState(Player& player):player_(player)
{
	//�X�e�[�g��JUMP�ɂ���
	player_.actorState_ = Player::STATE::JUMP;
	//�ō����x��ݒ�
	player_.SetMaxMoveSpeed(player_.MAX_JUMP_MOVE_SPEED);
	//�d�͂�0�ɂ���
	player_.SetGravityPow(0.0f);
	//�u�[�X�g�Q�[�W�����炷
	player_.ConsumeBoostGauge(Player::JUMP_FAST_RATE);	
	//�o�[�j�A�G�t�F�N�g���N��
	player_.PlayEffect(Player::EFFECT_TYPE::JET_BACK_LEFT,true);
	//�o�[�j�A�G�t�F�N�g���N��
	player_.PlayEffect(Player::EFFECT_TYPE::JET_BACK_RIGHT,true);

}

void JumpState::Update(void)
{
	//�X�e�B�b�N�̓|��Ă���������擾
	player_.GetMoveDir();
	//�ˌ�����
	player_.Shot();
	//�ړ�����
	player_.Move();

	//�W�����v�{�^����������Ă��āA�u�[�X�g�Q�[�W�����ȏ゠������
	if (player_.GetInput().IsPrassed("jump") && player_.IsBoostGaugeSufficient(player_.MIN_JUMP_COST))
	{
		player_.PlayAnim(static_cast<int>(Player::STATE::JUMP), true, false);
		//�W�����v����
		player_.Jump();
	}
	else //�łȂ���΃t�H�[����ԂɈڍs
	{
		player_.StopEffect(Player::EFFECT_TYPE::JET_BACK_LEFT);
		player_.StopEffect(Player::EFFECT_TYPE::JET_BACK_RIGHT);
		player_.ChangeState(std::make_unique<FallState>(player_));
		return;
	}
	//�ߐڍU���{�^����������Ă�����
	if (player_.GetInput().IsTriggerd("combat"))
	{
		//�X�e�[�g���ߐڍU���_�b�V����Ԃɂ���
		player_.actorState_ = Player::STATE::COMBAT_RUN;
		//�ߐڍU������
		player_.Combat();
		return;
	}
}

