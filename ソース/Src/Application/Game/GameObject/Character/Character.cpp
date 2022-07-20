#include"Character.h"

bool Character::CheckCollisionDamegeSphere(const RayInfo& info,DamegeResult& result)
{
	KdRayResult rayResult;
	float minDist = FLT_MAX;	// FLoaTMAX

	for (UINT i = 0; i < m_modelWork.GetDataNodes().size(); ++i)
	{
		const KdModelData::Node& dataNode = m_modelWork.GetDataNodes()[i];	// ノードデータの取得
		if (!dataNode.m_spMesh) { continue; }								// メッシュがないノード(カメラやライトもあるので)その時は判定をしない
		const KdModelWork::Node& workNode = m_modelWork.GetNodes()[i];		// ノードワークの取得
		KdRayToMesh(
			info.m_pos,								// 発射座標
			info.m_dir,								// 発射方向
			info.m_range,							// 判定する長さ
			*(dataNode.m_spMesh.get()),				// 相手のメッシュ情報
			workNode.m_worldTransform * m_mWorld,	// 相手の行列
			&rayResult);							// 出力結果

		if (rayResult.m_hit && rayResult.m_distance < minDist)	// 今までで一番近かったメッシュ(最初に当たったはずのメッシュ)
		{
			result.m_isHit = rayResult.m_hit;			//  当たったという結果を渡す
			minDist = rayResult.m_distance;				// 当たった座標までの距離を入れて、この距離以上の物とは判定しないようにする
			result.m_HitPos = rayResult.m_HitPos;		// hitした座標を渡す
			result.m_Distance = rayResult.m_distance;	// 距離を渡す
		}
	}
	return rayResult.m_hit;
}