#include "CrystalMine.h"
#include "../GameConstants.h"

CrystalMine::CrystalMine()
    : Building(BuildingType::CRYSTAL_MINE,
               "Crystal Mine",
               1000.0f, // cost
               0.2f,    // base production rate
               {Resource(ResourceType::ENERGY, "Energy", 0.0f, GameConstants::ENERGY_BASE_PRICE, false),
                Resource(ResourceType::IRON, "Iron", 0.0f, GameConstants::IRON_BASE_PRICE, false)}, // input resources
               {Resource(ResourceType::CRYSTAL, "Crystal", 0.0f, GameConstants::CRYSTAL_BASE_PRICE, false),
                Resource(ResourceType::GOLD, "Gold", 0.0f, GameConstants::GOLD_BASE_PRICE, false)}, // output resources
               GameConstants::CRYSTAL_MINE_MAINTENANCE,                                             // maintenance cost
               500.0f,                                                                              // upgrade cost
               25)                                                                                  // required reputation
{
}

void CrystalMine::UpdateEfficiency()
{
    // First use the base class efficiency calculation
    Building::UpdateEfficiency();

    // Crystal mines have additional efficiency requirements
    bool hasEnergy = false;
    bool hasIron = false;
    for (const auto &resource : GetInputResources())
    {
        if (resource.GetType() == ResourceType::ENERGY && resource.GetAmount() > 0)
        {
            hasEnergy = true;
        }
        else if (resource.GetType() == ResourceType::IRON && resource.GetAmount() > 0)
        {
            hasIron = true;
        }
    }

    // Apply additional efficiency factors based on resource availability
    float additionalEfficiency = 1.0f;
    if (!hasEnergy && !hasIron)
    {
        additionalEfficiency = 0.1f; // Very low efficiency without any resources
    }
    else if (!hasEnergy || !hasIron)
    {
        additionalEfficiency = 0.3f; // Partial efficiency with only one resource
    }

    // Combine base efficiency with additional factors
    SetEfficiency(GetEfficiency() * additionalEfficiency);
}

float CrystalMine::CalculateProduction(float deltaTime) const
{
    // Crystal mines have a chance to find valuable crystals based on level
    float baseProduction = Building::CalculateProduction(deltaTime);
    float crystalChance = GetLevel() * 0.02f; // 2% chance per level for valuable crystals

    if (static_cast<float>(rand()) / RAND_MAX < crystalChance)
    {
        return baseProduction * 3.0f; // Triple production for valuable finds
    }
    return baseProduction;
}