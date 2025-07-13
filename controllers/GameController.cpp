// GameController.cpp
#include "GameController.h"
#include "cocos2d.h"

USING_NS_CC;

GameController::~GameController() {
    CCLOG("GameController Destroyed");
}

GameController::GameController(std::vector<CardData>* playfield, std::vector<CardData>* stack)
    : _playfieldCards(playfield), _stackCards(stack) {
    CCLOG("GameController Created with %d Playfield cards and %d Stack cards",
        (int)_playfieldCards->size(), (int)_stackCards->size());

    logPlayfield();
    logStack();
}

void GameController::logPlayfield() const {
    CCLOG("Playfield Cards Count: %d", (int)_playfieldCards->size());
    for (size_t i = 0; i < _playfieldCards->size(); ++i) {
        const CardData& card = (*_playfieldCards)[i];
        CCLOG("Playfield[%zu]: Face=%d, Suit=%d, Position=(%d, %d)",
            i, card.cardFace, card.cardSuit, card.posX, card.posY);
    }
}

void GameController::logStack() const {
    CCLOG("Stack Cards Count: %d", (int)_stackCards->size());
    for (size_t i = 0; i < _stackCards->size(); ++i) {
        const CardData& card = (*_stackCards)[i];
        CCLOG("Stack[%zu]: Face=%d, Suit=%d, Position=(%d, %d)",
            i, card.cardFace, card.cardSuit, card.posX, card.posY);
    }
}