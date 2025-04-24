#include "DiamondMine.h"
#include "../GameConstants.h"
#include "../ResourceManager.h"

DiamondMine::DiamondMine()
    : Building(
          BuildingType::DIAMOND_MINE,
          "Diamond Mine",
          5000.0f,
          0.1f,
          {Resource(ResourceType::ENERGY, "Energy", 0.0f, GameConstants::ENERGY_BASE_PRICE, false),
           Resource(ResourceType::CRYSTAL, "Crystal", 0.0f, GameConstants::CRYSTAL_BASE_PRICE, false),
           Resource(ResourceType::GOLD, "Gold", 0.0f, GameConstants::GOLD_BASE_PRICE, false)},
          {Resource(ResourceType::DIAMOND, "Diamond", 0.0f, GameConstants::DIAMOND_BASE_PRICE, false)},
          GameConstants::CRYSTAL_MINE_MAINTENANCE * 2.0f,
          2000.0f,
          50)
{
}

void DiamondMine::UpdateEfficiency(float deltaTime)
{
    Building::UpdateEfficiency(deltaTime);

    auto &rm = ResourceManager::Instance();
    bool ok = true;
    for (auto const &req : GetInputResources())
        if (rm.Get(req.GetType()) <= 0.0f)
            ok = false;

    float factor = ok ? 1.0f : 0.3f;
    SetEfficiency(GetEfficiency() * factor);
}

float DiamondMine::CalculateProduction(float deltaTime) const
{
    if (!IsOperational() || !IsOwned())
        return 0.0f;
    return GetBaseProductionRate() * GetEfficiency() * deltaTime;
}