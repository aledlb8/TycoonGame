#include "Building.h"
#include <algorithm>

// Constants for building mechanics
const int MAX_LEVEL = 5;
const float UPGRADE_MULTIPLIER = 1.5f;

Building::Building(BuildingType type,
                   const std::string &name,
                   float cost,
                   float baseProductionRate,
                   const std::vector<Resource> &inputResources,
                   const std::vector<Resource> &outputResources,
                   float maintenanceCost,
                   float upgradeCost,
                   int requiredReputation)
    : m_type(type), m_name(name), m_cost(cost), m_baseProductionRate(baseProductionRate), m_inputResources(inputResources), m_outputResources(outputResources), m_isOperational(true), m_isOwned(false), m_efficiency(1.0f), m_level(1), m_maintenanceCost(maintenanceCost), m_upgradeCost(upgradeCost), m_requiredReputation(requiredReputation)
{
}

void Building::Update(float deltaTime)
{
    if (!m_isOperational || !m_isOwned)
        return;

    UpdateEfficiency();
    float production = CalculateProduction(deltaTime);

    // Process input resources
    for (auto &resource : m_inputResources)
    {
        float requiredAmount = production * resource.GetProductionRate();
        if (resource.GetAmount() >= requiredAmount)
        {
            resource.SetAmount(resource.GetAmount() - requiredAmount);
        }
    }

    // Generate output resources
    for (auto &resource : m_outputResources)
    {
        float generatedAmount = production * resource.GetProductionRate();
        resource.SetAmount(resource.GetAmount() + generatedAmount);
    }
}

bool Building::Upgrade()
{
    if (m_level >= MAX_LEVEL)
        return false;

    m_level++;
    m_baseProductionRate *= UPGRADE_MULTIPLIER;
    m_maintenanceCost *= UPGRADE_MULTIPLIER;
    m_upgradeCost *= UPGRADE_MULTIPLIER;
    return true;
}

void Building::UpdateEfficiency()
{
    // Calculate efficiency based on resource availability and amounts
    float totalEfficiency = 0.0f;
    int resourceCount = 0;

    for (const auto &resource : m_inputResources)
    {
        resourceCount++;
        // Calculate how much of the required resource we have
        float requiredAmount = m_baseProductionRate * resource.GetProductionRate();
        if (requiredAmount > 0)
        {
            float resourceEfficiency = std::min(resource.GetAmount() / requiredAmount, 1.0f);
            totalEfficiency += resourceEfficiency;
        }
    }

    // Average the efficiency across all required resources
    m_efficiency = resourceCount > 0 ? totalEfficiency / resourceCount : 1.0f;
}

float Building::CalculateProduction(float deltaTime) const
{
    return m_baseProductionRate * m_efficiency * deltaTime;
}