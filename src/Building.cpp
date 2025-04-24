#include "Building.h"
#include <algorithm>
#include "ResourceManager.h"

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

    // 1) compute & smooth efficiency
    UpdateEfficiency(deltaTime);

    // 2) produce/consume
    float production = CalculateProduction(deltaTime);

    // consume fuel
    for (auto const &req : m_inputResources)
    {
        float needed = production * req.GetProductionRate() * FUEL_CONSUMPTION_FACTOR;
        if (!ResourceManager::Instance().Consume(req.GetType(), needed))
        {
            production = 0.0f;
            break;
        }
    }

    // deposit outputs
    for (auto const &out : m_outputResources)
    {
        float amountOut = production * out.GetProductionRate();
        ResourceManager::Instance().Add(out.GetType(), amountOut);
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

void Building::UpdateEfficiency(float deltaTime)
{
    // If no inputs, full efficiency
    if (m_inputResources.empty())
    {
        m_efficiency = 1.0f;
        return;
    }

    auto &rm = ResourceManager::Instance();
    float totalEff = 0.0f;
    int realInputs = 0;

    // compute “raw” based purely on available fuel
    for (auto const &req : m_inputResources)
    {
        float rate = req.GetProductionRate();
        if (rate <= 0.0f)
            continue;

        float needPerSec = m_baseProductionRate * rate * FUEL_CONSUMPTION_FACTOR;
        float avail = rm.Get(req.GetType());
        float eff = std::min(avail / needPerSec, 1.0f);

        totalEff += eff;
        realInputs += 1;
    }

    float rawEff = realInputs > 0
                       ? (totalEff / realInputs)
                       : 1.0f;

    // **Smooth**: if rawEff >= current, snap up; if rawEff < current, decay slowly
    if (rawEff >= m_efficiency)
    {
        m_efficiency = rawEff;
    }
    else
    {
        float drop = Building::EFFICIENCY_DECAY_RATE * deltaTime;
        m_efficiency = std::max(rawEff, m_efficiency - drop);
    }
}

float Building::CalculateProduction(float deltaTime) const
{
    return m_baseProductionRate * m_efficiency * deltaTime;
}