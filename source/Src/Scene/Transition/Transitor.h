#pragma once
#include<DxLib.h>

class Transitor
{
protected:

	//�؂�ւ��ɂ����鎞��
	const float interval_;

	//�J�n���猻�݂܂ł̃t���[����
	float frame_ = 0;

	//�؂�ւ��O�̉��
	int oldRT_ = 0;

	//�؂�ւ���̉��
	int newRT_ = 0;

public:
	Transitor(float interval = 60.0f) : interval_(interval) {}
	virtual ~Transitor();

	//���o�J�n
	void Start();

	//Scene::Update�̍Ō�ɌĂ�
	virtual void Update() = 0;

	//Scene::Draw�̍Ō�ɌĂ�
	virtual void Draw() = 0;

	//���o���I������
	virtual bool IsEnd()const;

};

