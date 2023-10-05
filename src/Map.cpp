#include "../headers/Map.h"
#include "../headers/Player.h"
#include <iostream>

// Territory Implementation

Territory::Territory(const std::string &name, int xCoord, int yCoord, Continent *cont, Player *owner = nullptr, int armies = 0)
    : name(name), x(xCoord), y(yCoord), continent(cont), owner(owner), numberOfArmies(armies) {}

std::string Territory::getName() const
{
    return name;
}

void Territory::addAdjacentTerritory(Territory *adjTerritory)
{
    adjacentTerritories.push_back(adjTerritory);
}

Continent *Territory::getContinent() const
{
    return continent;
}

std::vector<Territory *> Territory::getAdjacentTerritories() const
{
    return adjacentTerritories;
}

void Territory::setOwner(Player *newOwner)
{
    owner = newOwner;
}

Player *Territory::getOwner() const
{
    return owner;
}

void Territory::setNumberOfArmies(int num)
{
    numberOfArmies = num;
}

int Territory::getNumberOfArmies() const
{
    return numberOfArmies;
}

// Continent Implementation

Continent::Continent(const std::string &name, int bonusValue) : name(name), bonus(bonusValue) {}

std::string Continent::getName() const
{
    return name;
}

int Continent::getBonus() const
{
    return bonus;
}

void Continent::addTerritory(Territory *territory)
{
    territories.push_back(territory);
}

std::vector<Territory *> Continent::getTerritories() const
{
    return territories;
}

// Map Implementation

Map::Map() {}

// Destructor
Map::~Map()
{
    // Delete all territories
    for (Territory *territory : territories)
    {
        delete territory;
    }

    // Delete all continents
    for (Continent *continent : continents)
    {
        delete continent;
    }
}

void Map::addTerritory(Territory *territory)
{
    territories.push_back(territory);
}

void Map::addContinent(Continent *continent)
{
    continents.push_back(continent);
}

// TODO: Implement validation based on the provided specifications
// Check if the map is connected, continents are subgraphs, and each territory belongs to only one continent.
bool Map::validate() const
{
    std::cout << "Validating map..." << std::endl;

    return true;
}

Continent *Map::getContinentByName(const std::string &name) const
{
    for (const auto &continent : continents)
    {
        if (continent->getName() == name)
        {
            return continent;
        }
    }
    return nullptr;
}

Territory *Map::getTerritoryByName(const std::string &name) const
{
    for (auto &territory : territories)
    {
        if (territory->getName() == name)
        {
            return territory;
        }
    }
    return nullptr;
}

const std::vector<Continent *> &Map::getContinents() const
{ // debug
    return continents;
}

void Map::printMap() const
{

    // Print continents and their bonus
    std::cout << "Continents:" << std::endl;
    for (const auto &continent : continents)
    {
        std::cout << continent->getName() << " (Bonus: " << continent->getBonus() << ")" << std::endl;

        // Print the territories data in each continent
        for (const auto &territory : continent->getTerritories())
        {
            std::cout << " - " << territory->getName()
                      << ": owner=" << (territory->getOwner() ? territory->getOwner()->getPlayerName() : "None")  //  CHANGED 10/4/2023
                      << "; armies=" << territory->getNumberOfArmies()
                      << "; " << territory->getAdjacentTerritories().size() << " adjacent territories: ";
            const auto &adjacentTerritories = territory->getAdjacentTerritories();
            for (size_t i = 0; i < adjacentTerritories.size(); ++i)
            {
                std::cout << adjacentTerritories[i]->getName();
                if (i != adjacentTerritories.size() - 1)
                { // If not the last neighbor
                    std::cout << ", ";
                }
            }
            std::cout << std::endl;
        }
    }
}

// MapLoader implementation

MapLoader::MapLoader(const std::string &filePath) : filePath(filePath) {}

enum class ParseState
{
    NONE,
    CONTINENTS,
    TERRITORIES
};

std::string MapLoader::trim(const std::string &str)
{
    // Ignore white spaces and carriage return
    size_t start = str.find_first_not_of(" \t\r");
    size_t end = str.find_last_not_of(" \t\r");
    if (start == std::string::npos || end == std::string::npos)
    {
        return "";
    }
    return str.substr(start, end - start + 1);
}

// Loop over all territories and link them to their adjacent territories
// For each territory, find its adjacent territories by name and add a pointer to them
void MapLoader::linkAdjacentTerritories(Map &map)
{
    for (const auto &territoryData : parsedTerritoryAdjacencies)
    {
        Territory *territory = map.getTerritoryByName(territoryData.first);

        if (!territory)
            continue;

        for (const auto &adjName : territoryData.second)
        {
            Territory *adjacentTerritory = map.getTerritoryByName(adjName);
            if (adjacentTerritory)
            {
                territory->addAdjacentTerritory(adjacentTerritory);
            }
        }
    }
}

Map MapLoader::load()
{
    Map map;
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Failed to open the map file." << std::endl;
        return map;
    }
    else
    {
        std::cout << "Successfully opened the map file." << std::endl;
    }

    std::string line;
    ParseState state = ParseState::NONE;

    // Read each line of the file
    while (getline(file, line))
    {

        // Remove any whitespace from the start/end of the line
        line = trim(line);

        // Detect "Continents" section
        if (line.find("[Continents]") != std::string::npos)
        {
            state = ParseState::CONTINENTS;
            continue;

            // Detect "Territories" section
        }
        else if (line.find("[Territories]") != std::string::npos)
        {
            state = ParseState::TERRITORIES;
            continue;
        }

        switch (state)
        {
        case ParseState::CONTINENTS:
            parseContinentLine(line, map);
            break;
        case ParseState::TERRITORIES:
            parseTerritoryLine(line, map);
            break;
        default:
            // Either the line is part of an unrecognized section or it's an error. For now skip it.
            break;
        }
    }

    // Once loaded all the territories, link them to their adjacent territories.
    linkAdjacentTerritories(map);

    file.close();

    std::cout << "Loaded " << map.getNumContinents() << " continents." << std::endl;
    std::cout << "Loaded " << map.getNumTerritories() << " territories." << std::endl
              << std::endl;

    return map;
}

void MapLoader::parseContinentLine(const std::string &line, Map &map)
{
    if (line.empty() || line.find('=') == std::string::npos)
    {
        return;
    }

    std::stringstream ss(line);
    std::string continentName, bonusStr;
    getline(ss, continentName, '=');
    getline(ss, bonusStr, '=');

    int bonusValue = stoi(bonusStr);
    Continent *newContinent = new Continent(continentName, bonusValue);
    map.addContinent(newContinent);
}

void MapLoader::parseTerritoryLine(const std::string &line, Map &map)
{

    std::stringstream ss(line);
    std::string territoryName, xStr, yStr, continentName, adjacentTerritory;

    getline(ss, territoryName, ',');
    getline(ss, xStr, ',');
    getline(ss, yStr, ',');
    getline(ss, continentName, ',');

    Continent *continentPtr = map.getContinentByName(continentName);

    // Check if continent exists
    if (!continentPtr)
    {
        std::cerr
            << "Error: territory \"" << territoryName
            << "\" is associated with continent \"" << continentName
            << "\" but \"" << continentName << "\" does not exist!" << std::endl;
        return;
    }

    Territory *newTerritory = new Territory(territoryName, stoi(xStr), stoi(yStr), continentPtr);

    continentPtr->addTerritory(newTerritory);
    map.addTerritory(newTerritory);

    std::vector<std::string> adjacentNames;

    getline(ss, adjacentTerritory, ',');
    if (adjacentTerritory.empty())
    {
        std::cerr << "Error: territory \"" << territoryName << "\" has no adjacent territories!" << std::endl;
    }

    while (ss.good())
    {
        getline(ss, adjacentTerritory, ',');
        if (!adjacentTerritory.empty())
        {
            adjacentNames.push_back(trim(adjacentTerritory));
        }
    }

    parsedTerritoryAdjacencies[territoryName] = adjacentNames;
}
