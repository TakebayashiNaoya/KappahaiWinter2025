#include "stdafx.h"
#include "Treasure.h"

Treasure::Treasure()
{
}

Treasure::~Treasure()
{
}

bool Treasure::Start()
{
	m_animationClip[enAnimationClip_Idle].Load("Assets/animData/Treasure/idle.tka");
	m_animationClip[enAnimationClip_Idle].SetLoopFlag(false);
	m_animationClip[enAnimationClip_Open].Load("Assets/animData/Treasure/open.tka");
	m_animationClip[enAnimationClip_Open].SetLoopFlag(false);
	m_modelRender.Init("Assets/modelData/Object/Treasure/Treasure.tkm", m_animationClip, enAnimationClip_Num, enModelUpAxisY);

	m_modelRender.PlayAnimation(enAnimationClip_Idle);

	m_scale = Vector3(200.0f, 200.0f, 200.0f);
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);
	m_modelRender.Update();
	return true;
}

void Treasure::Update()
{
	switch (m_state)
	{
	case enState_Init:
		m_modelRender.PlayAnimation(enAnimationClip_Idle);
		m_state = enState_Idle;
		break;

	case enState_Idle:
		if (m_isOpened) {
			m_modelRender.PlayAnimation(enAnimationClip_Open);
			m_state = enState_Opened;
		}
		break;

	case enState_Opened:
		break;
	}

	m_modelRender.Update();
}

void Treasure::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
