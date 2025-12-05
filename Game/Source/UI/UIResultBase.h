#pragma once

class UIResultBase : public IGameObject
{
public:
	UIResultBase() {}
	virtual ~UIResultBase() {}

	// 共通の終了判定フラグ取得
	const bool GetIsEnd() const
	{
		return m_isEnd;
	}

protected:
	bool m_isEnd = false; // 終了フラグ
};

