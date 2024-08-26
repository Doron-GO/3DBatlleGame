#include "CollisionCapsule.h"

CollisionCapsule::CollisionCapsule(Transform& transform, float top, float Down, float r):transform_(transform)
{
	top_ = top;
	down_ = Down;
	r_ = r;
}

void CollisionCapsule::Draw(void)
{
	auto radius_ = r_;
	auto COLOR = 0xffffff;
	// ã‚Ì‹…‘Ì
	VECTOR pos1 = GetPosTop();
	DrawSphere3D(pos1, radius_, 5, 0x00ff00,COLOR , true);

	// ‰º‚Ì‹…‘Ì
	VECTOR pos2 = GetPosDown();
	DrawSphere3D(pos2, radius_, 5, 0x0000ff, COLOR, true);

	VECTOR dir;
	VECTOR s;
	VECTOR e;

	// ‹…‘Ì‚ğŒq‚®ü(X+)
	dir = transform_.GetRight();
	s = VAdd(pos1, VScale(dir, radius_));
	e = VAdd(pos2, VScale(dir, radius_));
	DrawLine3D(s, e, COLOR);

	// ‹…‘Ì‚ğŒq‚®ü(X-)
	dir = transform_.GetLeft();
	s = VAdd(pos1, VScale(dir, radius_));
	e = VAdd(pos2, VScale(dir, radius_));
	DrawLine3D(s, e, COLOR);

	// ‹…‘Ì‚ğŒq‚®ü(Z+)
	dir = transform_.GetForward();
	s = VAdd(pos1, VScale(dir, radius_));
	e = VAdd(pos2, VScale(dir, radius_));
	DrawLine3D(s, e, COLOR);

	// ‹…‘Ì‚ğŒq‚®ü(Z-)
	dir = transform_.GetBack();
	s = VAdd(pos1, VScale(dir, radius_));
	e = VAdd(pos2, VScale(dir, radius_));
	DrawLine3D(s, e, COLOR);

	// ƒJƒvƒZƒ‹‚Ì’†S
	DrawSphere3D(GetCenter(), 5.0f, 10, COLOR, COLOR, true);


}

void CollisionCapsule::Update(void)
{
	Sync();
}

void CollisionCapsule::Sync()
{
	auto top =VAdd(transform_.pos,VScale(transform_.GetUp(), top_));
	SetRelativePosTop(top);
	auto down =VAdd(transform_.pos,VScale(transform_.GetUp(), down_));
	SetRelativePosDown(down);
}

void CollisionCapsule::SetRelativePosTop(const VECTOR& pos)
{
	relativePosTop_ = pos;
}

void CollisionCapsule::SetRelativePosDown(const VECTOR& pos)
{
	relativePosDown_ = pos;
}

VECTOR CollisionCapsule::GetPosTop(void) const
{
	return relativePosTop_;
}

VECTOR CollisionCapsule::GetPosDown(void) const
{
	return relativePosDown_;
}

VECTOR CollisionCapsule::GetCenter(void) const
{
	VECTOR top = GetPosTop();
	VECTOR down = GetPosDown();

	VECTOR diff = VSub(top, down);
	return VAdd(down, VScale(diff, 0.5f));
}

bool CollisionCapsule::Collision(int modelId)
{
	auto hits = MV1CollCheck_Capsule(modelId, -1,
		relativePosTop_,
		relativePosDown_,
		r_
	);

	//ŒŸo‚µ‚½ƒ|ƒŠƒSƒ“‚Æ‚ÌÕ“Ëî•ñ‚ğÁ‚·
	MV1CollResultPolyDimTerminate(hits);
	if (hits.HitNum>0)
	{
		return true;
	}
	return false;
}
