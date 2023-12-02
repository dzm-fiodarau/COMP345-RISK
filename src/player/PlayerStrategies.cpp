#include "../../headers/player/PlayerStrategies.h"
#include "../../headers/player/Player.h"
#include "../../headers/commandprocessing/CommandProcessing.h"
#include "../../headers/gameengine/GameEngine.h"
#include "../../headers/Orders.h"
#include "../../headers/Cards.h"
#include "../../headers/Map.h"
#include <sstream>
#include <algorithm>

PlayerStrategy::PlayerStrategy(Player *player)
{
    this->player = player;
}

PlayerStrategy::~PlayerStrategy() = default;

//----------------------------------------------------------------------------------------------------------------------
// HUMAN PLAYER STRATEGY
HumanPlayerStrategy::HumanPlayerStrategy(Player *owner) : PlayerStrategy(owner)
{
}

HumanPlayerStrategy::~HumanPlayerStrategy()
{
}

void HumanPlayerStrategy::play()
{
}

void HumanPlayerStrategy::issueOrders(GameEngine *gameEngine)
{
    //  Printing out prompt
    std::cout << "CURRENT PLAYER:\t[" << player->getName() << "]" << std::endl;
    std::cout << "Please issue an order \nOrder arguments in this order: <order name> <target territory> <# units> <source territory> <negotiate player>\nOnly enter the arguments that are needed for the specified order\n(enter 'finish' to finish issuing orders):\n";

    CommandProcessor *commandProcessor = gameEngine->getCommandProcessor();
    std::string command_str = commandProcessor->getCommand();

    if (command_str == "finish")
    {
        player->setIssuingOrders(false);
        return;
    }
    Command *command = new Command(command_str);
    vector<string> args = command->getRemainingTokens();
    Order *order = new DeployOrder();
    // issueOrder(Order::OrderType orderType, Territory * target, int armyUnits, Territory *source, Player *player);
    switch (order->parseOrderType(command->getFirstToken()))
    {
    case Order::OrderType::Deploy:
        player->issueOrder(Order::OrderType::Deploy, gameEngine->getMap()->getTerritoryByName(args[0]), stoi(args[1]), nullptr, nullptr);
        return;

    //  Issue an 'advance' order. No card has to be present like other orders.
    case Order::OrderType::Advance:
        player->issueOrder(Order::OrderType::Advance, gameEngine->getMap()->getTerritoryByName(args[0]), stoi(args[1]), gameEngine->getMap()->getTerritoryByName(args[2]), nullptr);
        return;

    //  We check if the player has a 'bomb' card. If so, we 'consume' the card and issue an order
    //  If not, print an error message, nothing is issued.
    case Order::OrderType::Bomb:
        player->issueOrder(Order::OrderType::Bomb, gameEngine->getMap()->getTerritoryByName(args[0]), 0, nullptr, nullptr);
        return;

    case Order::OrderType::Blockade:
        player->issueOrder(Order::OrderType::Blockade, gameEngine->getMap()->getTerritoryByName(args[0]), 0, nullptr, nullptr);
        return;

    case Order::OrderType::Airlift:
        player->issueOrder(Order::OrderType::Airlift, gameEngine->getMap()->getTerritoryByName(args[0]), stoi(args[1]), gameEngine->getMap()->getTerritoryByName(args[2]), nullptr);
        return;

    case Order::OrderType::Negotiate:
        player->issueOrder(Order::OrderType::Negotiate, nullptr, 0, nullptr, gameEngine->getPlayerByName(args[0]));
        return;

    default:
        cout << "Specified order is not allowed. No order was added to the orders list." << endl;
        return;
    }
}

std::vector<Territory *> HumanPlayerStrategy::toAttack()
{
    return std::vector<Territory *>();
}

std::vector<Territory *> HumanPlayerStrategy::toDefend()
{
    return std::vector<Territory *>();
}

PlayerStrategy *HumanPlayerStrategy::clone()
{
    return nullptr;
}

//----------------------------------------------------------------------------------------------------------------------
// AGRESSIVE PLAYER STRATEGY
AggressivePlayerStrategy::AggressivePlayerStrategy(Player *owner) : PlayerStrategy(owner)
{
}

AggressivePlayerStrategy::~AggressivePlayerStrategy()
{
}

void AggressivePlayerStrategy::play()
{
}

void AggressivePlayerStrategy::issueOrders(GameEngine *)
{
    cout << "In aggressive " << *player << endl;
    // Deploys all reinforcements to one of the strongest territories
    vector<Territory *> strongest = vector<Territory *>();
    for (Territory *territory : player->getTerritories())
    {
        if (strongest.size() == 0 || strongest[0]->getNumberOfArmies() < territory->getNumberOfArmies())
        {
            strongest.clear();
            strongest.push_back(territory);
        }
        else if (strongest[0]->getNumberOfArmies() > territory->getNumberOfArmies())
        {
            continue;
        }
        else
        {
            strongest.push_back(territory);
        }
    }
    srand(time(NULL));
    int strongest_index = rand() % strongest.size();
    player->issueOrder(Order::OrderType::Deploy, strongest[strongest_index], player->getReinforcementPool(), nullptr, nullptr);

    // Bombs an opponent's territory
    vector<const Card *> cards = player->getCards();
    for (int i = 0; i < cards.size(); i++)
    {
        if (cards[i]->getCardType() != type::bomb)
            continue;
        vector<Territory *> potential_bombing = vector<Territory *>();
        for (Territory *territory : player->getTerritories())
        {
            for (Territory *adjacent : territory->getAdjacentTerritories())
            {
                potential_bombing.push_back(adjacent);
            }
        }
        int bombing_index = rand() % potential_bombing.size();
        player->issueOrder(Order::OrderType::Bomb, potential_bombing[bombing_index], 0, nullptr, nullptr);
        break;
    }

    // Attacks or moves from strongest territory
    vector<Territory *> opponent_adjacent = vector<Territory *>();
    for (Territory *territory : strongest[strongest_index]->getAdjacentTerritories())
    {
        if (territory->getOwner() != player)
        {
            opponent_adjacent.push_back(territory);
        }
    }
    if (opponent_adjacent.size() > 0)
    {
        int attack_index = rand() % opponent_adjacent.size();
        if (strongest[strongest_index]->getNumberOfArmies() > 0)
        {
            player->issueOrder(Order::OrderType::Advance, opponent_adjacent[attack_index], strongest[strongest_index]->getNumberOfArmies() - 1, strongest[strongest_index], nullptr);
        }
    }
    else
    {
        int move_index = rand() % strongest[strongest_index]->getAdjacentTerritories().size();
        if (strongest[strongest_index]->getNumberOfArmies() > 0)
        {
            player->issueOrder(Order::OrderType::Advance, strongest[strongest_index]->getAdjacentTerritories()[move_index], strongest[strongest_index]->getNumberOfArmies() - 1, strongest[strongest_index], nullptr);
        }
    }
    player->setIssuingOrders(false);
}

std::vector<Territory *> AggressivePlayerStrategy::toAttack()
{
    return std::vector<Territory *>();
}

std::vector<Territory *> AggressivePlayerStrategy::toDefend()
{
    return std::vector<Territory *>();
}

PlayerStrategy *AggressivePlayerStrategy::clone()
{
    return nullptr;
}

//----------------------------------------------------------------------------------------------------------------------
// BENEVOLENT PLAYER STRATEGY
BenevolentPlayerStrategy::BenevolentPlayerStrategy(Player *owner) : PlayerStrategy(owner)
{
}

BenevolentPlayerStrategy::~BenevolentPlayerStrategy()
{
}

void BenevolentPlayerStrategy::play()
{
}

void BenevolentPlayerStrategy::issueOrders(GameEngine *gameEngine)
{
    cout << "In benevolent player issueOrders\n";
    // Deploys all reinforcements to the weakest territories
    vector<Territory *> weakest = vector<Territory *>();
    for (Territory *territory : player->getTerritories())
    {
        if (weakest.size() == 0 || weakest[0]->getNumberOfArmies() > territory->getNumberOfArmies())
        {
            weakest.clear();
            weakest.push_back(territory);
        }
        else if (weakest[0]->getNumberOfArmies() > territory->getNumberOfArmies())
        {
            continue;
        }
        else
        {
            weakest.push_back(territory);
        }
    }
    int reinforcementPart = player->getReinforcementPool() / weakest.size();
    for (Territory *territory : weakest)
    {
        int unitsToDeploy = territory == weakest[weakest.size() - 1] ? player->getReinforcementPool() : reinforcementPart;
        player->issueOrder(Order::OrderType::Deploy, territory, unitsToDeploy, nullptr, nullptr);
    }

    // Uses diplomacy card the player has it
    vector<const Card *> cards = player->getCards();
    for (int i = 0; i < cards.size(); i++)
    {
        if (cards[i]->getCardType() != type::diplomacy)
            continue;
        int bombing_index = 0;
        do
        {
            bombing_index = gameEngine->getPlayers().size();
        } while (gameEngine->getPlayers()[bombing_index] == player);
        player->issueOrder(Order::OrderType::Negotiate, nullptr, 0, nullptr, gameEngine->getPlayers()[bombing_index]);
        break;
    }
    player->setIssuingOrders(false);
}

std::vector<Territory *> BenevolentPlayerStrategy::toAttack()
{
    return std::vector<Territory *>();
}

std::vector<Territory *> BenevolentPlayerStrategy::toDefend()
{
    return std::vector<Territory *>();
}

PlayerStrategy *BenevolentPlayerStrategy::clone()
{
    return nullptr;
}

//----------------------------------------------------------------------------------------------------------------------
//  Neutral PLAYER STRATEGY
NeutralPlayerStrategy::NeutralPlayerStrategy(Player *owner) : PlayerStrategy(owner)
{
}

NeutralPlayerStrategy::~NeutralPlayerStrategy()
{
}

void NeutralPlayerStrategy::play()
{
}

void NeutralPlayerStrategy::issueOrders(GameEngine *)
{
    cout << "In neutral player issueOrders\n";
    player->setIssuingOrders(false);
}

std::vector<Territory *> NeutralPlayerStrategy::toAttack()
{
    return std::vector<Territory *>();
}

std::vector<Territory *> NeutralPlayerStrategy::toDefend()
{
    return std::vector<Territory *>();
}

PlayerStrategy *NeutralPlayerStrategy::clone()
{
    return nullptr;
}

//----------------------------------------------------------------------------------------------------------------------
//  CHEATER PLAYER STRATEGY
CheaterPlayerStrategy::CheaterPlayerStrategy(Player *owner) : PlayerStrategy(owner)
{
}

CheaterPlayerStrategy::~CheaterPlayerStrategy()
{
}

void CheaterPlayerStrategy::play()
{
}

void CheaterPlayerStrategy::issueOrders(GameEngine *)
{
    cout << "In cheater player issueOrders\n";
    vector<Territory *> toConquer = vector<Territory *>();
    for (Territory *territory : player->getTerritories())
    {
        for (Territory *adjacent : territory->getAdjacentTerritories())
        {
            toConquer.push_back(adjacent);
        }
    }

    for (Territory *territory : toConquer)
    {
        territory->getOwner()->removeTerritory(*territory);
        player->addTerritory(*territory);
        territory->setOwner(player);
    }
    player->setIssuingOrders(false);
}

std::vector<Territory *> CheaterPlayerStrategy::toAttack()
{
    return std::vector<Territory *>();
}

std::vector<Territory *> CheaterPlayerStrategy::toDefend()
{
    return std::vector<Territory *>();
}

PlayerStrategy *CheaterPlayerStrategy::clone()
{
    return nullptr;
}
