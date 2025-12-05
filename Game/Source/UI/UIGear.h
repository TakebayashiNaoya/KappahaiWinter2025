/// <summary>
/// ギアのUIを管理するクラス。
/// </summary>
#pragma once


class UIGear : public IGameObject
{
public:
	const int GetGotGearCount() const
	{
		return m_gotGearCount;
	}
	void SetGotGearCount(int count)
	{
		m_gotGearCount = count;
	}
	const int GetMaxGearCount() const
	{
		return m_maxGearCount;
	}
	void SetMaxGearCount(int maxGearCount)
	{
		m_maxGearCount = maxGearCount;
	}


public:
	UIGear();
	~UIGear();


private:
	bool Start() override final;
	void Update() override final;
	void Render(RenderContext& rc) override final;


private:
	SpriteRender	m_gearImage;
	FontRender		m_gotGearCountFR;
	int				m_gotGearCount = 0;
	int				m_maxGearCount = 0;
};