#include"../../Actor/Enemy/BossEnemy.h"
#include"BarrageState.h"
#include "WaitState.h"

WaitState::WaitState(BossEnemy& bossEnemy):bossEnemy_(bossEnemy)
{

}

void WaitState::Update()
{
	//�A�j���[�V����WAIT�ɂ���
	bossEnemy_.PlayAnim(static_cast<int>(BossEnemy::STATE::WAIT), false, true);

	//�A�j���[�V�����Đ����I����Ă�����
	if (bossEnemy_.IsAnimEnded())
	{
		//�U����ԂɈڍs
		bossEnemy_.ChangeState(std::make_unique<BarrageState>(bossEnemy_));
		return;
	}
}
