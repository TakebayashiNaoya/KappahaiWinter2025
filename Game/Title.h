#pragma once
class Title :public IGameObject
{
public:
	Title();
	~Title();

private:
	void Update() override final;
	void Render(RenderContext& rc) override final;

	SpriteRender m_spriteRender;
};