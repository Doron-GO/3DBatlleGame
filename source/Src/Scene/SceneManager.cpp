#include "SceneManager.h"
#include"../Scene/Scene.h"
#include"../_debug/_DebugConOut.h"
#include"../_debug/_DebugDispOut.h"
#include<EffekseerForDXLib.h>

void SceneManager::ChangeScene(std::shared_ptr<Scene> scene)
{
	if (scenes_.empty())
	{
		scenes_.push_front(scene);
	}
	else
	{
		scenes_.front() = (scene);
	}
}

void SceneManager::PopScene(void)
{
	if (scenes_.size() > 1)
	{
		scenes_.pop_front();
	}

}

void SceneManager::PushScene(std::shared_ptr<Scene>scene)
{
	scenes_.push_front(scene);

}

void SceneManager::Update(void)
{
	_dbgStartDraw();

	if (!scenes_.empty())
	{
		scenes_.front()->Update();
	}


}

void SceneManager::Draw()
{
	auto rit = scenes_.rbegin();
	for (; rit != scenes_.rend(); rit++)
	{
		(*rit)->Draw();

	}

}
