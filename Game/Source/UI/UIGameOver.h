#pragma once
class UIGameOver :public IGameObject
{
public:
	UIGameOver();
	~UIGameOver();


public:
	void SetIsDraw(const bool isDraw)
	{
		m_isDrawStop = isDraw;
	}


private:
	bool Start() override final;
	void Update() override final;
	void Render(RenderContext& rc) override final;


private:
	SpriteRender m_gameOverImage;
	bool m_isDrawStop = false;
};

