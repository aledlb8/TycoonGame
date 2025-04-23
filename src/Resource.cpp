#include "Resource.h"
#include <random>
#include <algorithm>
#include "GameConstants.h"

Resource::Resource(ResourceType type, const std::string &name, float amount, float basePrice, bool isOwned)
    : m_type(type), m_name(name), m_amount(amount), m_basePrice(basePrice), m_isOwned(isOwned)
{
}

void Resource::UpdatePrice(float volatility)
{
    // Generate a random price change within the volatility range
    std::random_device rd;
    std::mt19937 gen(rd());
    
    if (m_type == ResourceType::GOLD) {
        // Use gold-specific price changes with tighter bounds
        std::uniform_real_distribution<float> dis(-0.05f, 0.05f);
        float priceChange = dis(gen);
        float newPrice = m_basePrice * (1.0f + priceChange);
        m_basePrice = std::clamp(newPrice, 100.0f, 500.0f); // Gold price range: $100-$500
    } else if (m_type == ResourceType::DIAMOND) {
        // Use diamond-specific price changes with even tighter bounds
        std::uniform_real_distribution<float> dis(-0.03f, 0.03f);
        float priceChange = dis(gen);
        float newPrice = m_basePrice * (1.0f + priceChange);
        m_basePrice = std::clamp(newPrice, 400.0f, 1000.0f); // Diamond price range: $400-$1000
    } else {
        // Use default price changes for other resources
        std::uniform_real_distribution<float> dis(-volatility, volatility);
        float priceChange = dis(gen);
        float newPrice = m_basePrice * (1.0f + priceChange);
        
        // Set reasonable price ranges for each resource type
        switch (m_type) {
            case ResourceType::WOOD:
                m_basePrice = std::clamp(newPrice, 1.0f, 20.0f);
                break;
            case ResourceType::STONE:
                m_basePrice = std::clamp(newPrice, 2.0f, 30.0f);
                break;
            case ResourceType::IRON:
                m_basePrice = std::clamp(newPrice, 5.0f, 50.0f);
                break;
            case ResourceType::CRYSTAL:
                m_basePrice = std::clamp(newPrice, 50.0f, 200.0f);
                break;
            case ResourceType::ENERGY:
                m_basePrice = std::clamp(newPrice, 10.0f, 40.0f);
                break;
            default:
                m_basePrice = std::max(1.0f, newPrice);
                break;
        }
    }
}