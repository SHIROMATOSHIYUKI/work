#pragma once
#include"../TextureDraw.h"
// イージング画像を描画するクラス
class TextureEasing :public TextureDraw
{
public:
	TextureEasing(){}
	~TextureEasing(){}

	void Init(
		std::string name,
		float cvQty,
		std::string easingName,
		Math::Vector2 pos = { 0,0 },
		Math::Rectangle rect = { 0,0,1280,720 },
		Math::Color color = { 1,1,1,1 }		
	);
	void Update() override;
	void Draw2D() override;
	
private:
	enum easingName
	{
		eInOutSine,
		eOutBounce
	};
	easingName m_easingName;

	float m_alpha;	// 透明値
	float m_cvQty;	// 変化量(conversionQuantity)
	float m_accum;	// 累積(accumulation)
};