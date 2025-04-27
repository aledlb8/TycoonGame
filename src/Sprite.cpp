#include "Sprite.h"
#include "../lib/imgui.h"
#include "../lib/imgui_impl_dx11.h"
#include <stdexcept>

Sprite::Sprite() : m_texture(nullptr), m_width(0), m_height(0)
{
    m_color[0] = m_color[1] = m_color[2] = m_color[3] = 1.0f;
}

Sprite::~Sprite()
{
    // Textures are managed by TextureManager
}

bool Sprite::Initialize(const std::string &texturePath, float width, float height)
{
    try
    {
        if (width <= 0 || height <= 0)
        {
            return false;
        }

        m_width = width;
        m_height = height;

        m_texture = TextureManager::GetInstance().LoadTexture(texturePath);

        if (!m_texture)
        {
            return false;
        }

        return true;
    }
    catch (const std::exception &e)
    {
        return false;
    }
}

void Sprite::Render(float x, float y, float scale)
{
    if (!m_texture)
    {
        return;
    }

    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 size(m_width * scale, m_height * scale);
    ImGui::Image((ImTextureID)m_texture, size, ImVec2(0, 0), ImVec2(1, 1), ImVec4(m_color[0], m_color[1], m_color[2], m_color[3]), ImVec4(0, 0, 0, 0));
}

void Sprite::SetColor(float r, float g, float b, float a)
{
    m_color[0] = r;
    m_color[1] = g;
    m_color[2] = b;
    m_color[3] = a;
}