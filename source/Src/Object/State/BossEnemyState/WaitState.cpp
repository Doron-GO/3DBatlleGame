#include"../../Actor/Enemy/BossEnemy.h"
#include"BarrageState.h"
#include "WaitState.h"

WaitState::WaitState(BossEnemy& bossEnemy):bossEnemy_(bossEnemy)
{

}

void WaitState::Update()
{
	bossEnemy_.PlayAnim(static_cast<int>(BossEnemy::STATE::WAIT), false, true);

	if (bossEnemy_.IsAnimEnded())
	{
		bossEnemy_.ChangeState(std::make_unique<BarrageState>(bossEnemy_));
		return;
	}
}
