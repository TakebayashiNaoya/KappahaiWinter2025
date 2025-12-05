#pragma once
#include "Source/Actor/Actor.h"  // êÊÇ…ActorÇímÇÈ

class Treasure : public Actor
{
public:
	const bool GetIsOpened() const
	{
		return m_isOpened;
	}
	void SetIsOpened(const bool isOpened)
	{
		m_isOpened = isOpened;
	}


public:
	Treasure();
	~Treasure();


private:
	bool Start() override final;
	void Update() override final;
	void Render(RenderContext& rc) override final;

	enum EnAnimationClip {
		enAnimationClip_Idle,
		enAnimationClip_Open,
		enAnimationClip_Num
	};

	AnimationClip m_animationClip[enAnimationClip_Num];

	enum EnState {
		enState_Init,
		enState_Idle,
		enState_Opened
	};

	EnState m_state = enState_Init;

	bool m_isOpened = false;
};