#include "PowerPlant.h"
#include "../GameConstants.h"

PowerPlant::PowerPlant()
    : Building(BuildingType::POWER_PLANT,
               "Power Plant",
               800.0f, // cost
               1.0f,   // base production rate
               {Resource(ResourceType::WOOD, "Wood", 0.0f, GameConstants::WOOD_BASE_PRICE, false),
                Resource(ResourceType::STONE, "Stone", 0.0f, GameConstants::STONE_BASE_PRICE, false)},    // input resources
               {Resource(ResourceType::ENERGY, "Energy", 0.0f, GameConstants::ENERGY_BASE_PRICE, false)}, // output resources
               GameConstants::POWER_PLANT_MAINTENANCE,                                                    // maintenance cost
               400.0f,                                                                                    // upgrade cost
               15)                                                                                        // required reputation
{
}

void PowerPlant::UpdateEfficiency()
{
    // Power plants require both wood and stone to operate efficiently
    bool hasWood = false;
    bool hasStone = false;
    for (const auto &resource : GetInputResources())
    {
        if (resource.GetType() == ResourceType::WOOD && resource.GetAmount() > 0)
        {
            hasWood = true;
        }
        else if (resource.GetType() == ResourceType::STONE && resource.GetAmount() > 0)
        {
            hasStone = true;
        }
    }

    if (hasWood && hasStone)
    {
        SetEfficiency(1.0f);
    }
    else if (hasWood || hasStone)
    {
        SetEfficiency(0.4f); // Partial efficiency with only one resource
    }
    else
    {
        SetEfficiency(0.1f); // Very low efficiency without resources
    }
}

float PowerPlant::CalculateProduction(float deltaTime) const
{
    // Power plants have a chance to generate extra energy based on level
    float baseProduction = Building::CalculateProduction(deltaTime);
    float extraChance = GetLevel() * 0.04f; // 4% chance per level for extra energy

    if (static_cast<float>(rand()) / RAND_MAX < extraChance)
    {
        return baseProduction * 1.75f; // 75% bonus production
    }
    return baseProduction;
}