#pragma once

#include "FPSCamera.h"

class DebugCamera : public FPSCamera
{
public:
	DebugCamera() {}
	~DebugCamera(){}

	void Init();
	void Update() override;

	void SetCameraMatrix(const DirectX::SimpleMath::Matrix& mWorld) override;

private:
	Math::Matrix m_mWorld;
	POINT m_prevMousePos;

	Math::Vector3 m_viewDistance{ 0.0f,0.0f,-10.0f};


};