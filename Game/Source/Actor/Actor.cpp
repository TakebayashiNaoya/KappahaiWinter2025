#include "stdafx.h"
#include "Actor.h"


Actor::~Actor()
{
	// アクターが削除されるとき、音が鳴りっぱなしにならないように止める
	StopLoopSound();
}

void Actor::StartLoopSound(int soundId)
{
	// 既に再生中なら何もしない（多重再生防止）
	if (m_loopSound != nullptr) {
		return;
	}

	// int を EnSoundList にキャストして再生
	// ループ=true, 3D=true, 座標=自分の位置
	m_loopSound = SoundManager::Play((EnSoundList)soundId, true, true, m_position);
}

void Actor::StopLoopSound()
{
	if (m_loopSound != nullptr) {
		m_loopSound->Stop();
		DeleteGO(m_loopSound);
		m_loopSound = nullptr; // 安全のためnullptrを入れる
	}
}

void Actor::UpdateLoopSound()
{
	// 音源の位置を自分の位置に同期させる
	if (m_loopSound != nullptr) {
		m_loopSound->SetPosition(m_position);
	}
}