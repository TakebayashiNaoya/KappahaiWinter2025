#include "stdafx.h"
#include "FirstStage.h"


FirstStage::FirstStage()
{
	m_filePath = "firstPlanet";
	m_position = Vector3(0.0f, 0.0f, 0.0f);
	InitModel(m_filePath, m_position);
}


FirstStage::~FirstStage()
{
}


bool FirstStage::Start()
{

	return true;
}


void FirstStage::Update()
{

}


void FirstStage::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
