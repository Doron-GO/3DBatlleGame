#include "EffectManager.h"
#include<EffekseerForDXLib.h>
#include"../Manager/ResourceManager.h"
#include"../Utility/AsoUtility.h"

EffectManager::EffectManager(Transform& transform):transform_(transform)
{
}

EffectManager::~EffectManager()
{
	for (auto& effect: efects_)
	{
		Stop(effect.second.playHandleId_);
	}
}

void EffectManager::Add(int type, VECTOR scale,  VECTOR offset, VECTOR rot ,bool loop,bool isSync,int handlId)
{
	//�A�j���[�V�����^�C�v�������ĂȂ��ꍇ�H�����m�F
	if (efects_.count(type) == 0)
	{
		//�G�t�F�N�g�̃n���h��
		efects_[type].resHandleId_ = handlId;
		//�傫��
		efects_[type].scale_ = scale;
		//�I�t�Z�b�g
		efects_[type].offset_ = offset;
		//��]
		efects_[type].rot_ = rot;
		//�g�����X�t�H�[���̉�]�Ɠ��������邩����
		efects_[type].isSync_ = isSync;
		//���[�v�Đ��t���O
		efects_[type].isLoop = loop;
	}
}

void EffectManager::Update()
{
	for (auto& efect : efects_)
	{
		//�Đ����t���O��true�Ȃ�A�b�v�f�[�g�ɓ���v
		if (!efects_[efect.first].isPlay_)
		{
			continue;
		}

		//������������W
		VECTOR pos = transform_.pos;

		if (efects_[efect.first].isSync_)
		{
			//���W����]������
			const VECTOR lacalPos = transform_.quaRot.PosAxis(efects_[efect.first].offset_);
			pos = VAdd(transform_.pos, lacalPos);
		}
		else
		{	//���W�̂ݓ���(��]����)
			pos = VAdd(transform_.pos, efects_[efect.first].offset_);
		}

		//�I�C���[�p����N�H�[�^�j�I���ɕϊ�
		Quaternion eRot= Quaternion::Euler(efects_[efect.first].rot_);

		//�G�t�F�N�g�̉�]�𓯊���̉�]�ƍ���
		eRot = transform_.quaRot.Mult(eRot);

		//�I�C���[�p�ɕϊ�
		VECTOR rot= Quaternion::ToEuler(eRot);

		//�ʒu��ݒ�
		SetPosPlayingEffekseer3DEffect(efects_[efect.first].playHandleId_,
			pos.x, pos.y, pos.z);
		
		//�p�x��ݒ�
		SetRotationPlayingEffekseer3DEffect(efects_[efect.first].playHandleId_,
			rot.x, rot.y,rot.z);

		//�傫����ݒ�
		SetScalePlayingEffekseer3DEffect(efects_[efect.first].playHandleId_,
			efects_[efect.first].scale_.x, efects_[efect.first].scale_.y, efects_[efect.first].scale_.z);

		//�G�t�F�N�g���[�v�Đ�����Ȃ��A�Đ����I����Ă�����G�t�F�N�g���~�߂�
		if (!(efects_[efect.first].isLoop) &&IsEffekseer3DEffectPlaying(efects_[efect.first].playHandleId_)==-1)
		{
			//�G�t�F�N�g�̍Đ����X�g�b�v
			StopEffekseer3DEffect(efects_[efect.first].playHandleId_);
			//�Đ����t���O��false�ɂ���
			efects_[efect.first].isPlay_ = false;
		}
	}
	
}

void EffectManager::Play(int type)
{
	//���łɍĐ����̃G�t�F�N�g�o�Ȃ���΍Đ�����
	if (!efects_[type].isPlay_)
	{	
		//�Đ����t���O��true�ɂ���
		efects_[type].isPlay_ = true;

		//�Đ�
		efects_[type].playHandleId_=  PlayEffekseer3DEffect(efects_[type].resHandleId_);

		//������������W
		VECTOR pos = transform_.pos;

		if (efects_[type].isSync_)
		{
			//���W����]������
			const VECTOR lacalPos = transform_.quaRot.PosAxis(efects_[type].offset_);
			pos = VAdd(transform_.pos, lacalPos);
		}
		else
		{
			//���W�̂ݓ���
			pos = VAdd(transform_.pos, efects_[type].offset_);
		}

		//�I�C���[�p����N�H�[�^�j�I���ɕϊ�
		Quaternion eRot = Quaternion::Euler(efects_[type].rot_);

		//�G�t�F�N�g�̉�]�𓯊���̉�]�ƍ���
		eRot = transform_.quaRot.Mult(eRot);

		//�I�C���[�p�ɕϊ�
		VECTOR rot = Quaternion::ToEuler(eRot);

		//�ʒu
		SetPosPlayingEffekseer3DEffect(efects_[type].playHandleId_,
			pos.x, pos.y, pos.z);

		//�p�x
		SetRotationPlayingEffekseer3DEffect(efects_[type].playHandleId_,
			rot.x, rot.y, rot.z);

		//�傫��
		SetScalePlayingEffekseer3DEffect(efects_[type].playHandleId_,
			efects_[type].scale_.x, efects_[type].scale_.y, efects_[type].scale_.z);

	}
	
}

void EffectManager::Stop(int type)
{
	//�Đ��𒆎~
	StopEffekseer3DEffect(efects_[type].playHandleId_);
	efects_[type].isPlay_ = false;

}

void EffectManager::SetSpeed(int type, float speed)
{
	//�G�t�F�N�g�̍Đ����x��ݒ肷��
	SetSpeedPlayingEffekseer3DEffect(efects_[type].playHandleId_, speed);
}
