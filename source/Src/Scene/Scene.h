#pragma once
#include"../Common/Vector2.h"
#include<memory>
#include<map>

class Input;
class SceneManager;
class Transitor;
/// <summary>
/// シーンの規定クラス(抽象1クラス
/// </summary>

class Scene
{
public:
	Scene(SceneManager& scene,Transitor& transit,Input& input);
	virtual ~Scene() {};
	//virtual void Update(Input& input) = 0;
	virtual void Update() = 0;
	virtual void Draw() ;
protected:
	static constexpr int PAD_NUM = 1;

	int playMode_;
	SceneManager& sceneManager_;
	Transitor& sceneTransitor_;
	Input& input_;
	int screenID_;
};

