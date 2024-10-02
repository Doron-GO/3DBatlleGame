#pragma once
#include"../Common/Vector2.h"
#include<memory>
#include<map>

class Input;
class SceneManager;
class Transitor;

/// <summary>
/// �V�[���̋K��N���X(����1�N���X
/// </summary>

class Scene
{
public:
	Scene(SceneManager& scene,Transitor& transit,Input& input);

	virtual ~Scene() {};

	virtual void Update() = 0;

	virtual void Draw() ;

protected:

	//�V�[���Ǘ�
	SceneManager& sceneManager_;
	
	//�V�[���J�ډ��o
	Transitor& sceneTransitor_;

	//�R���g���[������
	Input& input_;
};

