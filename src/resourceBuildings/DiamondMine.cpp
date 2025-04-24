#include "DiamondMine.h"
#include "../GameConstants.h"

DiamondMine::DiamondMine()
    : Building(BuildingType::DIAMOND_MINE,
               "Diamond Mine",
               5000.0f, // Higher cost than crystal mine
               0.1f,    // Lower production rate than crystal mine
               {Resource(ResourceType::ENERGY, "Energy", 0.0f, GameConstants::ENERGY_BASE_PRICE, false),
                Resource(ResourceType::CRYSTAL, "Crystal", 0.0f, GameConstants::CRYSTAL_BASE_PRICE, false),
                Resource(ResourceType::GOLD, "Gold", 0.0f, GameConstants::GOLD_BASE_PRICE, false)}, // input resources
               {Resource(ResourceType::DIAMOND, "Diamond", 0.0f, GameConstants::DIAMOND_BASE_PRICE, false)}, // output resources
               GameConstants::CRYSTAL_MINE_MAINTENANCE * 2.0f, // Higher maintenance than crystal mine
               2000.0f, // Higher upgrade cost
               50) // Higher reputation requirement
{
}

void DiamondMine::UpdateEfficiency()
{
    // First use the base class efficiency calculation
    Building::UpdateEfficiency();

    // Diamond mines have additional efficiency requirements
    bool hasEnergy = false;
    bool hasCrystal = false;
    bool hasGold = false;
    
    for (const auto &resource : GetInputResources())
    {
        if (resource.GetType() == ResourceType::ENERGY && resource.GetAmount() > 0)
        {
            hasEnergy = true;
        }
        else if (resource.GetType() == ResourceType::CRYSTAL && resource.GetAmount() > 0)
        {
            hasCrystal = true;
        }
        else if (resource.GetType() == ResourceType::GOLD && resource.GetAmount() > 0)
        {
            hasGold = true;
        }
    }

    // Apply additional efficiency factors based on resource availability
    float additionalEfficiency = 1.0f;
    if (!hasEnergy || !hasCrystal || !hasGold)
    {
        // Reduce efficiency if any required resource is missing
        additionalEfficiency = 0.3f;
    }

    // Combine base efficiency with additional factors
    SetEfficiency(GetEfficiency() * additionalEfficiency);
}

float DiamondMine::CalculateProduction(float deltaTime) const
{
    if (!IsOperational() || !IsOwned())
        return 0.0f;

    return GetBaseProductionRate() * GetEfficiency() * deltaTime;
} 