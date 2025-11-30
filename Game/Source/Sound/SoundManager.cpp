#include "stdafx.h"
#include "SoundManager.h"
#include "Source/Actor/Character/Player/Player.h" 

// 実装ファイルではusingしてOK
using namespace nsK2EngineLow;

namespace
{
	const std::string SOUND_DIR = "Assets/sound/";
	const std::string EXTENSION = ".wav";
}

// =================================================================
// SoundManager (機能クラス) の実装
// =================================================================

SoundManager* SoundManager::m_instance = nullptr;

// サウンド定義
const SoundManager::SoundDef SoundManager::m_soundDefs[enSoundList_Num] =
{
	// fileName				volume
	{ "TitleBGM",			0.5f },
	{ "FirstStageBGM",		0.5f },
	{ "Push",				1.0f },
	{ "PlayerJump",			0.5f },
	{ "PlayerFirstStep",	0.5f },
	{ "PlayerSecondStep",	0.5f },
	{ "RunStart",			1.5f },
	{ "RunLoop",			1.5f },
	{ "Stomp",				1.5f },
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
	Player* player = FindGO<Player>("Player");
	if (player != nullptr) {
		g_soundEngine->SetListenerPosition(player->GetPosition());
	}

	// ▼▼▼ 2. フェードアウト処理の追加 ▼▼▼
	if (m_instance == nullptr) {
		return;
	}

	// リストの中身を走査 (イテレータを使用)
	auto it = m_instance->m_fadeList.begin();
	while (it != m_instance->m_fadeList.end())
	{
		FadeState& state = *it;

		// SoundSourceの現在の音量を取得して下げる
		// ※SoundSourceにはGetVolume/SetVolumeが元々あるのでそのまま使えます
		float currentVol = state.source->GetVolume();
		currentVol -= state.decreaseSpeed * g_gameTime->GetFrameDeltaTime();

		if (currentVol <= 0.0f) {
			// 音量が0になったら完全に消す
			state.source->SetVolume(0.0f);
			state.source->Stop();
			DeleteGO(state.source);

			// リストから削除して次へ
			it = m_instance->m_fadeList.erase(it);
		}
		else {
			// 音量を更新して次へ
			state.source->SetVolume(currentVol);
			++it;
		}
	}

	if (m_instance == nullptr) return;

	// ▼▼▼ フェードイン処理の追加 ▼▼▼
	auto itIn = m_instance->m_fadeInList.begin();
	while (itIn != m_instance->m_fadeInList.end())
	{
		FadeInState& state = *itIn;

		// 音量を上げる
		state.currentVolume += state.increaseSpeed * g_gameTime->GetFrameDeltaTime();

		// 目標音量を超えたら完了
		if (state.currentVolume >= state.targetVolume)
		{
			state.source->SetVolume(state.targetVolume);
			itIn = m_instance->m_fadeInList.erase(itIn); // リストから削除
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
	// インスタンスが無ければ再生できない
	if (!IsAvailable()) return nullptr;

	SoundManager* instance = GetInstance();

	// BGM等のループ再生ですでに登録済みなら、既存のものを返す（重複再生防止）
	if (isLoop && instance->m_bgmMap.count(soundId) > 0) {
		return instance->m_bgmMap[soundId];
	}

	// 新しい音源を作成
	SoundSource* ss = NewGO<SoundSource>(0);
	ss->Init(soundId, is3D);

	if (is3D) {
		ss->SetPosition(position);
	}

	ss->SetVolume(m_soundDefs[soundId].defaultVolume);
	ss->Play(isLoop);

	// ループかつ2D（BGMなど）の場合は管理マップに保存
	// ※移動するループSEは呼び出し元が管理することを想定しているため、ここには登録しない
	if (isLoop && !is3D) {
		instance->m_bgmMap[soundId] = ss;
	}

	return ss;
}


void SoundManager::StopBGM(EnSoundList soundId, float fadeTime)
{
	if (!IsAvailable()) return;

	SoundManager* instance = GetInstance();
	auto it = instance->m_bgmMap.find(soundId);

	// 管理マップに見つかった場合
	if (it != instance->m_bgmMap.end())
	{
		SoundSource* ss = it->second;

		if (ss != nullptr)
		{
			if (fadeTime > 0.0f)
			{
				// ▼ フェードアウトする場合：リストに移動する
				float currentVol = ss->GetVolume();
				float speed = currentVol / fadeTime; // 現在の音量 ÷ 秒数

				// フェードリストに追加
				instance->m_fadeList.push_back({ ss, speed });
			}
			else
			{
				// ▼ 即消しの場合
				ss->Stop();
				DeleteGO(ss);
			}
		}

		// 管理マップ（再生中リスト）からは外す
		// ※フェードリストに移管したため
		instance->m_bgmMap.erase(it);
	}
}

void SoundManager::StopAllBGM(float fadeTime)
{
	if (!IsAvailable()) return;

	SoundManager* instance = GetInstance();
	// 全ての登録済みBGMに対してStopBGMを呼ぶ
	// ※mapをループしながら消すと危険なので、一旦キーをコピーするか、
	//  イテレータを慎重に進める必要がありますが、今回は単純化して「全部フェードリストへ送る」実装にします

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
		// 削除して次へ
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

		// 本来の音量（定義値）を取得
		float targetVol = instance->m_soundDefs[pair.first].defaultVolume;

		// 現在の音量（恐らく0になっているはず）からスタート
		float currentVol = ss->GetVolume();

		// スピード計算: (目標 - 現在) / 時間
		float speed = (targetVol - currentVol) / fadeTime;

		// リストに登録
		instance->m_fadeInList.push_back({ ss, currentVol, targetVol, speed });
	}
}




/********************************/


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