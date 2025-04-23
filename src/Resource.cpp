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
        // Use gold-specific price changes
        std::uniform_real_distribution<float> dis(GameConstants::GOLD_MIN_PRICE_CHANGE, GameConstants::GOLD_MAX_PRICE_CHANGE);
        float priceChange = dis(gen);
        float newPrice = m_basePrice * (1.0f + priceChange);
        m_basePrice = std::max(GameConstants::GOLD_MIN_PRICE, newPrice);
    } else if (m_type == ResourceType::DIAMOND) {
        // Use diamond-specific price changes (even more stable than gold)
        std::uniform_real_distribution<float> dis(GameConstants::DIAMOND_MIN_PRICE_CHANGE, GameConstants::DIAMOND_MAX_PRICE_CHANGE);
        float priceChange = dis(gen);
        float newPrice = m_basePrice * (1.0f + priceChange);
        m_basePrice = std::max(GameConstants::DIAMOND_MIN_PRICE, newPrice);
    } else {
        // Use default price changes for other resources
        std::uniform_real_distribution<float> dis(-volatility, volatility);
        float priceChange = dis(gen);
        float newPrice = m_basePrice * (1.0f + priceChange);
        m_basePrice = std::max(GameConstants::MIN_PRICE, newPrice);
    }
}