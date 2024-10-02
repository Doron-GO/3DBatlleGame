#pragma once
#include"../../Manager/ResourceManager.h"
#include"../../Manager/EffectManager.h"
#include"../Comon/Transform.h"
#include<memory>

class ResourceManager;

class DamageObject
{
public:

	enum class DMG_OBJ_TYPE
	{
		NON,
		BEAM,
		STRONG_BEAM,
		SABEL
	};

	DamageObject(int playernum);
	//�`��
	virtual void Draw(void)=0;
	//�X�V
	virtual void Update(VECTOR enemyPos)=0;
	//�L����
	virtual void Activate(void);
	//��L����
	virtual void InActivate(void);

	//���W�̐ݒ�
	const void SetPos(VECTOR pos);
	//�v���C���[�^�C�v�̎擾
	virtual int GetPlayerType(void)const;
	//�_���[�W�ʂ̎擾
	virtual const float& GetDamage(void)const;
	//���f��ID�̎擾
	virtual const int& GetModelId(void)const;
	//�L����Ԃ��ǂ������擾
	virtual bool IsActive(void);

protected:

	//�q�b�g�G�t�F�N�g�̑傫��
	static constexpr VECTOR HIT_EFFECT_SCARE = { 35.0f,35.0f,35.0f };

	//���\�[�X�}�l�[�W���[
	ResourceManager& resMng_;

	//�g�����X�t�H�[��
	Transform transform_;

	//�L����ԉ��𔻒�
	bool activeFlag_ =false;

	//�G�t�F�N�g�Đ��}�l�[�W���[
	std::unique_ptr<EffectManager> effectManager_;

	//�_���[�W��
	float damage_;
	//�N�[���^�C��
	float coolTime_;

	//�f���^�^�C��
	float deltaTime_;

	//���f��ID
	int modelId_;

	//�v���C���[�^�C�v
	int playerType_;

private:

};

