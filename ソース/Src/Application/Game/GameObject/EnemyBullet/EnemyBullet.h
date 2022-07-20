#pragma once
#include "../GameObject.h"

class EnemyBullet : public GameObject
{
public:
	EnemyBullet(){}
	~EnemyBullet(){ Release(); }

	void Init()		override;
	void Update()	override;
	void DrawEffect() override;	// trail用
	void SetWorldMatrix(const Math::Matrix& world) { m_mWorld = world; }	// 向きと場所の設定
	void SetDir(const Math::Vector3& gunDir){ m_goalDir = gunDir; }
private:
	void Release() {}
	void UpdateCollision();
	Math::Vector3	m_goalDir;			// headから真っすぐの角度
	Math::Matrix	m_trans;
	int		m_lifeSpan = 0;				// 寿命のカウンタ
	Math::Vector3	m_prevPos = {};		// 動く前の位置
	KdTrailPolygon m_trail;				// 軌跡を描く

	bool	m_isStabbed = false;// 刺さったかどうかフラグ
};