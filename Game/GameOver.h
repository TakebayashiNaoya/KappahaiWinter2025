#pragma once
class GameOver :public IGameObject
{
public:
	GameOver();
	~GameOver();

private:
	void Update() override final;
	void Render(RenderContext& rc) override final;
	SpriteRender m_spriteRender;
};

