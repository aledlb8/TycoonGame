#include "Resource.h"
#include <random>
#include <algorithm>

Resource::Resource(ResourceType type, const std::string &name, float amount, float basePrice, bool isOwned)
    : m_type(type), m_name(name), m_amount(amount), m_basePrice(basePrice), m_isOwned(isOwned)
{
}

void Resource::UpdatePrice(float volatility)
{
    // Generate a random price change within the volatility range
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-volatility, volatility);

    float priceChange = dis(gen);
    float newPrice = m_basePrice * (1.0f + priceChange);

    // Ensure price doesn't go below minimum
    m_basePrice = std::max(0.1f, newPrice);
}