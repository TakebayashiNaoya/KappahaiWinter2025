#pragma once
#include "UIResultBase.h"

class UIGameOver :public UIResultBase
{
private:
	enum enState
	{
		enState_HideImageFadeIn,
		enState_EntryImage,
		enState_WaitBeforeSlip,
		enState_SlipImage,
		enState_WaitAfterSlip,
		enState_GameOverImageFadeOut,
		enState_End,
	};


public:
	UIGameOver();
	~UIGameOver();


private:
	bool Start() override final;
	void Update() override final;
	void Render(RenderContext& rc) override final;


private:
	SpriteRender m_gameOverImage;
	SpriteRender m_hideImage;
	Quaternion m_rotation = Quaternion::Identity;
	Vector3 m_position = Vector3::Zero;
	float m_hideImageAlpha = 0.0f;
	float m_gameOverImageAlpha = 0.0f;
	float m_entryTimer = 0.0f;
	float m_waitTimer = 0.0f;
	float m_slipTimer = 0.0f;
	int m_state = 0;
};

