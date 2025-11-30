#pragma once
#include <map>
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
	enSoundList_Num
};

/// <summary>
/// サウンド管理クラス（機能本体）
/// CollisionHitManagerと同じ立ち位置です。
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
	// メンバ変数
	static const SoundDef m_soundDefs[enSoundList_Num];
	std::map<EnSoundList, nsK2EngineLow::SoundSource*> m_bgmMap;


	// フェードアウト情報を管理する構造体
	struct FadeState
	{
		nsK2EngineLow::SoundSource* source; // 操作する音源
		float decreaseSpeed;                // 1秒で下げる量
	};
	// フェードアウト中の音リスト
	std::list<FadeState> m_fadeList;

	// フェードイン情報を管理する構造体
	struct FadeInState
	{
		nsK2EngineLow::SoundSource* source;
		float currentVolume;
		float targetVolume;
		float increaseSpeed; // 1秒で上げる量
	};
	// フェードインリスト
	std::list<FadeInState> m_fadeInList;


private:
	// シングルトン化のためコンストラクタはprivate
	SoundManager();
	~SoundManager();

public:
	// 初期化処理（リソースロードなど）
	void Init();
	// 更新処理（リスナー座標更新など）
	void Update();

public:
	/// <summary>
	/// <para>指定した音を再生する関数。</para>
	/// <para>　BGM：Play(ID, true);</para>
	/// <para>　2DSE：Play(ID);</para>
	/// <para>　単発3DSE：Play(ID, false, true, pos);</para>
	/// <para>　連続3DSE：ptr = Play(ID, true, true, pos); -> ptr->SetPosition(...);</para>
	/// </summary>
	static nsK2EngineLow::SoundSource* Play(
		EnSoundList soundId,
		bool isLoop = false,
		bool is3D = false,
		const Vector3& position = Vector3::Zero
	);

	static void StopBGM(EnSoundList soundId, float fadeTime = 0.0f);
	static void StopAllBGM(float fadeTime = 0.0f);

	// ▼▼▼ 追加: 指定したIDの音量を設定する関数（初期化用） ▼▼▼
	static void SetVolume(EnSoundList soundId, float volume);

	// ▼▼▼ 追加: 現在再生中のBGMをすべてフェードインさせる関数 ▼▼▼
	static void FadeInAllBGM(float fadeTime);

	/**
	 * シングルトン関連
	 * CollisionHitManagerと同様の実装
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
/// CollisionManagerObjectと同じ立ち位置です。
/// </summary>
class SoundManagerObject : public IGameObject
{
public:
	SoundManagerObject();
	~SoundManagerObject();
	bool Start() override;
	void Update() override;
};