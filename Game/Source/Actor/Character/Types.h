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

enum enDeformEnemyState {
	enDeformEnemyState_Idle,			// 待機。
	enDeformEnemyState_Escape,		// 逃走。
	enDeformEnemyState_Transform,	// 変形。
	enDeformEnemyState_Slide,		// 滑走。
	enDeformEnemyState_Die,			// 死亡。
	enDeformEnemyState_Num,
};

enum enBossEnemyState {
	enBossEnemyState_Idle,			// 待機。
	enBossEnemyState_Cooldown,		// クールダウン。
	enBossEnemyState_Walk,			// 歩き。
	enBossEnemyState_Run,			// 走り。
	enBossEnemyState_Attack,		// 攻撃。
	enBossEnemyState_Damage,		// 被弾。
	enBossEnemyState_Dead,			// 死亡。
	enBossEnemyState_Num,
};