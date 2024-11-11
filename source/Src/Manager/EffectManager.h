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
		//再生中かどうか
		bool isPlay_ =false;
		//ループ再生かどうか
		bool isLoop = false;
		//プレイヤーのジェットのように回転などに応じて、座標もも追従させるかどうか
		bool isSync_ = false;
		//座標調整用数値
		VECTOR offset_;
		//座標
		VECTOR pos_;
		//回転
		VECTOR rot_;

	};

	EffectManager(const Transform& transform);
	~EffectManager();
	//エフェクトの追加
	void Add(int type, VECTOR scale,VECTOR offset, VECTOR rot,bool loop, bool isSync,int handlId);
	//アップデート
	void Update();
	//エフェクトを再生する
	void Play(int type);
	//エフェクトの再生を止める
	void Stop(int type);
	//エフェクトの再生速度を設定する
	void SetSpeed(int type, float speed);



private:

	const Transform& transform_;

	// 種類別のアニメーションデータ
	//キー：エフェクトタイプ,バリュー：エフェクトデータ
	std::map<int, EffectData> efects_;

};

