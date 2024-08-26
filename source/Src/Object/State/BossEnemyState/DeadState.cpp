#include"../../Actor/Enemy/BossEnemy.h"
#include "DeadState.h"

DeadState::DeadState(BossEnemy& bossEnemy):bossEnemy_(bossEnemy)
{
	bossEnemy_.PlayAnim(static_cast<int>(BossEnemy::STATE::DEAD), false, false);
	bossEnemy_.PlayEffect(BossEnemy::STATE::DEAD);
}

void DeadState::Update()
{

}
