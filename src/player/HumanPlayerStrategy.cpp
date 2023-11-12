#include "../../headers/player/HumanPlayerStrategy.h"
#include "../../headers/player/Player.h"
#include "../../headers/commandprocessing/CommandProcessing.h"

HumanPlayerStrategy::HumanPlayerStrategy(Player *owner) : PlayerStrategy(owner) {

}

HumanPlayerStrategy::~HumanPlayerStrategy() {

}

void HumanPlayerStrategy::play() {

}

void HumanPlayerStrategy::issueOrders(CommandProcessor* commandProcessor) {
    //  Printing out prompt
    std::cout << "CURRENT PLAYER:\t[" << player->getName() << "]" << std::endl;
    std::cout << "Please issue an order (enter 'finish' to finish issuing orders):\n";

    do {
        std::string command = commandProcessor->getCommand();

        if (command == "finish")
            break;

        //  BIG TODO: find a way to parse names of players/territories into their actual objects (in pointers ofc)
        std::cout << "Tried instantiating an order" << std::endl;

    } while (true);
}

std::vector<Territory *> HumanPlayerStrategy::toAttack() {
    return std::vector<Territory *>();
}

std::vector<Territory *> HumanPlayerStrategy::toDefend() {
    return std::vector<Territory *>();
}

PlayerStrategy *HumanPlayerStrategy::clone() {
    return nullptr;
}