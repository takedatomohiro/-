#include "stdafx.h"
#include "BackGround.h"

BackGround::BackGround()
{
	m_modelRender.Init("Assets/stage/start.tkm");
	m_modelRender.SetScale({ 50.0f,0.0f,50.0f });
	m_modelRender.Update();
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
}

BackGround::~BackGround()
{
}

//void BackGround::Update()
//{
//	m_modelRender.Update();
//}

void BackGround::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}