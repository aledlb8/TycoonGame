#include "BuildingFactory.h"
#include "buildings/Woodcutter.h"
#include "buildings/Mine.h"
#include "buildings/CrystalMine.h"
#include "buildings/PowerPlant.h"
#include "buildings/ResearchLab.h"
#include "GameConstants.h"

std::unique_ptr<Building> BuildingFactory::CreateBuilding(BuildingType type)
{
    switch (type)
    {
    case BuildingType::WOODCUTTER:
        return std::make_unique<Woodcutter>();
    case BuildingType::MINE:
        return std::make_unique<Mine>();
    case BuildingType::POWER_PLANT:
        return std::make_unique<PowerPlant>();
    case BuildingType::CRYSTAL_MINE:
        return std::make_unique<CrystalMine>();
    case BuildingType::RESEARCH_LAB:
        return std::make_unique<ResearchLab>();
    default:
        return nullptr;
    }
}

std::vector<BuildingType> BuildingFactory::GetAvailableBuildingTypes()
{
    return {
        BuildingType::WOODCUTTER,
        BuildingType::MINE,
        BuildingType::CRYSTAL_MINE,
        BuildingType::POWER_PLANT,
        BuildingType::RESEARCH_LAB};
}