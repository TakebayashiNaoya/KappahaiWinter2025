#include "stdafx.h"
#include "SoundManager.h"
#include "Source/Actor/Character/Player/Player.h" 

using namespace nsK2EngineLow;

namespace
{
	const std::string SOUND_DIR = "Assets/sound/";
	const std::string EXTENSION = ".wav";

	// 音が聞こえる限界距離（これ以上離れると音量0）
	constexpr float MAX_SOUND_DISTANCE = 1500.0f;
}

// =================================================================
// ▼▼▼ 内部クラス: SoundSourceを継承したカスタムクラス ▼▼▼
// エンジン側のファイルを汚さずに、削除時の通知機能を追加します
// =================================================================
class GameSoundSource : public nsK2EngineLow::SoundSource
{
public:
	// デストラクタ：自分が消える時にマネージャーへ連絡する
	~GameSoundSource()
	{
		SoundManager::UnregisterPseudo3D(this);
	}
};

// =================================================================
// SoundManagerの実装
// =================================================================

SoundManager* SoundManager::m_instance = nullptr;

// サウンド定義
const SoundManager::SoundDef SoundManager::m_soundDefs[enSoundList_Num] =
{
	// fileName				volume
	{ "TitleBGM",			0.5f },
	{ "FirstStageBGM",		0.5f },
	{ "Push",				1.0f },
	{ "PlayerJump",			1.0f },
	{ "PlayerFirstStep",	0.5f },
	{ "PlayerSecondStep",	0.5f },
	{ "RunStart",			2.0f },
	{ "RunLoop",			2.0f },
	{ "Stomp",				1.5f },
	{ "SlidingStart",		1.0f },
	{ "SlidingLoop",		0.8f }, // 爆音防止のため1.0以下推奨
	{ "BossStep",			1.5f },
	{ "BossHowl",			1.0f },
	{ "BossAttack",			3.0f },
};

SoundManager::SoundManager()
{
}

SoundManager::~SoundManager()
{
	StopAllBGM();
}

void SoundManager::Init()
{
	// 一括ロード処理
	for (int i = 0; i < enSoundList_Num; i++)
	{
		std::string fullPath = SOUND_DIR + m_soundDefs[i].fileName + EXTENSION;
		g_soundEngine->ResistWaveFileBank(i, fullPath.c_str());
	}
}

void SoundManager::Update()
{
	if (m_instance == nullptr) return;

	// プレイヤー（リスナー）座標の取得
	Vector3 listenerPos = Vector3::Zero;
	Player* player = FindGO<Player>("Player");
	if (player != nullptr) {
		listenerPos = player->GetPosition();
		// エンジンの3D機能は使いませんが、念のため設定しておきます
		g_soundEngine->SetListenerPosition(listenerPos);
	}

	// ---------------------------------------------------------
	// ▼▼▼ 1. 疑似3Dサウンドの音量計算（距離減衰） ▼▼▼
	// ---------------------------------------------------------
	auto it3D = m_instance->m_pseudo3DList.begin();
	while (it3D != m_instance->m_pseudo3DList.end())
	{
		Pseudo3DState& state = *it3D;

		// 音源が無効、または停止していたらリストから削除
		if (state.source == nullptr || !state.source->IsPlaying())
		{
			it3D = m_instance->m_pseudo3DList.erase(it3D);
			continue;
		}

		if (player != nullptr) {
			// 距離を計算
			Vector3 soundPos = state.source->GetPosition();
			float distance = (listenerPos - soundPos).Length();

			// 距離減衰の計算 (線形: 近ければ1.0, 遠ければ0.0)
			float volumeRate = 1.0f - (distance / MAX_SOUND_DISTANCE);

			// クランプ (0.0 ～ 1.0)
			if (volumeRate < 0.0f) volumeRate = 0.0f;
			if (volumeRate > 1.0f) volumeRate = 1.0f;

			// 音量を適用 (元の音量 * 距離倍率)
			state.source->SetVolume(state.baseVolume * volumeRate);
		}

		++it3D;
	}

	// ---------------------------------------------------------
	// ▼▼▼ 2. フェードアウト処理 ▼▼▼
	// ---------------------------------------------------------
	auto itFade = m_instance->m_fadeList.begin();
	while (itFade != m_instance->m_fadeList.end())
	{
		FadeState& state = *itFade;

		float currentVol = state.source->GetVolume();
		currentVol -= state.decreaseSpeed * g_gameTime->GetFrameDeltaTime();

		if (currentVol <= 0.0f) {
			state.source->SetVolume(0.0f);
			state.source->Stop();
			DeleteGO(state.source);

			itFade = m_instance->m_fadeList.erase(itFade);
		}
		else {
			state.source->SetVolume(currentVol);
			++itFade;
		}
	}

	// ---------------------------------------------------------
	// ▼▼▼ 3. フェードイン処理 ▼▼▼
	// ---------------------------------------------------------
	auto itIn = m_instance->m_fadeInList.begin();
	while (itIn != m_instance->m_fadeInList.end())
	{
		FadeInState& state = *itIn;

		state.currentVolume += state.increaseSpeed * g_gameTime->GetFrameDeltaTime();

		if (state.currentVolume >= state.targetVolume)
		{
			state.source->SetVolume(state.targetVolume);
			itIn = m_instance->m_fadeInList.erase(itIn);
		}
		else
		{
			state.source->SetVolume(state.currentVolume);
			++itIn;
		}
	}
}

SoundSource* SoundManager::Play(EnSoundList soundId, bool isLoop, bool is3D, const Vector3& position)
{
	if (!IsAvailable()) return nullptr;
	SoundManager* instance = GetInstance();

	// ループ音（BGM等）の重複防止
	if (isLoop && instance->m_bgmMap.count(soundId) > 0) {
		return instance->m_bgmMap[soundId];
	}

	// ★★★ 重要変更 ★★★
	// GameSoundSource (デストラクタで自動登録解除してくれるクラス) を生成
	GameSoundSource* ss = NewGO<GameSoundSource>(0);

	// エンジンには常に「2D (false)」として初期化させ、爆音バグを防ぐ
	ss->Init(soundId, false);

	if (is3D) {
		// 3D指定の場合: 疑似3Dリストに登録する
		ss->SetPosition(position);

		Pseudo3DState newState;
		newState.source = ss;
		newState.baseVolume = m_soundDefs[soundId].defaultVolume;
		newState.isLoop = isLoop;

		// 生成直後の一瞬の爆音を防ぐため、初期音量を計算してセットしておく
		Player* player = FindGO<Player>("Player");
		if (player) {
			float dist = (player->GetPosition() - position).Length();
			float rate = 1.0f - (dist / MAX_SOUND_DISTANCE);
			if (rate < 0.0f) rate = 0.0f;
			if (rate > 1.0f) rate = 1.0f;
			ss->SetVolume(newState.baseVolume * rate);
		}
		else {
			ss->SetVolume(0.0f);
		}

		instance->m_pseudo3DList.push_back(newState);
	}
	else {
		// 完全2Dの場合: そのままの音量を設定
		ss->SetVolume(m_soundDefs[soundId].defaultVolume);
	}

	ss->Play(isLoop);

	// BGM管理マップへの登録（ループかつ非3Dのみ）
	if (isLoop && !is3D) {
		instance->m_bgmMap[soundId] = ss;
	}

	return ss;
}

void SoundManager::UnregisterPseudo3D(nsK2EngineLow::SoundSource* source)
{
	if (!IsAvailable()) return;
	SoundManager* instance = GetInstance();

	auto it = instance->m_pseudo3DList.begin();
	while (it != instance->m_pseudo3DList.end())
	{
		if (it->source == source)
		{
			instance->m_pseudo3DList.erase(it);
			break;
		}
		++it;
	}
}

void SoundManager::StopBGM(EnSoundList soundId, float fadeTime)
{
	if (!IsAvailable()) return;

	SoundManager* instance = GetInstance();
	auto it = instance->m_bgmMap.find(soundId);

	if (it != instance->m_bgmMap.end())
	{
		SoundSource* ss = it->second;
		if (ss != nullptr)
		{
			if (fadeTime > 0.0f) {
				// フェードリストへ移動
				float currentVol = ss->GetVolume();
				instance->m_fadeList.push_back({ ss, currentVol / fadeTime });
			}
			else {
				// 即停止
				ss->Stop();
				DeleteGO(ss);
			}
		}
		instance->m_bgmMap.erase(it);
	}
}

void SoundManager::StopAllBGM(float fadeTime)
{
	if (!IsAvailable()) return;

	SoundManager* instance = GetInstance();
	auto it = instance->m_bgmMap.begin();
	while (it != instance->m_bgmMap.end())
	{
		SoundSource* ss = it->second;
		if (ss != nullptr)
		{
			if (fadeTime > 0.0f) {
				float currentVol = ss->GetVolume();
				instance->m_fadeList.push_back({ ss, currentVol / fadeTime });
			}
			else {
				ss->Stop();
				DeleteGO(ss);
			}
		}
		it = instance->m_bgmMap.erase(it);
	}
}

void SoundManager::SetVolume(EnSoundList soundId, float volume)
{
	if (!IsAvailable()) return;
	SoundManager* instance = GetInstance();

	if (instance->m_bgmMap.count(soundId) > 0) {
		SoundSource* ss = instance->m_bgmMap[soundId];
		if (ss) {
			ss->SetVolume(volume);
		}
	}
}

void SoundManager::FadeInAllBGM(float fadeTime)
{
	if (!IsAvailable()) return;
	SoundManager* instance = GetInstance();

	if (fadeTime <= 0.0f) return;

	for (auto& pair : instance->m_bgmMap)
	{
		SoundSource* ss = pair.second;
		if (ss == nullptr) continue;

		float targetVol = instance->m_soundDefs[pair.first].defaultVolume;
		float currentVol = ss->GetVolume();
		float speed = (targetVol - currentVol) / fadeTime;

		instance->m_fadeInList.push_back({ ss, currentVol, targetVol, speed });
	}
}

// =================================================================
// SoundManagerObjectの実装
// =================================================================

SoundManagerObject::SoundManagerObject()
{
	SoundManager::CreateInstance();
}

SoundManagerObject::~SoundManagerObject()
{
	SoundManager::Delete();
}

bool SoundManagerObject::Start()
{
	if (SoundManager::IsAvailable()) {
		SoundManager::GetInstance()->Init();
	}
	return true;
}

void SoundManagerObject::Update()
{
	if (SoundManager::IsAvailable()) {
		SoundManager::GetInstance()->Update();
	}
}