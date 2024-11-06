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
	//アニメーションタイプが入ってない場合？ここ確認
	if (efects_.count(type) == 0)
	{
		//エフェクトのハンドル
		efects_[type].resHandleId_ = handlId;
		//大きさ
		efects_[type].scale_ = scale;
		//オフセット
		efects_[type].offset_ = offset;
		//回転
		efects_[type].rot_ = rot;
		//トランスフォームの回転と同期させるか判定
		efects_[type].isSync_ = isSync;
		//ループ再生フラグ
		efects_[type].isLoop = loop;
	}
}

void EffectManager::Update()
{
	for (auto& efect : efects_)
	{
		//再生中フラグたtrueならアップデートに入る」
		if (!efects_[efect.first].isPlay_)
		{
			continue;
		}

		//同期させる座標
		VECTOR pos = transform_.pos;

		if (efects_[efect.first].isSync_)
		{
			//座標を回転させる
			const VECTOR lacalPos = transform_.quaRot.PosAxis(efects_[efect.first].offset_);
			pos = VAdd(transform_.pos, lacalPos);
		}
		else
		{	//座標のみ同期(回転無し)
			pos = VAdd(transform_.pos, efects_[efect.first].offset_);
		}

		//オイラー角からクォータニオンに変換
		Quaternion eRot= Quaternion::Euler(efects_[efect.first].rot_);

		//エフェクトの回転を同期先の回転と合成
		eRot = transform_.quaRot.Mult(eRot);

		//オイラー角に変換
		VECTOR rot= Quaternion::ToEuler(eRot);

		//位置を設定
		SetPosPlayingEffekseer3DEffect(efects_[efect.first].playHandleId_,
			pos.x, pos.y, pos.z);
		
		//角度を設定
		SetRotationPlayingEffekseer3DEffect(efects_[efect.first].playHandleId_,
			rot.x, rot.y,rot.z);

		//大きさを設定
		SetScalePlayingEffekseer3DEffect(efects_[efect.first].playHandleId_,
			efects_[efect.first].scale_.x, efects_[efect.first].scale_.y, efects_[efect.first].scale_.z);

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
	{	
		//再生中フラグをtrueにする
		efects_[type].isPlay_ = true;

		//再生
		efects_[type].playHandleId_=  PlayEffekseer3DEffect(efects_[type].resHandleId_);

		//同期させる座標
		VECTOR pos = transform_.pos;

		if (efects_[type].isSync_)
		{
			//座標を回転させる
			const VECTOR lacalPos = transform_.quaRot.PosAxis(efects_[type].offset_);
			pos = VAdd(transform_.pos, lacalPos);
		}
		else
		{
			//座標のみ同期
			pos = VAdd(transform_.pos, efects_[type].offset_);
		}

		//オイラー角からクォータニオンに変換
		Quaternion eRot = Quaternion::Euler(efects_[type].rot_);

		//エフェクトの回転を同期先の回転と合成
		eRot = transform_.quaRot.Mult(eRot);

		//オイラー角に変換
		VECTOR rot = Quaternion::ToEuler(eRot);

		//位置
		SetPosPlayingEffekseer3DEffect(efects_[type].playHandleId_,
			pos.x, pos.y, pos.z);

		//角度
		SetRotationPlayingEffekseer3DEffect(efects_[type].playHandleId_,
			rot.x, rot.y, rot.z);

		//大きさ
		SetScalePlayingEffekseer3DEffect(efects_[type].playHandleId_,
			efects_[type].scale_.x, efects_[type].scale_.y, efects_[type].scale_.z);

	}
	
}

void EffectManager::Stop(int type)
{
	//再生を中止
	StopEffekseer3DEffect(efects_[type].playHandleId_);
	efects_[type].isPlay_ = false;

}

void EffectManager::SetSpeed(int type, float speed)
{
	//エフェクトの再生速度を設定する
	SetSpeedPlayingEffekseer3DEffect(efects_[type].playHandleId_, speed);
}
