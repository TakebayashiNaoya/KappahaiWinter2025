#pragma once

class UIPlayerHp;
class UIDamageFlash;

class UIInGame :public IGameObject
{
public:
	UIInGame();
	~UIInGame();

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

	UIPlayerHp* m_playerHpUI = nullptr;
	UIDamageFlash* m_damageFlashUI = nullptr;
};

