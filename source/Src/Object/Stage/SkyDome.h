#pragma once
#include "../Common/Transform.h"

class SkyDome
{
public:


	// ���
	enum class STATE
	{
		NONE,
		STAY,
		FOLLOW
	};

	SkyDome(void);	//�v���C���[�ɒǏ]����ꍇ�͂�����
	~SkyDome(void);

	void Update(void);
	void Draw(void);
	void Release(void);

private:
	// ���
	STATE state_;

	// ���f������̊�{���
	Transform transform_;

};
