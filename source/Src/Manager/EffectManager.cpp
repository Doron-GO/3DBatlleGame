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
	//アニメーションタイプが入ってない場合？ここ確認
	if (efects_.count(type) == 0)
	{
		// 追加
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
		//大きさ
		SetScalePlayingEffekseer3DEffect(efects_[efect.first].playHandleId_, efects_[efect.first].scale_.x, efects_[efect.first].scale_.y, efects_[efect.first].scale_.z);
		//角度
		SetRotationPlayingEffekseer3DEffect(efects_[efect.first].playHandleId_, transform_.rot.x, transform_.rot.y, transform_.rot.z);
		//位置
		SetPosPlayingEffekseer3DEffect(efects_[efect.first].playHandleId_, transform_.pos.x, transform_.pos.y, transform_.pos.z);
		
		//エフェクトループ再生じゃなく、再生が終わっていたらエフェクトを止める
		if (!(efects_[efect.first].isLoop) &&IsEffekseer3DEffectPlaying(efects_[efect.first].playHandleId_)==-1)
		{
			//エフェクトの再生をストップ
			StopEffekseer3DEffect(efects_[efect.first].playHandleId_);
			//再生中フラグをfalseにする
			efects_[efect.first].isPlay_ = false;
		}
	}

}

void EffectManager::Play(int type)
{
	//すでに再生中のエフェクト出なければ再生する
	if (!efects_[type].isPlay_)
	{	//再生中フラグをtrueにする
		efects_[type].isPlay_ = true;
	}
	//再生
	efects_[type].playHandleId_=  PlayEffekseer3DEffect(efects_[type].resHandleId_);
	
}

void EffectManager::Stop(int type)
{
	//再生を中止
	StopEffekseer3DEffect(efects_[type].playHandleId_);
	efects_[type].isPlay_ = false;

}
