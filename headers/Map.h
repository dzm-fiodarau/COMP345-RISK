#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <map>

class Continent;
class Territory;
class Player;

//placeholder for player
class Player{
public:
    std::string getName() const;

private:
    std::string name;
    int playerID;
}; 

class Continent {
public:
    Continent(const std::string& name, int bonusValue);
    std::string getName() const;
    int getBonus() const;
    void addTerritory(Territory* territory);
    std::vector<Territory*> getTerritories() const;

private:
    std::string name;
    int bonus;
    std::vector<Territory*> territories;
};

class Territory {
public:
    Territory(const std::string& name, int xCoord, int yCoord, Continent* cont, Player* owner, int armies);
    std::string getName() const;
    void addAdjacentTerritory(Territory* territory);
    Continent* getContinent() const;
    std::vector<Territory*> getAdjacentTerritories() const;
    void setOwner(Player* newOwner);
    Player* getOwner() const;
    void setNumberOfArmies(int num);
    int getNumberOfArmies() const;

private:
    std::string name;
    int x, y;  // Coordinates
    Continent* continent;
    std::vector<Territory*> adjacentTerritories;
    Player* owner;
    int numberOfArmies;
};

class Map {
public:
    Map();
    ~Map();
    bool validate() const; //TODO
    void addTerritory(Territory* territory);
    void addContinent(Continent* continent);
    Continent* getContinentByName(const std::string& name) const;
    Territory* getTerritoryByName(const std::string& name) const;
    const std::vector<Continent*>& getContinents() const;
    void printMap() const;
    size_t getNumContinents() const { return continents.size(); }
    size_t getNumTerritories() const { return territories.size(); }

private:
    std::vector<Continent*> continents;
    std::vector<Territory*> territories;
};

class MapLoader {
public:
    explicit MapLoader(const std::string& filePath);
    Map load();
    void linkAdjacentTerritories(Map& map);

private:
    std::string filePath;
    std::string trim(const std::string& str);
    std::unordered_map<std::string, std::vector<std::string> > parsedTerritoryAdjacencies;
    void parseContinentLine(const std::string& line, Map& map);
    void parseTerritoryLine(const std::string& line, Map& map);
};

#endif
