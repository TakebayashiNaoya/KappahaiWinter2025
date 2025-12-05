#pragma once
#include "Source/Actor/Actor.h"  // æ‚ÉActor‚ğ’m‚é

class Rocket : public Actor
{
public:
	Rocket();
	~Rocket();


public:
	const bool GetIsGooled() const
	{
		return m_isGooled;
	}
	const void SetIsGooled(const bool isGooled)
	{
		m_isGooled = isGooled;
	}


private:
	bool m_isGooled = false;


private:
	virtual bool Start() override final;
	virtual void Update() override final;
	virtual void Render(RenderContext& rc) override final;
};

