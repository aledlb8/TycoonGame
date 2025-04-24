#include "Mine.h"
#include "../GameConstants.h"
#include "../ResourceManager.h"

Mine::Mine()
    : Building(
          BuildingType::MINE,
          "Mine",
          500.0f,
          0.3f,
          {Resource(ResourceType::ENERGY, "Energy", 0.0f, GameConstants::ENERGY_BASE_PRICE, false)},
          {Resource(ResourceType::STONE, "Stone", 0.0f, GameConstants::STONE_BASE_PRICE, false),
           Resource(ResourceType::IRON, "Iron", 0.0f, GameConstants::IRON_BASE_PRICE, false)},
          GameConstants::MINE_MAINTENANCE,
          250.0f,
          10)
{
}

void Mine::UpdateEfficiency(float deltaTime)
{
    Building::UpdateEfficiency(deltaTime);

    auto &rm = ResourceManager::Instance();
    bool hasEnergy = rm.Get(ResourceType::ENERGY) > 0.0f;
    float factor = hasEnergy ? 1.0f : 0.2f;
    SetEfficiency(GetEfficiency() * factor);
}

float Mine::CalculateProduction(float deltaTime) const
{
    float baseProduction = Building::CalculateProduction(deltaTime);
    float rareChance = GetLevel() * 0.03f;
    if (static_cast<float>(rand()) / RAND_MAX < rareChance)
        return baseProduction * 2.0f;
    return baseProduction;
}