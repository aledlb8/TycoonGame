#include "CrystalMine.h"
#include "../GameConstants.h"
#include "../ResourceManager.h"

CrystalMine::CrystalMine()
    : Building(
          BuildingType::CRYSTAL_MINE,
          "Crystal Mine",
          1000.0f,
          0.2f,
          {Resource(ResourceType::ENERGY, "Energy", 0.0f, GameConstants::ENERGY_BASE_PRICE, false),
           Resource(ResourceType::IRON, "Iron", 0.0f, GameConstants::IRON_BASE_PRICE, false)},
          {Resource(ResourceType::CRYSTAL, "Crystal", 0.0f, GameConstants::CRYSTAL_BASE_PRICE, false),
           Resource(ResourceType::GOLD, "Gold", 0.0f, GameConstants::GOLD_BASE_PRICE, false)},
          GameConstants::CRYSTAL_MINE_MAINTENANCE,
          500.0f,
          25)
{
}

void CrystalMine::UpdateEfficiency(float deltaTime)
{
    Building::UpdateEfficiency(deltaTime);

    auto &rm = ResourceManager::Instance();
    bool e = rm.Get(ResourceType::ENERGY) > 0.0f;
    bool i = rm.Get(ResourceType::IRON) > 0.0f;
    float factor = (!e && !i)   ? 0.1f
                   : (!e || !i) ? 0.3f
                                : 1.0f;
    SetEfficiency(GetEfficiency() * factor);
}

float CrystalMine::CalculateProduction(float deltaTime) const
{
    float baseProd = Building::CalculateProduction(deltaTime);
    float crystalChance = GetLevel() * 0.02f;
    if (static_cast<float>(rand()) / RAND_MAX < crystalChance)
        return baseProd * 3.0f;
    return baseProd;
}