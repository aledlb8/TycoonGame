#include "Woodcutter.h"
#include "../GameConstants.h"

Woodcutter::Woodcutter()
    : Building(BuildingType::WOODCUTTER,
               "Woodcutter's Hut",
               200.0f,                                                                              // cost
               0.5f,                                                                                // base production rate
               {},                                                                                  // no input resources
               {Resource(ResourceType::WOOD, "Wood", 0.0f, GameConstants::WOOD_BASE_PRICE, false)}, // output resources
               GameConstants::WOODCUTTER_MAINTENANCE,                                               // maintenance cost
               100.0f,                                                                              // upgrade cost
               0)                                                                                   // required reputation
{
}

void Woodcutter::UpdateEfficiency()
{
    // Woodcutters use the base class efficiency calculation
    // They have no input resources, so they'll always operate at full efficiency
    Building::UpdateEfficiency();
}

float Woodcutter::CalculateProduction(float deltaTime) const
{
    // Woodcutters have a chance to produce bonus wood based on level
    float baseProduction = Building::CalculateProduction(deltaTime);
    float bonusChance = GetLevel() * 0.05f; // 5% chance per level for bonus

    if (static_cast<float>(rand()) / RAND_MAX < bonusChance)
    {
        return baseProduction * 1.5f; // 50% bonus production
    }
    return baseProduction;
}