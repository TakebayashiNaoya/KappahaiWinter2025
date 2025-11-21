#pragma once
class UIBossLife :public IGameObject
{
public:
	/** Å‘åHP‚ğİ’è‚·‚éB */
	void SetMaxLife(int hp)
	{
		m_maxLife = hp;
	}
	/** Œ»İ‚ÌHP‚ğİ’è‚·‚éB */
	void SetCurrentLife(int hp)
	{
		m_currentLife = hp;
	}


public:
	UIBossLife();
	~UIBossLife();


private:
	bool Start() override final;
	void Update() override final;
	void Render(RenderContext& rc) override final;


private:
	SpriteRender m_bossNameText;
	SpriteRender m_bossHpBarBack;
	SpriteRender m_bossHpBarFront;

	int m_maxLife = 0;
	int m_currentLife = 0;
};

