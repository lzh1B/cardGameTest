
#pragma once

#include <vector>
#include "models/CardData.h"
#include "managers/GameManager.h"

class GameController {
public:
    explicit GameController(std::vector<CardData>* playfield, std::vector<CardData>* stack);
    ~GameController();

    void logPlayfield() const;
    void logStack() const;

    std::vector<CardData>* getPlayfieldCards(){ return _manager->getPlayfieldCards(); }
    std::vector<CardData>* getStackCards(){ return _manager->getStackCards(); }
private:
    GameManager* _manager = nullptr;
};