
#pragma once

#include <vector>
#include "models/CardData.h"

class GameController {
public:
    explicit GameController(const std::vector<CardData>& playfield, const std::vector<CardData>& stack);
    ~GameController();

    void logPlayfield() const;
    void logStack() const;

private:
    std::vector<CardData> _playfieldCards;
    std::vector<CardData> _stackCards;
};