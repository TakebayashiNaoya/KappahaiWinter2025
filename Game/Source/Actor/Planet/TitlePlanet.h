#pragma once
#include "PlanetBase.h"
class TitlePlanet : public PlanetBase
{
public:
	TitlePlanet();
	~TitlePlanet();


private:
	bool Start() override final;
	void Update() override final;
	void Render(RenderContext& rc) override final;
};

