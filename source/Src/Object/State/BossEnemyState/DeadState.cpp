#include"../../Actor/Enemy/BossEnemy.h"
#include "DeadState.h"

DeadState::DeadState(BossEnemy& bossEnemy):bossEnemy_(bossEnemy)
{
	//アニメーションをDEADにする
	bossEnemy_.PlayAnim(static_cast<int>(BossEnemy::BOSS_STATE::DEAD), false, false);
	//死亡時エフェクトを再生
	bossEnemy_.PlayEffect(BossEnemy::BOSS_STATE::DEAD);
}

void DeadState::Update(void)
{

}
