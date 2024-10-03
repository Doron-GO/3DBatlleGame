#include "EffectManager.h"
#include<EffekseerForDXLib.h>
#include"../Manager/ResourceManager.h"

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

void EffectManager::Add(int type, VECTOR scale, bool loop,int handlId)
{
	//�A�j���[�V�����^�C�v�������ĂȂ��ꍇ�H�����m�F
	if (efects_.count(type) == 0)
	{
		// �ǉ�
		efects_[type].resHandleId_ = handlId;
		efects_[type].scale_ = scale;
		efects_[type].isLoop = loop;
	}
}

void EffectManager::Update()
{
	for (auto& efect : efects_)
	{
		if (!efects_[efect.first].isPlay_)
		{
			continue;
		}
		//�傫��
		SetScalePlayingEffekseer3DEffect(efects_[efect.first].playHandleId_, efects_[efect.first].scale_.x, efects_[efect.first].scale_.y, efects_[efect.first].scale_.z);
		//�p�x
		SetRotationPlayingEffekseer3DEffect(efects_[efect.first].playHandleId_, transform_.rot.x, transform_.rot.y, transform_.rot.z);
		//�ʒu
		SetPosPlayingEffekseer3DEffect(efects_[efect.first].playHandleId_, transform_.pos.x, transform_.pos.y, transform_.pos.z);
		
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
	{	//�Đ����t���O��true�ɂ���
		efects_[type].isPlay_ = true;
	}
	//�Đ�
	efects_[type].playHandleId_=  PlayEffekseer3DEffect(efects_[type].resHandleId_);
	
}

void EffectManager::Stop(int type)
{
	//�Đ��𒆎~
	StopEffekseer3DEffect(efects_[type].playHandleId_);
	efects_[type].isPlay_ = false;

}
