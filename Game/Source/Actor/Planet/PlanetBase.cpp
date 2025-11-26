#include "stdafx.h"
#include "PlanetBase.h"

PlanetBase::PlanetBase()
{
}


PlanetBase::~PlanetBase()
{
}


bool PlanetBase::Start()
{
	return true;
}


void PlanetBase::Update()
{
}


void PlanetBase::Render(RenderContext& rc)
{
}

void PlanetBase::InitModel(const std::string& filePath, const Vector3& position)
{
	std::string fullFilePath = "Assets/modelData/stage/" + filePath + ".tkm";
	m_modelRender.Init(fullFilePath.c_str());
	m_modelRender.SetPosition(position);
	physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
}