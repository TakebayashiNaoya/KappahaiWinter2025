/// <summary>
/// 各キャラクターのステートを定義するヘッダー。
/// </summary>

#pragma once
enum enPlayerState {
	enPlayerState_Idle,			// 待機。
	enPlayerState_Walk,			// 歩き。
	enPlayerState_Run,			// 走り。
	enPlayerState_Jump,			// ジャンプ。
	enPlayerState_KnockBack,	// ノックバック。
	enPlayerState_KneelDown,	// 死亡。
	enPlayerState_Num,
};

enum enBasicEnemyState {
	enEnemyState_Idle,		// 待機。
	enEnemyState_Chase,		// 追跡。
	enEnemyState_CoolDown,	// クールダウン。
	enEnemyState_Die,		// 死亡。
	enEnemyState_Num,
};

enum enTransformEnemyState {
	enEnemyState_Idle,		// 待機。
	enEnemyState_Escape,	// 逃走。
	enEnemyState_Transform,	// 変形。
	enEnemyState_Slide,		// 滑走。
	enEnemyState_Die,		// 死亡。
	enEnemyState_Num,
};