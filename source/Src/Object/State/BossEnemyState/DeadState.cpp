#include"../../Actor/Enemy/BossEnemy.h"
#include "DeadState.h"

DeadState::DeadState(BossEnemy& bossEnemy):bossEnemy_(bossEnemy)
{
	//�A�j���[�V������DEAD�ɂ���
	bossEnemy_.PlayAnim(static_cast<int>(BossEnemy::BOSS_STATE::DEAD), false, false);
	//���S���G�t�F�N�g���Đ�
	bossEnemy_.PlayEffect(BossEnemy::BOSS_STATE::DEAD);
}

void DeadState::Update(void)
{

}
