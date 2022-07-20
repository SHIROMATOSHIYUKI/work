#pragma once
#include"../EnemyBase.h"

class Player;

class Turret : public EnemyBase
{
public:
	Turret(){}
	~Turret(){}

	void Init() override;
	void Update() override;
	void Draw2D() override;
	void UpdateDebug();				// デバック
	void ImGuiProcess() override;	// デバッグウインドウ

	void SetPos(const Math::Vector3& pos) override { m_worldPos = pos; }
	// 頭の初期向き設定
	void SetHeadRot(const Math::Vector3& Angle) { m_headRot = Angle; }
	// 当たり判定
	//bool CheckCollisionDamegeSphere(const SphereInfo& info, DamegeResult& result) override;
	
	// 最後の敵ならフラグを立てる
	void SetLastEnemyFlg(){ m_lastEnemyFlg = true; }
	bool GetLastFlg() { return m_lastEnemyFlg; }

	
private:
	void Release(){}

	//=======-====
	//　弾発射関連
	//============
	void	ShotBullet();
	int		m_shotBreakCount = 0;	// 弾が発射できるかどうか
	int		m_shotCount = 0;

	//=======-====
	//　頭の回転
	//============
	void HeadRotate();		// 回転に関する処理
	float GetBetweenAngX(const Math::Vector3& dir1, const Math::Vector3& dir2);
	float GetBetweenAngY(const Math::Vector3& dir1, const Math::Vector3& dir2);
	float betweenAngY = 0.0f;
	Math::Vector3 m_worldPos;		// 座標
	Math::Vector3 m_headRot;		// 回転
	std::shared_ptr<Player> m_spTarget;
	KdModelWork::Node* m_pHead;
	KdModelWork::Node* m_pMuzzlePoint;
	//========
	//視野判定
	//========
	void FovCollision();// 視野判定統合
	void FovAngle();	// 視野円錐判定
	void FovSphere();	// 視野球判定

	float m_fovAngle = 0.0f;

	SphereInfo fovSphereInfo;
	bool m_sphereHit = false;
	bool m_angleHit = false;
	bool m_fovOllHit = false;
	float m_betweenDegree = 0.0f;

	void FovObject();	// オブジェクト判定 
	bool m_visiblePlayerFlg = false;	// Playerが見えない状態

	// 最後の部屋の敵か確認
	bool m_lastEnemyFlg = false;

	//==========
	//見失った時
	//==========
	std::shared_ptr<KdTexture> m_spQuestionTex;
	//==========
	//攻撃警告
	//==========
	std::shared_ptr<KdTexture> m_spWarningTex;	// 警告画像
	int m_warningCount = 0;

	//=========
	// リロード
	//=========
	bool	m_reloadSoundFlg = true;
	int		m_reloadCount = 0;
	std::shared_ptr<KdSoundInstance> m_reloadSound;

	//=======
	// 死亡時
	//=======
	bool DeathMoment = true;
	int SmokeCnt = 0;
	
	//========================
	// ステートパターンの基礎=
	//========================
	class ActionBase
	{
	public:
		virtual void Update(Turret& owner) = 0;
		virtual void Draw2D(Turret& owner) = 0;
	};
	//=======
	// 各行動
	//=======
	class ActionSearch : public ActionBase
	{	// 探索
	public:
		void Update(Turret& owner) override;
		void Draw2D(Turret& owner) override {};
	};
	class ActionChase	: public ActionBase
	{	// 追跡
	public:
		void Update(Turret& owner) override;
		void Draw2D(Turret& owner) override {};
	};	
	class ActionMiss : public ActionBase
	{	// 見失う
	public:
		void Update(Turret& owner) override;
		void Draw2D(Turret& owner) override;
	};
	class ActionWarning : public ActionBase
	{	// 警告
	public:
		void Update(Turret& owner) override;
		void Draw2D(Turret& owner) override;
	};
	class ActionAttack	: public ActionBase
	{	// 射撃
	public:
		void Update(Turret& owner) override;
		void Draw2D(Turret& owner) override {};
	};
	class ActionReload : public ActionBase
	{	// リロード
	public:
		void Update(Turret& owner) override;
		void Draw2D(Turret& owner) override {};
	};
	class ActionDeath	: public ActionBase
	{	// 死亡
	public:
		void Update(Turret& owner) override;
		void Draw2D(Turret& owner) override {};
	};

	std::shared_ptr<ActionBase> m_spActionState = nullptr;

};