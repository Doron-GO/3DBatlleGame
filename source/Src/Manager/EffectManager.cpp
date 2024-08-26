#include "EffectManager.h"
#include<EffekseerForDXLib.h>
#include"../Manager/ResourceManager.h"

EffectManager::EffectManager(Transform& transform):transform_(transform)
{
}

EffectManager::~EffectManager()
{
	for (auto& effect: efect_)
	{
		Stop(effect.second.playHandleId_);
	}
}

void EffectManager::Add(int type, VECTOR scale, bool loop,int handlId)
{
	//アニメーションタイプが入ってない場合？ここ確認
	if (efect_.count(type) == 0)
	{
		// 追加
		efect_[type].resHandleId_ = handlId;
		efect_[type].scale_ = scale;
		efect_[type].isLoop = loop;
	}
}

void EffectManager::Update()
{
	for (auto& efect : efect_)
	{
		if (!efect_[efect.first].isPlay_)
		{
			continue;
		}
		//大きさ
		SetScalePlayingEffekseer3DEffect(efect_[efect.first].playHandleId_, efect_[efect.first].scale_.x, efect_[efect.first].scale_.y, efect_[efect.first].scale_.z);
		//角度
		SetRotationPlayingEffekseer3DEffect(efect_[efect.first].playHandleId_, transform_.rot.x, transform_.rot.y, transform_.rot.z);
		//位置
		SetPosPlayingEffekseer3DEffect(efect_[efect.first].playHandleId_, transform_.pos.x, transform_.pos.y, transform_.pos.z);
		if (!(efect_[efect.first].isLoop) &&IsEffekseer3DEffectPlaying(efect_[efect.first].playHandleId_)==-1)
		{
			StopEffekseer3DEffect(efect_[efect.first].playHandleId_);
			efect_[efect.first].isPlay_ = false;
		}
		
	}

}

void EffectManager::Play(int type)
{
	if (!efect_[type].isPlay_)
	{
		efect_[type].isPlay_ = true;
	}
	efect_[type].playHandleId_=  PlayEffekseer3DEffect(efect_[type].resHandleId_);
	
}

void EffectManager::Stop(int type)
{
	StopEffekseer3DEffect(efect_[type].playHandleId_);
	efect_[type].isPlay_ = false;

}
