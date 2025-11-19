#pragma once
class UIBossHp :public IGameObject
{
public:
	UIBossHp();
	~UIBossHp();

private:
	bool Start() override final;
	void Update() override final;
	void Render(RenderContext& rc) override final;

private:
	FontRender m_bossHpFR;
	SpriteRender m_bossHpBarBack;
	SpriteRender m_bossHpBarFront;
};

