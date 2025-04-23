#include "BuildingFactory.h"
#include "resourceBuildings/Woodcutter.h"
#include "resourceBuildings/Mine.h"
#include "resourceBuildings/CrystalMine.h"
#include "resourceBuildings/PowerPlant.h"
#include "resourceBuildings/ResearchLab.h"
#include "resourceBuildings/DiamondMine.h"
#include "productionBuildings/Furniture.h"
#include "productionBuildings/Tools.h"
#include "productionBuildings/Railroads.h"
#include "productionBuildings/Jewelry.h"
#include "GameConstants.h"

//Buildings
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
    case BuildingType::DIAMOND_MINE:
        return std::make_unique<DiamondMine>();
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
        BuildingType::RESEARCH_LAB,
        BuildingType::DIAMOND_MINE};
}
//Productions
std::unique_ptr<Production> BuildingFactory::CreateProduction(ProductionType type)
{
    switch (type)
    {
    case ProductionType::FURNITURE:
        return std::make_unique<Furniture>();
    case ProductionType::TOOLS:
        return std::make_unique<Tools>();
    case ProductionType::RAILROADS:
        return std::make_unique<Railroads>();
    case ProductionType::JEWELRY:
        return std::make_unique<Jewelry>();
    default:
        return nullptr;
    }
}

std::vector<ProductionType> BuildingFactory::GetAvailableProductionTypes()
{
    return {
        ProductionType::FURNITURE,
        ProductionType::TOOLS,
        ProductionType::RAILROADS,
        ProductionType::JEWELRY};
}