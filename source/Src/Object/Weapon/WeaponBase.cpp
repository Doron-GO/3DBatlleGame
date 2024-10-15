#include "WeaponBase.h"
#include"../Actor/Player/Player.h"

WeaponBase::WeaponBase(int playerType, const Transform& playerTransform):resMng_(ResourceManager::GetInstance()),
playerTransform_(playerTransform), playerType_(playerType)
{
	activeFlag_ = false;
}

void WeaponBase::Activate(void)
{
	//�L����
	activeFlag_ = true;
}

void WeaponBase::InActivate(void)
{
	//��L����
	activeFlag_ = false;
}

void WeaponBase::SyncPosition(void)
{
	//�E��̍s��
	MATRIX rightHandMat = MV1GetFrameLocalWorldMatrix(playerTransform_.modelId, ATTACH_RIGHT_HAND_FRAME);
	//����̃��[�J����]
	Quaternion quaLocal = transform_.quaRotLocal;
	//����̃��[�J�����W
	MATRIX offset = MGetTranslate(VECTOR{5.0f,43.0f,-3.0f });
	//����̃��[�J����]�ƃ��[�J�����W���E��̍s��ƍ���
	rightHandMat = MMult(quaLocal.ToMatrix(), MMult(offset, rightHandMat));
	//����̃��f���̍s���ݒ�
	MV1SetMatrix(transform_.modelId, MMult(transform_.matScl, rightHandMat));
	transform_.pos = MV1GetFramePosition(playerTransform_.modelId, 96);
}
