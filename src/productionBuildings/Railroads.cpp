#include "Railroads.h"
#include "../GameConstants.h"

Railroads::Railroads()
    : Production(ProductionType::RAILROADS,                                                                       // type
               "Railroad Station",                                                                                // name
               980.60f,                                                                                           // cost
               {},                                                                                                // no input resources
               { Resource(ResourceType::MONEY, "Money", 0.0f, 1000, false) },                                     // output resources
               0.0f,                                                                                              // time
               910.0f,                                                                                            // completionTime
               1870.00f,                                                                                          // completionAmount
               17)                                                                                                // required reputation
{
}
