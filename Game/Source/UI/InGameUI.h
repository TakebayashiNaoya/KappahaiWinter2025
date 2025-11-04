#pragma once

class InGameUI :public IGameObject
{
public:
	InGameUI();
	~InGameUI();

	void SetLife(int life)
	{
		m_lifeCounts = life;
	}

private:
	bool Start() override final;
	void Update() override final;
	void Render(RenderContext& rc) override final;

	int m_lifeCounts = 0;	//ライフの数。
	FontRender m_lifeFR;	//ライフの数を表示するフォントレンダー。
};

