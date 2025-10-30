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
	enBasicEnemyState_Idle,			// 待機。
	enBasicEnemyState_Chase,		// 追跡。
	enBasicEnemyState_CoolDown,		// クールダウン。
	enBasicEnemyState_Die,			// 死亡。
	enBasicEnemyState_Num,
};

enum enTransformEnemyState {
	enTransformEnemyState_Idle,			// 待機。
	enTransformEnemyState_Escape,		// 逃走。
	enTransformEnemyState_Transform,	// 変形。
	enTransformEnemyState_Slide,		// 滑走。
	enTransformEnemyState_Die,			// 死亡。
	enTransformEnemyState_Num,
};