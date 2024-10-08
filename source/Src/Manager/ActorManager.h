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
	void DrawCommonUI(const float&startCount, const bool& isGameSet,const int& rematchMode);

	//�R���W�����̒ǉ�
	void AddClliders(Collider* collider);	
	
	//�e�Ƃ̓�����͔���
	void DamegeShotCollision(void);
	
	//���Ƃ̓����蔻��
	void DamegeSwordCollision(void);
	
	//�{�X�̍U���Ƃ̓����蔻��
	void DmageBossSpMoveCollision(void);

	//�{�X�ƒe�Ƃ̓����蔻��
	void DmageBossBeamCollision(void);

	//�����ꂩ�̃v���C���[�����񂾂��ǂ�����Ԃ�
	const bool& IsDeadAnyPlayer(void)const;

	//�f�o�b�O�p
	void DrawDebug(int playerType);
	void DrawAnimeDebug(int playerType);

private:

	void (ActorManager::* _update)();

	std::vector<std::unique_ptr<Player>> players_;

	//�r�[�����C�t����r�[���T�[�x��
	std::vector<std::unique_ptr<DamageObject>> damageObject_;

	//HP��u�[�X�g�Q�[�W�̕\��
	std::vector<std::unique_ptr<UserInterface>> userInterfaces_;

	//��l�p���[�h�G
	std::unique_ptr<BossEnemy> bossEnemy_;

	//�ǂ��炩�̃v���C���[�����񂾂��ǂ����𔻒�
	bool isDeadAnyPlayer_;

	int loserPlayerType_;

	int winnerPlayerType_;

	//�v���C���[�h
	int playMode_;

	//���҂����܂������ǂ����𔻒�
	void IsWin(void);

	//���҂����܂������ǂ����𔻒�(�V���O�����[�h)
	void IsSingleModeWin(void);

	//�ΐ탂�[�h������
	void InitBattleMode(int numberofPlayers);

	//�V���O�����[�h������
	void InitSIngleMode(int numberofPlayers);

	//��O�ɗ��������ǂ���
	bool StageOut(const VECTOR& pos);

	//�e�v���C���[�������A�s�k��Ԃɕς���
	void ChangeStateGameSet(void);

	//
	bool IsEqual(int playerNum, int damageNum);

	//�ΐ탂�[�h��UI
	void CreateBattleUI(void);
	//�V���O�����[�h��UI
	void CreateSingleUI(void);

};
