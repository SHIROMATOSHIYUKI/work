#include"Effect2D.h"
#include "../Bullet/Bullet.h"

void Effect2D::Init()
{
	m_poly.Init(2.0f, 2.0f,kWhiteColor);	// (w横幅,h高さ{R,G,B,R})
	m_lifeSpan = 200;
}

void Effect2D::Update()
{	
	// エフェクトの移動設定
	if (m_movePos != Math::Vector3::Zero)	
	{
		m_mWorld.Translation(m_mWorld.Translation() + m_movePos);
	}
	// サイズ変更設定
	if (m_size != Math::Vector3::Zero)
	{
		m_mWorld._11 *= m_size.x;
		m_mWorld._22 *= m_size.y;
		m_mWorld._33 *= m_size.z;
	}

	// アニメーションしないか
	if (m_poly.GetAnimationSize() <= 1|| !m_isLoop)	
	{
		if (--m_lifeSpan <= 0)
		{
			m_isAlive = false;
		}
	}

	// アニメーションがないならアニメーションの更新・終了判定を行わない
	if (m_poly.GetAnimationSize() <= 1) { return; }	

	m_poly.Animation(m_animSpd,m_isLoop );

	if (m_poly.IsAnimationEnd())
	{
		m_isAlive = false;	// ゲームシステム側でm_isAliveをみてfalseだったら自動的に解放してくれる、スマートポインタだからアクセスしてなかったら自動的に消える！
	}
}

void Effect2D::DrawEffect()
{
	Math::Matrix mDraw = m_mWorld;
	// 行列の拡大 = S
	Math::Vector3 scale;
	scale.x = m_mWorld.Right().Length();	// 右方向の拡大率
	scale.y = m_mWorld.Up().Length();		// 上方向の拡大率
	scale.z = m_mWorld.Forward().Length();	// 前方向の拡大率

	mDraw = Math::Matrix::CreateScale(scale);

	// 行列の回転 = R
	mDraw *= Math::Matrix::CreateRotationZ(m_angleZ);

	std::shared_ptr<KdCamera> gameCam = GameInstance.GetCamera();

	if (gameCam)
	{
		// カメラの逆行列の合成 = ワールド行列->カメラから見た行列に対する
		Math::Matrix mCamInv = gameCam->GetCameraMatrix();
		mCamInv.Invert();

		// カメラの目の前に持ってくる
		mDraw *= mCamInv;
	}
	
	// 行列の移動 = T
	mDraw.Translation(m_mWorld.Translation());

	SHADER->m_effectShader.DrawSquarePolygon(m_poly, mDraw);
}

void Effect2D::SetTexture(const std::shared_ptr<KdTexture> spTex, float angleZ,float w, float h, Math::Color col)
{
	if (!spTex) { return; }
	m_poly.SetTexture(spTex);
	// ポリゴンの設定
	m_poly.Init(w, h, col);

	m_angleZ = angleZ;
}

void Effect2D::SetAnimation(int splitX, int splitY, float speed, bool isLoop)
{
	m_poly.SetAnimationInfo(splitX, splitY);
	// 他のアニメーションも一緒に設定
	m_animSpd = speed;
	m_isLoop = isLoop;
}