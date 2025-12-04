#include "stdafx.h"
#include "Rocket.h"


Rocket::Rocket()
{
}


Rocket::~Rocket()
{
}


bool Rocket::Start()
{
	m_modelRender.Init("Assets/modelData/Object/Rocket/rocket.tkm");
	m_scale = Vector3(200.0f, 200.0f, 200.0f);
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);
	m_modelRender.Update();
	return true;
}


void Rocket::Update()
{
}


void Rocket::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
