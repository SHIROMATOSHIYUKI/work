#pragma once
#include"../GameObject.h"

class Effect2D :public GameObject
{
public:
	Effect2D()	{}
	~Effect2D()	{}

	void Init()			override;
	void Update()		override;
	void Draw()			override{}	// 中身空っぽに
	void DrawEffect()	override;

	// 使用するテクスチャを設定
	void SetTexture(const std::shared_ptr<KdTexture> spTex
		, float angleZ =0.0f, float w = 1.0f, float h = 1.0f, Math::Color col = kWhiteColor);

	void SetAnimation(int splitX, int splitY, float speed = 1.0f, bool isLoop = false);// アニメーションの設定

	int GetAnimationSize() const { return m_poly.GetAnimationSize(); }//アニメーションのコマ数を取得(縦のコマ数*横のコマ数)

	void SetLifeSpan(int lifeSpan) { m_lifeSpan = lifeSpan; }
	// 移動量セット
	void SetMove(Math::Vector3 pos) { m_movePos = pos; }
	// サイズセット
	void SetSize(Math::Vector3 size) { m_size =size;}
private:
	KdSquarePolygon m_poly;	// 四角ポリゴン

	Math::Vector3 m_targetPos;

	float	m_animSpd = 0.0f;
	bool	m_isLoop = false;

	int	m_splitX = 0;	// 横のコマ数
	int	m_splitY = 0;
	float m_animPos = 0;

	int		m_lifeSpan = 0;		// 寿命のカウンタ

	float	m_angleZ = 0.0f;

	// 移動量関連
	Math::Vector3 m_movePos;

	// サイズ関連
	Math::Vector3 m_size;
};