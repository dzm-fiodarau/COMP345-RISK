#include "Map.h"
#include <iostream>

// TODO: load reading a set of conquest map files and 
// creates a map object for valid map files, and rejects various different invalid map files
void testLoadMaps() {
    MapLoader loadMap("Cornwall.map");
    Map map = loadMap.load();

    if (map.validate()) {
        std::cout << "Map is valid!" << std::endl << std::endl;
    } else {
        std::cout << "Map is not valid." << std::endl << std::endl;
    }
    map.printMap();
}

int main() {
    testLoadMaps();

    return 0;
}
