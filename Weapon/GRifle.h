#pragma once

#include "IGun.h"

class GRifle :public IGun
{
public:
	GRifle();
	~GRifle();
	// IGun을(를) 통해 상속됨
	virtual void Init() override;

	virtual void Update() override;

	virtual void Render() override;

};
