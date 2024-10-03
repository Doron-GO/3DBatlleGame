#pragma once
#include <string>
#include <chrono>
#include <map>
#include"../Object/Time/DeltaTime.h"

class RobotAnimeController 
{
public:
	
	enum class Body
	{
		UP,
		LOW,
	};
	enum class STATE
	{
		NONE,
		RUN,
		JUMP,
		DAMAGE,
		DOWN,
		SHOT,
		IDLE,
		BOOST,
		BOOST_DASH,
		FALL,
		COMBAT1,
		COMBAT_RUN,
		WIN,
		LOSE
	};

	// アニメーションデータ
	struct Animation
	{
		//モデルID
		int modelId_ = -1;
		//アタッチ番号
		int attachNo_ = -1;
		//アニメーション番号
		int animIndex_ = 0;
		//再生スピード
		float speed_ = 0.0f;
		//アニメーションの総再生時間
		float totalTime_ = 0.0f;
		//アニメーションの現再生時間
		float step_ = 0.0f;
		//アニメーションの優先権
		bool priority_ = false;
		//ループするかどうか
		bool isLoop_ = false;
		//一回再生かどうか
		bool isStop_ = false;
		//一回再生が終わったか
		bool isEnd = false;
	};

	//切り替えられるアニメーション(古いほう)
	struct Blend
	{	
		//アニメーションのアタッチ番号
		int attachNo_ = -1;
		//アニメーションタイプ
		STATE type_ = STATE::NONE;
		//ブレンド中かどうか
		bool blendFlag_ = false;
		//ブレンド率
		float rate_ = 0;
		//ブレンド率加算
		float stepBlend_ = 0;
	};

	RobotAnimeController(int modelId);
	~RobotAnimeController();

	//総合アップデート
	void Update(void);

	void Draw(void);

	//アニメーションを追加する
	void Add(int type, const std::string& path, float speed,float totalTime);

	//上半身アニメーションを設定する
	void UpperBodyPlay(int type, bool priority=false,bool isLoop = true,
		bool isStop = false, float endStep = -1.0f,float startStep = 0.0f , bool isForce = false);
	
	//下半身アニメーションを設定する
	void LowerBodyPlay(int type, bool priority = false, bool isLoop = true,
		bool isStop = false, float endStep = -1.0f, float startStep = 0.0f, bool isForce = false);

	void DebugDraw();

	//上半身のアニメーションが再生が終わったかどうか(1ループ終わったら終了とする)
	bool IsUpperEnd(void);

	//下半身のアニメーションが再生が終わったかどうか(1ループ終わったら終了とする)
	bool IsLowerEnd(void);

	//全身のアニメーション再生が終わったかどうかを判定
	bool IsEnd(void);

private:

	// モデルのハンドルID
	int modelId_;
	
	//ブレンドされる側アニメーション情報
	std::map<Body, Blend> blend_;

	//現在再生中アニメーション
	std::map<Body,Animation > playAnim_;

	std::map<int, Animation> animations_;

	//現在のアニーションタイプが返ってくる
	std::map<Body, STATE> playType_;
	
	std::map<STATE, int> attachedTypeNum_;

	//すでにアタッチ済みのアニメーションかどうかが返ってくる
	//first:アニメーションタイプ second:アタッチしているかどうか (trueはアタッチされている)
	std::map<STATE, bool> isAttach_;

	//デルタタイム
	float deltaTime_;

	//上半身アニメーションアップデート
	void UpperBodyUpdate(void);

	//下半身アニメーションアップデート
	void LowerBodyUpdate(void);

	//アニメーションを外す
	void DetachAnim(void);

};

