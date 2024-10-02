#pragma once
#include<DxLib.h>
#include<chrono>


class DeltaTime
{
public:

	static DeltaTime& GetInstsnce()
	{
		static DeltaTime s_Instance;
		return s_Instance;
	}
	//�X�V
	void Update();

	//�f���^�^�C���̎擾
	const float GetDeltaTime()const;

	//�o�ߎ��Ԃ̎擾
	const float GetElapsedTime()const;

	//�v���̊J�n
	void SetStart();

	//�o�ߎ��Ԃ̃��Z�b�g
	void Reset();

private:

	DeltaTime();

	~DeltaTime();

	//�f���^�^�C��
	float deltaTime_;

	//�f���^�^�C���v���p�̍ŐV����
	std::chrono::system_clock::time_point preTime_;
	
	//�o�ߎ���
	float elapsedTime_;

};

