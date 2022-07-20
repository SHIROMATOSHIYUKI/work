#pragma once
#include"../TextureDraw.h"
// �C�[�W���O�摜��`�悷��N���X
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

	float m_alpha;	// �����l
	float m_cvQty;	// �ω���(conversionQuantity)
	float m_accum;	// �ݐ�(accumulation)
};