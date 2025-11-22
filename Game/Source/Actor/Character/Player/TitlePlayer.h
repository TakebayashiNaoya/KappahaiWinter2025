#pragma once
#include "Player.h"

class TitleCamera;

class TitlePlayer : public Player
{
private:
	/// <summary>
	/// アニメーションのクリップを表す列挙型です。
	/// PlayerStateMachineでアニメを切り替えるために使用します。
	/// </summary>
	enum EnAnimationClip
	{
		enAnimationClip_Idle,		// 待機アニメーション。
		enAnimationClip_Walk,		// 歩きアニメーション。
		enAnimationClip_Run,		// 走りアニメーション。
		enAnimationClip_Down,		// ダウンアニメーション。
		enAnimationClip_Dead,		// 死亡アニメーション。
		enAnimationClip_Num,
	};


public:
	TitlePlayer();
	~TitlePlayer();

	const Vector3& GetFrontDirection() const
	{
		return m_frontDirection;
	}

	const Vector3& GetRightDirection() const
	{
		return m_rightDirection;
	}

private:
	bool Start() override final;
	void Update() override final;
	void Render(RenderContext& rc) override final;

	TitleCamera* m_titleCamera = nullptr;

	/// <summary>
	/// 正面方向と右方向を計算します。
	/// </summary>
	//void CalcFrontAndRightDirection();

	Vector3 m_frontDirection = Vector3::Zero;
	Vector3 m_rightDirection = Vector3::Zero;

	/// <summary>
	/// 移動方向を返します。
	/// </summary>
	/// <returns> 移動方向。</returns>
	//const Vector3 ComputeMoveDirection()const override final;

	// クラススコープで宣言し、cppで定義。
	static const Character::AnimationOption TITLE_PLAYER_ANIMATION_OPTIONS[];

	AnimationClip m_animationClip;
};

