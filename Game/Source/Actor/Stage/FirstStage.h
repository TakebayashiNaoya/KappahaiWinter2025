#pragma once
#include "StageBase.h"
class FirstStage : public StageBase
{
public:
	FirstStage();
	~FirstStage();

private:
	bool Start() override final;
	void Update() override final;
	void Render(RenderContext& rc) override final;
};

