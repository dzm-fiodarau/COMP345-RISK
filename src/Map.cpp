#include "../headers/Map.h"
#include "../headers/Player.h"
#include <iostream>

// Territory Implementation

Territory::Territory(const std::string &name, int xCoord, int yCoord, Continent *cont, Player *owner, int armies)
    : name(name), x(xCoord), y(yCoord), continent(cont), owner(owner), numberOfArmies(armies)
{
    if (owner)
    {
        owner->territory.push_back(this);
    }
}

// Copy constructor
Territory::Territory(const Territory &other)
    : name(other.name), x(other.x), y(other.y), continent(other.continent), owner(other.owner), numberOfArmies(other.numberOfArmies)
{
    for (Territory *territory : other.adjacentTerritories)
    {
        adjacentTerritories.push_back(territory);
    }
}

// Assignment operator
Territory &Territory::operator=(const Territory &other)
{
    if (this != &other)
    { // Check for self-assignment
        name = other.name;
        x = other.x;
        y = other.y;
        continent = other.continent;
        owner = other.owner;
        numberOfArmies = other.numberOfArmies;

        // Clear current adjacent territories
        adjacentTerritories.clear();

        for (Territory *territory : other.adjacentTerritories)
        {
            adjacentTerritories.push_back(territory);
        }
    }
    return *this;
}

// Stream insertion
std::ostream &operator<<(std::ostream &os, const Territory &territory)
{
    os << "Territory Name: " << territory.name
       << ", X Coordinate: " << territory.x
       << ", Y Coordinate: " << territory.y;

    // Check for null owner pointer
    if (territory.owner)
    {
        os << ", Owner: " << territory.owner->getPlayerName();
    }
    else
    {
        os << ", Owner: None";
    }

    os << ", Number of Armies: " << territory.numberOfArmies;

    return os;
}

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

// Copy constructor
Continent::Continent(const Continent &other)
    : name(other.name), bonus(other.bonus), territories(other.territories) {}

// Assignment operator
Continent &Continent::operator=(const Continent &other)
{
    if (this != &other)
    {
        name = other.name;
        bonus = other.bonus;
        territories = other.territories;
    }
    return *this;
}

// Stream insertion operator
std::ostream &operator<<(std::ostream &os, const Continent &continent)
{
    os << "Continent Name: " << continent.name
       << ", Bonus Value: " << continent.bonus
       << ", Territories: ";
    for (const auto &territory : continent.territories)
    {
        os << territory->getName() << " ";
    }
    return os;
}

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

// Copy constructor
Map::Map(const Map &other)
{
    // Deep copy continents
    for (const auto &continent : other.continents)
    {
        continents.push_back(new Continent(*continent));
    }
    // Deep copy territories
    for (const auto &territory : other.territories)
    {
        territories.push_back(new Territory(*territory));
    }
}

// Assignment operator
Map &Map::operator=(const Map &other)
{
    if (this != &other)
    {
        for (auto territory : territories)
        {
            delete territory;
        }
        for (auto continent : continents)
        {
            delete continent;
        }

        territories.clear();
        continents.clear();

        // Deep copy continents
        for (const auto &continent : other.continents)
        {
            continents.push_back(new Continent(*continent));
        }
        // Deep copy territories
        for (const auto &territory : other.territories)
        {
            territories.push_back(new Territory(*territory));
        }
    }
    return *this;
}

// Stream insertion operator
std::ostream &operator<<(std::ostream &os, const Map &map)
{
    os << "Map Details:\n";
    os << "Number of Continents: " << map.getNumContinents() << "\n";
    os << "Number of Territories: " << map.getNumTerritories() << "\n";
    for (const auto &continent : map.getContinents())
    {
        os << continent->getName() << " (Bonus: " << continent->getBonus() << ")\n";

        for (const auto &territory : continent->getTerritories())
        {
            os << " - " << territory->getName()
               << ": owner=" << (territory->getOwner() ? territory->getOwner()->getPlayerName() : "None")
               << "; armies=" << territory->getNumberOfArmies()
               << "; " << territory->getAdjacentTerritories().size() << " adjacent territories: ";

            const auto &adjacentTerritories = territory->getAdjacentTerritories();
            for (size_t i = 0; i < adjacentTerritories.size(); ++i)
            {
                os << adjacentTerritories[i]->getName();
                if (i != adjacentTerritories.size() - 1)
                { // If not the last neighbor
                    os << ", ";
                }
            }
            os << "\n";
        }
    }
    return os;
}

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
{
    return continents;
}

// Check if the map is connected, continents are subgraphs, and each territory belongs to only one continent.
bool Map::validate() const
{
    std::cout << "Validating map...\n";

    // Check if the map is deemed invalid during loading
    if (!getIsValid())
    {
        return false;
    }

    // Check 1: Map is connected
    if (territories.size() > 0 && !isConnected(*territories[0], territories))
    {
        std::cerr << "Error: Map is not connected!\n";
        return false;
    }

    // Check 2: Continents are connected subgraphs
    for (const auto &continent : continents)
    {
        if (continent->getTerritories().size() > 0 && !isConnected(*(continent->getTerritories()[0]), continent->getTerritories()))
        {
            std::cerr << "Error: Continents are not connected subgraphs!\n";
            return false;
        }
    }

    // Check 3: Each territory belongs to one and only one continent
    for (const auto &territory : territories)
    {
        int count = 0;
        for (const auto &continent : continents)
        {
            for (const auto &contTerritory : continent->getTerritories())
            {
                if (territory->getName() == contTerritory->getName())
                {
                    count++;
                }
            }
        }
        if (count != 1)
        {
            std::cerr << "Error: Territory does not belong to one and only one continent!\n";
            return false;
        }
    }

    return true;
}

// DFS Check if a given territory is connected to all other territories in a given list
bool Map::isConnected(const Territory &territory, const std::vector<Territory *> &territoryList) const
{
    std::unordered_set<std::string> visited;
    std::stack<const Territory *> stack;

    stack.push(&territory);

    while (!stack.empty())
    {
        const Territory *currTerritory = stack.top();
        stack.pop();
        visited.insert(currTerritory->getName());

        for (const auto &adjTerritory : currTerritory->getAdjacentTerritories())
        {
            if (visited.find(adjTerritory->getName()) == visited.end() && territoryExistsInList(adjTerritory, territoryList))
            {
                stack.push(adjTerritory);
            }
        }
    }
    return visited.size() == territoryList.size();
}

bool Map::territoryExistsInList(const Territory *territory, const std::vector<Territory *> &territoryList) const
{
    return std::find_if(territoryList.begin(), territoryList.end(),
                        [&territory](const Territory *t)
                        { return t->getName() == territory->getName(); }) != territoryList.end();
}

// Print out valid maps
void Map::printMap() const
{
    // Do not print if map is an invalid map file
    if (!getIsValid())
    {
        return;
    }

    // Print continents and their bonus
    std::cout << "Continents:\n";
    for (const auto &continent : continents)
    {
        std::cout << continent->getName() << " (Bonus: " << continent->getBonus() << "):\n";

        // Print the territories data in each continent
        for (const auto &territory : continent->getTerritories())
        {
            std::cout << " - " << territory->getName()
                      << ": owner=" << (territory->getOwner() ? territory->getOwner()->getPlayerName() : "None")
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

// Copy constructor
MapLoader::MapLoader(const MapLoader &other) : filePath(other.filePath),
                                               parsedTerritoryAdjacencies(other.parsedTerritoryAdjacencies) {}

// Assignement operator
MapLoader &MapLoader::operator=(MapLoader other)
{
    std::swap(filePath, other.filePath);
    std::swap(parsedTerritoryAdjacencies, other.parsedTerritoryAdjacencies);
    return *this;
}

// Stream insertion
std::ostream &operator<<(std::ostream &os, const MapLoader &loader)
{
    os << "MapLoader for file: " << loader.filePath << std::endl;
    os << "Parsed Territory Adjacencies:\n";
    for (const auto &pair : loader.parsedTerritoryAdjacencies)
    {
        os << pair.first << ": ";
        for (const auto &adjacentName : pair.second)
        {
            os << adjacentName << ", ";
        }
        os << std::endl;
    }
    return os;
}

enum class ParseState
{
    NONE,
    CONTINENTS,
    TERRITORIES
};

// Ignore white spaces and carriage return
std::string MapLoader::trim(const std::string &str)
{
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

// Load map file
Map *MapLoader::load()
{
    Map *map = new Map();

    // Check if file exists
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        (*map).setIsValid(false);
        std::cerr << "Error: Failed to open the map file. \"" << filePath << "\" does not exist!\n";
        delete map;
        return nullptr;
    }
    else
    {
        std::cout << "Successfully opened \"" << filePath << "\"\n";
    }

    // Check if file is empty
    if (file.peek() == EOF)
    {
        (*map).setIsValid(false);
        std::cerr << "Error: File is empty!\n";
        delete map;
        return nullptr;
    }

    std::string line;
    ParseState state = ParseState::NONE;

    // Flags to check presence of [Continents] and [Territories]
    bool hasContinents = false, hasTerritories = false;

    // Read each line of the file
    while (getline(file, line))
    {

        // Remove any whitespace from the start/end of the line
        line = trim(line);

        // Detect "Continents" section
        if (line.find("[Continents]") != std::string::npos)
        {
            state = ParseState::CONTINENTS;
            hasContinents = true;
            continue;

            // Detect "Territories" section
        }
        else if (line.find("[Territories]") != std::string::npos)
        {
            state = ParseState::TERRITORIES;
            hasTerritories = true;
            continue;
        }

        switch (state)
        {
        case ParseState::CONTINENTS:
            parseContinentLine(line, *map);
            break;
        case ParseState::TERRITORIES:
            parseTerritoryLine(line, *map);
            break;
        default:
            // Either the line is part of an unrecognized section or it's an error. For now skip it.
            break;
        }
    }

    // Check if map file is missing [Continents] and/or [Territories] sections
    if (!hasContinents || !hasTerritories)
    {
        (*map).setIsValid(false);
        if (!hasContinents)
        {
            std::cerr << "Error: The map file \"" << filePath << "\" does not have a [Continents] section.\n";
        }
        if (!hasTerritories)
        {
            std::cerr << "Error: The map file \"" << filePath << "\" does not have a [Territories] section.\n";
        }
        delete map;
        return nullptr;
    }

    // Once loaded all the territories, link them to their adjacent territories.
    linkAdjacentTerritories(*map);

    file.close();

    std::cout << "Loaded " << (*map).getNumContinents() << " continents.\n";
    std::cout << "Loaded " << (*map).getNumTerritories() << " territories.\n\n";

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
            << "\" but \"" << continentName << "\" does not exist!\n";
        return;
    }

    Territory *newTerritory = new Territory(territoryName, stoi(xStr), stoi(yStr), continentPtr);

    continentPtr->addTerritory(newTerritory);
    map.addTerritory(newTerritory);

    std::vector<std::string> adjacentNames;

    getline(ss, adjacentTerritory, ',');
    if (adjacentTerritory.empty())
    {
        std::cerr << "Error: territory \"" << territoryName << "\" has no adjacent territories!\n";
    }
    else
    {
        adjacentNames.push_back(trim(adjacentTerritory));
    }

    while (ss.good())
    {
        getline(ss, adjacentTerritory, ',');
        if (!adjacentTerritory.empty())
        {
            adjacentNames.push_back(trim(adjacentTerritory));
        }
    }

    // Store the adjacent territory
    parsedTerritoryAdjacencies[territoryName] = adjacentNames;
}
