#include "StageMap.h"

void StageMap::Init()
{
	m_modelWork.SetModel(GameResourceFactory.GetModelData("Data/Models/Map/Map.gltf"));// モデルの読み込み
}

void StageMap::onhit()
{
	//std::shared_ptr<KdSoundInstance> spSoundInstance = nullptr;
	//spSoundInstance = GameAudio.Play3D("Data/Audio/Discovery.wav",GetPos());
	//spSoundInstance->SetVolume(0.1);
}

bool StageMap::CheckCollisionBump(const SphereInfo& info, BumpResult& result)
{
	for (UINT i = 0; i < m_modelWork.GetDataNodes().size(); ++i)	// StageMapのメッシュ達戸当たり判定
	{		
		const KdModelData::Node dataNode = m_modelWork.GetDataNodes()[i];// 該当のノードを取得
		if (!dataNode.m_spMesh) { continue; }								// メッシュでなかったら判定しない
		// ノードの現在の位置や回転情報					↓変更可能データ
		const KdModelWork::Node workNode = m_modelWork.GetNodes()[i];

		// 最終的にすべての当たった面から差し戻されるベクトル
		// 押し戻された後の座標
		Math::Vector3 localPushedPos = {};

		if (KdSphereToMesh(info.m_pos, info.m_radius,
			*(dataNode.m_spMesh.get()), workNode.m_worldTransform * m_mWorld, localPushedPos))
		{
			result.m_isHit = true;
			result.m_pushVec += localPushedPos - info.m_pos ;
		}
	}

	return result.m_isHit;
}

void StageMap::Release()
{

}