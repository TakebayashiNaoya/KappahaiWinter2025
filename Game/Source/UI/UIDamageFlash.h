#pragma once
#include "Source/Actor/Character/Types.h"

class UIDamageFlash : public IGameObject
{
public:
	UIDamageFlash();
	~UIDamageFlash();


public:
	void SetPlayerHp(int hp)
	{
		m_playerHp = hp;
	}


private:
	bool Start() override final;
	void Update() override final;
	void Render(RenderContext& rc) override final;


private:
	SpriteRender m_flashSprites[enPlayerCondition_Num];
	int m_playerHp = 0;
};

