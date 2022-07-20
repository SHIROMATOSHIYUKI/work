#pragma once

class GameObject;
class DebugCamera;
class EnemyManager;
class Player;
class FadeInOut;

class GameSystem
{
public:

	void ImGuiProcess();	// GameSystem内のImGui関連の処理

	void Init();	// 初期化
	void Update();	// 更新
	void Draw();	// 描画
	void RequestCangeScene(const std::string& name);	// リクエストを受け取る関数
	
	void AddObject( std::shared_ptr<GameObject> spObject) // オブジェクトの追加
	{
		if (spObject) { m_spObjects.push_back( spObject ); }
	}

	// ゲームオブジェクトのリスト毎取得する
	const std::list<std::shared_ptr<GameObject>>& GetObjects()
	{
		return m_spObjects;
	}

	//? shared=shared : 参照カウンタが1つ増える
	void SetCamera(std::shared_ptr<KdCamera> spCamera) { m_spCamera = spCamera; }
	const std::shared_ptr<KdCamera> GetCamera() const;

	// リソースファクトリーの実体を返す
	ResourceFactory& WorkResourceFactory() { return m_resourceFactory; }
	// インプットコントローラーを返す
	const InputController& GetInputController () { return m_input; }
	// エネミーマネージャーを返す
	std::shared_ptr<EnemyManager> WorkEnemyManeger() { return m_spEnemyManager; }
	// プレイヤークラスを返す
	const std::weak_ptr<Player>& GetPlayer() { return m_wpPlayer; }
	std::weak_ptr<Player> WorkPlayer() { return m_wpPlayer; }
	// オーディオを返す
	KdAudioManager& WorkAudnioManager() { return m_audioManager; }
	// Jsonを返す
	json11::Json& WorkJson() { return m_jsonObj; }

	// デバッグフラグ、trueだと更新をすべて止める
	bool m_debugFreeze = false;
	bool m_isPushedDebugFreezeKey = false;

	// キャラクターにかかる重力
	static const float s_worldGravity;

private:
	KdModelWork m_sky;						// 空のモデル
	DirectX::SimpleMath::Matrix m_skyMat;	// 空のワールド行列

	std::weak_ptr<Player>			m_wpPlayer;
	std::shared_ptr<EnemyManager>	m_spEnemyManager;
	ResourceFactory m_resourceFactory;		// リソース管理クラス
	InputController m_input;				// 入力管理クラス
	KdAudioManager	m_audioManager;			// オーディオ管理クラス
	json11::Json	m_jsonObj;

	std::shared_ptr<KdCamera>	m_spCamera = nullptr;	// カメラ
	std::shared_ptr<DebugCamera>m_spDebugCamera = nullptr;

	std::shared_ptr<FadeInOut> m_spFadeInOut;	// フェードイン

	void Release();	// 解放

	std::list<std::shared_ptr<GameObject>> m_spObjects;	// 住所が入っていて確認できるゲームオブジェクトリスト
	
	void ChangeScene();	// 実際にシーンを変更する関数
	void TitleInit();	// 初期化：タイトル
	void GameInit();	// 初期化：ゲーム
	void ResultInit();	// 初期化：リザルト
	void DeathInit();	// 初期化：リザルト
	
	std::string m_nextSceneName = "";
	bool m_isRequestCangeScene = false;
	

	GameSystem() {}				// コンストラクター
	~GameSystem() { Release(); }// デストラクター

public:
	// GameSystemインスタンスの取得static関数はインスタンス化されていなくても呼び出せるただし、同じくstaticなメンバにしかアクセス出来ない
	static GameSystem& GetInstance()
	{
		// 唯一のインスタンス
		static GameSystem instance;
		return instance;
	}
};

// GameSystemを呼び出すマクロ
#define GameInstance GameSystem::GetInstance()

// リソースファクトリーを取得するマクロ
#define GameResourceFactory GameInstance.WorkResourceFactory()

// エネミーマネージャーを取得するマクロ
#define GameEnemyManeger GameInstance.WorkEnemyManeger()

// プレイヤーを取得するマクロ
#define GameEnemyManeger GameInstance.WorkEnemyManeger()

// インプットコントローラーを取得するマクロ
#define GameInput GameInstance.GetInputController()

// オーディオを取得するマクロ
#define GameAudio GameInstance.WorkAudnioManager()

// Jsonを取得するマクロ
#define GameJson GameInstance.WorkJson()