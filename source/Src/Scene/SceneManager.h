#pragma once
#include<memory>
#include<list>
class Input;
class Scene;

class SceneManager
{
public:

	static constexpr float SCREEN_SIZE_X = 1600.0f;
	static constexpr float SCREEN_SIZE_Y = 1000.0f;
	
	/// <summary>
	/// �擪��(Update���Ă΂��)�V�[����؂�ւ���
	/// </summary>
	/// <param name="scene">�؂�ւ���̃V�[��</param>
	void ChangeScene(std::shared_ptr<Scene>scene);
	/// <summary>
	/// �X�^�b�N�̓��̃V�[�����폜����
	/// �������A�X�^�b�N��ɂP�����V�[�����Ȃ��ꍇ��
	/// �폜���Ȃ�
	/// </summary>
	/// <param name="scene"></param>
	/// <summary>
	void PopScene();
	/// �V�[�����v�b�V������B�X�^�b�N�̐���������
	/// ��ԏ�Update�����Ă΂�܂���
	/// </summary>
	/// <param name="scene"></param>
	void PushScene(std::shared_ptr<Scene>scene);
	/// <summary>
	/// �����Ă���V�[���̃A�b�v�f�[�g���Ăт���
	/// </summary>
	/// <param name="input"></param>
	//void Update(Input& input);
	void Update();
	void Draw();

private:

	std::list<std::shared_ptr<Scene>> scenes_;//���ݐi�s���̃V�[��


};

