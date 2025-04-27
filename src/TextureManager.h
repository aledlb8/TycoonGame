#pragma once
#include <d3d11.h>
#include <string>
#include <map>

class TextureManager {
public:
    static TextureManager& GetInstance() {
        static TextureManager instance(nullptr, nullptr);
        return instance;
    }

    bool Initialize(ID3D11Device* device, ID3D11DeviceContext* context) {
        m_device = device;
        m_context = context;
        return (device != nullptr && context != nullptr);
    }

    void Shutdown() {
        ReleaseTextures();
    }

    ID3D11ShaderResourceView* LoadTexture(const std::string& texturePath);
    ID3D11ShaderResourceView* GetTexture(const std::string& texturePath) const;

private:
    TextureManager(ID3D11Device* device, ID3D11DeviceContext* context);
    ~TextureManager();
    void ReleaseTextures();

    ID3D11Device* m_device;
    ID3D11DeviceContext* m_context;
    std::map<std::string, ID3D11ShaderResourceView*> m_textures;
}; 