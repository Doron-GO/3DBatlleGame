#include"../../Utility/AsoUtility.h"
#include "BeamSaber.h"
#include<DxLib.h>


#pragma region Parameter

constexpr VECTOR BEAM_SABER_SCALE = { 0.02f,0.02f,0.02f };

constexpr VECTOR BEAM_SABER_LOCAL_ROT = { 0.0f,0.0f,-90.0f };


#pragma endregion




BeamSaber::BeamSaber(int playerType, Transform& player):WeaponBase(playerType, player)
{
	//���f���̓ǂݍ���
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::BEAMSABER));
	//���W
	transform_.pos = MV1GetFramePosition(playerTransform_.modelId, RIGHT_HAND_FRAME);
	//�傫��
	transform_.scl = { 0.02f,0.02f,0.02f };
	//��]
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler(BEAM_SABER_LOCAL_ROT.x,
			BEAM_SABER_LOCAL_ROT.y,
			AsoUtility::Deg2RadF(BEAM_SABER_LOCAL_ROT.z));
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
	if (isActive_)
	{
		MV1DrawModel(transform_.modelId);
	}	
}

Saber& BeamSaber::GetSaber(void) const
{
	//�����蔻��N���XSaber���擾����
	return *saber_;
}

