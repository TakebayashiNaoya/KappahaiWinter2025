#pragma once

class UITitle :public IGameObject
{
public:
	UITitle();
	~UITitle();


private:
	bool Start() override final;
	void Update() override final;
	void Render(RenderContext& rc) override final;


private:
	SpriteRender m_backGroundImage;
};

