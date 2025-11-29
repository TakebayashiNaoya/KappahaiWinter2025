#include "stdafx.h"
#include "Source/Actor/Character/Player/Player.h"
#include "Source/Actor/Character/Player/PlayerStateMachine.h"
#include "Source/Collision/CollisionManager.h"

namespace
{
	constexpr float STOMP_COLLIDER_RADIUS = 40.0f;	// 踏みつけ用コライダーの半径。
	constexpr float JUMP_POWER = 40.0f;				// ジャンプパワー。
	constexpr float STICK_DEAD_ZONE = 0.01f;		// スティックのデッドゾーン。
	const float DASH_SPEED = 15.0f;					// ダッシュスピード。
	const float WALK_SPEED = 10.0f;					// 歩くスピード。

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
			GetOwner<Player>()->SetSpeedBeforeJump(WALK_SPEED);
		}


		bool IdleState::RequestState(int& requestStateId)
		{
			if (GetOwner<Player>()->IsAttacked()) {
				requestStateId = enPlayerState_KnockBack;
				return true;
			}

			if (IsLeftStick()) {
				requestStateId = enPlayerState_Walk;
				return true;
			}

			if (g_pad[0]->IsTrigger(enButtonA)) {
				GetOwner<Player>()->ApplyJumpImpulse(JUMP_POWER);
				requestStateId = enPlayerState_Jump;
				return true;
			}

			if (!GetOwner<Player>()->IsOnGround()) {
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
			GetOwner<Player>()->MoveUpdate(WALK_SPEED);
			GetOwner<Player>()->CalcCameraRotation();
			GetOwner<Player>()->ModelRotation();
		}


		void app::player::WalkState::Exit()
		{
			GetOwner<Player>()->SetSpeedBeforeJump(WALK_SPEED);
		}


		bool app::player::WalkState::RequestState(int& requestStateId)
		{
			if (GetOwner<Player>()->IsAttacked()) {
				requestStateId = enPlayerState_KnockBack;
				return true;
			}

			if (!IsLeftStick()) {
				requestStateId = enPlayerState_Idle;
				return true;
			}

			if (g_pad[0]->IsPress(enButtonB)) {
				requestStateId = enPlayerState_Run;
				return true;
			}

			if (g_pad[0]->IsTrigger(enButtonA)) {
				GetOwner<Player>()->ApplyJumpImpulse(JUMP_POWER);
				requestStateId = enPlayerState_Jump;
				return true;
			}

			if (!GetOwner<Player>()->IsOnGround()) {
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
			GetOwner<Player>()->MoveUpdate(DASH_SPEED);
			GetOwner<Player>()->CalcCameraRotation();
			GetOwner<Player>()->ModelRotation();
		}


		void app::player::RunState::Exit()
		{
			GetOwner<Player>()->SetSpeedBeforeJump(DASH_SPEED);
		}


		bool app::player::RunState::RequestState(int& requestStateId)
		{
			if (GetOwner<Player>()->IsAttacked()) {
				requestStateId = enPlayerState_KnockBack;
				return true;
			}

			if (!IsLeftStick()) {
				requestStateId = enPlayerState_Idle;
				return true;
			}

			if (!g_pad[0]->IsPress(enButtonB)) {
				requestStateId = enPlayerState_Walk;
				return true;
			}

			if (g_pad[0]->IsTrigger(enButtonA)) {
				GetOwner<Player>()->ApplyJumpImpulse(JUMP_POWER);
				requestStateId = enPlayerState_Jump;
				return true;
			}

			if (!GetOwner<Player>()->IsOnGround()) {
				requestStateId = enPlayerState_Jump;
				return true;
			}

			return false;
		}




		/*************************************/


		void app::player::JumpState::Enter()
		{
			GetOwner<Player>()->PlayAnimation(Player::enAnimationClip_Run);

			// 踏みつけ用コライダーの作成。
			GetOwner<Player>()->SetAttackCollider(
				CollisionHitManager::GetInstance()->CreateCollider(
					GetOwner<Player>(), enCollisionType_Player, STOMP_COLLIDER_RADIUS, true));
		}


		void app::player::JumpState::Update()
		{
			GetOwner<Player>()->MoveUpdate(GetOwner<Player>()->GetSpeedBeforeJump());
			GetOwner<Player>()->CalcCameraRotation();
			GetOwner<Player>()->ModelRotation();

			// 踏みつけ用コライダーの更新。
			CollisionHitManager::GetInstance()->UpdateCollider(
				GetOwner<Player>(), GetOwner<Player>()->GetAttackCollider());
		}


		void app::player::JumpState::Exit()
		{
			// 踏みつけ用コライダーの削除。
			GetOwner<Player>()->SetAttackCollider(
				CollisionHitManager::GetInstance()->DeleteCollider(
					GetOwner<Player>()->GetAttackCollider()));

		}


		bool app::player::JumpState::RequestState(int& requestStateId)
		{
			if (GetOwner<Player>()->IsAttacked()) {
				requestStateId = enPlayerState_KnockBack;
				return true;
			}

			if (GetOwner<Player>()->IsOnGround()) {
				if (IsLeftStick()) {
					if (g_pad[0]->IsPress(enButtonB)) {
						requestStateId = enPlayerState_Run;
						return true;
					}
					else {
						requestStateId = enPlayerState_Walk;
						return true;
					}
				}
				else {
					requestStateId = enPlayerState_Idle;
					return true;
				}
			}
			return false;
		}




		/*************************************/


		void app::player::KnockBackState::Enter()
		{
			GetOwner<Player>()->TakeDamage();
			GetOwner<Player>()->PlayAnimation(Player::enAnimationClip_Down);
			GetOwner<Player>()->SetIsInvincible(true);
		}


		void app::player::KnockBackState::Update()
		{
			GetOwner<Player>()->KnockedBack();
			GetOwner<Player>()->CalcCameraRotation();
			GetOwner<Player>()->ModelRotation();
		}


		void app::player::KnockBackState::Exit()
		{
			GetOwner<Player>()->ResetKnockBackTimer();
		}


		bool app::player::KnockBackState::RequestState(int& requestStateId)
		{
			if (GetOwner<Player>()->IsAttacked()) {
				return false;
			}

			if (GetOwner<Player>()->IsDying()) {
				requestStateId = enPlayerState_Dead;
				return true;
			}

			if (GetOwner<Player>()->IsOnGround()) {
				if (IsLeftStick()) {
					if (g_pad[0]->IsPress(enButtonB)) {
						requestStateId = enPlayerState_Run;
						return true;
					}
					else {
						requestStateId = enPlayerState_Walk;
						return true;
					}
				}
				else {
					requestStateId = enPlayerState_Idle;
					return true;
				}
			}
			else {
				requestStateId = enPlayerState_Jump;
				return true;
			}
			return false;
		}


		/*************************************/


		void app::player::DieState::Enter()
		{
			GetOwner<Player>()->PlayAnimation(Player::enAnimationClip_Dead);
			GetOwner<Player>()->SetIsInvincible(false);
		}


		void app::player::DieState::Update()
		{
			if (GetOwner<Player>()->IsPlayingAnimation() == false) {
				GetOwner<Player>()->SetIsDead(true);
			}
		}


		void app::player::DieState::Exit()
		{
		}


		bool app::player::DieState::RequestState(int& requestStateId)
		{
			return false;
		}
	}
}