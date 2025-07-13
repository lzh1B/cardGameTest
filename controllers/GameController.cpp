#include "GameController.h"
#include "cocos2d.h"
#include <algorithm>
#include "views/GameScene.h"

USING_NS_CC;

GameController::GameController(GameScene* scene, std::vector<CardData>* playfield, std::vector<CardData>* stack)
    : _gameScene(scene),
    _handCardManager(this),
    _deskCardManager(this)
{
    _gameManager = new GameManager(playfield, stack);
    logPlayfield();
    logStack();
}

GameController::~GameController() {
    delete _gameManager;
    CCLOG("GameController Destroyed");
}

void GameController::logPlayfield() const {
    for (size_t i = 0; i < _gameManager->getPlayfieldCards()->size(); ++i) {
        const CardData& card = (*_gameManager->getPlayfieldCards())[i];
        CCLOG("Playfield[%zu]: Face=%d, Suit=%d, Position=(%d, %d)",
            i, card.cardFace, card.cardSuit, card.posX, card.posY);
    }
}

void GameController::logStack() const {
    for (size_t i = 0; i < _gameManager->getStackCards()->size(); ++i) {
        const CardData& card = (*_gameManager->getStackCards())[i];
        CCLOG("Stack[%zu]: Face=%d, Suit=%d, Position=(%d, %d)",
            i, card.cardFace, card.cardSuit, card.posX, card.posY);
    }
}

void GameController::handleUndo() {
    CCLOG("Undo operation triggered by controller");

    if (_undoManager.isEmpty()) {
        CCLOG("Nothing to undo");
        return;
    }

    OperationRecord record = _undoManager.popRecord();

    switch (record.type) {
    case OperationType::HAND_SWAP:
        _handCardManager.undoOperation(record);
        CCLOG("Undo: Hand cards swapped back");
        break;

    case OperationType::DESK_TO_HAND:
        _deskCardManager.undoOperation(record);
        CCLOG("Undo: Desk card moved back to original position");
        break;
    }
}

void GameController::handleDeskCardTouch(CardView* card) {
    _deskCardManager.handleCardTouch(card);
}

void GameController::handleHandCardTouch(CardView* card) {
    _handCardManager.handleCardTouch(card);
}