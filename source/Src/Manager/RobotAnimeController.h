#pragma once
#include <string>
#include <chrono>
#include <map>
#include"../Object/Time/DeltaTime.h"

class RobotAnimeController 
{
public:
	
	enum class Body
	{
		UP,
		LOW,
	};
	enum class STATE
	{
		NONE,
		RUN,
		JUMP,
		DAMAGE,
		DOWN,
		SHOT,
		IDLE,
		BOOST,
		BOOST_DASH,
		FALL,
		COMBAT1,
		COMBAT_RUN,
		WIN,
		LOSE
	};

	// �A�j���[�V�����f�[�^
	struct Animation
	{
		int model_ = -1;
		int attachNo_ = -1;
		int animIndex_ = 0;
		float speed_ = 0.0f;
		float totalTime_ = 0.0f;
		float step_ = 0.0f;
		bool priority_ = false;
		bool isLoop_ = false;
		bool isStop_ = false;
		bool isEnd = false;
	};

	struct Blend
	{
		int attachNo_ = -1;//�u�����h�����A�j���[�V�����̃A�^�b�`�ԍ�
		STATE type_ = STATE::NONE;		//�u�����h�����A�j���[�V����
		bool blendFlag_ = false;//�u�����h�����ǂ���
		float rate_ = 0;	//�u�����h��
		float stepBlend_ = 0;//�u�����h�����Z
		float blendTime_ = 0;//�u�����h���0�`1�ɂ��邽�߂̂���
	};


	int upplayType_;
	int lowlayType_;


	RobotAnimeController(int modelId);
	~RobotAnimeController();

	void Update(void);

	void Draw(void);

	//�A�j���[�V������ǉ�����
	void Add(int type, const std::string& path, float speed,float totalTime);

	//�㔼�g�A�j���[�V������ݒ肷��
	void UpperBodyPlay(int type, bool priority=false,bool isLoop = true,
		bool isStop = false, float endStep = -1.0f,float startStep = 0.0f , bool isForce = false);
	
	//�����g�A�j���[�V������ݒ肷��
	void LowerBodyPlay(int type, bool priority = false, bool isLoop = true,
		bool isStop = false, float endStep = -1.0f, float startStep = 0.0f, bool isForce = false);

	void DebugDraw();

	//�㔼�g�̃A�j���[�V�������Đ����I��������ǂ���(1���[�v�I�������I���Ƃ���)
	bool IsUpperEnd(void);

	//�����g�̃A�j���[�V�������Đ����I��������ǂ���(1���[�v�I�������I���Ƃ���)
	bool IsLowerEnd(void);

	//�S�g�̃A�j���[�V�����Đ����I��������ǂ����𔻒�
	bool IsEnd(void);

private:

	// ���f���̃n���h��ID
	int modelId_;
	
	//�u�����h����鑤�A�j���[�V�������
	std::map<Body, Blend> blend_;

	//���ݍĐ����A�j���[�V����
	std::map<Body,Animation > playAnim_;

	std::map<int, Animation> animations_;

	//���݂̃A�j�[�V�����^�C�v���Ԃ��Ă���
	std::map<Body, STATE> playType_;
	
	std::map<STATE, int> attachedTypeNum_;

	//���łɃA�^�b�`�ς݂̃A�j���[�V�������ǂ������Ԃ��Ă���
	//first:�A�j���[�V�����^�C�v second:�A�^�b�`���Ă��邩�ǂ��� (true�̓A�^�b�`����Ă���)
	std::map<STATE, bool> isAttach_;

	float blendTime_;

	float deltaTime_;

	void UpperBodyUpdate(void);

	void LowerBodyUpdate(void);

	void DetachAnim(void);

};

