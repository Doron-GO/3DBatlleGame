#include "ActorManager.h"
#include"../Object/Actor/ActorBase.h"
#include"../Object/Actor/Enemy/BossEnemy.h"
#include"../Object/Actor/Player/Player.h"
#include"../Object/DamageObject/Saber.h"
#include"../Object/Common/Collider.h"
#include"../UI/UserInterface.h"

#pragma region Parameter

//�v���C���[�P�̃^�C�v
constexpr int PLAYER_TYPE = 0;

constexpr int SINGLE_PLAY = 0;

constexpr float STAGE_OUT_Y = -1000.0f;

#pragma endregion



ActorManager::ActorManager(int playMode)
{
	playMode_ = playMode;
	InitActor();
	if (playMode_ == SINGLE_PLAY)
	{
		//InitSingleMode();
		//_update = &ActorManager::UpdateSingleMode;
		update_ = std::bind(&ActorManager::UpdateSingleMode, this);
	}
	else
	{	
		//InitActor();
		//_update = &ActorManager::UpdateBattleMode;
		update_ = std::bind(&ActorManager::UpdateBattleMode, this);
	}
	//�ǂ��炩�̃v���C���[�����񂾂��ǂ����𔻒�
	isDeadAnyPlayer_ = false;
}

void ActorManager::InitActor(void)
{
	// �v���C���[�̐����Ə�����
	for (int playerType = 0; playerType <= playMode_; playerType++)
	{
		std::unique_ptr player = std::make_unique<Player>(playerType, playMode_);
		//�v���C���[�̏�����
		player->Init();
		//�z��Ɋi�[
		players_.emplace_back(std::move(player));
	}

	//�v���C���[1
	int player1 = static_cast<int>(ActorBase::ACTOR_TYPE::PLAYER_1);

	//�V���O�����[�h�Ȃ�v���C���[�P��UI�ƃ{�X�̏���ݒ�
	if (playMode_== SINGLE_PLAY)
	{
		//�{�X�̐���
		int boss = static_cast<int>(ActorBase::ACTOR_TYPE::BOSS);
		bossEnemy_ = std::make_unique<BossEnemy>(boss);


		//�v���C���[�̃p�����[�^���{�X���ɓn��
		SetEnemyInfo(*bossEnemy_, *players_[player1]) ;

		//�{�X���̃p�����[�^���v���C���[�ɓn��
		SetEnemyInfo(*players_[player1], *bossEnemy_);

		//�{�X�̍U���I�u�W�F�N�g����
		bossEnemy_->MakeSpMoveObjects();

		//�v���C���[1��UI�𐶐�
		//CreateUserInterface(*players_[player1], *bossEnemy_);

	}
	else
	{
		//�v���C���[2
		int player2 = static_cast<int>(ActorBase::ACTOR_TYPE::PLAYER_2);

		//�G�v���C���[�̃p�����[�^�������Ă���
		SetEnemyInfo(*players_[player1], *players_[player2]);
		SetEnemyInfo(*players_[player2], *players_[player1]);

		//�v���C���[�P�ƃv���C���[�Q��UI�𐶐�
		//CreateUserInterface(*players_[player1], *players_[player2]);
		//CreateUserInterface(*players_[player2], *players_[player1]);

	}
}

void ActorManager::Update()
{
	for (auto& player : players_)
	{
		player->Update();
	}
	//(this->*_update)();
	update_();
}

void ActorManager::UpdateBattleMode(void)
{
	//�e�Ƃ̓����蔻��
	DamegeShotCollision();
	//���Ƃ̓����蔻��
	DamegeSwordCollision();
	//���҂����܂������ǂ����𔻒�
	IsWin();
}

void ActorManager::UpdateSingleMode(void)
{
	//�{�X�̃A�b�v�f�[�g
	bossEnemy_->Update();
	//�{�X�̍U���Ƃ̓����蔻��
	DmageBossSpMoveCollision();
	//�{�X�ƒe�Ƃ̓����蔻��
	DmageBossBeamCollision();
	//���҂����܂������ǂ����𔻒�
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

const std::vector<std::unique_ptr<Player>>& ActorManager::GetPlayers(void)const
{
	return players_;
}

const std::unique_ptr<BossEnemy>& ActorManager::GetBossEnemy(void)const
{
	return bossEnemy_;
}

std::vector<std::unique_ptr<UserInterface>> ActorManager::MoveUI(void)
{
	return std::move(userInterfaces_);
}

bool& ActorManager::IsDeadAnyPlayer(void)
{
	return isDeadAnyPlayer_;
}

void ActorManager::DrawCamera(int playerType)
{
	players_[playerType]->CameraDraw();
}

void ActorManager::DrawUI(int playerType)
{
	userInterfaces_[playerType]->Draw(isDeadAnyPlayer_);
}

void ActorManager::DrawCommonUI(const float& startCount, const bool& isGameSet, const int& rematchMode)
{
	userInterfaces_[PLAYER_TYPE]->DrawCommonUI(startCount, isGameSet, rematchMode);
}


void ActorManager::AddColliders(Collider* collider)
{
	for (auto& player : players_)
	{
		player->AddCollider(collider);
	}
}


void ActorManager::DamegeShotCollision(void)
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
		//�Z�ƃv���C���[���������Ă�����v���C���[���_�E����Ԃɂ���HP�����炷
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
		if (0.0f>= player->GetHP() || StageOut(player->GetPlayerPos()))
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
	if (0.0f >= players_[SINGLE_PLAY]->GetHP() || StageOut(players_[SINGLE_PLAY]->GetPlayerPos()))
	{	//�s�k��ԂɈڍs
		players_[SINGLE_PLAY]->Lose();
		isDeadAnyPlayer_ = true;
	}
	else if(0.0f >= bossEnemy_->GetHP())
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

bool ActorManager::IsEqual(int Type1, int Type2)
{	
	//�^�C�v�P�ƃ^�C�v�Q�����^�C�v���ǂ����𔻒�
	if (Type1 == Type2)
	{
		return true;
	}
	return false;
}

void ActorManager::SetEnemyInfo(ActorBase& player, ActorBase& target)
{
	//�G�v���C���[�̍��W�AHP�A�X�e�[�g��ݒ�
	if (player.GetActorType() != ActorBase::ACTOR_TYPE::BOSS)
	{
		//�G��HP�����Z�b�g
		player.SetEnemyHp(&target.GetHP());
		//�G�̃X�e�[�g�����Z�b�g
		player.SetEnemyState(&target.GetState());
	}
	//�G�̍��W�����Z�b�g
	player.SetEnemyPosition(&target.GetTransform().pos);
}

void ActorManager::CreateUserInterface(ActorBase& player, ActorBase& target)
{

	//�v���C���[�^�C�v
	int playerType = static_cast<int>(player.GetActorType());

	//UI�𐶐�
	userInterfaces_.emplace_back(
		std::make_unique<UserInterface>(
			playMode_,
			playerType,
			player.GetEnemyDistance(),
			player.GetNumnberOfBullets(),
			player.GetBoostFuel(),
			player.GetHP(),
			player.IsWin(),
			target.GetTransform().pos,
			target.GetHP()
			)
		);
}


