#pragma once
#include "StageBase.h"
class TitleStage : public StageBase
{
public:
	TitleStage();
	~TitleStage();


private:
	bool Start() override final;
	void Update() override final;
	void Render(RenderContext& rc) override final;
};

