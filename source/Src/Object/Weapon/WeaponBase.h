#pragma once
#include"../Common/Transform.h"
#include"../../Manager/ResourceManager.h"

class Player;

class WeaponBase
{
public:

	//���f���̉E��t���[���ԍ�
	static constexpr int ATTACH_RIGHT_HAND_FRAME = 93;
	
	//���f���̍����t���[���ԍ�
	static constexpr int ATTACH_LEFT_HAND = 71;

	WeaponBase(int playerType, const Transform& playerTransform);
	virtual ~WeaponBase(void) {};

	//�X�V
	virtual void Update(void) = 0;

	//�`��
	virtual void Draw(void)=0;

	//�L��������
	virtual void Activate(void);

	//��L��������
	virtual void InActivate(void);

	//���{�b�g���f���ƍ��W�A��]�𓯊�
	virtual void SyncPosition(void);

protected:

	// �V���O���g���Q��
	ResourceManager& resMng_;

	//�g�����X�t�H�[��
	Transform transform_;

	//�v���C���[�̃g�����X�t�H�[��
	const Transform& playerTransform_;

	//�v���C���[�̃^�C�v
	int playerType_;

	//�L�����ǂ����𔻒�
	bool activeFlag_;

};

