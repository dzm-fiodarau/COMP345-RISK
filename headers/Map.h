#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <map>
#include "Player.h"

class Continent;
class Player;
class Territory;

//  README
//  CHANGED 10/4/2023
//  Changed so that the project can compile. Issue: the placeholder classes were interfering with the linking during
//  compile time. Therefore, project could not compile.

//  CHANGES
//  - Added include directive: #include "Player.h"
//  - Removed the first declaration of the Player class: class Player;
//  - Removed the placeholder class and corresponding methods in the .cpp file
//  - Adjusted method call in the .cpp file
//          from 'territory->getOwner()->getName()' to 'territory->getOwner()->getPlayerName()'

class Continent
{
public:
    Continent(const std::string &name, int bonusValue);
    std::string getName() const;
    int getBonus() const;
    void addTerritory(Territory *territory);
    std::vector<Territory *> getTerritories() const;

private:
    std::string name;
    int bonus;
    std::vector<Territory *> territories;
};

class Territory
{
public:
    int numberOfArmies;
    std::vector<Territory *> adjacentTerritories;

    Territory(const std::string &name, int xCoord, int yCoord, Continent *cont, Player *owner, int armies);
    std::string getName() const;
    void addAdjacentTerritory(Territory *territory);
    Continent *getContinent() const;
    std::vector<Territory *> getAdjacentTerritories() const;
    void setOwner(Player *newOwner);
    Player *getOwner() const;
    void setNumberOfArmies(int num);
    int getNumberOfArmies() const;

private:
    std::string name;
    int x, y; // Coordinates
    Continent *continent;
    Player *owner;
};

class Map
{
public:
    Map();
    ~Map();
    bool validate() const; // TODO
    void addTerritory(Territory *territory);
    void addContinent(Continent *continent);
    Continent *getContinentByName(const std::string &name) const;
    Territory *getTerritoryByName(const std::string &name) const;
    const std::vector<Continent *> &getContinents() const;
    void printMap() const;
    size_t getNumContinents() const { return continents.size(); }
    size_t getNumTerritories() const { return territories.size(); }

private:
    std::vector<Continent *> continents;
    std::vector<Territory *> territories;
};

class MapLoader
{
public:
    explicit MapLoader(const std::string &filePath);
    Map load();
    void linkAdjacentTerritories(Map &map);

private:
    std::string filePath;
    std::string trim(const std::string &str);
    std::unordered_map<std::string, std::vector<std::string>> parsedTerritoryAdjacencies;
    void parseContinentLine(const std::string &line, Map &map);
    void parseTerritoryLine(const std::string &line, Map &map);
};

#endif
