#include"TextureDraw.h"

void TextureDraw::Init(std::string name, Math::Vector2 pos, Math::Rectangle rect, Math::Color color)
{
	m_spTex = GameResourceFactory.GetTexture(name);
	m_pos = pos;
	m_rect = rect;
	m_color = color;
}

void TextureDraw::Draw2D()
{
	if (m_spTex == nullptr) { return; }
	SHADER->m_spriteShader.DrawTex(m_spTex.get(),
		(int)m_pos.x, (int)m_pos.y, &m_rect, &m_color);
}
