#pragma once
#include<string>
#include<map>
#include"../Object/Comon/Transform.h"


class EffectManager
{
public:

	struct EffectData
	{
		//���\�[�X�̃n���h��ID
		int resHandleId_;
		//���\�[�X�̃n���h��D���g�p���Đ�������Đ��p�n���h��
		int playHandleId_;
		//�G�t�F�N�g�̑傫��
		VECTOR scale_ ={1.0f,1.0f,1.0f};
		//
		bool isPlay_ =false;
		//
		bool isLoop = false;
	};

	EffectManager(Transform& transform);
	~EffectManager();
	void Add(int type, VECTOR scale, bool loop,int handlId);
	void Update();
	void Play(int type);
	void Stop(int type);



private:

	Transform& transform_;

	// ��ޕʂ̃A�j���[�V�����f�[�^
	std::map<int, EffectData> efect_;


};

