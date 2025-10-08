#include "stdafx.h"
#include "Source/Actor/Character/Player/Player.h"
#include "Source/Actor/Character/Player/PlayerStateMachine.h"

namespace
{
	constexpr float JUMP_POWER = 300.0f;		// ジャンプパワー。
	constexpr float STICK_DEAD_ZONE = 0.01f;	// スティックのデッドゾーン。
	const float DASH_SPEED = 300.0f;			// ダッシュスピード。
	const float WALK_SPEED = 150.0f;			// 歩くスピード。

	/// <summary>
	/// 左スティックの現在の位置を取得します。
	/// </summary>
	/// <returns>左スティックの位置を表す Vector3 オブジェクト。x 成分は水平方向、z 成分は垂直方向の入力値です。</returns>
	const Vector3 GetLeftStick()
	{
		Vector3 stickL = Vector3::Zero;
		stickL.x = g_pad[0]->GetLStickXF();
		stickL.z = g_pad[0]->GetLStickYF();
		return stickL;
	}

	/// <summary>
	/// 左スティックが入力されているかどうかを判定します。
	/// </summary>
	/// <returns>左スティックの長さが0.01より大きい場合はtrue、それ以外はfalseを返します。</returns>
	const bool IsLeftStick()
	{
		if (GetLeftStick().Length() > STICK_DEAD_ZONE) {
			return true;
		}
		return false;
	}
}

namespace app
{
	namespace player
	{
		void PlayerStateMachine::Update()
		{
			StateMachineBase::Update();
		}




		/*************************************/


		void IdleState::Enter()
		{
			GetOwner<Player>()->PlayAnimation(Player::enAnimationClip_Idle);
		}


		void IdleState::Update()
		{
		}


		void IdleState::Exit()
		{
		}


		bool IdleState::RequestState(int& requestStateId)
		{
			if (IsLeftStick()) {
				requestStateId = enPlayerState_Walk;
				return true;
			}

			if (g_pad[0]->IsTrigger(enButtonA)) {
				requestStateId = enPlayerState_Jump;
				return true;
			}

			if (!GetOwner<Player>()->GetCharacterController().IsOnGround()) {
				requestStateId = enPlayerState_Jump;
				return true;
			}

			return false;
		}




		/*************************************/


		void WalkState::Enter()
		{
			GetOwner<Player>()->PlayAnimation(Player::enAnimationClip_Walk);
		}


		void app::player::WalkState::Update()
		{
			GetOwner<Player>()->Move(WALK_SPEED);
			GetOwner<Player>()->Rotation();
		}


		void app::player::WalkState::Exit()
		{
		}


		bool app::player::WalkState::RequestState(int& requestStateId)
		{
			if (!IsLeftStick()) {
				requestStateId = enPlayerState_Idle;
				return true;
			}

			if (g_pad[0]->IsPress(enButtonB)) {
				requestStateId = enPlayerState_Run;
				return true;
			}

			if (g_pad[0]->IsTrigger(enButtonA)) {
				requestStateId = enPlayerState_Jump;
				return true;
			}

			if (!GetOwner<Player>()->GetCharacterController().IsOnGround()) {
				requestStateId = enPlayerState_Jump;
				return true;
			}

			return false;
		}




		/*************************************/


		void app::player::RunState::Enter()
		{
			GetOwner<Player>()->PlayAnimation(Player::enAnimationClip_Run);
		}


		void app::player::RunState::Update()
		{
			GetOwner<Player>()->Move(DASH_SPEED);
			GetOwner<Player>()->Rotation();
		}


		void app::player::RunState::Exit()
		{
		}


		bool app::player::RunState::RequestState(int& requestStateId)
		{
			if (!IsLeftStick()) {
				requestStateId = enPlayerState_Idle;
				return true;
			}

			if (!g_pad[0]->IsPress(enButtonB)) {
				requestStateId = enPlayerState_Walk;
				return true;
			}

			if (g_pad[0]->IsTrigger(enButtonA)) {
				requestStateId = enPlayerState_Jump;
				return true;
			}

			if (!GetOwner<Player>()->GetCharacterController().IsOnGround()) {
				requestStateId = enPlayerState_Jump;
				return true;
			}

			return false;
		}




		/*************************************/


		void app::player::JumpState::Enter()
		{
			GetOwner<Player>()->PlayAnimation(Player::enAnimationClip_Jump);

			if (GetOwner<Player>()->GetCharacterController().IsOnGround()) {
				GetOwner<Player>()->ApplyJumpImpulse(JUMP_POWER);
			}
		}


		void app::player::JumpState::Update()
		{
			GetOwner<Player>()->Move(WALK_SPEED);
			GetOwner<Player>()->Rotation();
		}


		void app::player::JumpState::Exit()
		{
		}


		bool app::player::JumpState::RequestState(int& requestStateId)
		{
			if (GetOwner<Player>()->GetCharacterController().IsOnGround()) {
				requestStateId = enPlayerState_Idle;
				return true;
			}
			return false;
		}
	}
}