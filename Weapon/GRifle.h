#pragma once

#include "IGun.h"

class GRifle :public IGun
{
public:
	GRifle();
	~GRifle();
	// IGun��(��) ���� ��ӵ�
	virtual void Init() override;

	virtual void Update() override;

	virtual void Render() override;

};
