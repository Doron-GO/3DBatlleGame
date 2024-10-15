#pragma once
#include<string>
#include<map>
#include"../Object/Common/Transform.h"


class EffectManager
{
public:

	struct EffectData
	{
		//リソースのハンドルID
		int resHandleId_;
		//リソースのハンドルDを使用して生成する再生用ハンドル
		int playHandleId_;
		//エフェクトの大きさ
		VECTOR scale_ ={1.0f,1.0f,1.0f};
		//
		bool isPlay_ =false;
		//
		bool isLoop = false;
	};

	EffectManager(Transform& transform);
	~EffectManager();
	void Add(int type, VECTOR scale, bool loop,int handlId);
	void Update();
	void Play(int type);
	void Stop(int type);



private:

	Transform& transform_;

	// 種類別のアニメーションデータ
	//キー：エフェクトタイプ,バリュー：エフェクトデータ
	std::map<int, EffectData> efects_;


};

