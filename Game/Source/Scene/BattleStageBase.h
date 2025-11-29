#pragma once
#include "SceneManager.h"


class Player;
class BossEnemy;
class GameCamera;
class UIInGame;
class BattleManagerObject;
class CollisionManagerObject;
class UIResultBase;


// バトルがあるステージ共通の親クラス
class BattleStageBase : public IScene
{
public:
	BattleStageBase();
	virtual ~BattleStageBase();


protected:
	virtual bool Start() override;

	/// <summary>
	/// 派生先で更新処理を実装するための仮想関数。
	/// </summary>
	virtual void OnUpdate() {};

	/// <summary>
	/// レベルの初期化を行う仮想関数です。
	/// 派生クラスでオーバーライドして初期化してください。
	/// </summary>
	virtual void InitLevel() {};

	// 勝敗判定に必要なため基底クラスに持たせています。
	// 各ステージで生成したPlayer、BossEnemyのポインタをセットしてください。
	Player* m_player = nullptr;
	BossEnemy* m_bossEnemy = nullptr;	// ※派生先でDeleteGOしてください。




	/////////////////////////////////////////////////////////////////
	///--- ゲームの進行状態を管理し、共通の勝敗判定を行います。---///
	/////////////////////////////////////////////////////////////////
private:
	/// <summary>
	/// 戦闘の進行状態を表す列挙型。
	/// </summary>
	enum enBattlePhase
	{
		enBattlePhase_Battle,
		enBattlePhase_BattleFinish,
		enBattlePhase_WaitFinishAnimation,
		enBattlePhase_GameOver,
		enBattlePhase_GameClear,
		enBattlePhase_WaitEnd,
		enBattlePhase_End
	};
	enBattlePhase m_battlePhase = enBattlePhase_Battle;

	/// <summary>
	/// 勝敗結果を表す列挙型。
	/// </summary>
	enum enResult
	{
		enResult_None,
		enResult_PlayerWin,
		enResult_PlayerLose
	};
	enResult m_result = enResult_None;

	/// <summary>
	/// 全てのステージで共通する勝敗判定を行います。
	/// </summary>
	void Update() override final;




	/////////////////////////////////////////////////////////////////////
	///--- 全てのステージで共通するオブジェクトの初期化を行います。---///
	/////////////////////////////////////////////////////////////////////
private:
	/// <summary>
	/// 全てのステージで共通するオブジェクトの初期化。
	/// NOTE:ロード画面を円滑に動かすため、ステートマシン形式で初期化を分割しています。
	/// </summary>
	void InitObjects();

	/// <summary>
	/// InitObjectsで実行するタスクを登録します。
	/// </summary>
	void RegisterLoadingTasks();

	std::vector<std::function<void()>> m_loadingTasks;	// タスクのリスト
	int m_currentTaskIndex = 0;							// 実行中のタスク番号。


	BattleManagerObject* m_battleManager = nullptr;			// バトルマネージャー。
	CollisionManagerObject* m_collisionManager = nullptr;	// コリジョンマネージャー。
	UIInGame* m_inGameUI = nullptr;							// インゲームUI。
	GameCamera* m_gameCamera = nullptr;						// ゲームカメラ。
	UIResultBase* m_uiResult = nullptr;						// リザルトUI。


	/// <summary>
	/// 空を初期化。
	/// </summary>
	void InitSky();

	SkyCube* m_skyCube = nullptr;				// スカイキューブ。
	int m_skyCubeType = enSkyCubeType_Night;	// スカイキューブのタイプ。
};