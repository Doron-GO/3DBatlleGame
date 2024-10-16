#pragma once
#include "../Common/Transform.h"

class SkyDome
{
public:

	static constexpr float SCALE = 0.5f;
	static constexpr VECTOR SCALES = { SCALE, SCALE, SCALE };

	// ���
	enum class STATE
	{
		NONE,
		STAY,
		FOLLOW
	};

	SkyDome(const Transform& syncTransform);	//�v���C���[�ɒǏ]����ꍇ�͂�����
	~SkyDome(void);

	void Update(void);
	void Draw(void);
	void Release(void);

private:

	// �v���C���[�̏��
	const Transform& syncTransform_;

	// ���
	STATE state_;

	// ���f������̊�{���
	Transform transform_;

};
