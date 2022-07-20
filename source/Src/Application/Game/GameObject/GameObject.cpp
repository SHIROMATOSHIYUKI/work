#include "GameObject.h"
#include "../../Utility/DebugObject/DebugObject.h"

void GameObject::Draw()
{
	SHADER->m_standardShader.DrawModel(m_modelWork, m_mWorld);
}

bool GameObject::CheckCollisionBump(const SphereInfo& info,BumpResult& result)
{
	Math::Vector3 selfPos = GetPos() + m_bumpSphereInfo.m_pos;

	// 相手の座標と自分の座標を計算(自分から相手に向かうベクトル)
	Math::Vector3 betweenVec = info.m_pos - selfPos;	// ここ授業でやるよ

	// ２つのキャラの間の距離
	float distanceSqr = betweenVec.LengthSquared();

	// ２つの屋良の当たり判定球の半径の合計
	float hitRadius = m_bumpSphereInfo.m_radius + info.m_radius;

	// ２つのキャラの間の距離 <= ２つのキャラの当たり判定球の半径の合計
	result.m_isHit = (distanceSqr <= (hitRadius * hitRadius));

	// 当たってたら
	if (result.m_isHit)
	{
		// 押し戻すのに都合のいいベクトル
		result.m_pushVec = betweenVec;
		result.m_pushVec.Normalize();

		// どの程度押し戻すか？
		float distance = std::sqrtf(distanceSqr);
		result.m_pushVec *= hitRadius - distance;
	}
	return result.m_isHit;
}

// オブジェクトの衝突判定
bool GameObject::CheckCollisionRay(const RayInfo& info, BumpResult& result)
{
	KdRayResult rayResult;
	float minDist = FLT_MAX;	// FLoaTMAX

	for(UINT i=0; i < m_modelWork.GetDataNodes().size(); ++i)
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
	
		if(rayResult.m_hit && rayResult.m_distance < minDist)				// 今までで一番近かったメッシュ(最初に当たったはずのメッシュ)
		{
			result.m_isHit = rayResult.m_hit;								//  当たったという結果を渡す
			result.m_pushVec = info.m_dir * (
				rayResult.m_distance - info.m_range);						// 押し返す分のベクトルを計算する
			minDist = rayResult.m_distance;									// 当たった座標までの距離を入れて、この距離以上の物とは判定しないようにする
			result.m_HitPos = rayResult.m_HitPos;							// hitした座標を渡す
			result.m_Distance = rayResult.m_distance;						// 距離を渡す
		}
	}
	return rayResult.m_hit;
}

bool GameObject::CheckCollisionRay(const RayInfo& info, bool& result)
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

		if (rayResult.m_hit && rayResult.m_distance < minDist)				// 今までで一番近かったメッシュ(最初に当たったはずのメッシュ)
		{
			result = true;
		}
	}
	return result;
}

bool GameObject::CheckCollisionRay(const RayInfo& info, HitResult& result)
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

		if (rayResult.m_hit && rayResult.m_distance < minDist)				// 今までで一番近かったメッシュ(最初に当たったはずのメッシュ)
		{
			result.m_isHit = rayResult.m_hit;								//  当たったという結果を渡す
			result.m_pushVec = info.m_dir * (
				rayResult.m_distance - info.m_range);						// 押し返す分のベクトルを計算する
			minDist = rayResult.m_distance;									// 当たった座標までの距離を入れて、この距離以上の物とは判定しないようにする
			result.m_HitPos = rayResult.m_HitPos;							// hitした座標を渡す
			result.m_Distance = rayResult.m_distance;						// 距離を渡す
		}
	}
	return rayResult.m_hit;
}

void GameObject::SetJson(std::string fileName)
{
	// JSONファイルを開く
	std::ifstream ifs(fileName);
	// 読み込めているか確認
	if (ifs.fail()) { assert(0 && "Jsonファイルのパスが間違っています"); }
	// 文字列として格納する
	std::string strJson((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	// 文字列のJSONを解析(パースする)
	std::string err;
	m_json = json11::Json::parse(strJson, err);
	if (err.size() > 0) { assert(0 && "読み込んだファイルのJson変換に失敗"); }
}

Math::Rectangle GameObject::JsonR(std::string name)
{
	Math::Rectangle value;
	auto& object = m_json[name].array_items();
	value = {
		(long)object[0].number_value(),
		(long)object[1].number_value(),
		(long)object[2].number_value(),
		(long)object[3].number_value()
	};
	return value;
}

Math::Rectangle GameObject::JsonR(std::string arrayName, std::string name)
{
	Math::Rectangle value;
	auto& array = m_json[arrayName].object_items();
	value = {
		(long)array[name][0].number_value(),
		(long)array[name][1].number_value(),
		(long)array[name][2].number_value(),
		(long)array[name][3].number_value()
	};
	return value;
}

Math::Vector4 GameObject::JsonV4(std::string name)
{
	Math::Vector4 value;
	auto& object = m_json[name].array_items();
	value = {
		(float)object[0].number_value(),
		(float)object[1].number_value(),
		(float)object[2].number_value(),
		(float)object[3].number_value()
	};
	return value;
}

Math::Vector4 GameObject::JsonV4(std::string arrayName, std::string name)
{
	Math::Vector4 value;
	auto& array = m_json[arrayName].object_items();
	value = {
		(float)array[name][0].number_value(),
		(float)array[name][1].number_value(),
		(float)array[name][2].number_value(),
		(float)array[name][3].number_value()
	};
	return value;
}

// jsonからVector3取得
Math::Vector3 GameObject::JsonV3(std::string name)
{
	Math::Vector3 value;
	auto& object = m_json[name].array_items();
	value = { 
		(float)object[0].number_value(),
		(float)object[1].number_value(),
		(float)object[2].number_value()
	};
	return value;
}
// jsonの配列からVector3取得
Math::Vector3 GameObject::JsonV3(std::string arrayName,std::string name)
{	
	Math::Vector3 value;
	auto& array = m_json[arrayName].object_items();
	value = {	
		(float)array[name][0].number_value(),
		(float)array[name][1].number_value(),
		(float)array[name][2].number_value() 
	};
	return value;
}
// jsonからVector2取得
Math::Vector2 GameObject::JsonV2(std::string name)
{
	Math::Vector2 value;
	auto& object = m_json[name].array_items();
	value = {
		(float)object[0].number_value(),
		(float)object[1].number_value()
	};
	return value;
}
// jsonの配列からVector2取得
Math::Vector2 GameObject::JsonV2(std::string arrayName, std::string name)
{
	Math::Vector2 value;
	auto& array = m_json[arrayName].object_items();
	value = {
		(float)array[name][0].number_value(),
		(float)array[name][1].number_value()
	};
	return value;
}

void GameObject::DrawDebug()
{
	if (!m_spDebugObject) { return; }
	m_spDebugObject->DrawDebug();
}

//bool GameObject::CheckCollision(const Math::Vector3& rayPos, const Math::Vector3& rayDir, float hitRange)
//{
//	KdRayResult result;
//
//	const std::shared_ptr<KdMesh>& spMesh = m_modelWork.GetDataNodes()[0].m_spMesh;
//
//	KdRayToMesh(rayPos,
//		rayDir,
//		hitRange, *(spMesh.get()), m_mWorld, &result);
//
//	return result.m_hit;
//}