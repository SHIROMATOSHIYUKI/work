#include "DebugObject.h"

void DebugObject::AddDebugLine(const Math::Vector3& p1, const Math::Vector3& p2, const Math::Color& color)
{
	KdEffectShader::Vertex ver;
	ver.Color = color;
	ver.UV = { 0.0f,0.0f };

	ver.Pos = p1;
	m_debugVerTex.push_back(ver);

	ver.Pos = p2;
	m_debugVerTex.push_back(ver);
}

void DebugObject::AddDebugSphere(const Math::Vector3& pos, float radius, const Math::Color& color)
{
	KdEffectShader::Vertex ver;
	ver.Color = color;
	ver.UV = { 0.0f,0.0f };

	int kDetail = 16;
	for (int i = 0; i < kDetail + 1; i++)
	{
		// XZ平面
		ver.Pos = pos;
		ver.Pos.x += cos((float)i * (360 / kDetail) * KdToRadians) * radius;
		ver.Pos.z += sin((float)i * (360 / kDetail) * KdToRadians) * radius;
		m_debugVerTex.push_back(ver);

		ver.Pos = pos;
		ver.Pos.x += cos((float)(i + 1) * (360 / kDetail) * KdToRadians) * radius;
		ver.Pos.z += sin((float)(i + 1) * (360 / kDetail) * KdToRadians) * radius;
		m_debugVerTex.push_back(ver);

		// XY平面
		ver.Pos = pos;
		ver.Pos.x += cos((float)i * (360 / kDetail) * KdToRadians) * radius;
		ver.Pos.y += sin((float)i * (360 / kDetail) * KdToRadians) * radius;
		m_debugVerTex.push_back(ver);

		ver.Pos = pos;
		ver.Pos.x += cos((float)(i + 1) * (360 / kDetail) * KdToRadians) * radius;
		ver.Pos.y += sin((float)(i + 1) * (360 / kDetail) * KdToRadians) * radius;
		m_debugVerTex.push_back(ver);

		// YZ平面
		ver.Pos = pos;
		ver.Pos.y += cos((float)i * (360 / kDetail) * KdToRadians) * radius;
		ver.Pos.z += sin((float)i * (360 / kDetail) * KdToRadians) * radius;
		m_debugVerTex.push_back(ver);

		ver.Pos = pos;
		ver.Pos.y += cos((float)(i + 1) * (360 / kDetail) * KdToRadians) * radius;
		ver.Pos.z += sin((float)(i + 1) * (360 / kDetail) * KdToRadians) * radius;
		m_debugVerTex.push_back(ver);
	}
}

void DebugObject::DrawDebug()
{
	if (m_debugVerTex.size() >= 2)
	{
		SHADER->m_effectShader.SetWorldMatrix(Math::Matrix());
		SHADER->m_effectShader.DrawVertices(m_debugVerTex, D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	}

	// 登録されたデバック情報は描画後一旦クリア
	m_debugVerTex.clear();
}

void DebugObject::Release()
{
	m_debugVerTex.clear();
}