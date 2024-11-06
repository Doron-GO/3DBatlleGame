#include "FallState.h"
#include"IdleState.h"
#include"../../Actor/Player/Player.h"
#include"JumpState.h"
#include"BoostState.h"

FallState::FallState(Player& player):player_(player)
{
	//�X�e�[�g��FALL��Ԃɂ���
	player_.actorState_ = Player::STATE::FALL;
	//�A�j���[�V������FALL��Ԃɂ���
	player_.PlayAnim(static_cast<int>(Player::STATE::FALL), false, true, true);
	//�ō����x��ݒ�
	player_.SetMaxMoveSpeed(player_.FALL_MAX_MOVE_SPEED);
}

void FallState::Update(void)
{

	//�W�����v�ʂ����X�Ɍ��炷
	player_.JumpPowZero();
	//�A�j���[�V������JUMP�ɂ���
	player_.PlayAnim(static_cast<int>(Player::STATE::JUMP), false, false, false);
	//�ړ�����
	player_.Move();
	//�ˌ�����
	player_.Shot();
	//�d�͏���
	player_.CalcGravity();

	//�n�ʂɒ��n������
	if (player_.IsGround())
	{	
		//�A�j���[�V������IDLE�ɂ���
		player_.PlayAnim(static_cast<int>(Player::STATE::IDLE), false, false);
		player_.PlayEffect(Player::EFFECT_TYPE::DUST_CLOUD);
		//�A�C�h����ԂɈڍs
		player_.ChangeState(std::make_unique<IdleState>(player_));
		return;
	}
	//�ߐڍU���{�^������������
	if (player_.GetInput().IsTriggerd("combat"))
	{
		//�ߐڍU������
		player_.Combat();
		return;
	}
	//�W�����v�{�^������񑁂������ꂽ���A�u�[�X�g�Q�[�W��20�ȏ゠������
	if (player_.GetInput().IsDoublePrassed("jump") && player_.IsBoostGaugeSufficient(player_.MIN_BOOST_COST))
	{
		//�u�[�X�g��ԂɈڍs
		player_.ChangeState(std::make_unique<BoostState>(player_));
		return;
	}
	//�W�����v�{�^����������Ă��邩�A�u�[�X�g�Q�[�W���P�ȏ゠������
	else if (player_.GetInput().IsHold("jump") && player_.IsBoostGaugeSufficient(player_.MIN_JUMP_COST))
	{
		//�A�j���[�V������JUMP�ɂ���
		player_.PlayAnim(static_cast<int>(Player::STATE::JUMP), true, false, true);
		//�W�����v��ԂɈڍs
		player_.ChangeState(std::make_unique<JumpState>(player_));
		return;
	}

}
