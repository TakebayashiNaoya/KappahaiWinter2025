#include "stdafx.h"
#include "TitleStage.h"


TitleStage::TitleStage()
{
	m_filePath = "TitlePlanet/TitlePlanet";
	m_position = Vector3(0.0f, 0.0f, 0.0f);
	InitModel(m_filePath, m_position);
}


TitleStage::~TitleStage()
{
}


bool TitleStage::Start()
{
	return true;
}


void TitleStage::Update()
{
	m_rotation.AddRotationX(-0.005f);
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.Update();
}


void TitleStage::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
