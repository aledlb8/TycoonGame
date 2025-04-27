#pragma once
#include <d3d11.h>
#include <string>
#include "TextureManager.h"
#include "../lib/imgui.h"

class Sprite {
public:
    Sprite();
    ~Sprite();

    // Copy constructor
    Sprite(const Sprite& other) {
        m_texture = other.m_texture;
        m_width = other.m_width;
        m_height = other.m_height;
        m_color[0] = other.m_color[0];
        m_color[1] = other.m_color[1];
        m_color[2] = other.m_color[2];
        m_color[3] = other.m_color[3];
    }
    
    // Assignment operator
    Sprite& operator=(const Sprite& other) {
        if (this != &other) {
            m_texture = other.m_texture;
            m_width = other.m_width;
            m_height = other.m_height;
            m_color[0] = other.m_color[0];
            m_color[1] = other.m_color[1];
            m_color[2] = other.m_color[2];
            m_color[3] = other.m_color[3];
        }
        return *this;
    }

    bool Initialize(const std::string& texturePath, float width, float height);
    void Render(float x, float y, float scale = 1.0f);
    void SetColor(float r, float g, float b, float a = 1.0f);
    ID3D11ShaderResourceView* GetTexture() const { return m_texture; }

private:
    ID3D11ShaderResourceView* m_texture;
    float m_width;
    float m_height;
    float m_color[4];
}; 