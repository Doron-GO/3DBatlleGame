#include"../../Actor/Enemy/BossEnemy.h"
#include"BarrageState.h"
#include "WaitState.h"

WaitState::WaitState(BossEnemy& bossEnemy):bossEnemy_(bossEnemy)
{

}

void WaitState::Update()
{
	//アニメーションWAITにする
	bossEnemy_.PlayAnim(static_cast<int>(BossEnemy::STATE::WAIT), false, true);

	//アニメーション再生が終わっていたら
	if (bossEnemy_.IsAnimEnded())
	{
		//攻撃状態に移行
		bossEnemy_.ChangeState(std::make_unique<BarrageState>(bossEnemy_));
		return;
	}
}
