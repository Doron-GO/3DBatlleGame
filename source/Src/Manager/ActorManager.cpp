#include "ActorManager.h"
#include"../Object/DamageObject/Saber.h"
#include"../Object/Comon/Collider.h"
#include"../UI/UserInterface.h"

#pragma region Parameter

//�v���C���[�P�̃^�C�v
constexpr int PLAYER_TYPE = 0;

static constexpr int PLAYER_NUM_MAX = 2;

static constexpr int PLAYER_NUM_MIN = 1;

static constexpr int SINGLE_PLAY = 0;


static constexpr float STAGE_OUT_Y = -1000.0f;

#pragma endregion



ActorManager::ActorManager(int numberofPlayers)
{
	playMode_ = numberofPlayers;
	if (playMode_ == SINGLE_PLAY)
	{
		InitSIngleMode(numberofPlayers);
		_update = &ActorManager::UpdateSingleMode;
	}
	else
	{	
		InitBattleMode(numberofPlayers);
		_update = &ActorManager::UpdateBattleMode;
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
	bossEnemy_->MakeSpMoveObjects();
}

void ActorManager::Update()
{
	for (auto& player : players_)
	{
		player->Update();
	}
	(this->*_update)();
}

void ActorManager::UpdateBattleMode(void)
{
	DamegeBeamCollision();
	DamegeSwordCollision();
	IsWin();
}

void ActorManager::UpdateSingleMode(void)
{
	bossEnemy_->Update();
	DmageBossSpMoveCollision();
	DmageBossBeamCollision();
	IsSingleModeWin();
}

void ActorManager::Draw(void)
{
	for (auto& player : players_)
	{
		player->Draw();
	}
	//�V���O�����[�h�Ȃ�{�X��`��
	if (playMode_ == SINGLE_PLAY)
	{
		bossEnemy_->Draw();
	}
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
	for (int idx = 0; idx < PLAYER_NUM_MAX; idx++)
	{
		//�ݒ肷�鑤�v���C���[�^�C�v
		int playerType1 = 1 - idx;
		//�ݒ肳��鑤�v���C���[�^�C�v
		int playerType2 = 0+ idx;
		if (playerType1 <= -1)
		{
			playerType1 = 0;
		}
		if (playerType2 >= 1)
		{
			playerType2 = 1;
		}
		//�G�v���C���[�̍��W�AHP�A�X�e�[�g��ݒ�
		players_[playerType1]->SetEnemyPosition(&(players_[playerType2]->GetTransform().pos));
		players_[playerType1]->SetEnemyHp(&(players_[playerType2]->GetPlayerHP()));
		players_[playerType1]->SetEnemyState(&(players_[playerType2]->pState_));
	}
}

void ActorManager::SetBossEnemyInfo(void)
{
	//�{�X�̍��W�AHP�A�X�e�[�g��ݒ�
	players_[SINGLE_PLAY]->SetEnemyPosition(&bossEnemy_->GetBossEnemyPos());
	players_[SINGLE_PLAY]->SetEnemyHp(&bossEnemy_->GetPlayerHP());
	players_[SINGLE_PLAY]->SetEnemyState(nullptr);
}

void ActorManager::DamegeBeamCollision(void)
{
	//�����蔻����Ƃ�v���C���[(���Ă��鑤)
	for (auto& player : players_)
	{
		//�e��łv���C���[(���Ă鑤)
		for (auto& playerShots : players_)
		{
			auto& beams= playerShots->GetBeamRifle().GetBeams();
			//�e
			for (auto& beam : beams)
			{
				//�v���C���[�ƒe�̃v���C���[�^�C�v���Ⴆ�Δ�����
				if (IsEqual(player->GetPlayerType(), beam->GetPlayerType()))
				{
					break;
				}
				//�e���A�N�e�B�u��ԂłȂ��A�e�Ɠ������ĂȂ��Ȃ�߂�
				if (!(beam->IsActive()) ||
					!player->GetCapsule().Collision(beam->GetModelId()))
				{
					continue;
				}			
				beam->Hit();

				//���G���Ԃ��I����Ă�����X�[�p�[�A�[�}�[�����
				if (!player->IsSafeTimeSufficient())
				{
					continue;
				}
				player->DamageSuperArmor();

				//�����������ɃX�[�p�[�A�[�}�[�������A�i���ڍs��Ԃ�������
				if (player->IsSuperArmor())
				{
					continue;
				}
				//�i���ڍs��ԂȂ��_���[�W�ƃ_�E�����
				if(player->GetState() == Player::STATE::COMBAT_RUN)
				{
					//�_���[�W�𔽉f���v���C���[���_���[�W�X�e�[�g�ɕύX
					player->Damage(beam->GetBigDamage());
					player->ChangeDwonState();
				}
				//���̑��Ȃ�ʏ�_���[�W
				else
				{
					//�_���[�W�𔽉f���v���C���[���_���[�W�X�e�[�g�ɕύX
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
			//�U�����̃T�[�x��
			auto& saber = sabers->GetBeamSaber().GetSaber();			
			//�@�v���C���[�ƌ��̃v���C���[�^�C�v���Ⴄ�A
			//�A���G���Ԃ��I����ĂȂ�
			//�B�����A�N�e�B�u��ԂłȂ�
			//��ł����Ă͂܂��Ă�����߂�
			if (IsEqual(player->GetPlayerType() , sabers->GetPlayerType()) ||!(saber.IsActive())
				|| !player->IsSafeTimeSufficient())
			{	
				continue;
			}
			//�T�[�x���ƃJ�v�Z�������蔻�肪�ڐG���Ă�����
			if (player->GetCapsule().Collision(saber.GetModelId()))
			{
				//�T�[�x�����A�N�e�B�u��
				saber.Hit();
				//�_���[�W
				player->Damage(saber.GetDamage());
				//�_�E����ԂɈڍs
				player->ChangeDwonState();
			}
		}
	}
}

void ActorManager::DmageBossSpMoveCollision(void)
{
	auto& spMoves = bossEnemy_->GetSpMove();
	for (auto& spmove : spMoves)
	{
		//�{�X�̋Z���A�N�e�B�u��Ԃ̎��������̓v���C���[�̖��G���Ԃ��I����Ă��Ȃ�������
		if (!spmove.second->IsActive()|| !players_[SINGLE_PLAY]->IsSafeTimeSufficient())
		{
			continue;
		}
		//�킴�ƃb�v���C���[���������Ă�����v���C���[���_�E����Ԃɂ���HP�����炷
		if (players_[SINGLE_PLAY]->GetCapsule().Collision(spmove.second->GetModelId()))
		{
			//���G���Ԃ�ݒ�
			players_[SINGLE_PLAY]->SetSafeTime(30.0f);
			//�_�E����ԂɈڍs
			players_[SINGLE_PLAY]->ChangeDwonState();
			//�_���[�W
			players_[SINGLE_PLAY]->Damage(spmove.second->GetDamage());
		}
	}
}

void ActorManager::DmageBossBeamCollision(void)
{
	auto& beams= players_[SINGLE_PLAY]->GetBeamRifle().GetBeams();
	for (auto& beam : beams)
	{
		//�e���A�N�e�B�u��Ԃ���Ȃ��������́A�N�[���^�C�����I����Ă��Ȃ����
		if (!(beam->IsActive()))
		{
			continue;
		}
		//�������Ă�����
		if (bossEnemy_->GetCapsule().Collision(beam->GetModelId()))
		{
			//�r�[�����A�N�e�B�u��
			beam->Hit();
			//�_���[�W
			bossEnemy_->Damage(beam->GetDamage());
		}
	}
	
}

void ActorManager::InitUI(void)
{
	//UI���v���C���[����󂯎��
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
{	//�ǂ��炩�̃v���C���[������ł�����߂�
	if (isDeadAnyPlayer_)
	{
		return;
	}
	for (auto& player : players_)
	{
		//�̗͂��[���ɂȂ�������̓X�e�[�W���痎�����畉���Ƃ���
		if (0.0f>= player->GetPlayerHP() || StageOut(player->GetPlayerPos()))
		{
			//�s�k���v���C���[�^�C�v���i�[
			loserPlayerType_ = player->GetPlayerType();
			//�������v���C���[�^�C�v���i�[
			winnerPlayerType_ = 1 - loserPlayerType_;
			//�e�v���C���[��������ԁA�s�k��ԂɈڍs
			ChangeStateGameSet();
			//�ǂ��炩���S�ς݃t���O��true
			isDeadAnyPlayer_ = true;
		}
	}
}

void ActorManager::IsSingleModeWin(void)
{	//�ǂ��炩�̃v���C���[������ł�����߂�
	if (isDeadAnyPlayer_)
	{
		return;
	}
	//�̗͂��[���ɂȂ�������̓X�e�[�W���痎�����畉���Ƃ���
	if (0.0f >= players_[SINGLE_PLAY]->GetPlayerHP() || StageOut(players_[SINGLE_PLAY]->GetPlayerPos()))
	{	//�s�k��ԂɈڍs
		players_[SINGLE_PLAY]->Lose();
		isDeadAnyPlayer_ = true;
	}
	else if(0.0f >= bossEnemy_->GetPlayerHP())
	{
		isDeadAnyPlayer_ = true;
		//������ԂɈڍs
		players_[SINGLE_PLAY]->Win();
		//�{�X�����S��ԂɈڍs
		bossEnemy_->ChangeDeathState();
	}
	else
	{
	}
}

bool ActorManager::StageOut(const VECTOR& pos) 
{
	//�v���C���[Y���W�����ȉ��ɂȂ�����
	if (pos.y < STAGE_OUT_Y)
	{	
		return true;
	}
	return false;
}

void ActorManager::ChangeStateGameSet(void)
{	//�s�k��ԁA������ԂɈڍs������
	players_[loserPlayerType_]->Lose();
	players_[winnerPlayerType_]->Win();
}


bool ActorManager::IsEqual(int playerNum, int damageNum)
{	//�U�����鑤�^�C�v�ƍU������鑤�^�C�v���������ǂ���
	if (playerNum == damageNum)
	{
		return true;
	}
	return false;
}
