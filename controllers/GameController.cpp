// GameController.cpp
#include "GameController.h"
#include "cocos2d.h"

USING_NS_CC;

GameController::~GameController() {
    delete _manager;
    CCLOG("GameController Destroyed");
}

GameController::GameController(std::vector<CardData>* playfield, std::vector<CardData>* stack)
   {
    _manager = new GameManager(playfield, stack);
    logPlayfield();
    logStack();
}

void GameController::logPlayfield() const {
    for (size_t i = 0; i < _manager->getPlayfieldCards()->size(); ++i) {
        const CardData& card = (*_manager->getPlayfieldCards())[i];
        CCLOG("Playfield[%zu]: Face=%d, Suit=%d, Position=(%d, %d)",
            i, card.cardFace, card.cardSuit, card.posX, card.posY);
    }
}

void GameController::logStack() const {
    for (size_t i = 0; i < _manager->getStackCards()->size(); ++i) {
        const CardData& card = (*_manager->getStackCards())[i];
        CCLOG("Stack[%zu]: Face=%d, Suit=%d, Position=(%d, %d)",
            i, card.cardFace, card.cardSuit, card.posX, card.posY);
    }
}
void GameController::handleUndo() {
    CCLOG("Undo operation triggered by controller");
    // ������ӳ����߼�
}
// �������濨Ƭ����
void GameController::handleDeskCardTouch(CardView* card) {
    CCLOG("Controller: Desk card touched , Position: (%.0f, %.0f)",
        card->getPositionX(), card->getPositionY());
}

// �������ƿ�Ƭ����
void GameController::handleHandCardTouch(CardView* card) {
    if (card->isTopCard()) {
        CCLOG("Controller: Hand card touched this is Top Card!!!, Position: (%.0f, %.0f)",
            card->getPositionX(), card->getPositionY());
    }
    else {
        CCLOG("Controller: Bottom card touched : Position: (%.0f, %.0f)",
            card->getPositionX(), card->getPositionY());
    }
}