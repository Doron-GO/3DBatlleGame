#include"../../Actor/Enemy/BossEnemy.h"
#include "BarrageState.h"
#include"../../DamageObject/DeathBall.h"
#include "WaitState.h"
#include "DeadState.h"

BarrageState::BarrageState(BossEnemy& bossEnemy) :bossEnemy_(bossEnemy)
{
	bossEnemy_.PlayAnim(static_cast<int>(BossEnemy::BOSS_STATE::SHOOTING), false, true);
	bossEnemy_.ActiveSpMove(BossEnemy::SP_MOVE::DEATH_BALL);
}

void BarrageState::Update(void)
{
	if (bossEnemy_.IsAnimEnded() && !(bossEnemy_.IsActiveSpMove(BossEnemy::SP_MOVE::DEATH_BALL)))
	{
		bossEnemy_.ChangeState(std::make_unique<WaitState>(bossEnemy_));
		return;
	}

}
