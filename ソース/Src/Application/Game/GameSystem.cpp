#include "GameSystem.h"
#include "../main.h"
// インスタンス化には必要
#include "Camera/DebugCamera.h"

#include "GameObject/SceneObject/TitleObject/TitleObject.h"
#include "GameObject/SceneObject/ResultObject/ResultObject.h"
#include "GameObject/SceneObject/GameoverObject/GameoverObject.h"

#include "GameObject/Character/Player/Player.h"
#include "GameObject/Bullet/Bullet.h"
#include "GameObject/StageMap/StageMap.h"
#include "GameObject/Manager/EnemyManager/EnemyManager.h"

#include "GameObject/Effect2D/Effect2D.h"
//#include "GameObject/SceneObject/Event/Event.h"
#include "GameObject/SceneObject/GameEventObject/GameEventObject.h"

#include"GameObject/TextureDraw/FadeInOut/FadeInOut.h"

void GameSystem::ImGuiProcess()
{
	return;
	if (ImGui::Begin("GameSystem"))
	{
		for (std::shared_ptr<GameObject>& spObject : m_spObjects)
		{
			spObject->ImGuiProcess();					
		}
		ImGui::End();
	}
}

// 呼び出されたらセットされたシーンに移行
void GameSystem::ChangeScene()
{
	if (m_nextSceneName == "Title") { TitleInit(); }
	if (m_nextSceneName == "Game") { GameInit(); }
	if (m_nextSceneName == "Result") { ResultInit(); }
	if (m_nextSceneName == "Death") { DeathInit(); }

	m_isRequestCangeScene = false;
}

// 移行してほしいと伝え、移行先クラスを引数で指定
void GameSystem::RequestCangeScene(const std::string& name)
{
	m_nextSceneName = name;			// 名前を上書き
	m_isRequestCangeScene = true;	// チェンジリクエストが来たことを伝える
}

void GameSystem::TitleInit()
{
	Release();// 前シーンの情報は解放して何もない状態にする
	std::shared_ptr<TitleObject> spTitle = std::make_shared<TitleObject>();
	spTitle->Init();
	AddObject(spTitle);
}

void GameSystem::GameInit()
{
	// 前シーンの情報は解放して何もない状態にする
	Release();
	// カーソル消去
	ShowCursor(false);

	// 空モデル
	m_sky.SetModel(m_resourceFactory.GetModelData("Data/Models/Sky/Sky.gltf"));
	m_skyMat = m_skyMat.CreateScale(500.0f); // 空を拡大させる
	
	// 各オブジェクトのインスタンス化
	std::shared_ptr<StageMap> spStage = std::make_shared<StageMap>();
	spStage->Init();
	AddObject(spStage);

	std::shared_ptr<Player> m_spPlayer = std::make_shared<Player>();
	m_spPlayer->Init();
	AddObject(m_spPlayer);
	m_wpPlayer = m_spPlayer;

	m_spEnemyManager = std::make_shared<EnemyManager>();
	m_spEnemyManager->Init();

	std::shared_ptr<GameEventObject> m_spEvent = std::make_shared<GameEventObject>();
	m_spEvent->Init();
	AddObject(m_spEvent);

	m_spDebugCamera = std::make_shared<DebugCamera>();
	m_spDebugCamera->Init();
}

void GameSystem::ResultInit()
{
	Release();	// 前シーンの情報は解放して何もない状態にする
	std::shared_ptr<ResultObject> spResult = std::make_shared<ResultObject>();
	spResult->Init();
	AddObject(spResult);
}

void GameSystem::DeathInit()
{
	Release();	// 前シーンの情報は解放して何もない状態にする
	std::shared_ptr<GameoverObject> spDeath = std::make_shared<GameoverObject>();
	spDeath->Init();
	AddObject(spDeath);
}

void GameSystem::Init()
{
	// AudioManager呼び出し
	m_audioManager.Init();

	// ゲームタイトルから
	TitleInit();
}

void GameSystem::Update()
{
	m_input.Update();
	if (GameInput.GetKeyState("End") & eKeyHold)
	{
		ShowCursor(true);	// 確認画面ではカーソルを描画
		if (MessageBoxA(APP.m_window.GetWndHandle(),"本当にゲームを終了しますか？", "確認",MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDYES)
		{
			APP.End();
		}
		else{ShowCursor(false);}// 確認画面からゲームに戻るときにカーソルを消す
		SetCursorPos(FPSCamera::s_fixMousePos.x, FPSCamera::s_fixMousePos.y);
	}

	// デバッグカメラ起動キー
	if (GameInput.GetKeyState("Debug") & eKeyPress)
	{
		if (!m_isPushedDebugFreezeKey)
		{
			m_debugFreeze = !m_debugFreeze;
		}
		m_isPushedDebugFreezeKey = true;
	}
	else// TABが押されていなかったら
	{
		m_isPushedDebugFreezeKey = false;
	}

	if (m_debugFreeze)
	{
		if (m_spDebugCamera)
		{
			m_spDebugCamera->Update();

			m_spDebugCamera->SetCameraMatrix(Math::Matrix());
		}
		return;
	}

	if (m_isRequestCangeScene)	// シーン切り替え
	{
		// フェードアウトの終了を監視して終了したら画面を切り替える
		ChangeScene();
		return;
	}

	for (std::shared_ptr<GameObject>& spObject : m_spObjects)// ゲームオブジェクトの更新
	{spObject->Update();}

	if (m_spEnemyManager)
	{
		m_spEnemyManager->Update();
	}
	std::list<std::shared_ptr<GameObject>>::iterator objItr = m_spObjects.begin();
	while (objItr != m_spObjects.end())// 死んだオブジェクトの除外を行う
	{
		if (!(*objItr)->IsAlive())
		{
			// スマートポインタの解放は自動で行われる
			objItr->reset(/*新しくシェアするポインタ*/);
			objItr = m_spObjects.erase(objItr);

			continue;
		}
		++objItr;
	}
	// サウンド関連の更新
	{
		m_audioManager.Update(Math::Vector3::Zero, Math::Vector3::Backward);
	}
}

void GameSystem::Draw()
{
	if (m_debugFreeze)
	{
		if (m_spDebugCamera)
		{
			m_spDebugCamera->SetToShader();
		}
	}
	else
	{
		// カメラの情報をシェーダーに渡す
		if (m_spCamera)
		{
			m_spCamera->SetToShader();
		}
	}

	// ①不透明物の描画から
	// 不透明物描画用シェーダーに切り替え
	SHADER->m_standardShader.SetToDevice();

	// ゲームオブジェクトの(不透明物)描画(範囲ベースfor文)
	for (std::shared_ptr<GameObject>& spObject : m_spObjects)
	{
		spObject->Draw();
	}

	// ②透明物の描画
	SHADER->m_effectShader.SetToDevice();

	// 拡大行列を適用する
	SHADER->m_effectShader.DrawModel(m_sky, m_skyMat);

	{
		// ZWriteDisable 奥にあるものは奥に書くけど、隠れてても描画して！
		D3D.WorkDevContext()->OMSetDepthStencilState(SHADER->m_ds_ZEnable_ZWriteDisable, 0);

		// カリング無し(両面描画)
		D3D.WorkDevContext()->RSSetState(SHADER->m_rs_CullNone);

		// Droweffectを呼ぶ
		for (std::shared_ptr<GameObject>& spObject : m_spObjects)
		{
			spObject->DrawEffect();
		}

		D3D.WorkDevContext()->OMSetDepthStencilState(SHADER->m_ds_ZEnable_ZWriteEnable, 0);
		// 裏面カリング(表面のみ描画)
		D3D.WorkDevContext()->RSSetState(SHADER->m_rs_CullBack);
	}

	// 2D系描画をまとめて実行
	SHADER->m_spriteShader.Begin();	// 2D描画はbeginとendの間だけ
	{
		for (std::shared_ptr<GameObject>& spObject : m_spObjects)
		{
			spObject->Draw2D();
		}
	}
	SHADER->m_spriteShader.End();

	// デバック描画
	SHADER->m_effectShader.SetToDevice();
	SHADER->m_effectShader.SetTexture(D3D.GetWhiteTex()->WorkSRView());
	{
		for (std::shared_ptr<GameObject>& spObject : m_spObjects)
		{
			spObject->DrawDebug();
		}
	}
}

const std::shared_ptr<KdCamera> GameSystem::GetCamera() const
{
	return m_spCamera;
}

void GameSystem::Release()
{
	m_audioManager.StopAllSound();
	// リストのリセット
	m_spObjects.clear();
}
