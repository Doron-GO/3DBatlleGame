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
		int model_ = -1;
		int attachNo_ = -1;
		int animIndex_ = 0;
		float speed_ = 0.0f;
		float totalTime_ = 0.0f;
		float step_ = 0.0f;
		bool priority_ = false;
		bool isLoop_ = false;
		bool isStop_ = false;
		bool isEnd = false;
	};

	struct Blend
	{
		int attachNo_ = -1;//ブレンドされるアニメーションのアタッチ番号
		STATE type_ = STATE::NONE;		//ブレンドされるアニメーション
		bool blendFlag_ = false;//ブレンド中かどうか
		float rate_ = 0;	//ブレンド率
		float stepBlend_ = 0;//ブレンド率加算
		float blendTime_ = 0;//ブレンドりつを0～1にするためのもの
	};


	int upplayType_;
	int lowlayType_;


	RobotAnimeController(int modelId);
	~RobotAnimeController();

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

	float blendTime_;

	float deltaTime_;

	void UpperBodyUpdate(void);

	void LowerBodyUpdate(void);

	void DetachAnim(void);

};

