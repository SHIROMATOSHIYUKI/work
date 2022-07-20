#pragma once
#include "../GameObject.h"

class Bullet : public GameObject
{
public:
	Bullet(){}
	~Bullet() { Release(); }

	void Init()		override;
	void Update()	override;
	void DrawEffect() override;	// trail用
	void SetWorldMatrix(const Math::Matrix& world) { m_mWorld = world; }	// 向きと場所の設定
	void SetDir(const Math::Vector3& gun, const Math::Vector3& goal, const Math::Vector3& gunDir) {
		m_gunPos = gun; m_goalPos = goal; m_goalDir = gunDir;}
	void SetRayHit(bool hit) { rayHit = hit; }
private:
	void Release(){}
	void UpdateCollision();

	Math::Vector3	m_gunPos;	// 銃座標
	Math::Vector3	m_goalPos;	// 目的座標
	Math::Vector3	m_goalDir;	// 銃から目的への角度
	Math::Matrix	m_trans;

	bool rayHit;

	int		m_lifeSpan = 0;				// 寿命のカウンタ
	Math::Vector3	m_prevPos = {};		// 動く前の位置
	KdTrailPolygon m_trail;				// 軌跡を描く

	//=======
	// 判定用
	//=======
	bool	m_isStabbed = false;// 刺さったかどうかフラグ
	SphereInfo m_damegeSphereInfo;
};