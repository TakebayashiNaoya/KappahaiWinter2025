#include "stdafx.h"
#include "TitlePlanet.h"


TitlePlanet::TitlePlanet()
{
	m_filePath = "TitlePlanet/TitlePlanet";
	m_position = Vector3(0.0f, 0.0f, 0.0f);
	InitModel(m_filePath, m_position);
}


TitlePlanet::~TitlePlanet()
{
}


bool TitlePlanet::Start()
{
	return true;
}


void TitlePlanet::Update()
{
	m_rotation.AddRotationX(-0.005f);
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.Update();
}


void TitlePlanet::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
