#pragma once
#include"../../Common/Quaternion.h"
#include<DxLib.h>

class Input;

class StateBase
{
public:

	virtual ~ StateBase(){};
	virtual void Update()=0;

};

