#include <DxLib.h>
#include "Resource.h"
#include "ResourceManager.h"
#include"../../Config.h"

ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::Init(void)
{
	Resource res;

	// �^�C�g���摜

	// �X�J�C�h�[��
	res = Resource(Resource::TYPE::MODEL,PATH_MODEL + "SkyDome/SkyDome.mv1");
	resouceMap_.emplace(SRC::SKYDOME, res);

	//���{�b�g
	res = Resource(Resource::TYPE::MODEL, PATH_MODEL + "Player/78.mv1");
	resouceMap_.emplace(SRC::ROBOT, res);

	//���{�b�g(��)
	res = Resource(Resource::TYPE::MODEL, PATH_MODEL + "Player/78-Casval.mv1");
	resouceMap_.emplace(SRC::ROBOT_CASVAL, res);

	//Stage
	res = Resource(Resource::TYPE::MODEL, PATH_MODEL + "Stage/Ground3.mv1");
	resouceMap_.emplace(SRC::STAGE, res);

	//�r�[�����C�t��
	res = Resource(Resource::TYPE::MODEL, PATH_MODEL + "Weapon/rifle1.mv1");
	resouceMap_.emplace(SRC::BEAMRIFLE, res);
	
	//�r�[���T�[�x��
	res = Resource(Resource::TYPE::MODEL, PATH_MODEL + "Weapon/saber2.mv1");
	resouceMap_.emplace(SRC::BEAMSABER, res);
	
	//�r�[��
	res = Resource(Resource::TYPE::EFFEKSEER, PATH_EFFECT + "drill.efkefc");
	resouceMap_.emplace(SRC::BEAM, res);

	//�o�[�j�A�G�t�F�N�g
	res = Resource(Resource::TYPE::EFFEKSEER, PATH_EFFECT + "jet.efkefc");
	resouceMap_.emplace(SRC::VERNIER, res);

	//�y���G�t�F�N�g
	res = Resource(Resource::TYPE::EFFEKSEER, PATH_EFFECT + "DustCloud.efkefc");
	resouceMap_.emplace(SRC::DUST_CLOUD, res);

	//�唚���G�t�F�N�g
	res = Resource(Resource::TYPE::EFFEKSEER, PATH_EFFECT + "Explosion.efkefc");
	resouceMap_.emplace(SRC::EXPLOSION, res);

	//�o���A�h�[���G�t�F�N�g
	res = Resource(Resource::TYPE::EFFEKSEER, PATH_EFFECT + "Barrior_Dome.efkefc");
	resouceMap_.emplace(SRC::BARRIOR_DOME, res);

	//�q�b�g�G�t�F�N�g
	res = Resource(Resource::TYPE::EFFEKSEER, PATH_EFFECT + "Hit.efkefc");
	resouceMap_.emplace(SRC::HIT, res);

	//�a���G�t�F�N�g
	res = Resource(Resource::TYPE::EFFEKSEER, PATH_EFFECT + "Slash.efkefc");
	resouceMap_.emplace(SRC::SLASH, res);

	//�u�[�X�g�G�t�F�N�g
	res = Resource(Resource::TYPE::EFFEKSEER, PATH_EFFECT + "Boost.efkefc");
	resouceMap_.emplace(SRC::BOOST, res);

	//�f�X�{�[���G�t�F�N�g
	res = Resource(Resource::TYPE::EFFEKSEER, PATH_EFFECT + "DeathBall.efkefc");
	resouceMap_.emplace(SRC::DEATH_BALL, res);

	//�r�[�������W����
	res = Resource(Resource::TYPE::MODEL, PATH_MODEL + "Collision/Collision_Beam.mv1");
	resouceMap_.emplace(SRC::BEAM_COLLISION, res);
	
	//�u�[�X�g�Q�[�W�摜
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Ui/Gauge.png");
	resouceMap_.emplace(SRC::BOOSTGAUGE_IMAGE, res);
	
	//�u�[�X�g�Q�[�W�摜(�P�[�X)
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Ui/Gauge_case.png");
	resouceMap_.emplace(SRC::BOOSTGAUGE_CASE_IMAGE , res);

	//�GHp�Q�[�W�摜
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Ui/EnemyHpGauge.png");
	resouceMap_.emplace(SRC::ENEMY_HP_GAUGE , res);

	//�GHp�Q�[�W�摜
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Ui/Rifle.png");
	resouceMap_.emplace(SRC::RIFLE_IMAGE , res);
	
	//�^�C�g�����S�摜
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Title/Title.png");
	resouceMap_.emplace(SRC::TIRLE_LOGO_IMAGE, res);
	
	//A�{�^���������Ă�
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Title/PleaseAButton.png");
	resouceMap_.emplace(SRC::PLEASE_A, res);

	//�~�{�^���������Ă�
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Title/PleaseCrossButton.png");
	resouceMap_.emplace(SRC::PLEASE_CROSS, res);

	//�������摜
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Title/OneMoreFight.png");
	resouceMap_.emplace(SRC::ONE_MOR_FIGHT, res);

	//�^�C�g����ʂɖ߂�摜
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Title/BackToTitle.png");
	resouceMap_.emplace(SRC::BACK_TO_TITLE, res);

	//�Z���N�g�J�[�\��
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Title/Triangle.png");
	resouceMap_.emplace(SRC::TRIANGLE, res);

	//�}���`�v���C�摜
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Title/MultiPlay.png");
	resouceMap_.emplace(SRC::MULTI_PLAY_LOGO, res);

	//�V���O���v���C�摜
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Title/SinglePlay.png");
	resouceMap_.emplace(SRC::SINGLE_PLAY_LOGO, res);

	//READY�摜
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Game/Ready.png");
	resouceMap_.emplace(SRC::READY_IMAGE, res);

	//FIGHT�摜
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Game/Fight.png");
	resouceMap_.emplace(SRC::FIGHT_IMAGE, res);
	
	//�s�k��ʉ摜
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Ui/You_Lose.png");
	resouceMap_.emplace(SRC::LOSE, res);
	
	//������ʉ摜
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Ui/You_Win.png");
	resouceMap_.emplace(SRC::WIN, res);

	//�^�[�Q�b�g�摜
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Ui/Target.png");
	resouceMap_.emplace(SRC::TARGET, res);
	
	//�^�[�Q�b�g(��)�摜
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Ui/Target_Red.png");
	resouceMap_.emplace(SRC::TARGET_RED, res);

	//���G�t�F�N�g
	res = Resource(Resource::TYPE::EFFEKSEER, PATH_EFFECT + "Lightning.efkefc");
	resouceMap_.emplace(SRC::LIGHTNING, res);

	//�g�����W�V����X�摜
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Ui/X_2.png");
	resouceMap_.emplace(SRC::TRANSITION, res);

	//�g�����W�V����X�g�摜
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Ui/X_Frame_2.png"); 
	resouceMap_.emplace(SRC::TRANSITION_FRAME, res);

	//�G�t�F�N�g
	res = Resource(Resource::TYPE::EFFEKSEER, PATH_EFFECT + "Atomosphere.efkefc");
	resouceMap_.emplace(SRC::ATOMOSPHERE, res);

	//�����摜
	res = Resource(Resource::TYPE::IMGS, PATH_IMAGE + "Ui/BulletNum.png",10,1,80,80);
	resouceMap_.emplace(SRC::NUMBER, res);

	//�c�e�����摜
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Ui/Hp_Back.png");
	resouceMap_.emplace(SRC::HP_BACK, res);

	//�c�e�����摜
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Ui/Hp_Back.png");
	resouceMap_.emplace(SRC::HP_BACK, res);

	//�Q�[���X�^�[�g�摜
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Title/StartGame.png");
	resouceMap_.emplace(SRC::GAME_START, res);

	//�`���[�g���A��������摜
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Title/GoToTutorial.png");
	resouceMap_.emplace(SRC::WATCH_TUTORIAL, res);

}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second->Release();
		delete p.second;
	}

	loadedMap_.clear();
}

Resource ResourceManager::Load(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return Resource();
	}
	Resource ret = *res;
	return *res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res->handleId_);
	res->duplicateModelIds_.push_back(duId);

	return duId;
}

ResourceManager::ResourceManager(void)
{
}

ResourceManager::~ResourceManager(void)
{
	delete instance_;
}

Resource* ResourceManager::_Load(SRC src)
{
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return lPair->second;
	}

	const auto& rPair = resouceMap_.find(src);
	if (rPair == resouceMap_.end())
	{
		// �o�^����Ă��Ȃ�
		return nullptr;
	}

	rPair->second.Load();

	// �O�̂��߃R�s�[�R���X�g���N�^
	Resource* ret = new Resource(rPair->second);
	loadedMap_.emplace(src, ret);

	return ret;
}
