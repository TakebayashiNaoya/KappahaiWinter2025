#pragma once
#include <map>
#include <list>
#include <vector>
#include <string>

// エンジンのクラスを前方宣言
namespace nsK2EngineLow {
	class SoundSource;
}

// サウンドリスト
enum EnSoundList
{
	enSoundList_TitleBGM,
	enSoundList_FirstStageBGM,
	enSoundList_SelectSE,
	enSoundList_PlayerJumpSE,
	enSoundList_PlayerStep1SE,
	enSoundList_PlayerStep2SE,
	enSoundList_PlayerRunStart,
	enSoundList_PlayerRunLoop,
	enSoundList_Stomp,
	enSoundList_SlidingStart,
	enSoundList_SlidingLoop,
	enSoundList_BossStep,
	//
	enSoundList_Num
};

/// <summary>
/// サウンド管理クラス（機能本体）
/// エンジンの3D機能を使わず、自前で距離減衰を計算します。
/// </summary>
class SoundManager
{
private:
	// サウンド定義構造体
	struct SoundDef
	{
		const char* fileName; // ファイル名（拡張子なし）
		float defaultVolume;  // 基本音量
	};

	// 疑似3Dサウンド（距離減衰させる音）の管理構造体
	struct Pseudo3DState
	{
		nsK2EngineLow::SoundSource* source;	// 音源
		float baseVolume;					// 元の音量
		bool isLoop;						// ループ音か？
	};

	// フェードアウト情報を管理する構造体
	struct FadeState
	{
		nsK2EngineLow::SoundSource* source; // 操作する音源
		float decreaseSpeed;                // 1秒で下げる量
	};

	// フェードイン情報を管理する構造体
	struct FadeInState
	{
		nsK2EngineLow::SoundSource* source;
		float currentVolume;
		float targetVolume;
		float increaseSpeed; // 1秒で上げる量
	};

	// メンバ変数
	static const SoundDef m_soundDefs[enSoundList_Num];

	// 管理リスト
	std::map<EnSoundList, nsK2EngineLow::SoundSource*> m_bgmMap; // BGM（2Dループ）
	std::list<Pseudo3DState> m_pseudo3DList;					 // 疑似3D音源
	std::list<FadeState> m_fadeList;							 // フェードアウト待ち
	std::list<FadeInState> m_fadeInList;						 // フェードイン待ち

private:
	// シングルトン化のためコンストラクタはprivate
	SoundManager();
	~SoundManager();

public:
	// 初期化処理（リソースロードなど）
	void Init();
	// 更新処理（距離減衰・フェード計算）
	void Update();

public:
	/// <summary>
	/// 指定した音を再生する関数。
	/// </summary>
	static nsK2EngineLow::SoundSource* Play(
		EnSoundList soundId,
		bool isLoop = false,
		bool is3D = false,
		const Vector3& position = Vector3::Zero
	);

	static void StopBGM(EnSoundList soundId, float fadeTime = 0.0f);
	static void StopAllBGM(float fadeTime = 0.0f);

	/// <summary>
	/// 指定したIDの音量を設定する（初期化用）
	/// </summary>
	static void SetVolume(EnSoundList soundId, float volume);

	/// <summary>
	/// 現在再生中のBGMをすべてフェードインさせる
	/// </summary>
	static void FadeInAllBGM(float fadeTime);

	/// <summary>
	/// 疑似3Dリストから音源を登録解除する。
	/// SoundSourceが削除される直前に必ず呼ぶ必要がある。
	/// </summary>
	static void UnregisterPseudo3D(nsK2EngineLow::SoundSource* source);

	/**
	 * シングルトン関連
	 */
private:
	static SoundManager* m_instance;

public:
	static SoundManager* CreateInstance()
	{
		if (m_instance == nullptr) {
			m_instance = new SoundManager();
		}
		return m_instance;
	}
	static SoundManager* GetInstance()
	{
		return m_instance;
	}
	static bool IsAvailable()
	{
		return m_instance != nullptr;
	}
	static void Delete()
	{
		if (m_instance != nullptr) {
			delete m_instance;
			m_instance = nullptr;
		}
	}
};


/// <summary>
/// サウンドマネージャーを動かすためのゲームオブジェクト
/// </summary>
class SoundManagerObject : public IGameObject
{
public:
	SoundManagerObject();
	~SoundManagerObject();
	bool Start() override;
	void Update() override;
};