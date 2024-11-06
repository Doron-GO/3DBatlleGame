#include "../../Manager/ResourceManager.h"
#include "../../Utility/AsoUtility.h"
#include "SkyDome.h"

constexpr float SCALE = 5.0f;
constexpr VECTOR SCALES = { SCALE, SCALE, SCALE };


SkyDome::SkyDome(void)
{
	state_ = STATE::NONE;
	// ÉÇÉfÉãêßå‰ÇÃäÓñ{èÓïÒ
	transform_.SetModel(
		lpResourceMng.LoadModelDuplicate(ResourceManager::SRC::SKYDOME));
	transform_.scl = SCALES;
	transform_.pos = AsoUtility::VECTOR_ZERO;
	transform_.quaRot = Quaternion::Euler(
		0.0f, 
		AsoUtility::Deg2RadF(180.0f),
		0.0f
	);
	transform_.quaRotLocal = Quaternion();
	transform_.Update();

}

SkyDome::~SkyDome(void)
{

}

void SkyDome::Update(void)
{
	transform_.Update();
}

void SkyDome::Draw(void)
{
	MV1DrawModel(transform_.modelId);
}

void SkyDome::Release(void)
{
}
