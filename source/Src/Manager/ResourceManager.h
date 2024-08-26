#pragma once
#include <map>
#include <string>
#include "Resource.h"
#define lpResourceMng  ResourceManager::GetInstance()

class ResourceManager
{

public:

	// ���\�[�X��
	enum class SRC
	{
		TITLE,
		PUSH_SPACE,
		SKYDOME,
		STAGE,
		PLAYER,
		PLAYER_SHADOW,
		START,
		BEAMRIFLE,
		BEAM,
		BEAMSABER,
		BEAM_COLLISION,
		BOOSTGAUGE_IMAGE,
		BOOSTGAUGE_CASE_IMAGE,
		TIRLE_LOGO_IMAGE,
		PLEASE_A,
		PLEASE_CROSS,
		ONE_MOR_FIGHT,
		BACK_TO_TITLE,
		TRIANGLE,
		EXPLOSION,
		HIT,
		SLASH,
		WIN,
		LOSE,
		DEATH_BALL,
		SINGLE_PLAY_LOGO,
		MULTI_PLAY_LOGO,
		TARGET,
		TARGET_RED,
		ENEMY_HP_GAUGE,
		RIFLE_IMAGE,
		READY_IMAGE,
		FIGHT_IMAGE
	};


	// �����I�ɃC���X�e���X�𐶐�����
	static void CreateInstance(void);

	// �ÓI�C���X�^���X�̎擾
	static ResourceManager& GetInstance(void);

	// ������
	void Init(void);

	// ���(�V�[���ؑ֎��Ɉ�U���)
	void Release(void);

	// ���\�[�X�̃��[�h
	Resource Load(SRC src);

	// ���\�[�X�̕������[�h(���f���p)
	int LoadModelDuplicate(SRC src);

private:

	// �ÓI�C���X�^���X
	static ResourceManager* instance_;

	// ���\�[�X�Ǘ��̑Ώ�
	std::map<SRC, Resource> resouceMap_;

	// �ǂݍ��ݍς݃��\�[�X
	std::map<SRC, Resource*> loadedMap_;

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	ResourceManager(void);

	// �f�X�g���N�^�����l
	~ResourceManager(void);

	// �������[�h
	Resource* _Load(SRC src);

};

