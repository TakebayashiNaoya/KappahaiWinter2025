#pragma once
#include "UIResultBase.h"

class UIGameClear : public UIResultBase
{
private:
	enum enState
	{
		enState_JumpArc,		// A地点からB地点へ放物線ジャンプ
		enState_Bouncing,		// B地点でその場バウンド
		enState_Wait,			// 待機
		enState_Shrink,			// 収縮
		enState_LoadingWait,	// ローディング待ち
		enState_End,			// 終了
	};

public:
	UIGameClear();
	~UIGameClear();

private:
	bool Start() override final;
	void Update() override final;
	void Render(RenderContext& rc) override final;

private:
	SpriteRender m_gameClearImage;

	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale = Vector3::One;
	float m_alpha = 0.0f;

	Quaternion m_rotation = Quaternion::Identity;
	float m_angle = 0.0f; // 現在の角度（ラジアン）

	// 物理演算用
	Vector3 m_velocity = Vector3::Zero; // 3次元の速度

	float m_timer = 0.0f;
	int m_state = 0;
};