#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <map>
#include <stack>
#include <unordered_set>
#include "Player.h"

// Forward declaration
class Continent;
class Territory;
class Player;

/** \class Continent
 *  \brief Represents a continent in the game. */
class Continent
{
public:
    /** \brief Constructs a Continent with the specified name and bonus value.
     *  \param name Name of the continent.
     *  \param bonusValue Bonus value associated with the continent. */
    Continent(const std::string &name, int bonusValue);

    /** \brief Copy constructor.
     *  \param other Other Continent to copy from. */
    Continent(const Continent &other);

    /** \brief Assignment operator.
     *  \param other Other Continent to assign from.
     *  \return Reference to the assigned continent. */
    Continent &operator=(const Continent &other);

    /** \brief Stream insertion operator.
     *  \param os Output stream.
     *  \param continent Continent to output.
     *  \return Modified output stream. */
    friend std::ostream &operator<<(std::ostream &os, const Continent &continent);

    /** \brief Gets the name of the continent.
     *  \return Name of the continent. */
    std::string getName() const;

    /** \brief Gets the bonus value of the continent.
     *  \return Bonus value of the continent. */
    int getBonus() const;

    /** \brief Adds a territory to the continent.
     *  \param territory Pointer to the territory to add. */
    void addTerritory(Territory *territory);

    /** \brief Gets the territories of the continent.
     *  \return Vector of pointers to the territories. */
    std::vector<Territory *> getTerritories() const;

private:
    std::string name; //Name of the continent.
    int bonus;       //Bonus value associated with the continent.
    std::vector<Territory *> territories; //Vector of territories in the continent.
};

/** 
 * \class Territory
 * \brief Represents a territory in the game.
 */
class Territory
{
public:
    /**
     * \brief Constructs a Territory with specified parameters.
     * \param name Name of the territory.
     * \param xCoord X-coordinate of the territory on the game map.
     * \param yCoord Y-coordinate of the territory on the game map.
     * \param cont Pointer to the continent the territory belongs to.
     * \param owner Pointer to the player who owns the territory (nullptr by default).
     * \param armies Number of armies in the territory (0 by default).
     */
    Territory(const std::string &name, int xCoord, int yCoord, Continent *cont, Player * = nullptr, int armies = 0);

    /** 
     * \brief Copy constructor.
     * \param other Other Territory to copy from.
     */
    Territory(const Territory &other);

    /** 
     * \brief Assignment operator.
     * \param other Other Territory to assign from.
     * \return Reference to the assigned territory.
     */
    Territory &operator=(const Territory &other);

    /** 
     * \brief Stream insertion operator.
     * \param os Output stream.
     * \param territory Territory to output.
     * \return Modified output stream.
     */
    friend std::ostream &operator<<(std::ostream &os, const Territory &territory);

    /**
     * \brief Gets the name of the territory.
     * \return Name of the territory.
     */
    std::string getName() const;

    /**
     * \brief Adds an adjacent territory.
     * \param territory Pointer to the territory to be added as adjacent.
     */
    void addAdjacentTerritory(Territory *territory);

    /** 
     * \brief Gets the continent of the territory.
     * \return Pointer to the continent the territory belongs to.
     */
    Continent *getContinent() const;

    /** 
     * \brief Gets all adjacent territories.
     * \return Vector of pointers to adjacent territories.
     */
    std::vector<Territory *> getAdjacentTerritories() const;

    /**
     * \brief Sets the owner of the territory.
     * \param newOwner Pointer to the player to be set as owner.
     */
    void setOwner(Player *newOwner);

    /** 
     * \brief Gets the owner of the territory.
     * \return Pointer to the player who owns the territory.
     */
    Player *getOwner() const;

    /**
     * \brief Sets the number of armies in the territory.
     * \param num Number of armies.
     */
    void setNumberOfArmies(int num);

    /**
     * \brief Gets the number of armies in the territory.
     * \return Number of armies in the territory.
     */
    int getNumberOfArmies() const;

    int numberOfArmies;                           // Number of armies present in the territory.
    std::vector<Territory *> adjacentTerritories; // Vector of territories adjacent to this territory.

private:
    std::string name;     // Name of the territory.
    int x, y;             // Coordinates of the territory on the game map.
    Continent *continent; // Pointer to the continent the territory belongs to.
    Player *owner;        // Pointer to the player who owns the territory.
};

/**
 * \class Map
 * \brief Represents the game map consisting of territories and continents.
 */
class Map
{
public:
    /**
     * \brief Default constructor for Map.
     */
    Map();

    /**
     * \brief Destructor for Map.
     */
    ~Map();

    /**
     * \brief Copy constructor.
     * \param other Other Map to copy from.
     */
    Map(const Map &other);

    /**
     * \brief Assignment operator.
     * \param other Other Map to assign from.
     * \return Reference to the assigned map.
     */
    Map &operator=(const Map &other);

    /**
     * \brief Stream insertion operator for Map.
     * \param os Output stream.
     * \param map Map to output.
     * \return Modified output stream.
     */
    friend std::ostream &operator<<(std::ostream &os, const Map &map);

    /**
     * \brief Adds a territory to the map.
     * \param territory Pointer to the territory to be added.
     */
    void addTerritory(Territory *territory);

    /**
     * \brief Adds a continent to the map.
     * \param continent Pointer to the continent to be added.
     */
    void addContinent(Continent *continent);

    /**
     * \brief Gets a continent by its name.
     * \param name Name of the continent to fetch.
     * \return Pointer to the continent, or nullptr if not found.
     */
    Continent *getContinentByName(const std::string &name) const;

    /**
     * \brief Gets a territory by its name.
     * \param name Name of the territory to fetch.
     * \return Pointer to the territory, or nullptr if not found.
     */
    Territory *getTerritoryByName(const std::string &name) const;

    /**
     * \brief Retrieves all continents in the map.
     * \return Vector of pointers to continents in the map.
     */
    const std::vector<Continent *> &getContinents() const;

    /**
     * \brief Validates the map to check if
     *        1) the map is a connected graph, 
     *        2) continents are connected subgraphs 
     *        3) each country belongs to one and only one continent
     * \return True if the map is valid, false otherwise.
     */
    bool validate() const;

    /**
     * \brief Helper to validate(): Checks if a territory is connected to a list of territories.
     * \param territory Territory to check.
     * \param territoryList List of territories to check against.
     * \return True if the territory is connected, false otherwise.
     */
    bool isConnected(const Territory &territory, const std::vector<Territory *> &territoryList) const;

    /**
     * \brief Prints the map details.
     */
    void printMap() const;

    /** 
     * \brief Gets the number of continents in the map.
     * \return Number of continents.
     */
    size_t getNumContinents() const { return continents.size(); }

    /** 
     * \brief Gets the number of territories in the map.
     * \return Number of territories.
     */
    size_t getNumTerritories() const { return territories.size(); }

    /** 
     * \brief Checks if the map is valid map file.
     * \return True if valid, false otherwise.
     */
    bool getIsValid() const { return isValid; }

    /**
     * \brief Sets the validity status of the map file.
     * \param valid Validity status to set.
     */
    void setIsValid(bool valid) { isValid = valid; }

private:
    std::vector<Continent *> continents;    // Vector of continents present in the map.
    std::vector<Territory *> territories;   // Vector of territories present in the map.
    bool isValid = true;                    // Flag indicating if the map is valid.

    /**
     * \brief Helper to isConnected(): Checks if a territory exists in a given list.
     * \param territory Territory to check.
     * \param territoryList List of territories to check against.
     * \return True if territory exists in the list, false otherwise.
     */
    bool territoryExistsInList(const Territory *territory, const std::vector<Territory *> &territoryList) const;
};


/**
 * \class MapLoader
 * \brief Responsible for loading game maps from file and creating Map objects.
 */
class MapLoader
{
public:
    /**
     * \brief Constructor that takes a file path as parameter.
     * \param filePath Path to the map file to be loaded.
     */
    MapLoader(const std::string &filePath);

    /**
     * \brief Copy constructor.
     * \param other Other MapLoader instance to copy from.
     */
    MapLoader(const MapLoader &other);

    /**
     * \brief Assignment operator.
     * \param other Other MapLoader instance to assign from.
     * \return Reference to the assigned MapLoader.
     */
    MapLoader &operator=(MapLoader other);

    /**
     * \brief Stream insertion operator for MapLoader.
     * \param os Output stream.
     * \param loader MapLoader instance to output.
     * \return Modified output stream.
     */
    friend std::ostream &operator<<(std::ostream &os, const MapLoader &loader);

    /**
     * \brief Loads a game map from .map file and returns a Map object.
     * \return Pointer to the loaded Map object.
     */
    Map *load();

    /**
     * \brief Links adjacent territories in the provided Map object.
     * \param map The Map object where territories need to be linked.
     */
    void linkAdjacentTerritories(Map &map);

private:
    std::string filePath;  /**< Path to the map file. */
    
    /**
     * \brief Helper function to trim whitespace and carriage return from strings.
     * \param str String to be trimmed.
     * \return Trimmed string.
     */
    std::string trim(const std::string &str);

    /**
     * \brief Container to store parsed territory adjacencies.
     */
    std::unordered_map<std::string, std::vector<std::string>> parsedTerritoryAdjacencies;

    /**
     * \brief Helper function to parse continent data from a line and update the Map object.
     * \param line Line from the file containing continent data.
     * \param map The Map object to be updated.
     */
    void parseContinentLine(const std::string &line, Map &map);

    /**
     * \brief Helper function to parse territory data from a line and update the Map object.
     * \param line Line from the file containing territory data.
     * \param map The Map object to be updated.
     */
    void parseTerritoryLine(const std::string &line, Map &map);
};

#endif
