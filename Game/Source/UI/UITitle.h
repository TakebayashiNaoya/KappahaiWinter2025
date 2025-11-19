#pragma once
class UITitle :public IGameObject
{
public:
	UITitle();
	~UITitle();


public:
	//void SetIsDraw(const bool isDraw)
	//{
	//	m_isDrawStop = isDraw;
	//}


private:
	bool Start() override final;
	void Update() override final;
	void Render(RenderContext& rc) override final;


private:
	SpriteRender m_backGroundImage;
	//bool m_isDrawStop = false;
};

