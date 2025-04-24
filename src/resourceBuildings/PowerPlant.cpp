#include "PowerPlant.h"
#include "../GameConstants.h"
#include "../ResourceManager.h"

PowerPlant::PowerPlant()
    : Building(
          BuildingType::POWER_PLANT,
          "Power Plant",
          800.0f,
          1.0f,
          {Resource(ResourceType::WOOD, "Wood", 0.0f, GameConstants::WOOD_BASE_PRICE, false),
           Resource(ResourceType::STONE, "Stone", 0.0f, GameConstants::STONE_BASE_PRICE, false)},
          {Resource(ResourceType::ENERGY, "Energy", 0.0f, GameConstants::ENERGY_BASE_PRICE, false)},
          GameConstants::POWER_PLANT_MAINTENANCE,
          400.0f,
          15)
{
}

void PowerPlant::UpdateEfficiency(float deltaTime)
{
    Building::UpdateEfficiency(deltaTime);

    auto &rm = ResourceManager::Instance();
    bool w = rm.Get(ResourceType::WOOD) > 0.0f;
    bool s = rm.Get(ResourceType::STONE) > 0.0f;
    float factor = (!w && !s)   ? 0.1f
                   : (!w || !s) ? 0.4f
                                : 1.0f;
    SetEfficiency(GetEfficiency() * factor);
}

float PowerPlant::CalculateProduction(float deltaTime) const
{
    float baseProd = Building::CalculateProduction(deltaTime);
    float extraChance = GetLevel() * 0.04f;
    if (static_cast<float>(rand()) / RAND_MAX < extraChance)
        return baseProd * 1.75f;
    return baseProd;
}