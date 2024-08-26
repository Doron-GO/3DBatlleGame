#include "Scene.h"
#include<DxLib.h>
Scene::Scene(SceneManager& scene,Transitor& transit, Input& input):sceneManager_(scene),sceneTransitor_(transit), input_(input)
{
	playMode_ = 1;
}

void Scene::Draw()
{
}
