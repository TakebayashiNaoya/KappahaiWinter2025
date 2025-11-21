#pragma once
class UIControls : public IGameObject
{
public:
	UIControls();
	~UIControls();


private:
	bool Start() override final;
	void Update() override final;
	void Render(RenderContext& rc) override final;


private:
	SpriteRender m_jumpImage;
	SpriteRender m_dashImage;
	SpriteRender m_buttonA;
	SpriteRender m_buttonB;
};

