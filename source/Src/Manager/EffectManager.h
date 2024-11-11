#pragma once
#include<string>
#include<map>
#include"../Object/Common/Transform.h"


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
		//�Đ������ǂ���
		bool isPlay_ =false;
		//���[�v�Đ����ǂ���
		bool isLoop = false;
		//�v���C���[�̃W�F�b�g�̂悤�ɉ�]�Ȃǂɉ����āA���W�����Ǐ]�����邩�ǂ���
		bool isSync_ = false;
		//���W�����p���l
		VECTOR offset_;
		//���W
		VECTOR pos_;
		//��]
		VECTOR rot_;

	};

	EffectManager(const Transform& transform);
	~EffectManager();
	//�G�t�F�N�g�̒ǉ�
	void Add(int type, VECTOR scale,VECTOR offset, VECTOR rot,bool loop, bool isSync,int handlId);
	//�A�b�v�f�[�g
	void Update();
	//�G�t�F�N�g���Đ�����
	void Play(int type);
	//�G�t�F�N�g�̍Đ����~�߂�
	void Stop(int type);
	//�G�t�F�N�g�̍Đ����x��ݒ肷��
	void SetSpeed(int type, float speed);



private:

	const Transform& transform_;

	// ��ޕʂ̃A�j���[�V�����f�[�^
	//�L�[�F�G�t�F�N�g�^�C�v,�o�����[�F�G�t�F�N�g�f�[�^
	std::map<int, EffectData> efects_;

};

