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

	virtual ~Scene(void) {};

	virtual void Update(void) = 0;

	virtual void Draw(void) ;

protected:

	//シーン管理
	SceneManager& sceneManager_;
	
	//シーン遷移演出
	Transitor& sceneTransitor_;

	//コントローラ入力
	Input& input_;
};

