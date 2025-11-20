#pragma once
#include "Source/Actor/Character/Types.h"

class UIPlayerLife : public IGameObject
{
public:
	UIPlayerLife();
	~UIPlayerLife();

	void SetPlayerHp(int hp)
	{
		m_playerHp = hp;
	}

private:
	bool Start() override final;
	void Update() override final;
	void Render(RenderContext& rc) override final;

private:
	std::array<SpriteRender, enPlayerCondition_Num> m_hpSprites;
	SpriteRender* m_displayHpSprite = nullptr;
	int m_playerHp = 0;
};

