#pragma once
#include <string>

// Resource type enum
enum class ResourceType
{
    MONEY,
    WOOD,
    STONE,
    IRON,
    GOLD,
    CRYSTAL,
    ENERGY
};

class Resource
{
public:
    Resource() = default;
    Resource(ResourceType type, const std::string &name, float amount, float basePrice, bool isOwned);
    virtual ~Resource() = default;

    // Getters
    ResourceType GetType() const { return m_type; }
    const std::string &GetName() const { return m_name; }
    float GetAmount() const { return m_amount; }
    float GetBasePrice() const { return m_basePrice; }
    bool IsOwned() const { return m_isOwned; }

    // Setters
    void SetAmount(float amount) { m_amount = amount; }
    void SetBasePrice(float price) { m_basePrice = price; }
    void SetOwned(bool owned) { m_isOwned = owned; }

    // Virtual methods that can be overridden by specific resource types
    virtual void UpdatePrice(float volatility);
    virtual float GetProductionRate() const { return 1.0f; }

protected:
    ResourceType m_type;
    std::string m_name;
    float m_amount;
    float m_basePrice;
    bool m_isOwned;
};