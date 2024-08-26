#pragma once
#include"../Collision/CollisionCapsule.h"

#include<memory>

class RobotAnimeController;
class ResourceManager;

class ActorBase
{
public:

	ActorBase();

	//���f���̑S�g�̃A�j���[�V�����̍Đ����s��
	virtual void PlayAnim(int state, bool priority, bool isLoop, bool isStop = false, float endStep = -1.0f, float startStep = 0.0f);

	//���f���̏㔼�g�̃A�j���[�V�����̍Đ����s��
	virtual void PlayUpperAnim(int state, bool priority, bool isLoop, bool isStop = false, float endStep = -1.0f, float startStep = 0.0f);

	//���f���̉����g�̃A�j���[�V�����̍Đ����s��
	virtual void PlayLowerAnim(int state, bool priority, bool isLoop, bool isStop = false, float endStep = -1.0f, float startStep = 0.0f);

	const bool IsAnimEnded(void)const;

	const float& GetDeltaTime(void)const;

	const float& GetPlayerHP(void)const;

	//�����̐��l��HP�����炷
	void Damage(float damage);

	CollisionCapsule& GetCapsule(void)const;


protected:

	// �V���O���g���Q��
	ResourceManager& resMng_;

	//�A�j���[�V�����Ǘ�
	std::unique_ptr<RobotAnimeController> roboAnimeController_;

	//�J�v�Z����̓����蔻��p
	std::unique_ptr<CollisionCapsule> capsule_;


	virtual void InitAnimation(void);

	//�f���^�^�C��
	float deltaTime_;

	//�v���C���[HP
	float playerHp_;

};

