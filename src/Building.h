#pragma once
#include <string>
#include <vector>
#include "Resource.h"

// Building type enum
enum class BuildingType
{
    WOODCUTTER,
    MINE,
    CRYSTAL_MINE,
    POWER_PLANT,
    RESEARCH_LAB,
    DIAMOND_MINE
};

class Building
{
public:
    Building(BuildingType type,
             const std::string &name,
             float cost,
             float baseProductionRate,
             const std::vector<Resource> &inputResources,
             const std::vector<Resource> &outputResources,
             float maintenanceCost,
             float upgradeCost,
             int requiredReputation = 0);

    virtual ~Building() = default;

    // Getters
    BuildingType GetType() const { return m_type; }
    const std::string &GetName() const { return m_name; }
    float GetCost() const { return m_cost; }
    float GetBaseProductionRate() const { return m_baseProductionRate; }
    const std::vector<Resource> &GetInputResources() const { return m_inputResources; }
    const std::vector<Resource> &GetOutputResources() const { return m_outputResources; }
    bool IsOperational() const { return m_isOperational; }
    bool IsOwned() const { return m_isOwned; }
    float GetEfficiency() const { return m_efficiency; }
    int GetLevel() const { return m_level; }
    float GetMaintenanceCost() const { return m_maintenanceCost; }
    float GetUpgradeCost() const { return m_upgradeCost; }
    int GetRequiredReputation() const { return m_requiredReputation; }

    // Setters
    void SetOperational(bool operational) { m_isOperational = operational; }
    void SetOwned(bool owned) { m_isOwned = owned; }
    void SetEfficiency(float efficiency) { m_efficiency = efficiency; }
    void SetLevel(int level) { m_level = level; }
    void SetMaintenanceCost(float cost) { m_maintenanceCost = cost; }
    void SetUpgradeCost(float cost) { m_upgradeCost = cost; }
    void SetRequiredReputation(int reputation) { m_requiredReputation = reputation; }
    void SetBaseProductionRate(float rate) { m_baseProductionRate = rate; }

    // Virtual methods that can be overridden by specific building types
    virtual void Update(float deltaTime);
    virtual bool Upgrade();
    virtual void UpdateEfficiency(float deltaTime);
    virtual float CalculateProduction(float deltaTime) const;

    // how fast we lose efficiency when fuel == 0 (per second)
    static constexpr float EFFICIENCY_DECAY_RATE = 0.03f; // 3% per second

    // globally throttle fuel consumption
    static constexpr float FUEL_CONSUMPTION_FACTOR = 0.1f; // use only .1 the resources

protected:
    BuildingType m_type;
    std::string m_name;
    float m_cost;
    float m_baseProductionRate;
    std::vector<Resource> m_inputResources;
    std::vector<Resource> m_outputResources;
    bool m_isOperational;
    bool m_isOwned;
    float m_efficiency;
    int m_level;
    float m_maintenanceCost;
    float m_upgradeCost;
    int m_requiredReputation;
};