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

	// タイトル画像

	// スカイドーム
	res = Resource(Resource::TYPE::MODEL,PATH_MODEL + "SkyDome/SkyDome.mv1");
	resouceMap_.emplace(SRC::SKYDOME, res);

	//プレイヤー
	res = Resource(Resource::TYPE::MODEL, PATH_MODEL + "Player/78.mv1");
	resouceMap_.emplace(SRC::PLAYER, res);

	//Stage
	res = Resource(Resource::TYPE::MODEL, PATH_MODEL + "Stage/Stage.mv1");
	resouceMap_.emplace(SRC::STAGE, res);

	//ビームライフル
	res = Resource(Resource::TYPE::MODEL, PATH_MODEL + "Weapon/rifle1.mv1");
	resouceMap_.emplace(SRC::BEAMRIFLE, res);
	
	//ビームサーベル
	res = Resource(Resource::TYPE::MODEL, PATH_MODEL + "Weapon/saber2.mv1");
	resouceMap_.emplace(SRC::BEAMSABER, res);
	
	//ビーム
	res = Resource(Resource::TYPE::EFFEKSEER, PATH_EFFECT + "drill.efkefc");
	resouceMap_.emplace(SRC::BEAM, res);

	//大爆発エフェクト
	res = Resource(Resource::TYPE::EFFEKSEER, PATH_EFFECT + "Explosion.efkefc");
	resouceMap_.emplace(SRC::EXPLOSION, res);

	//ヒットエフェクト
	res = Resource(Resource::TYPE::EFFEKSEER, PATH_EFFECT + "Hit.efkefc");
	resouceMap_.emplace(SRC::HIT, res);

	//斬撃エフェクト
	res = Resource(Resource::TYPE::EFFEKSEER, PATH_EFFECT + "Slash.efkefc");
	resouceMap_.emplace(SRC::SLASH, res);

	//デスボールエフェクト
	res = Resource(Resource::TYPE::EFFEKSEER, PATH_EFFECT + "DeathBall.efkefc");
	resouceMap_.emplace(SRC::DEATH_BALL, res);

	//ビームこリジョン
	res = Resource(Resource::TYPE::MODEL, PATH_MODEL + "Collision/Collision_Beam.mv1");
	resouceMap_.emplace(SRC::BEAM_COLLISION, res);
	
	//ブーストゲージ画像
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Ui/Gauge.png");
	resouceMap_.emplace(SRC::BOOSTGAUGE_IMAGE, res);
	
	//ブーストゲージ画像(ケース)
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Ui/Gauge_case.png");
	resouceMap_.emplace(SRC::BOOSTGAUGE_CASE_IMAGE , res);

	//敵Hpゲージ画像
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Ui/EnemyHpGauge.png");
	resouceMap_.emplace(SRC::ENEMY_HP_GAUGE , res);

	//敵Hpゲージ画像
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Ui/Rifle.png");
	resouceMap_.emplace(SRC::RIFLE_IMAGE , res);
	
	//タイトルロゴ画像
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Title/Title.png");
	resouceMap_.emplace(SRC::TIRLE_LOGO_IMAGE, res);
	
	//Aボタンを押してね
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Title/PleaseAButton.png");
	resouceMap_.emplace(SRC::PLEASE_A, res);

	//×ボタンを押してね
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Title/PleaseCrossButton.png");
	resouceMap_.emplace(SRC::PLEASE_CROSS, res);

	//もう一戦画像
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Title/OneMoreFight.png");
	resouceMap_.emplace(SRC::ONE_MOR_FIGHT, res);

	//タイトル画面に戻る画像
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Title/BackToTitle.png");
	resouceMap_.emplace(SRC::BACK_TO_TITLE, res);

	//セレクトカーソル
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Title/Triangle.png");
	resouceMap_.emplace(SRC::TRIANGLE, res);

	//マルチプレイ画像
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Title/MultiPlay.png");
	resouceMap_.emplace(SRC::MULTI_PLAY_LOGO, res);

	//シングルプレイ画像
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Title/SinglePlay.png");
	resouceMap_.emplace(SRC::SINGLE_PLAY_LOGO, res);

	//READY画像
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Game/Ready.png");
	resouceMap_.emplace(SRC::READY_IMAGE, res);

	//FIGHT画像
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Game/Fight.png");
	resouceMap_.emplace(SRC::FIGHT_IMAGE, res);
	
	//敗北画面画像
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Ui/You_Lose.png");
	resouceMap_.emplace(SRC::LOSE, res);
	
	//勝利画面画像
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Ui/You_Win.png");
	resouceMap_.emplace(SRC::WIN, res);

	//ターゲット画像
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Ui/Target.png");
	resouceMap_.emplace(SRC::TARGET, res);
	
	//ターゲット(赤)画像
	res = Resource(Resource::TYPE::IMG, PATH_IMAGE + "Ui/Target_Red.png");
	resouceMap_.emplace(SRC::TARGET_RED, res);
	
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
		// 登録されていない
		return nullptr;
	}

	rPair->second.Load();

	// 念のためコピーコンストラクタ
	Resource* ret = new Resource(rPair->second);
	loadedMap_.emplace(src, ret);

	return ret;
}
