#pragma once
#include"../Object/Actor/Enemy/BossEnemy.h"
#include"../Object/Actor/Player/Player.h"
#include<memory>
#include<vector>
#include<DxLib.h>

class Collider;
class DamageObject;
class UserInterface;

class ActorManager
{
public:


	ActorManager(int numberofPlayers);
	
	//�A�b�v�f�[�g
	void Update();

	//�ΐ탂�[�h�A�b�v�f�[�g
	void UpdateBattleMode(void);
	//�V���O�����[�h�A�b�v�f�[�g
	void UpdateSingleMode(void);

	//�`��
	void Draw(void);

	//�J�����`��
	void DrawCamera(int playerType);
	
	//�c�e����HP�Q�[�W���̕\��
	void DrawUI(int playerType);

	//�Q�[���X�^�[�g�Ȃǂ̋���UI
	void DrawCommonUI(const float&startCount, const bool& isGameSet,const float& rematchMode);

	//�R���W�����̒ǉ�
	void AddClliders(Collider* collider);
	
	//
	void SetEnemyInfo(void);
	
	
	void SetBossEnemyInfo(void);
	
		
	
	void DamegeBeamCollision(void);
	
	
	void DamegeSwordCollision(void);
	
	
	void DmageBossSpMoveCollision(void);
	void DmageBossBeamCollision(void);
	void InitUI(void);
	const bool& IsDeadAnyPlayer(void)const;

	//�f�o�b�O�p
	void DrawDebug(int playerType);
	void DrawAnimeDebug(int playerType);

private:

	void (ActorManager::* _update)();

	std::vector< std::unique_ptr<Player>>players_;

	//�r�[�����C�t����r�[���T�[�x��
	std::vector<std::unique_ptr<DamageObject>>damageObject_;

	//HP��u�[�X�g�Q�[�W�̕\��
	std::vector< std::unique_ptr<UserInterface>>userInterface_;

	//��l�p���[�h�G
	std::unique_ptr<BossEnemy> bossEnemy_;

	//�ǂ��炩�̃v���C���[�����񂾂��ǂ����𔻒�
	bool isDeadAnyPlayer_;

	int loserPlayerType_;

	int winnerPlayerType_;

	//�v���C���[�h
	int playMode_;

	void IsWin(void);

	void IsSingleModeWin(void);

	void InitBattleMode(int numberofPlayers);

	void InitSIngleMode(int numberofPlayers);

	bool StageOut(const VECTOR& pos);

	void ChangeStateGameSet(void);

	bool IsEqual(int playerNum, int damageNum);
};

