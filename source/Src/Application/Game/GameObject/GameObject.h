#pragma once

class DebugObject;

// 球体の当たり判定情報
struct SphereInfo
{
	// 引数なしのコンストラクタ = デフォルトコンストラクタ
	SphereInfo() = default;
	// 引数付きコンストラクタ
	SphereInfo(const Math::Vector3& pos, float radius) :
		m_pos(pos),
		m_radius(radius) {}

	Math::Vector3 m_pos;
	float m_radius = 1.0f;
};
// 物体動詞の当たり判定結果
struct BumpResult
{
	bool			m_isHit = false;	// 当たったか
	Math::Vector3	m_pushVec;			// 押し戻す距離
	Math::Vector3	m_HitPos;			// レイが当たった座標
	float			m_Distance = 0.0f;	// レイがHitした際の本体との距離
};
// ダメージ処理の時の当たり判定結果
struct DamegeResult
{
	bool			m_isHit = false;	// 当たったか
	int				m_Damege;			// 何ダメージ与えるか
	Math::Vector3	m_HitPos;			// レイが当たった座標
	float			m_Distance = 0.0f;	// レイがHitした際の本体との距離
};

struct HitResult
{
	bool			m_isHit = false;	// 当たったか
	Math::Vector3	m_pushVec;			// 押し戻す距離
	Math::Vector3	m_HitPos;			// レイが当たった座標
	float			m_Distance = 0.0f;	// レイがHitした際の本体との距離
};
// レイ判定に使う情報
struct RayInfo
{
	RayInfo(){}// デフォルトコンストラクタ(無かったら自動作成)

	RayInfo(const Math::Vector3& pos, const Math::Vector3& dir, float range) :
		m_pos(pos),
		m_dir(dir),
		m_range(range) { m_dir.Normalize(); }


	// デフォルトコンストラクタ以外のコンストラクタを作ること
	// = コンストラクタオーバーロード
	// オーバーロード==引数を好きなよう弄ること

	Math::Vector3 m_pos;
	Math::Vector3 m_dir;
	float m_range = 0.0f;
};

class GameObject
{
public:
	enum ClassID
	{
		eBase,
		eStage,
		ePlayer,
		eEnemy,
		eEnemyManager
	};

	GameObject() {}

	virtual ~GameObject() { Release(); }

	virtual void Init() {}
	virtual void Update() {}
	virtual void Draw();
	virtual void DrawEffect() {}
	virtual void DrawDebug();
	virtual void Draw2D() {}
	virtual void ImGuiProcess() {}
	virtual void onhit() {}

	void Destroy() { m_isAlive = false; }

	// クラスIDセット
	virtual const GameObject::ClassID GetClassID() const { return GameObject::eBase; }
	// 座標の確認
	virtual const Math::Vector3 GetPos() const { return m_mWorld.Translation(); }
	// 座標の設定
	virtual void SetPos(const Math::Vector3& pos) { m_mWorld.Translation(pos); }

	bool IsAlive() { return m_isAlive; }

	virtual bool CheckCollisionBump(const SphereInfo& info, BumpResult& result);		// 球対何かの当たり判定
	bool CheckCollisionRay(const RayInfo& info, BumpResult& result);				// レイ対なにかの当たり判定
	bool CheckCollisionRay(const RayInfo& info, bool& result);
	virtual bool CheckCollisionRay(const RayInfo& info, HitResult& result);

	virtual bool CheckCollisionDamegeSphere(const SphereInfo& info, DamegeResult& result) { return false; }
	virtual bool CheckCollisionDamegeSphere(const RayInfo& info, DamegeResult& result) { return false; }
	virtual bool CheckCollisionDamegeRay(const RayInfo& info, DamegeResult& result) { return false; }
	
	KdModelWork GetModel() const { return m_modelWork; }
	const Math::Matrix GetWorldMatrix()const { return m_mWorld; }

	bool GetDeathFlg() { return DeathFlg; }
	// jsonセット
	void SetJson(std::string fileName);
	// Jsonファイルからの値取得を楽にする変数。戻り値の型に使える。配列
	Math::Rectangle	JsonR(std::string name);
	Math::Rectangle	JsonR(std::string arrayName, std::string name);
	Math::Vector4	JsonV4(std::string name);
	Math::Vector4	JsonV4(std::string arrayName, std::string name);
	Math::Vector3	JsonV3(std::string name);
	Math::Vector3	JsonV3(std::string arrayName,std::string name);
	Math::Vector2	JsonV2(std::string name);
	Math::Vector2	JsonV2(std::string arrayName, std::string name);
	float			JsonF(std::string name) { return (float)m_json[name].number_value(); }
	float			JsonF(std::string arrayName, std::string name){ return (float)m_json[arrayName].object_items()[name].number_value(); }
	int				JsonI(std::string name) { return m_json[name].int_value(); }
	int				JsonI(std::string arrayName, std::string name) { return m_json[arrayName].object_items()[name].int_value(); }
	std::string		JsonS(std::string name) { return m_json[name].string_value(); }
	std::string		JsonS(std::string arrayName, std::string name) { return m_json[arrayName].object_items()[name].string_value(); }

protected:
	KdModelWork m_modelWork;
	Math::Matrix m_mWorld;	// 場所や回転 and scale 情報
	bool m_isAlive = true;

	// このキャラクターの球情報
	SphereInfo m_bumpSphereInfo;
		
	// デバッグオブジェクト
	std::shared_ptr<DebugObject> m_spDebugObject = nullptr;
	bool DeathFlg = false;

	// jsonファイル格納用
	json11::Json m_json;
private:
	void Release() {}
};