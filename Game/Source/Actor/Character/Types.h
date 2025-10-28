/// <summary>
/// 各キャラクターのステートを定義するヘッダー。
/// </summary>

#pragma once
enum enPlayerState {
	enPlayerState_Idle,		// 待機状態。
	enPlayerState_Walk,		// 歩き状態。
	enPlayerState_Run,		// 走り状態。
	enPlayerState_Jump,		// ジャンプ状態。
	enPlayerState_KnockBack,	// 被ダメージ状態。
	enPlayerState_Num,
};

enum enEnemyState {
	enEnemyState_Idle,		// 待機状態。
	enEnemyState_Walk,		// 歩き状態。
	enEnemyState_CoolDown,	// クールダウン状態。
	enEnemyState_Die,		// ダウン状態。
	enEnemyState_Num,
};