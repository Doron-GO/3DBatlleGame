#pragma once
#include<DxLib.h>

class Transitor
{
protected:
	const float interval_;//�؂�ւ��ɂ����鎞��
	float frame_ = 0;//�J�n���猻�݂܂ł̃t���[����
	int oldRT_ = 0;//�؂�ւ��O�̉��
	int newRT_ = 0;//�؂�ւ���̉��
public:
	Transitor(float interval = 60.0f) : interval_(interval) {}
	virtual ~Transitor();
	void Start();//���o�J�n
	virtual void Update() = 0;//Scene::Update�̍Ō�ɌĂ�ł�
	virtual void Draw() = 0;//Scene::Draw�̍Ō�ɌĂ�ł�
	virtual bool IsEnd()const;//���o���I������

};

