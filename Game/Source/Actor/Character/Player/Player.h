/// <summary>
/// プレイヤーを管理するクラス。
/// </summary>
#pragma once
#include "Source/Actor/Character/Types.h"
#include "Source/Actor/Character/Character.h"

namespace app {
	namespace player {
		class PlayerStateMachine;
	}
}

class Player :public Character
{
public:
	Player();

	/// <summary>
	/// アニメーションのクリップを表す列挙型です。
	/// PlayerStateMachineでアニメを切り替えるために使用します。
	/// </summary>
	enum EnAnimationClip
	{
		enAnimationClip_Idle,	// 待機アニメーション。
		enAnimationClip_Walk,	// 歩きアニメーション。
		enAnimationClip_Run,	// 走りアニメーション。
		enAnimationClip_Jump,	// ジャンプアニメーション。
		enAnimationClip_Num,
	};

	/// <summary>
	/// プレイヤーの座標を取得します。
	/// </summary>
	/// <returns>プレイヤーの現在の座標値（float型）。</returns>
	const Vector3 GetPosition()const
	{
		return m_position;
	}

	/// <summary>
	/// 毎フレームのXZ軸回転角度を取得します。
	/// </summary>
	/// <returns>XZ軸回転角度。</returns>
	const Quaternion& GetAdditionalRot() const
	{
		return m_xzAdditionalRot;
	}

	/// <summary>
	/// カメラの回転角度を計算します。
	/// </summary>
	void CalcCameraRotation();

	/// <summary>
	/// 入力方向、速度、重力に基づいてキャラクターを移動させます。
	/// </summary>
	/// <param name="speed">移動速度を表す値。</param>
	void MoveUpdate(const float speed);

private:
	bool Start()override final;
	void Update()override final;
	void Render(RenderContext& rc)override final;

	/// <summary>
	/// 移動方向を返します。
	/// </summary>
	/// <returns>移動方向。</returns>
	const Vector3 ComputeMoveDirection()const;

	/// <summary>
	/// 移動方向に速度を乗算して返します。
	/// </summary>
	/// <param name="speed"> 移動速度。</param>
	/// <returns> 移動先の相対座標。</returns>
	const Vector3 ComputeAddMoveSpeed(const float speed)const;

	Quaternion	m_xzAdditionalRot;		// 毎フレームのXZ軸回転角度。

	/// プレイヤーのステートマシン。
	std::unique_ptr<app::player::PlayerStateMachine> m_stateMachine;

	// クラススコープで宣言し、cppで定義。
	static const Character::AnimationOption PLAYER_ANIMATION_OPTIONS[];
};