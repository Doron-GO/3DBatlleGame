#include"../../Utility/AsoUtility.h"
#include "BeamSaber.h"
#include<DxLib.h>

BeamSaber::BeamSaber(int playerType, Transform& player):WeaponBase(playerType, player)
{
	//���f���̓ǂݍ���
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::BEAMSABER));
	//���W
	transform_.pos = MV1GetFramePosition(playerTransform_.modelId, ATTACH_RIGHT_HAND_FRAME);
	//�傫��
	transform_.scl = { 0.02f,0.02f,0.02f };
	//��]
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler(AsoUtility::Deg2RadF(0.0f),0.0f, AsoUtility::Deg2RadF(-90.0f));
	transform_.Update();
	//�����蔻��N���X�̐���
	saber_ = std::make_unique<Saber>(playerType,transform_);

}

void BeamSaber::Update(void)
{
	//���W��]�̓���
	SyncPosition();
	//�����蔻��N���X�̍X�V
	saber_->Update(transform_.pos);
}

void BeamSaber::Draw(void)
{
	//�L��������Ă�����`�悷��
	if (activeFlag_)
	{
		MV1DrawModel(transform_.modelId);
	}	
}

Saber& BeamSaber::GetSaber(void) const
{
	//�����蔻��N���XSaber���擾����
	return *saber_;
}

