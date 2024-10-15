#include "LoseState.h"
#include"../../Actor/Player/Player.h"
LoseState::LoseState(Player& player):player_(player)
{
	//�A�j���[�V������DOWN�ɐݒ�
	player_.PlayAnim(static_cast<int>(Player::STATE::DOWN), true, false);
	//�����G�t�F�N�g���N��
	player_.PlayEffect(Player::STATE::LOSE);
	//�W�����v�͂�0�ɂ���
	player_.JumpStop();
	//�ړ��ʂ�0�ɂ���
	player_.MoveStop();
}

void LoseState::Update(void)
{
	//�A�j���[�V������DOWN�ɐݒ�
	player_.PlayAnim(static_cast<int>(Player::STATE::DOWN), false, false);

}
