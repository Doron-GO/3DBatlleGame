#include"../../Actor/Enemy/BossEnemy.h"
#include "DeadState.h"

DeadState::DeadState(BossEnemy& bossEnemy):bossEnemy_(bossEnemy)
{
	//アニメーションをDEADにする
	bossEnemy_.PlayAnim(static_cast<int>(BossEnemy::STATE::DEAD), false, false);
	//死亡時エフェクトを再生
	bossEnemy_.PlayEffect(BossEnemy::STATE::DEAD);
}

void DeadState::Update()
{

}
