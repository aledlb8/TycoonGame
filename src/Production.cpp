#include "Production.h"
#include <algorithm>

Production::Production(ProductionType type,
                        const std::string &name,
                        float cost,
                        const std::vector<Resource>& inputResources,
                        const std::vector<Resource>& outputResources,
                        float time,
                        float completionTime,
                        float completionAmount,
                        int requiredReputation)
    : m_type(type), m_name(name), m_cost(cost), m_inputResources(inputResources), m_outputResources(outputResources), m_currentTime(time), m_completionTime(completionTime), m_completionAmount(completionAmount), m_requiredReputation(requiredReputation), m_invested(false), m_isOwned(false)
{
}