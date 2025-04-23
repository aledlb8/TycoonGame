#include "Tools.h"
#include "../GameConstants.h"

Tools::Tools()
    : Production(ProductionType::TOOLS,                                                                           // type
               "Tool Yard",                                                                                       // name
               380.0f,                                                                                            // cost
               {},                                                                                                // no input resources
               { Resource(ResourceType::MONEY, "Money", 0.0f, 1000, false) },                                     // output resources
               0.0f,                                                                                              // time
               490.0f,                                                                                            // completionTime
               845.50f,                                                                                           // completionAmount
               11)                                                                                                // required reputation
{
}
