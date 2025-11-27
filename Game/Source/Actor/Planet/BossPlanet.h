#pragma once
#include "PlanetBase.h"
class BossPlanet : public PlanetBase
{
public:
	BossPlanet();
	~BossPlanet();

private:
	bool Start() override final;
	void Update() override final;
	void Render(RenderContext& rc) override final;
};