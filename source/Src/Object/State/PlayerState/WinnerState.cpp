#include "WinnerState.h"
#include"../../Actor/Player/Player.h"

WinnerState::WinnerState(Player& player):player_(player)
{
	//�A�j���[�V�����������|�[�Y�ɐݒ�
	player_.PlayAnim(static_cast<int>(Player::STATE::WIN), true, false);
	//�W�����v�͂�����
	player_.JumpStop();
	//�ړ��ʂ�����
	player_.MoveStop();

}

void WinnerState::Update()
{
	player_.PlayAnim(static_cast<int>(Player::STATE::WIN), true, false);
}
