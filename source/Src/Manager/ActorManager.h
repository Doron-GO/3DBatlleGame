#pragma once
#include<functional>
#include<memory>
#include<vector>
#include<DxLib.h>

class Collider;
class DamageObject;
class UserInterface;
class ActorBase;
class Player;
class BossEnemy;

class ActorManager
{
public:


	ActorManager(int playMode);
	
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
	void AddColliders(Collider* collider);	
	
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

	//UI�𐶐�����Ƃ��Ɏg���Q�b�g�֐�
	const std::vector<std::unique_ptr<Player>>& GetPlayers(void)const;
	//UI�𐶐�����Ƃ��Ɏg���Q�b�g�֐�
	const std::unique_ptr<BossEnemy>& GetBossEnemy(void)const;

	std::vector<std::unique_ptr<UserInterface>>MoveUI(void);
	
	bool& IsDeadAnyPlayer(void);

private:

	//��ԊǗ�(�X�V�X�e�b�v)
	std::function<void(void)>update_;

	std::vector<std::unique_ptr<Player>> players_;

	//�r�[�����C�t����r�[���T�[�x��
	std::vector<std::unique_ptr<DamageObject>> damageObject_;

	//HP��u�[�X�g�Q�[�W�̕\��
	std::vector<std::unique_ptr<UserInterface>> userInterfaces_;

	//��l�p���[�h�G
	std::unique_ptr<BossEnemy> bossEnemy_;

	//�ǂ��炩�̃v���C���[�����񂾂��ǂ����𔻒�
	bool isDeadAnyPlayer_;

	//�s�k�v���C���[�^�C�v
	int loserPlayerType_;

	//�����v���C���[�^�C�v
	int winnerPlayerType_;

	//�v���C���[�h
	int playMode_;

	//���҂����܂������ǂ����𔻒�
	void IsWin(void);

	//���҂����܂������ǂ����𔻒�(�V���O�����[�h)
	void IsSingleModeWin(void);

	//�A�N�^�[�̏�����
	void InitActor(void);

	//��O�ɗ��������ǂ���
	bool StageOut(const VECTOR& pos);

	//�e�v���C���[�������A�s�k��Ԃɕς���
	void ChangeStateGameSet(void);

	//�^�C�v�P�ƃ^�C�v�Q�����^�C�v���ǂ����𔻒�
	bool IsEqual(int Type1, int Type2);

	//�G�̏����Z�b�g����
	void SetEnemyInfo(ActorBase& player, ActorBase& target);


};
