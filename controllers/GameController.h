
#pragma once

#include <vector>
#include "models/CardData.h"

class GameController {
public:
    explicit GameController(std::vector<CardData>* playfield, std::vector<CardData>* stack);
    ~GameController();

    void logPlayfield() const;
    void logStack() const;

    std::vector<CardData>* getPlayfieldCards(){ return _playfieldCards; }
    std::vector<CardData>* getStackCards(){ return _stackCards; }
private:
    std::vector<CardData>* _playfieldCards = nullptr;
    std::vector<CardData>* _stackCards = nullptr;

};