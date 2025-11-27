#pragma once
#include "PlanetBase.h"
class FirstPlanet : public PlanetBase
{
public:
	FirstPlanet();
	~FirstPlanet();

private:
	bool Start() override final;
	void Update() override final;
	void Render(RenderContext& rc) override final;
};