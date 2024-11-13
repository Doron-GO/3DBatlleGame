#include "Transitor.h"

Transitor::~Transitor()
{
	DeleteGraph(oldRT_);
	DeleteGraph(newRT_);
}

void Transitor::Start()
{
	VECTOR size = { 1600,1000 };
	int sizeX = static_cast<int>(size.x);
	int sizeY = static_cast<int>(size.y);

	oldRT_ = MakeScreen(sizeX, sizeY);
	newRT_ = MakeScreen(sizeX, sizeY);

	int result = GetDrawScreenGraph(0, 0, sizeX, sizeY, oldRT_);
	frame_ = 0;
}


bool Transitor::IsEnd() const
{
	return frame_ >= interval_;
}

int Transitor::GetMainScreen() const
{
	return mainScreen_;
}
