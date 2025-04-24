#pragma once
#include <string>
#include <vector>
#include "Resource.h"

// Production type enum
enum class ProductionType
{
    FURNITURE,
    TOOLS,
    RAILROADS,
    JEWELRY
};

class Production
{
public:
    Production(ProductionType type,
                const std::string &name,
                float cost,
                const std::vector<Resource>& inputResources,
                const std::vector<Resource>& outputResources,
                float time,
                float completionTime,
                float compeltionAmount,
                int requiredReputation = 0);
    virtual ~Production() = default;

    // Getters
    ProductionType GetType() const { return m_type; }
    const std::string &GetName() const { return m_name; }
    float GetCost() const { return m_cost; }
    float GetTime() const { return m_currentTime; }
    float GetCompletionTime() const { return m_completionTime; }
    float GetCompletionAmount() const { return m_completionAmount; }
    bool IsOwned() const { return m_isOwned; }
    int GetRequiredReputation() const { return m_requiredReputation; }
    int IsInvested() const { return m_invested; }
    

    // Setters
    void SetTime(float amount) { m_currentTime = amount; }
    void SetCompletionTime(float price) { m_completionTime = price; }
    void SetOwned(bool owned) { m_isOwned = owned; }
    void SetIsInvested(bool invested) { m_invested = invested; }
    void SetCost(float cost) { m_cost = cost; }
    void SetCompletionAmount(float amount) { m_completionAmount = amount; }
    void SetRequiredReputation(int reputation) { m_requiredReputation = reputation; }


protected:
    ProductionType m_type;
    std::string m_name;
    float m_cost;
    float m_currentTime;
    float m_completionTime;
    float m_completionAmount;
    std::vector<Resource> m_inputResources;
    std::vector<Resource> m_outputResources;
    bool m_isOperational;
    bool m_isOwned = false;
    int m_requiredReputation;
    bool m_invested;
};