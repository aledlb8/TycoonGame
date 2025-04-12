#pragma once

namespace GameConstants
{
    // Resource update intervals
    constexpr float ECONOMY_UPDATE_INTERVAL = 1.0f;
    constexpr float RESOURCE_UPDATE_INTERVAL = 0.5f;
    constexpr float REPUTATION_UPDATE_INTERVAL = 10.0f;
    constexpr float MAINTENANCE_UPDATE_INTERVAL = 0.1f;
    constexpr float FPS_UPDATE_INTERVAL = 1.0f;
    constexpr float MAX_DELTA_TIME = 0.1f;

    // Starting values
    constexpr float STARTING_MONEY = 500.0f;
    constexpr int STARTING_REPUTATION = 0;

    // Maintenance costs (per second)
    constexpr float WOODCUTTER_MAINTENANCE = 0.01f;
    constexpr float MINE_MAINTENANCE = 0.02f;
    constexpr float CRYSTAL_MINE_MAINTENANCE = 0.05f;
    constexpr float POWER_PLANT_MAINTENANCE = 0.03f;
    constexpr float RESEARCH_LAB_MAINTENANCE = 1.00f;

    // Production multipliers
    constexpr float BASE_PRODUCTION_MULTIPLIER = 1.0f;
    constexpr float REPUTATION_BONUS_MULTIPLIER = 0.01f;
    constexpr float RESEARCH_LAB_BONUS_MULTIPLIER = 0.05f;
    constexpr float UPGRADE_PRODUCTION_MULTIPLIER = 1.2f;
    constexpr float UPGRADE_COST_MULTIPLIER = 1.5f;

    // Price volatility
    constexpr float MIN_RESOURCE_PRICE = 0.1f;
    constexpr float PRICE_VOLATILITY = 0.1f;

    // Resource base prices
    constexpr float WOOD_BASE_PRICE = 5.0f;
    constexpr float STONE_BASE_PRICE = 8.0f;
    constexpr float IRON_BASE_PRICE = 15.0f;
    constexpr float GOLD_BASE_PRICE = 50.0f;
    constexpr float CRYSTAL_BASE_PRICE = 100.0f;
    constexpr float ENERGY_BASE_PRICE = 20.0f;

    // Price volatility
    constexpr float MIN_PRICE_CHANGE = -0.15f;
    constexpr float MAX_PRICE_CHANGE = 0.15f;
    constexpr float MIN_PRICE = 1.0f;
}