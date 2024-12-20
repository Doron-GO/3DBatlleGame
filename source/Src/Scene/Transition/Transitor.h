#pragma once
#include<DxLib.h>

class Transitor
{
public:
	Transitor(float interval = 120.0f) : interval_(interval) {}
	virtual ~Transitor();

	//演出開始
	virtual void Start();

	//Scene::Updateの最後に呼ぶ
	virtual void Update() = 0;

	//Scene::Drawの最後に呼ぶ
	virtual void Draw() = 0;

	//演出が終了した
	virtual bool IsEnd()const;

	int GetMainScreen()const;

protected:

	//切り替えにかかる時間
	const float interval_;

	//開始から現在までのフレーム数
	float frame_ = 0;

	//切り替え前の画面
	int oldRT_ = 0;

	//切り替え後の画面
	int newRT_ = 0;

	int mainScreen_ = 0;


};

