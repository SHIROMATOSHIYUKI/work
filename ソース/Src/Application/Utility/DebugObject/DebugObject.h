#pragma once

class DebugObject
{
public:
	~DebugObject() { Release(); }
	// デバックライン情報の登録
	void AddDebugLine(const Math::Vector3& p1, const Math::Vector3& p2,
		const Math::Color& color = { 1.0f,1.0f,1.0f,1.0f });
	// デバッグスフィア情報登録
	void AddDebugSphere(const Math::Vector3& pos, float radius, const Math::Color& color = { 1.0f,1.0f,1.0f,1.0f });

	// 登録されたすべてのデバッグ情報を描画
	void DrawDebug();

private:
	void Release();

	// デバック描画用の頂点配列
	std::vector<KdEffectShader::Vertex>m_debugVerTex;

};