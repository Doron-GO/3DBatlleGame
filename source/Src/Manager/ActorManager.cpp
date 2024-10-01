#include "ActorManager.h"
#include"../Object/DamageObject/Saber.h"
#include"../Object/Comon/Collider.h"
#include"../UI/UserInterface.h"

#pragma region Parameter

//プレイヤー１のタイプ
constexpr int PLAYER_TYPE = 0;

#pragma endregion



ActorManager::ActorManager(int numberofPlayers)
{
	if (numberofPlayers ==0)
	{
		InitSIngleMode(numberofPlayers);
		_update = &ActorManager::UpdateSingleMode;
		_draw = &ActorManager::DrawSingleMode;
	}
	else
	{	
		InitBattleMode(numberofPlayers);
		_update = &ActorManager::UpdateBattleMode;
		_draw = &ActorManager::DrawBattleMode;
	}
	InitUI();
	isDeadAnyPlayer_ = false;
}

void ActorManager::InitBattleMode(int numberofPlayers)
{
	for (int num = 0; num < PLAYER_NUM_MAX; num++)
	{
		players_.emplace_back(std::make_unique<Player>(num,numberofPlayers));
	}
	SetEnemyInfo();
	for (auto& player : players_)
	{
		player->Init();
	}

}

void ActorManager::InitSIngleMode(int numberofPlayers)
{
	players_.emplace_back(std::make_unique<Player>(SINGLE_PLAY, numberofPlayers));
	bossEnemy_ = std::make_unique<BossEnemy>();
	SetBossEnemyInfo();
	players_[SINGLE_PLAY]->Init();
	bossEnemy_->SetEnemyPosition(&(players_[SINGLE_PLAY]->GetPlayerPos()));
	bossEnemy_->MakeObjects();
}

void ActorManager::Update()
{
	(this->*_update)();
}

void ActorManager::UpdateBattleMode(void)
{
	for (auto& player : players_)
	{
		player->Update();
	}
	DamegeBeamCollision();
	DamegeSwordCollision();
	IsWin();
}

void ActorManager::UpdateSingleMode(void)
{
	for (auto& player : players_)
	{
		player->Update();
	}
	bossEnemy_->Update();
	DmageBossSpMoveCollision();
	DmageBossBeamCollision();
	IsSingleModeWin();
}

void ActorManager::Draw(void)
{
	(this->*_draw)();
}

void ActorManager::DrawBattleMode(void)
{
	for (auto& player : players_)
	{
		player->Draw();
	}

}

void ActorManager::DrawSingleMode(void)
{
	players_[SINGLE_PLAY]->Draw();
	bossEnemy_->Draw();
}

void ActorManager::DrawDebug(int playerType)
{
	for (auto& player : players_)
	{
		player->PlayerDebugDraw(playerType);
	}
}

void ActorManager::DrawAnimeDebug(int playerType)
{
	for (auto& player : players_)
	{
		player->RobotAnimDebugDraw(playerType);
	}
}

void ActorManager::DrawCamera(int playerType)
{
	players_[playerType]->CameraDraw();
}

void ActorManager::DrawUI(int playerType)
{
	userInterface_[playerType]->Draw(isDeadAnyPlayer_);
}

void ActorManager::DrawCommonUI(const float& startCount, const bool& isGameSet, const float& rematchMode)
{
	userInterface_[PLAYER_TYPE]->DrawCommonUI(startCount, isGameSet, rematchMode);
}


void ActorManager::AddClliders(Collider* collider)
{
	for (auto& player : players_)
	{
		player->AddCollider(collider);
	}
}


void ActorManager::SetEnemyInfo(void)
{
	for (int player = 0; player < PLAYER_NUM_MAX; player++)
	{
		int num = 1 - player;
		int num2 = 0+ player;
		if (num <= -1)
		{
			num = 0;
		}
		if (num2 >= 1)
		{
			num2 = 1;
		}
		players_[num]->SetEnemyPosition(&(players_[num2]->GetTransform().pos));
		players_[num]->SetEnemyHp(&(players_[num2]->GetPlayerHP()));
		players_[num]->SetEnemyState(&(players_[num2]->pState_));
	}
}

void ActorManager::SetBossEnemyInfo(void)
{
	int SINGLE_PLAY = 0;

	players_[SINGLE_PLAY]->SetEnemyPosition(&bossEnemy_->GetBossEnemyPos());
	players_[SINGLE_PLAY]->SetEnemyHp(&bossEnemy_->GetPlayerHP());

	players_[SINGLE_PLAY]->SetEnemyState(nullptr);

}

void ActorManager::DamageObjectInit(void)
{

}

void ActorManager::DamegeBeamCollision(void)
{
	//当たり判定をとるプレイヤー(当てられる側)
	for (auto& player : players_)
	{
		//弾を打つプレイヤー(当てる側)
		for (auto& playerShots : players_)
		{
			auto& beams= playerShots->GetBeamRifle().GetBeams();
			//弾
			for (auto& beam : beams)
			{
				//プレイヤーと弾のプレイヤータイプが違えば抜ける
				if (IsEqual(player->GetPlayerNum(), beam->GetPlayerNum()))
				{
					break;
				}
				//弾がアクティブ状態でない、弾と当たってないなら戻る
				if (!(beam->IsActive()) ||
					!player->GetCapsule().Collision(beam->GetModelId()))
				{
					continue;
				}			
				beam->Hit();

				//無敵時間が終わっていたらスーパーアーマーを削る
				if (!player->IsSafeTimeSufficient())
				{
					continue;
				}
				player->DamageSuperArmor();

				//当たった時にスーパーアーマーが無く、格闘移行状態だったら
				if (player->IsSuperArmor())
				{
					continue;
				}
				//格闘移行状態なら大ダメージとダウン状態
				if(player->GetState() == Player::STATE::COMBAT_RUN)
				{
					//ダメージを反映しプレイヤーをダメージステートに変更
					player->Damage(beam->GetBigDamage());
					player->ChangeDwonState();
				}
				//その他なら通常ダメージ
				else
				{
					//ダメージを反映しプレイヤーをダメージステートに変更
					player->Damage(beam->GetDamage());
					player->ChangeDamageState();
				}
			}
		}
	}
}

void ActorManager::DamegeSwordCollision(void)
{
	for (auto& player : players_)
	{
		for (auto& sabers : players_)
		{
			auto& saber = sabers->GetBeamSaber().GetSaber();			
			//プレイヤーと剣のプレイヤータイプが違うもしくは無敵時間が終わってないまたは剣がアクティブ状態でない
			if (IsEqual(player->GetPlayerNum() , sabers->GetPlayerNum()) ||!(saber.IsActive())
				|| !player->IsSafeTimeSufficient())
			{	
				continue;
			}
			if (player->GetCapsule().Collision(saber.GetModelId()))
			{
				saber.Hit();
				player->Damage(saber.GetDamage());
				player->ChangeDwonState();
			}
		}
	}
}

void ActorManager::DmageBossSpMoveCollision(void)
{
	int SINGLE_PLAY = 0;
	auto& spmoves = bossEnemy_->GetSpMove();
	for (auto& spmove : spmoves)
	{
		//ボスの技がアクティブ状態の時もしくはプレイヤーの無敵時間が終わっていなかったら
		if (!spmove.second->IsActive()|| !players_[SINGLE_PLAY]->IsSafeTimeSufficient())
		{
			continue;
		}
		//わざとップレイヤーが当たっていたらプレイヤーをダウン状態にしてHPを減らす
		if (players_[SINGLE_PLAY]->GetCapsule().Collision(spmove.second->GetModelId()))
		{
			players_[SINGLE_PLAY]->SetSafeTime(30.0f);
			players_[SINGLE_PLAY]->ChangeDwonState();
			players_[SINGLE_PLAY]->Damage(spmove.second->GetDamage());
		}
	}
}

void ActorManager::DmageBossBeamCollision(void)
{
	int SINGLE_PLAY = 0;

	auto& beams= players_[SINGLE_PLAY]->GetBeamRifle().GetBeams();
	for (auto& beam : beams)
	{
		//弾がアクティブ状態じゃないもしくは、クールタイムが終わっていなければ
		if (!(beam->IsActive()))
		{
			continue;
		}
		if (bossEnemy_->GetCapsule().Collision(beam->GetModelId()))
		{
			beam->Hit();
			bossEnemy_->Damage(beam->GetDamage());
		}
	}
	
}

void ActorManager::InitUI(void)
{
	for (auto& player : players_)
	{
		userInterface_.emplace_back(player->MoveUI());
	}
}

const bool& ActorManager::IsDeadAnyPlayer(void)const
{
	return isDeadAnyPlayer_;
}

void ActorManager::IsWin(void)
{
	if (isDeadAnyPlayer_)
	{
		return;
	}
	for (auto& player : players_)
	{
		//体力がゼロになるもしくはステージから落ちたら負けとする
		if (0.0f>= player->GetPlayerHP() || StageOut(player->GetPlayerPos()))
		{
			loserNum_ = player->GetPlayerNum() ;
			winnerNum_ = 1 - loserNum_;
			ChangeStateGameSet();
			isDeadAnyPlayer_ = true;
		}
	}
}

void ActorManager::IsSingleModeWin(void)
{
	if (isDeadAnyPlayer_)
	{
		return;
	}
	int SINGLE_PLAY = 0;
	//体力がゼロになるもしくはステージから落ちたら負けとする
	if (0.0f >= players_[SINGLE_PLAY]->GetPlayerHP() || StageOut(players_[SINGLE_PLAY]->GetPlayerPos()))
	{
		players_[SINGLE_PLAY]->Lose();
		isDeadAnyPlayer_ = true;
	}
	else if(0.0f >= bossEnemy_->GetPlayerHP())
	{
		isDeadAnyPlayer_ = true;
		players_[SINGLE_PLAY]->Win();
		bossEnemy_->ChangeDeathState();
	}
	else
	{
	}
}

bool ActorManager::StageOut(const VECTOR& pos) 
{
	if (pos.y < STAGE_OUT_Y)
	{	
		return true;
	}
	return false;
}

void ActorManager::ChangeStateGameSet(void)
{
	players_[loserNum_]->Lose();
	players_[winnerNum_]->Win();
}


bool ActorManager::IsEqual(int playerNum, int damageNum)
{
	if (playerNum == damageNum)
	{
		return true;
	}
	return false;
}
