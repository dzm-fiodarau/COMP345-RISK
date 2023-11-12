#include <iostream>

#include "../../headers/Map.h"



// Load and validate a set of conquest map files
void testLoadMaps()
{
    std::vector<std::string> mapFiles = {"../maps/Cornwall.map", "../maps/EmptyMap.map", "../maps/BadMap.map", "../maps/DoesNotExist.map"};

    for (const auto &mapFile : mapFiles)
    {
        std::cout << "Loading \"" << mapFile << "\"...\n";
        MapLoader loadMap(mapFile);
        Map *mapPtr = loadMap.load();

        if (mapPtr)
        {
            Map map = *mapPtr;

            if (map.validate())
            {
                std::cout << "\"" << mapFile << "\" is valid!\n\n";
            }
            else
            {
                std::cout << "\"" << mapFile << "\" is not valid!\n\n";
            }

            map.printMap();
            std::cout << std::endl;

            delete mapPtr; // Delete allocated memory
        }
        else
        {
            std::cout << "\"" << mapFile << "\" is not valid!\n\n";
        }
    }
}