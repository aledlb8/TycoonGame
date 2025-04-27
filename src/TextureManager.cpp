#include "TextureManager.h"
#include <DirectXTK/WICTextureLoader.h>
#include <stdexcept>

TextureManager::TextureManager(ID3D11Device* device, ID3D11DeviceContext* context)
    : m_device(device)
    , m_context(context)
{
}

TextureManager::~TextureManager()
{
    ReleaseTextures();
}

ID3D11ShaderResourceView* TextureManager::LoadTexture(const std::string& texturePath)
{
    // Check if texture is already loaded
    auto it = m_textures.find(texturePath);
    if (it != m_textures.end()) {
        return it->second;
    }

    // Load the texture
    ID3D11ShaderResourceView* texture = nullptr;
    std::wstring widePath(texturePath.begin(), texturePath.end());
    HRESULT hr = DirectX::CreateWICTextureFromFile(m_device, m_context, widePath.c_str(), nullptr, &texture);
    
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to load texture: " + texturePath);
    }

    m_textures[texturePath] = texture;
    return texture;
}

ID3D11ShaderResourceView* TextureManager::GetTexture(const std::string& texturePath) const
{
    auto it = m_textures.find(texturePath);
    if (it != m_textures.end()) {
        return it->second;
    }
    return nullptr;
}

void TextureManager::ReleaseTextures()
{
    for (auto& pair : m_textures) {
        if (pair.second) {
            pair.second->Release();
        }
    }
    m_textures.clear();
} 