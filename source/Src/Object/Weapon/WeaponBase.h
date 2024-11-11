#pragma once
#include"../Common/Transform.h"
#include"../../Manager/ResourceManager.h"

class Player;

class WeaponBase
{
public:

	//�E��e�w�t���[���ԍ�
	static constexpr int RIGHT_HAND_THUMB_FRAME = 96;

	//���f���̍����t���[���ԍ�
	static constexpr int ATTACH_LEFT_HAND = 71;

	//���f���̉E��t���[���ԍ�
	static constexpr int RIGHT_HAND_FRAME = 93;

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

	void AddCollider(Collider* collider);

	const Transform& GetTransform(void) const;


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
	bool isActive_;

	//�X�e�[�W�Ȃǂ̏�Q�������蔻��p
	std::vector<Collider*>colliders_;


};

