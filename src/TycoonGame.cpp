#include "TycoonGame.h"
#include "BuildingFactory.h"
#include "../lib/imgui.h"
#include <algorithm>
#include <random>
#include <map>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <fstream>

TycoonGame::TycoonGame()
    : m_gameTime(0.0f), m_isPaused(false), m_economyUpdateTimer(0.0f), m_resourceUpdateTimer(0.0f), m_reputationUpdateTimer(0.0f), m_maintenanceUpdateTimer(0.0f), m_lastFrameTime(0.0f), m_fps(0.0f), m_frameCount(0), m_fpsUpdateTimer(0.0f)
{
    try
    {
        Initialize();
    }
    catch (const std::exception &e)
    {
        fprintf(stderr, "Error initializing game: %s\n", e.what());
    }
}

TycoonGame::~TycoonGame() = default;

// Initializing
void TycoonGame::Initialize()
{
    try
    {
        // Try to load the saved game first
        if (!LoadGame("savegame.dat"))
        {
            // If loading fails, initialize with default values
            // Initialize player
            m_player.name = "Player";
            m_player.money = GameConstants::STARTING_MONEY;
            m_player.reputation = GameConstants::STARTING_REPUTATION;
            m_player.totalEarnings = 0.0f;
            m_player.totalSpent = 0.0f;
            m_player.achievements = 0;

            InitializeResources();
            InitializeBuildingTypes();
            InitializeProductionTypes();

            // Reset timers
            m_gameTime = 0.0f;
            m_economyUpdateTimer = 0.0f;
            m_resourceUpdateTimer = 0.0f;
            m_reputationUpdateTimer = 0.0f;
            m_maintenanceUpdateTimer = 0.0f;
            m_lastFrameTime = 0.0f;
            m_fps = 0.0f;
            m_frameCount = 0;
            m_fpsUpdateTimer = 0.0f;
        }
    }
    catch (const std::exception &e)
    {
        std::stringstream ss;
        ss << "Failed to initialize game: " << e.what();
        throw std::runtime_error(ss.str());
    }
}

void TycoonGame::InitializeResources()
{
    try
    {
        // Initialize starting resources
        m_player.resources.clear(); // Clear existing resources first
        m_player.resources[ResourceType::MONEY] = Resource(ResourceType::MONEY, "Money", GameConstants::STARTING_MONEY, 1.0f, true);
        m_player.resources[ResourceType::WOOD] = Resource(ResourceType::WOOD, "Wood", 0.0f, GameConstants::WOOD_BASE_PRICE, false);
        m_player.resources[ResourceType::STONE] = Resource(ResourceType::STONE, "Stone", 0.0f, GameConstants::STONE_BASE_PRICE, false);
        m_player.resources[ResourceType::IRON] = Resource(ResourceType::IRON, "Iron", 0.0f, GameConstants::IRON_BASE_PRICE, false);
        m_player.resources[ResourceType::GOLD] = Resource(ResourceType::GOLD, "Gold", 0.0f, GameConstants::GOLD_BASE_PRICE, false);
        m_player.resources[ResourceType::CRYSTAL] = Resource(ResourceType::CRYSTAL, "Crystal", 0.0f, GameConstants::CRYSTAL_BASE_PRICE, false);
        m_player.resources[ResourceType::ENERGY] = Resource(ResourceType::ENERGY, "Energy", 0.0f, GameConstants::ENERGY_BASE_PRICE, false);
        m_player.resources[ResourceType::DIAMOND] = Resource(ResourceType::DIAMOND, "Diamond", 0.0f, GameConstants::DIAMOND_BASE_PRICE, false);
    }
    catch (const std::exception &e)
    {
        std::stringstream ss;
        ss << "Failed to initialize resources: " << e.what();
        throw std::runtime_error(ss.str());
    }
}

void TycoonGame::InitializeBuildingTypes()
{
    try
    {
        // Clear existing buildings
        m_player.buildings.clear();

        // Get available building types from factory
        auto buildingTypes = BuildingFactory::GetAvailableBuildingTypes();

        // Create one of each building type
        for (auto type : buildingTypes)
        {
            if (auto building = BuildingFactory::CreateBuilding(type))
            {
                m_player.buildings.push_back(std::move(building));
            }
        }
    }
    catch (const std::exception &e)
    {
        std::stringstream ss;
        ss << "Failed to initialize building types: " << e.what();
        throw std::runtime_error(ss.str());
    }
}

void TycoonGame::InitializeProductionTypes()
{
    try
    {
        m_player.productions.clear();
        auto productionTypes = BuildingFactory::GetAvailableProductionTypes();
        for (auto type : productionTypes)
        {
            if (auto production = BuildingFactory::CreateProduction(type))
            {
                m_player.productions.push_back(std::move(production));
            }
        }
    }
    catch (const std::exception &e)
    {
        std::stringstream ss;
        ss << "Failed to initialize production types: " << e.what();
        throw std::runtime_error(ss.str());
    }
}

void TycoonGame::Update(float deltaTime)
{
    try
    {
        // Cap delta time to prevent large jumps
        deltaTime = std::min(deltaTime, GameConstants::MAX_DELTA_TIME);

        // Update FPS counter
        m_frameCount++;
        m_fpsUpdateTimer += deltaTime;
        if (m_fpsUpdateTimer >= GameConstants::FPS_UPDATE_INTERVAL)
        {
            m_fps = static_cast<float>(m_frameCount) / m_fpsUpdateTimer;
            m_frameCount = 0;
            m_fpsUpdateTimer = 0.0f;
        }

        if (!m_isPaused)
        {
            m_gameTime += deltaTime;

            // Update resources
            m_resourceUpdateTimer += deltaTime;
            if (m_resourceUpdateTimer >= GameConstants::RESOURCE_UPDATE_INTERVAL)
            {
                UpdateResources(GameConstants::RESOURCE_UPDATE_INTERVAL);
                m_resourceUpdateTimer = 0.0f;
            }

            // Update economy
            m_economyUpdateTimer += deltaTime;
            if (m_economyUpdateTimer >= GameConstants::ECONOMY_UPDATE_INTERVAL)
            {
                UpdateEconomy(GameConstants::ECONOMY_UPDATE_INTERVAL);
                m_economyUpdateTimer = 0.0f;
            }

            // Update reputation
            m_reputationUpdateTimer += deltaTime;
            if (m_reputationUpdateTimer >= GameConstants::REPUTATION_UPDATE_INTERVAL)
            {
                UpdateReputation();
                m_reputationUpdateTimer = 0.0f;
            }

            // Update maintenance costs
            m_maintenanceUpdateTimer += deltaTime;
            if (m_maintenanceUpdateTimer >= GameConstants::MAINTENANCE_UPDATE_INTERVAL)
            {
                float totalMaintenance = 0.0f;

                // Calculate maintenance costs for owned buildings
                for (const auto &building : m_player.buildings)
                {
                    if (building && building->IsOwned())
                    {
                        totalMaintenance += building->GetMaintenanceCost();
                    }
                }

                // Deduct maintenance cost if player has enough money
                if (m_player.money >= totalMaintenance)
                {
                    m_player.money -= totalMaintenance;
                    m_player.totalSpent += totalMaintenance;
                }
                else
                {
                    float availableMoney = m_player.money;
                    m_player.money = 0.0f;
                    m_player.totalSpent += availableMoney;
                }

                m_maintenanceUpdateTimer = 0.0f;
            }

            // Update all buildings
            for (auto &building : m_player.buildings)
            {
                if (building && building->IsOwned())
                {
                    building->Update(deltaTime);
                }
            }

            // Update all productions
            for (auto &production : m_player.productions)
            {
                if (production && production->IsInvested())
                {
                    production->SetTime(production->GetTime() + deltaTime);
                    if (production->GetTime() >= production->GetCompletionTime())
                    {
                        production->SetIsInvested(false);
                        production->SetTime(0.0f);
                        m_player.money += production->GetCompletionAmount();
                    }
                }
            }
        }
    }
    catch (...)
    {
        // Log error but don't crash
        fprintf(stderr, "Error in Update\n");
    }
}

// Functions
float TycoonGame::CalculateProductionMultiplier() const
{
    try
    {
        float multiplier = GameConstants::BASE_PRODUCTION_MULTIPLIER;

        // Add reputation bonus
        multiplier += m_player.reputation * GameConstants::REPUTATION_BONUS_MULTIPLIER;

        // Add Research Lab bonus
        for (const auto &building : m_player.buildings)
        {
            if (building && building->IsOwned() && building->GetType() == BuildingType::RESEARCH_LAB)
            {
                multiplier += GameConstants::RESEARCH_LAB_BONUS_MULTIPLIER * building->GetLevel();
            }
        }

        return std::max(multiplier, GameConstants::BASE_PRODUCTION_MULTIPLIER);
    }
    catch (...)
    {
        return GameConstants::BASE_PRODUCTION_MULTIPLIER;
    }
}

void TycoonGame::UpdateResources(float deltaTime)
{
    float productionMultiplier = CalculateProductionMultiplier();

    // Update resources for each operational building
    for (const auto &building : m_player.buildings)
    {
        if (!building || !building->IsOperational() || !building->IsOwned())
            continue;

        // Check if we have enough input resources
        bool canProduce = true;
        for (const auto &input : building->GetInputResources())
        {
            auto it = m_player.resources.find(input.GetType());
            if (it == m_player.resources.end() || it->second.GetAmount() < input.GetAmount())
            {
                canProduce = false;
                break;
            }
        }

        if (canProduce)
        {
            // Consume input resources
            for (const auto &input : building->GetInputResources())
            {
                m_player.resources[input.GetType()].SetAmount(
                    m_player.resources[input.GetType()].GetAmount() - input.GetAmount());
            }

            // Produce output resources
            float production = building->CalculateProduction(deltaTime) * productionMultiplier;
            for (const auto &output : building->GetOutputResources())
            {
                if (m_player.resources[output.GetType()].GetAmount() < 100)
                {
                    m_player.resources[output.GetType()].SetAmount(
                        m_player.resources[output.GetType()].GetAmount() + production);
                    m_player.resources[output.GetType()].SetOwned(true);
                }
                else if (m_player.resources[output.GetType()].GetAmount() > 100)
                {
                    m_player.resources[output.GetType()].SetAmount(100);
                    m_player.resources[output.GetType()].SetOwned(true);
                }
            }
        }
    }
}

void TycoonGame::UpdateEconomy(float deltaTime)
{
    // Update prices for all resources
    for (auto &[type, resource] : m_player.resources)
    {
        if (type != ResourceType::MONEY)
        {
            resource.UpdatePrice(GameConstants::PRICE_VOLATILITY);
        }
    }
}

void TycoonGame::UpdateReputation()
{
    // Gain reputation based on owned buildings and their efficiency
    int newReputation = 0;
    for (const auto &building : m_player.buildings)
    {
        if (building && building->IsOwned())
        {
            // Base reputation gain from each building
            newReputation += 1;

            // Additional reputation for efficient buildings
            if (building->GetEfficiency() > 0.8f)
            {
                newReputation += 1;
            }
        }
    }

    // Add reputation with diminishing returns
    if (newReputation > 0)
    {
        m_player.reputation += static_cast<int>(newReputation * (100.0f / (100.0f + m_player.reputation)));
    }
}

bool TycoonGame::BuildStructure(BuildingType type)
{
    try
    {
        // Find building template
        for (const auto &building : m_player.buildings)
        {
            if (building && building->GetType() == type)
            {
                // Check if player has enough money and reputation
                if (m_player.money < building->GetCost() || m_player.reputation < building->GetRequiredReputation())
                    return false;

                // Check if player already owns this type of building
                if (building->IsOwned())
                    return false;

                // Deduct cost and mark as owned
                m_player.money -= building->GetCost();
                m_player.totalSpent += building->GetCost();
                building->SetOwned(true);
                return true;
            }
        }
        return false;
    }
    catch (...)
    {
        return false;
    }
}

bool TycoonGame::BeginProduction(ProductionType type)
{
    try
    {
        // Find production template
        for (const auto &production : m_player.productions)
        {
            if (production && production->GetType() == type)
            {
                // Check if player has enough money and reputation
                if (m_player.money < production->GetCost() || m_player.reputation < production->GetRequiredReputation())
                    return false;

                // Check if player already invested in this type of production
                if (production->IsInvested())
                    return false;

                // Deduct cost and mark as invested
                m_player.money -= production->GetCost();
                m_player.totalSpent += production->GetCost();
                production->SetIsInvested(true);
                return true;
            }
        }
        return false;
    }
    catch (...)
    {
        return false;
    }
}

bool TycoonGame::SellStructure(int buildingIndex)
{
    try
    {
        if (buildingIndex < 0 || buildingIndex >= static_cast<int>(m_player.buildings.size()))
            return false;

        auto &building = m_player.buildings[buildingIndex];
        if (!building || !building->IsOwned())
            return false;

        // Return 50% of the building's cost
        float refund = building->GetCost() * 0.5f;
        m_player.money += refund;
        m_player.totalEarnings += refund;

        // Create a new building of the same type to replace the sold one
        auto newBuilding = BuildingFactory::CreateBuilding(building->GetType());
        if (newBuilding)
        {
            m_player.buildings[buildingIndex] = std::move(newBuilding);
            return true;
        }
        return false;
    }
    catch (...)
    {
        return false;
    }
}

bool TycoonGame::UpgradeBuilding(int buildingIndex)
{
    try
    {
        if (buildingIndex < 0 || buildingIndex >= static_cast<int>(m_player.buildings.size()))
            return false;

        auto &building = m_player.buildings[buildingIndex];
        if (!building || !building->IsOwned())
            return false;

        if (m_player.money < building->GetUpgradeCost())
            return false;

        m_player.money -= building->GetUpgradeCost();
        m_player.totalSpent += building->GetUpgradeCost();
        return building->Upgrade();
    }
    catch (...)
    {
        return false;
    }
}

bool TycoonGame::BuyResource(ResourceType type, float amount)
{
    try
    {
        auto it = m_player.resources.find(type);
        if (it == m_player.resources.end())
            return false;

        float totalCost = amount * it->second.GetBasePrice();
        if (m_player.money < totalCost)
            return false;

        m_player.money -= totalCost;
        m_player.totalSpent += totalCost;
        it->second.SetAmount(it->second.GetAmount() + amount);
        it->second.SetOwned(true);
        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool TycoonGame::SellResource(ResourceType type, float amount)
{
    try
    {
        auto it = m_player.resources.find(type);
        if (it == m_player.resources.end() || !it->second.IsOwned())
            return false;

        if (it->second.GetAmount() < amount)
            return false;

        float totalEarnings = amount * it->second.GetBasePrice();
        m_player.money += totalEarnings;
        m_player.totalEarnings += totalEarnings;
        it->second.SetAmount(it->second.GetAmount() - amount);
        return true;
    }
    catch (...)
    {
        return false;
    }
}

float TycoonGame::CalculateResourcePrice(ResourceType type) const
{
    auto it = m_player.resources.find(type);
    if (it == m_player.resources.end())
        return 0.0f;
    return it->second.GetBasePrice();
}

// Rendering
void TycoonGame::Render()
{
    try
    {
        RenderMainMenu();
        RenderResourcesWindow();
        RenderProductionWindow();
        RenderPurchaseBuildingsWindow();
        RenderBuildingsWindow();
        RenderMarketWindow();
        RenderStockWindow();
    }
    catch (...)
    {
        // Log error but don't crash the game
        // In a real game, you might want to show an error message to the user
    }
}

void TycoonGame::RenderMainMenu()
{
    bool confirm_popup = false;
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Menu"))
        {
            if (ImGui::MenuItem("New Game"))
            {
                confirm_popup = true;
            }
            if (ImGui::MenuItem("Save Game"))
            {
                if (SaveGame("savegame.dat"))
                {
                }
            }
            if (ImGui::MenuItem("Load Save"))
            {
                if (LoadGame("savegame.dat"))
                {
                }
            }
            if (ImGui::MenuItem(m_isPaused ? "Resume" : "Pause"))
            {
                m_isPaused = !m_isPaused;
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit"))
            {
                SaveGame("savegame.dat");
                exit(0);
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("About"))
            {
                // Show about dialog
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Statistics"))
        {
            // Stats header
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.0f, 0.8f, 1.0f));
            ImGui::Text("Statistics");
            ImGui::PopStyleColor();
            ImGui::Separator();

            // Game time
            ImGui::Text("Time: %.1f seconds", m_gameTime);

            // Reputation with progress bar
            ImGui::Separator();
            ImGui::Text("Reputation: %d", m_player.reputation);
            float repProgress = std::min(m_player.reputation / 200.0f, 1.0f);
            ImGui::ProgressBar(repProgress, ImVec2(-1.0f, 0.0f));
            ImGui::Separator();

            // Financial stats
            ImGui::Text("Total Earnings: $%.1f", m_player.totalEarnings);
            ImGui::Text("Total Spent: $%.1f", m_player.totalSpent);
            ImGui::Text("Net Profit: $%.1f", m_player.totalEarnings - m_player.totalSpent);
            ImGui::Separator();

            // Buildings owned
            int ownedBuildings = static_cast<int>(std::count_if(m_player.buildings.begin(), m_player.buildings.end(),
                                                                [](const std::unique_ptr<Building> &b)
                                                                { return b->IsOwned(); }));
            ImGui::Text("Buildings Owned: %d", ownedBuildings);

            // Resources owned
            int ownedResources = static_cast<int>(std::count_if(m_player.resources.begin(), m_player.resources.end(),
                                                                [](const auto &pair)
                                                                { return pair.second.IsOwned(); }));
            ImGui::Text("Resources Owned: %d", ownedResources);

            // Production multiplier
            ImGui::Text("Production Mult: %.1fx", CalculateProductionMultiplier());

            ImGui::EndMenu();
        }

        // Display FPS in the menu bar
        ImGui::SameLine(ImGui::GetWindowWidth() - 100);
        ImGui::Text("FPS: %.1f", m_fps);

        ImGui::EndMainMenuBar();
    }
    // Confirm New Game Popup
    if (confirm_popup)
    {
        ImGui::OpenPopup("Confirm New Game");
    }
    if (ImGui::BeginPopupModal("Confirm New Game", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Are you sure you want to create a new game?");
        ImGui::Text("WARNING: Deletes current save!");

        if (ImGui::Button("Confirm"))
        {
            std::remove("savegame.dat");
            Initialize();
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void TycoonGame::RenderResourcesWindow()
{
    constexpr float storageX = 8.0f;
    constexpr float storageY = 30.0f;
    constexpr float storageHeight = 538.0f;
    constexpr float marketX = 160.0f;
    constexpr float gap = 8.0f;
    float storageWidth = marketX - storageX - gap;

    ImGui::SetNextWindowPos(ImVec2(storageX, storageY), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(storageWidth, storageHeight), ImGuiCond_Always);
    ImGui::Begin(
        "Resources",
        nullptr,
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.84f, 0.0f, 1.0f));
    ImGui::Text("Storage");
    ImGui::PopStyleColor();
    ImGui::Separator();

    ImGui::BeginChild("ResourcesList", ImVec2(-1.0f, -1.0f), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
    for (const auto &[type, resource] : m_player.resources)
    {
        if (type == ResourceType::MONEY)
            continue;

        ImVec4 color;
        const char *symbol;
        switch (type)
        {
        case ResourceType::WOOD:
            symbol = "[W]";
            color = ImVec4(0.55f, 0.27f, 0.07f, 1.0f);
            break;
        case ResourceType::STONE:
            symbol = "[S]";
            color = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
            break;
        case ResourceType::IRON:
            symbol = "[I]";
            color = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
            break;
        case ResourceType::GOLD:
            symbol = "[G]";
            color = ImVec4(1.0f, 0.84f, 0.0f, 1.0f);
            break;
        case ResourceType::CRYSTAL:
            symbol = "[C]";
            color = ImVec4(0.5f, 0.0f, 0.5f, 1.0f);
            break;
        case ResourceType::ENERGY:
            symbol = "[E]";
            color = ImVec4(0.0f, 0.8f, 1.0f, 1.0f);
            break;
        case ResourceType::DIAMOND:
            symbol = "[D]";
            color = ImVec4(0.0f, 0.8f, 0.8f, 1.0f);
            break;
        default:
            symbol = "[?]";
            color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
            break;
        }

        ImGui::PushStyleColor(ImGuiCol_Text, color);
        ImGui::Text("%s %s:", symbol, resource.GetName().c_str());
        ImGui::PopStyleColor();

        float maxAmount = 100.0f;
        float progress = std::min(resource.GetAmount() / maxAmount, 1.0f);
        char decimal[32];
        snprintf(decimal, sizeof(decimal), "%.1f%%", progress * 100.0f);
        ImGui::ProgressBar(progress, ImVec2(-1.0f, 0.0f), decimal);

        ImGui::Text("$%.2f per", resource.GetBasePrice());
        ImGui::Separator();
    }
    ImGui::EndChild();

    ImGui::End();
}

bool showHelpWindow = false;
void TycoonGame::RenderProductionWindow()
{
    ImGui::SetNextWindowPos(ImVec2(10, 574), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(746, 182), ImGuiCond_FirstUseEver);
    ImGui::Begin("Production", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    ImGui::Separator();

    // Production display with icons and progress bars
    for (const auto &production : m_player.productions)
    {
        if (production->IsOwned())
            continue;
        ProductionType type = production->GetType();
        ImVec4 color = ImVec4(0.0f, 1.0f, 0.0f, 0.75f);
        const char *symbol = "";

        if (m_player.reputation >= production->GetRequiredReputation())
        {
            switch (type)
            {
            case ProductionType::FURNITURE:
                symbol = "[WJ]";
                break;
            case ProductionType::RAILROADS:
                symbol = "[WJ]";
                break;
            case ProductionType::TOOLS:
                symbol = "[WJ]";
                break;
            case ProductionType::JEWELRY:
                symbol = "[WJ]";
                break;
            default:
                symbol = "[?J]";
                color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // Default white
                break;
            }
            ImGui::PushStyleColor(ImGuiCol_Text, color);
            ImGui::Text("%s %s:", symbol, production->GetName().c_str());
            ImGui::PopStyleColor();
            ImGui::SameLine(300.0f);
            float completionTime = production->GetCompletionTime();
            float progress = (completionTime > 0) ? std::min(production->GetTime() / completionTime, 1.0f) : 0.0f;
            ImGui::ProgressBar(progress, ImVec2(120.0f, 0.0f)); // Adjust width as needed
            ImGui::SameLine();
            ImGui::Text("$%.2f per unit", production->GetCompletionAmount());
            ImGui::SameLine();
            if (!production->IsInvested())
            {
                if (m_player.money >= production->GetCost())
                {
                    if (ImGui::Button(("Invest##" + production->GetName()).c_str()))
                    {
                        BeginProduction(production->GetType());
                    }
                    if (ImGui::IsItemHovered())
                    {
                        ImGui::BeginTooltip();
                        ImGui::Text("Click to invest!");
                        std::ostringstream oss;
                        oss << "Costs " << std::fixed << std::setprecision(2) << production->GetCost() << "$ to invest here";
                        std::string temp = oss.str();
                        ImGui::SeparatorText(temp.c_str());
                        ImGui::Text("Returns %.2f$ from investment", production->GetCompletionAmount());
                        ImGui::EndTooltip();
                    }
                }
                else
                {
                    ImGui::BeginDisabled();
                    if (ImGui::Button(("Invest##" + production->GetName()).c_str()))
                    {
                        // Invest action
                    }
                    if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                    {
                        ImGui::BeginTooltip();
                        ImGui::Text("Keep saving!");
                        std::ostringstream oss;
                        oss << "Costs " << std::fixed << std::setprecision(2) << production->GetCost() << "$ to invest here";
                        std::string temp = oss.str();
                        ImGui::SeparatorText(temp.c_str());
                        ImGui::Text("Returns %.2f$ from investment", production->GetCompletionAmount());
                        ImGui::EndTooltip();
                    }
                    ImGui::EndDisabled();
                }
            }
            else
            {
                ImGui::BeginDisabled();
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 1.0f)); // Green
                if (ImGui::Button(("Invested##" + production->GetName()).c_str()))
                {
                    BeginProduction(production->GetType());
                }
                if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                {
                    ImGui::BeginTooltip();
                    ImGui::Text("Currently Invested!");
                    ImGui::Separator();
                    ImGui::Text("Returns %.2f$ when complete", production->GetCompletionAmount());
                    ImGui::Separator();
                    ImGui::EndTooltip();
                }
                ImGui::PopStyleColor();
                ImGui::EndDisabled();
            }
            ImGui::Separator();
        }
    }
    ImGui::NewLine();
    if (ImGui::Button("?"))
    {
        showHelpWindow = true;
    }

    if (showHelpWindow)
    {
        ImGui::SetNextWindowPos(ImVec2(250, 400));
        ImGui::SetNextWindowSize(ImVec2(300, 190));
        if (ImGui::Begin("Help Window", nullptr,
                         ImGuiWindowFlags_NoTitleBar |
                             ImGuiWindowFlags_AlwaysAutoResize |
                             ImGuiWindowFlags_NoResize |
                             ImGuiWindowFlags_NoMove |
                             ImGuiWindowFlags_NoScrollbar |
                             ImGuiWindowFlags_NoSavedSettings |
                             ImGuiWindowFlags_NoCollapse))
        {
            ImGui::SeparatorText("Investing Tips:");
            ImGui::NewLine();
            ImGui::Separator();
            ImGui::BulletText("Gain more reputation to attract \nmore investors");
            ImGui::BulletText("Invest when stocks are low!");
            ImGui::Separator();
            ImGui::NewLine();
            if (ImGui::Button("Close"))
            {
                showHelpWindow = false;
            }
        }
        ImGui::End();
    }

    ImGui::End();
}

void TycoonGame::RenderPurchaseBuildingsWindow()
{
    ImGui::SetNextWindowPos(ImVec2(765, 30), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(254, 188), ImGuiCond_FirstUseEver);
    ImGui::Begin("Purchase Buildings", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    // Available buildings header with reputation
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.8f, 0.0f, 1.0f));
    ImGui::Text("Available Buildings");
    ImGui::PopStyleColor();
    ImGui::Text("* Reputation: %d", m_player.reputation);
    ImGui::Separator();

    // Building buttons with icons
    int availableBuildingIndex = 0; // Add counter for unique IDs
    for (const auto &building : m_player.buildings)
    {
        if (building->IsOwned())
            continue;

        std::string buttonText = "";
        const char *symbol = "";

        switch (building->GetType())
        {
        case BuildingType::WOODCUTTER:
            symbol = "[WC]";
            break;
        case BuildingType::MINE:
            symbol = "[MN]";
            break;
        case BuildingType::CRYSTAL_MINE:
            symbol = "[CM]";
            break;
        case BuildingType::POWER_PLANT:
            symbol = "[PP]";
            break;
        case BuildingType::RESEARCH_LAB:
            symbol = "[RL]";
            break;
        case BuildingType::DIAMOND_MINE:
            symbol = "[DM]";
            break;

    }

        // Create unique button text with ID
        std::string uniqueButtonText = std::string(symbol) + " " + building->GetName() + " ($" +
                                       std::to_string(static_cast<int>(building->GetCost())) + ")##available_" + std::to_string(availableBuildingIndex);

        if (m_player.reputation >= building->GetRequiredReputation())
        {

            if (m_player.money >= building->GetCost())
            {
                if (ImGui::Button(uniqueButtonText.c_str()))
                {
                    BuildStructure(building->GetType());
                }
                if (ImGui::IsItemHovered())
                {
                    ImGui::BeginTooltip();
                    ImGui::Text("Click to purchase!");
                    ImGui::EndTooltip();
                }
            }
            else
            {
                ImGui::BeginDisabled();

                if (ImGui::Button(uniqueButtonText.c_str()))
                {
                    BuildStructure(building->GetType());
                }
                if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                {
                    ImGui::BeginTooltip();
                    ImGui::Text("Keep Saving!");
                    ImGui::EndTooltip();
                }
                ImGui::EndDisabled();
            }
        }
        else
        {
            ImGui::BeginDisabled();
            ImGui::Button(uniqueButtonText.c_str());
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
            {
                ImGui::BeginTooltip();
                ImGui::Text("Requires %s reputation.", std::to_string(building->GetRequiredReputation()).c_str());
                ImGui::EndTooltip();
            }
            ImGui::EndDisabled();
        }

        availableBuildingIndex++; // Increment counter for next button
    }

    ImGui::Separator();

    ImGui::End();
}

void TycoonGame::RenderBuildingsWindow()
{
    ImGui::SetNextWindowPos(ImVec2(766, 226), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(253, 531), ImGuiCond_FirstUseEver);
    ImGui::Begin("Owned Buildings", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    // Owned buildings section
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.6f, 1.0f, 1.0f));
    ImGui::Text("Owned Buildings");
    ImGui::PopStyleColor();

    // Create a map to track unique building types that are owned
    std::map<BuildingType, int> ownedBuildingIndices;

    // First, find the indices of owned buildings in the original vector
    for (size_t j = 0; j < m_player.buildings.size(); j++)
    {
        if (m_player.buildings[j]->IsOwned())
        {
            // Only keep the first occurrence of each building type
            if (ownedBuildingIndices.find(m_player.buildings[j]->GetType()) == ownedBuildingIndices.end())
            {
                ownedBuildingIndices[m_player.buildings[j]->GetType()] = static_cast<int>(j);
            }
        }
    }

    // Now iterate through the map of unique owned buildings
    int buildingIndex = 0;
    for (const auto &pair : ownedBuildingIndices)
    {
        const auto &building = m_player.buildings[pair.second];
        int originalIndex = pair.second;

        // Use a unique ID for each tree node to prevent duplicates
        std::string treeNodeId = building->GetName() + "##" + std::to_string(buildingIndex++);

        if (ImGui::TreeNode(treeNodeId.c_str()))
        {
            // Level and efficiency
            ImGui::Text("Level: %d", building->GetLevel());
            ImGui::Text("Efficiency: %.1f%%", building->GetEfficiency() * 100.0f);
            ImGui::ProgressBar(building->GetEfficiency(), ImVec2(-1.0f, 0.0f));

            // Production rate
            ImGui::Text("Production Rate: %.1f/s", building->GetBaseProductionRate() * CalculateProductionMultiplier());

            // Maintenance cost
            ImGui::Text("Maintenance: $%.2f/s", building->GetMaintenanceCost());

            // Upgrade button - use a unique ID for each button (show if has enough to upgrade & not max level; hardcoded to 5)
            if (static_cast<int>(building->GetUpgradeCost()) < m_player.money && static_cast<int>(building->GetLevel()) < 5)
            {
                std::string upgradeButtonId = "Upgrade ($" + std::to_string(static_cast<int>(building->GetUpgradeCost())) + ")##upgrade" + std::to_string(originalIndex);
                if (ImGui::Button(upgradeButtonId.c_str()))
                {
                    UpgradeBuilding(originalIndex);
                }
                if (ImGui::IsItemHovered())
                {
                    ImGui::BeginTooltip();
                    ImGui::Text("Click to upgrade!");
                    ImGui::EndTooltip();
                }
            }

            // Sell button - use a unique ID for each button
            std::string sellButtonId = "Sell##building" + std::to_string(originalIndex);
            if (ImGui::Button(sellButtonId.c_str()))
            {
                SellStructure(originalIndex);
            }

            ImGui::TreePop();
        }
    }

    ImGui::End();
}

void TycoonGame::RenderMarketWindow()
{
    ImGui::SetNextWindowPos(ImVec2(160, 30), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(280, 538), ImGuiCond_FirstUseEver);
    ImGui::Begin("Market", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    // Market header
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.5f, 0.0f, 1.0f)); // Orange
    ImGui::Text("Market Prices");
    ImGui::PopStyleColor();
    ImGui::Separator();

    // Money display with icon
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.84f, 0.0f, 1.0f)); // Gold color
    ImGui::Text("$ Money: %.2f", m_player.money);
    ImGui::PopStyleColor();
    ImGui::Separator();

    // Get all resource types that are produced by owned buildings
    std::vector<ResourceType> producibleResources;
    for (const auto &building : m_player.buildings)
    {
        if (building->IsOwned())
        {
            for (const auto &output : building->GetOutputResources())
            {
                if (std::find(producibleResources.begin(), producibleResources.end(), output.GetType()) == producibleResources.end())
                {
                    producibleResources.push_back(output.GetType());
                }
            }
        }
    }

    // Show only resources that can be produced
    for (const auto &[type, resource] : m_player.resources)
    {
        if (type != ResourceType::MONEY &&
            std::find(producibleResources.begin(), producibleResources.end(), type) != producibleResources.end())
        {

            // Resource name and symbol
            const char *symbol = "";
            ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

            switch (type)
            {
            case ResourceType::WOOD:
                symbol = "[W]";
                color = ImVec4(0.55f, 0.27f, 0.07f, 1.0f); // Brown
                break;
            case ResourceType::STONE:
                symbol = "[S]";
                color = ImVec4(0.5f, 0.5f, 0.5f, 1.0f); // Gray
                break;
            case ResourceType::IRON:
                symbol = "[I]";
                color = ImVec4(0.7f, 0.7f, 0.7f, 1.0f); // Light gray
                break;
            case ResourceType::GOLD:
                symbol = "[G]";
                color = ImVec4(1.0f, 0.84f, 0.0f, 1.0f); // Gold
                break;
            case ResourceType::CRYSTAL:
                symbol = "[C]";
                color = ImVec4(0.5f, 0.0f, 0.5f, 1.0f); // Purple
                break;
            case ResourceType::ENERGY:
                symbol = "[E]";
                color = ImVec4(0.0f, 0.8f, 1.0f, 1.0f); // Blue
                break;
            case ResourceType::DIAMOND:
                symbol = "[D]";
                color = ImVec4(0.0f, 0.8f, 0.8f, 1.0f); // Cyan
                break;
            }

            ImGui::PushStyleColor(ImGuiCol_Text, color);
            ImGui::Text("%s %s", symbol, resource.GetName().c_str());
            ImGui::PopStyleColor();

            ImGui::Text("$ Current Price: %.2f", resource.GetBasePrice());
            ImGui::Text("Storage Used: %.1f%%", resource.GetAmount());

            if (resource.GetAmount() > 0)
            {
                ImGui::BeginGroup();
                std::string resourceId = std::to_string(static_cast<int>(type));
                if (ImGui::Button(("Sell 1%##" + resourceId).c_str()))
                {
                    SellResource(type, 1.0f);
                }
                ImGui::SameLine();
                if (ImGui::Button(("Sell Half##" + resourceId).c_str()))
                {
                    SellResource(type, resource.GetAmount() * 0.5f);
                }
                ImGui::SameLine();
                if (ImGui::Button(("Sell All##" + resourceId).c_str()))
                {
                    SellResource(type, resource.GetAmount());
                }
                ImGui::EndGroup();
            }
            else
            {
                ImGui::BeginDisabled();
                std::string resourceId = std::to_string(static_cast<int>(type));
                ImGui::Button(("Sell 1##" + resourceId).c_str());
                ImGui::SameLine();
                ImGui::Button(("Sell Half##" + resourceId).c_str());
                ImGui::SameLine();
                ImGui::Button(("Sell All##" + resourceId).c_str());
                ImGui::EndDisabled();
            }
            ImGui::Separator();
        }
    }

    ImGui::End();
}

constexpr int kHistorySize = 100;
static std::map<ResourceType, std::vector<float>> resourceHistory;
static std::map<ResourceType, int> historyOffset;
static std::map<ResourceType, int> historyCount;

void TycoonGame::RenderStockUnlockButton()
{
    if (m_player.hasStocksUnlocked) return;

    bool canUnlock = m_player.reputation >= 40;
    bool canAfford = m_player.money >= GameConstants::STOCK_GRAPH_UNLOCK_PRICE;
    bool isDisabled = !canUnlock || !canAfford;

    if (isDisabled)
    {
        ImGui::BeginDisabled();
    }

    if (ImGui::Button("Unlock Stocks!"))
    {
        if (canUnlock && canAfford)
        {
            m_player.money -= GameConstants::STOCK_GRAPH_UNLOCK_PRICE;
            m_player.totalSpent += GameConstants::STOCK_GRAPH_UNLOCK_PRICE;
            m_player.hasStocksUnlocked = true;
        }
    }

    if (isDisabled)
    {
        ImGui::EndDisabled();
    }

    if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
    {
        ImGui::BeginTooltip();
        
        if (!canUnlock)
        {
            ImGui::Text("Gain more reputation to purchase stock graphs!");
        }
        else if (!canAfford)
        {
            ImGui::Text("Keep Saving, costs %.2f$ to unlock stock graphs!", GameConstants::STOCK_GRAPH_UNLOCK_PRICE);
        }
        else
        {
            ImGui::Text("Click to unlock stock graphs!");
            std::stringstream ss;
            ss << std::fixed << std::setprecision(2) << GameConstants::STOCK_GRAPH_UNLOCK_PRICE;
            std::string costText = "Costs " + ss.str() + "$";
            ImGui::SeparatorText(costText.c_str());
        }
        
        ImGui::EndTooltip();
    }
}

void TycoonGame::RenderStockWindow()
{
    ImGui::SetNextWindowPos(ImVec2(443, 30), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(313, 538), ImGuiCond_FirstUseEver);
    ImGui::Begin("Stock", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    // Market header
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.5f, 0.0f, 1.0f)); // Orange
    ImGui::Text("Market Stocks");
    ImGui::PopStyleColor();
    ImGui::Separator();

    if (!m_player.hasStocksUnlocked)
    {
        RenderStockUnlockButton();
    }
    else
    {
        // Money display with icon
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.84f, 0.0f, 1.0f)); // Gold color
        ImGui::Text("Live:");
        ImGui::PopStyleColor();
        ImGui::Separator();

        // Get all resource types that are produced by owned buildings
        std::vector<ResourceType> producibleResources;
        for (const auto &building : m_player.buildings)
        {
            if (building->IsOwned())
            {
                for (const auto &output : building->GetOutputResources())
                {
                    if (std::find(producibleResources.begin(), producibleResources.end(), output.GetType()) == producibleResources.end())
                    {
                        producibleResources.push_back(output.GetType());
                    }
                }
            }
        }

        // Define history size for 60 seconds (1 sample per second)
        const int kHistorySize = 60;

        // Static map to track last update time per resource
        static std::map<ResourceType, float> lastUpdateTime;

        // Show only resources that can be produced
        for (const auto &[type, resource] : m_player.resources)
        {
            if (type != ResourceType::MONEY &&
                std::find(producibleResources.begin(), producibleResources.end(), type) != producibleResources.end())
            {
                // Colors and symbols
                const char *symbol = "";
                ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

                switch (type)
                {
                case ResourceType::WOOD:
                    symbol = "[W]";
                    color = ImVec4(0.55f, 0.27f, 0.07f, 1.0f);
                    break;
                case ResourceType::STONE:
                    symbol = "[S]";
                    color = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
                    break;
                case ResourceType::IRON:
                    symbol = "[I]";
                    color = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
                    break;
                case ResourceType::GOLD:
                    symbol = "[G]";
                    color = ImVec4(1.0f, 0.84f, 0.0f, 1.0f);
                    break;
                case ResourceType::CRYSTAL:
                    symbol = "[C]";
                    color = ImVec4(0.5f, 0.0f, 0.5f, 1.0f);
                    break;
                case ResourceType::ENERGY:
                    symbol = "[E]";
                    color = ImVec4(0.0f, 0.8f, 1.0f, 1.0f);
                    break;
                case ResourceType::DIAMOND:
                    symbol = "[D]";
                    color = ImVec4(0.0f, 0.8f, 0.8f, 1.0f); // Cyan
                    break;
                }

                // Initialize plot buffer if needed
                if (resourceHistory[type].empty())
                {
                    resourceHistory[type].resize(kHistorySize, resource.GetBasePrice());
                    historyOffset[type] = 0;
                    historyCount[type] = 1;                  // Start with one valid entry
                    lastUpdateTime[type] = ImGui::GetTime(); // Initialize with current time
                }

                // Update history buffer every second
                float currentTime = ImGui::GetTime();
                if (currentTime - lastUpdateTime[type] >= 1.0f) // Update every 1 second
                {
                    resourceHistory[type][historyOffset[type]] = resource.GetBasePrice();
                    historyOffset[type] = (historyOffset[type] + 1) % kHistorySize;

                    // Update the count, up to max size
                    if (historyCount[type] < kHistorySize)
                        historyCount[type]++;

                    // Update last update time
                    lastUpdateTime[type] = currentTime;
                }

                // Reorder data for plotting (oldest to newest)
                std::vector<float> orderedHistory(historyCount[type]);
                for (int i = 0; i < historyCount[type]; ++i)
                {
                    int index = (historyOffset[type] - historyCount[type] + i + kHistorySize) % kHistorySize;
                    orderedHistory[i] = resourceHistory[type][index];
                }

                // Compute min and max over valid data
                float minVal = *std::min_element(orderedHistory.begin(), orderedHistory.end());
                float maxVal = *std::max_element(orderedHistory.begin(), orderedHistory.end());

                // Handle flat line
                if (minVal == maxVal)
                {
                    maxVal += 1.0f;
                    minVal -= 1.0f;
                }

                // Zoom-out padding (25% of range)
                float padding = (maxVal - minVal) * 0.25f;
                minVal -= padding;
                maxVal += padding;

                // Resource label
                ImGui::PushStyleColor(ImGuiCol_Text, color);
                ImGui::Text("%s %s", symbol, resource.GetName().c_str());
                ImGui::PopStyleColor();

                // Live plot
                std::string plotId = "##ResourcePlot_" + std::to_string(static_cast<int>(type));
                ImGui::PlotLines(plotId.c_str(), orderedHistory.data(), orderedHistory.size(),
                                 0, nullptr, minVal, maxVal, ImVec2(0, 60));

                ImGui::Separator();
            }
        }
    }
    ImGui::End();
}

// Save/Load Game
bool TycoonGame::SaveGame(const std::string &filename) const
{
    try
    {
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open())
        {
            return false;
        }

        // Save game time and state
        file.write(reinterpret_cast<const char *>(&m_gameTime), sizeof(m_gameTime));
        file.write(reinterpret_cast<const char *>(&m_isPaused), sizeof(m_isPaused));

        // Save player data
        size_t nameLength = m_player.name.length();
        file.write(reinterpret_cast<const char *>(&nameLength), sizeof(nameLength));
        file.write(m_player.name.c_str(), nameLength);
        file.write(reinterpret_cast<const char *>(&m_player.money), sizeof(m_player.money));
        file.write(reinterpret_cast<const char *>(&m_player.reputation), sizeof(m_player.reputation));
        file.write(reinterpret_cast<const char *>(&m_player.totalEarnings), sizeof(m_player.totalEarnings));
        file.write(reinterpret_cast<const char *>(&m_player.totalSpent), sizeof(m_player.totalSpent));
        file.write(reinterpret_cast<const char *>(&m_player.achievements), sizeof(m_player.achievements));

        // Save resources
        size_t resourceCount = m_player.resources.size();
        file.write(reinterpret_cast<const char *>(&resourceCount), sizeof(resourceCount));
        for (const auto &[type, resource] : m_player.resources)
        {
            int resourceType = static_cast<int>(type);
            file.write(reinterpret_cast<const char *>(&resourceType), sizeof(resourceType));

            nameLength = resource.GetName().length();
            file.write(reinterpret_cast<const char *>(&nameLength), sizeof(nameLength));
            file.write(resource.GetName().c_str(), nameLength);

            float amount = resource.GetAmount();
            float basePrice = resource.GetBasePrice();
            bool owned = resource.IsOwned();

            file.write(reinterpret_cast<const char *>(&amount), sizeof(amount));
            file.write(reinterpret_cast<const char *>(&basePrice), sizeof(basePrice));
            file.write(reinterpret_cast<const char *>(&owned), sizeof(owned));
        }

        // Save buildings
        size_t buildingCount = m_player.buildings.size();
        file.write(reinterpret_cast<const char *>(&buildingCount), sizeof(buildingCount));
        for (const auto &building : m_player.buildings)
        {
            if (building)
            {
                int buildingType = static_cast<int>(building->GetType());
                file.write(reinterpret_cast<const char *>(&buildingType), sizeof(buildingType));

                nameLength = building->GetName().length();
                file.write(reinterpret_cast<const char *>(&nameLength), sizeof(nameLength));
                file.write(building->GetName().c_str(), nameLength);

                float cost = building->GetCost();
                int level = building->GetLevel();
                bool isOwned = building->IsOwned();
                bool isOperational = building->IsOperational();
                float efficiency = building->GetEfficiency();

                file.write(reinterpret_cast<const char *>(&cost), sizeof(cost));
                file.write(reinterpret_cast<const char *>(&level), sizeof(level));
                file.write(reinterpret_cast<const char *>(&isOwned), sizeof(isOwned));
                file.write(reinterpret_cast<const char *>(&isOperational), sizeof(isOperational));
                file.write(reinterpret_cast<const char *>(&efficiency), sizeof(efficiency));
            }
        }

        // Save productions
        size_t productionCount = m_player.productions.size();
        file.write(reinterpret_cast<const char *>(&productionCount), sizeof(productionCount));
        for (const auto &production : m_player.productions)
        {
            int productionType = static_cast<int>(production->GetType());
            file.write(reinterpret_cast<const char *>(&productionType), sizeof(productionType));

            nameLength = production->GetName().length();
            file.write(reinterpret_cast<const char *>(&nameLength), sizeof(nameLength));
            file.write(production->GetName().c_str(), nameLength);

            bool isOwned = production->IsOwned();
            float currentTime = production->GetTime();
            bool isInvested = production->IsInvested();

            file.write(reinterpret_cast<const char *>(&isOwned), sizeof(isOwned));
            file.write(reinterpret_cast<const char *>(&currentTime), sizeof(currentTime));
            file.write(reinterpret_cast<const char *>(&isInvested), sizeof(isInvested));
        }

        // Save has stock graph
        size_t hasStocksUnlocked = m_player.hasStocksUnlocked ? 1:0;
        file.write(reinterpret_cast<const char*>(&hasStocksUnlocked), sizeof(hasStocksUnlocked));


        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool TycoonGame::LoadGame(const std::string &filename)
{
    try
    {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open())
        {
            return false;
        }

        // Load game time and state
        file.read(reinterpret_cast<char *>(&m_gameTime), sizeof(m_gameTime));
        file.read(reinterpret_cast<char *>(&m_isPaused), sizeof(m_isPaused));

        // Load player data
        size_t nameLength;
        file.read(reinterpret_cast<char *>(&nameLength), sizeof(nameLength));
        m_player.name.resize(nameLength);
        file.read(&m_player.name[0], nameLength);
        file.read(reinterpret_cast<char *>(&m_player.money), sizeof(m_player.money));
        file.read(reinterpret_cast<char *>(&m_player.reputation), sizeof(m_player.reputation));
        file.read(reinterpret_cast<char *>(&m_player.totalEarnings), sizeof(m_player.totalEarnings));
        file.read(reinterpret_cast<char *>(&m_player.totalSpent), sizeof(m_player.totalSpent));
        file.read(reinterpret_cast<char *>(&m_player.achievements), sizeof(m_player.achievements));

        // Load resources
        size_t resourceCount;
        file.read(reinterpret_cast<char *>(&resourceCount), sizeof(resourceCount));
        m_player.resources.clear();
        for (size_t i = 0; i < resourceCount; ++i)
        {
            int resourceType;
            file.read(reinterpret_cast<char *>(&resourceType), sizeof(resourceType));

            file.read(reinterpret_cast<char *>(&nameLength), sizeof(nameLength));
            std::string name(nameLength, '\0');
            file.read(&name[0], nameLength);

            float amount, basePrice;
            bool owned;
            file.read(reinterpret_cast<char *>(&amount), sizeof(amount));
            file.read(reinterpret_cast<char *>(&basePrice), sizeof(basePrice));
            file.read(reinterpret_cast<char *>(&owned), sizeof(owned));

            m_player.resources[static_cast<ResourceType>(resourceType)] =
                Resource(static_cast<ResourceType>(resourceType), name, amount, basePrice, owned);
        }

        // Load buildings
        size_t buildingCount;
        file.read(reinterpret_cast<char *>(&buildingCount), sizeof(buildingCount));
        m_player.buildings.clear();
        for (size_t i = 0; i < buildingCount; ++i)
        {
            int buildingType;
            file.read(reinterpret_cast<char *>(&buildingType), sizeof(buildingType));

            file.read(reinterpret_cast<char *>(&nameLength), sizeof(nameLength));
            std::string name(nameLength, '\0');
            file.read(&name[0], nameLength);

            float cost;
            int level;
            bool isOwned, isOperational;
            float efficiency;

            file.read(reinterpret_cast<char *>(&cost), sizeof(cost));
            file.read(reinterpret_cast<char *>(&level), sizeof(level));
            file.read(reinterpret_cast<char *>(&isOwned), sizeof(isOwned));
            file.read(reinterpret_cast<char *>(&isOperational), sizeof(isOperational));
            file.read(reinterpret_cast<char *>(&efficiency), sizeof(efficiency));

            auto building = BuildingFactory::CreateBuilding(static_cast<BuildingType>(buildingType));
            if (building)
            {
                building->SetLevel(level);
                building->SetOwned(isOwned);
                building->SetOperational(isOperational);
                building->SetEfficiency(efficiency);
                m_player.buildings.push_back(std::move(building));
            }
        }

        // Load productions
        InitializeProductionTypes();
        size_t productionCount;
        file.read(reinterpret_cast<char *>(&productionCount), sizeof(productionCount));
        for (size_t i = 0; i < productionCount; ++i)
        {
            int productionType;
            file.read(reinterpret_cast<char *>(&productionType), sizeof(productionType));

            file.read(reinterpret_cast<char *>(&nameLength), sizeof(nameLength));
            std::string name(nameLength, '\0');
            file.read(&name[0], nameLength);
            ;

            bool isOwned;
            float currentTime;
            bool isInvested;
            file.read(reinterpret_cast<char *>(&isOwned), sizeof(isOwned));
            file.read(reinterpret_cast<char *>(&currentTime), sizeof(currentTime));
            file.read(reinterpret_cast<char *>(&isInvested), sizeof(isInvested));

            auto it = std::find_if(
                m_player.productions.begin(),
                m_player.productions.end(),
                [&name](const auto &existing)
                {
                    return existing->GetName() == name;
                });

            if (it != m_player.productions.end())
            {
                (*it)->SetOwned(isOwned);
                (*it)->SetTime(currentTime);
                (*it)->SetIsInvested(isInvested);
            }
        }

        // Load has stock graphs
        size_t hasStocksUnlocked;
        file.read(reinterpret_cast<char*>(&hasStocksUnlocked), sizeof(hasStocksUnlocked));
        m_player.hasStocksUnlocked = (hasStocksUnlocked==1);

        return true;
    }
    catch (...)
    {
        return false;
    }
}