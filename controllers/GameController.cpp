// GameController.cpp
#include "GameController.h"
#include "cocos2d.h"
#include <algorithm>
#include "views/GameScene.h"
USING_NS_CC;

GameController::~GameController() {
    delete _gameManager;
    CCLOG("GameController Destroyed");
}

GameController::GameController(GameScene* scene, std::vector<CardData>* playfield, std::vector<CardData>* stack) : _gameScene(scene)
   {
    _gameManager = new GameManager(playfield, stack);
    logPlayfield();
    logStack();
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
    // 这里添加撤回逻辑
    /******************7.13 **************/


    if (_undoStack.empty()) {
        CCLOG("Nothing to undo");
        return;
    }

    OperationRecord record = _undoStack.top();
    _undoStack.pop();

    switch (record.type) {
    case OperationType::HAND_SWAP: {
        // 交换回位置
        cocos2d::Vec2 tempPos = record.card1->getPosition();
        record.card1->setPosition(record.card2->getPosition());
        record.card2->setPosition(tempPos);

        // 恢复顶部牌身份
        record.card1->setIsTopCard(record.wasTopCard);
        record.card2->setIsTopCard(!record.wasTopCard);

        // 更新当前顶部牌
        if (record.card1->isTopCard()) {
            _currentTopCard = record.card1;
        }
        else {
            _currentTopCard = record.card2;
        }

        CCLOG("Undo: Hand cards swapped back");
        break;
    }
    case OperationType::DESK_TO_HAND: {
        // 恢复桌面牌位置和状态
        record.card1->setPosition(record.originalPosition);
        record.card1->setIsTopCard(false);
        record.card1->setVisible(true);

        // 恢复原顶部牌
        record.card2->setVisible(true);
        record.card2->setIsTopCard(true);

        // 更新当前顶部牌
        _currentTopCard = record.card2;

        // 通知场景更新卡片归属
        if (_gameScene) {
            _gameScene->moveCardToTopLayer(record.card1);
        }

        CCLOG("Undo: Desk card moved back to original position");
        break;
        }
    }


    /******************7.13 **************/////////////////////
}
// 处理桌面卡片触摸
void GameController::handleDeskCardTouch(CardView* card) {
    CCLOG("Controller: Desk card touched , Position: (%.0f, %.0f)",
        card->getPositionX(), card->getPositionY());

    /******************7.13 **************/

    // 确保有顶部牌
    if (!_currentTopCard) {
        CCLOG("No top card in hand stack");
        return;
    }

    // 检查差值是否为1
    int diff = std::abs(card->getCardFace() - _currentTopCard->getCardFace());
    if (diff != 1) {
        CCLOG("Difference is not 1: %d", diff);
        return;
    }

    // 创建操作记录
    OperationRecord record;
    record.type = OperationType::DESK_TO_HAND;
    record.card1 = card; // 桌面牌
    record.card2 = _currentTopCard; // 原顶部牌
    record.originalPosition = card->getPosition();
    record.wasTopCard = false;

    // 执行操作
    // 桌面牌移动到顶部牌位置
    card->setPosition(_currentTopCard->getPosition());
    card->setIsTopCard(true);

    // 原顶部牌隐藏并失去身份
    _currentTopCard->setVisible(false);
    _currentTopCard->setIsTopCard(false);

    // 更新当前顶部牌
    _currentTopCard = card;

    // 保存操作记录
    _undoStack.push(record);

    // 通知场景更新卡片归属
    if (_gameScene) {
        _gameScene->moveCardToBottomLayer(card);
    }

    CCLOG("Desk card moved to hand top position");

    /******************7.13 **************/////////////////////

}

// 处理手牌卡片触摸
void GameController::handleHandCardTouch(CardView* card) {
    if (card->isTopCard()) {
        CCLOG("Controller: Hand card touched this is Top Card!!!, Position: (%.0f, %.0f)",
            card->getPositionX(), card->getPositionY());
        return;
    }

    /******************7.13 **************/////////////////////

    CCLOG("Controller: Bottom card touched: Position: (%.0f, %.0f)",
        card->getPositionX(), card->getPositionY());

    // 确保有顶部牌
    if (!_currentTopCard) {
        CCLOG("No top card in hand stack");
        return;
    }

    // 创建操作记录
    OperationRecord record;
    record.type = OperationType::HAND_SWAP;
    record.card1 = card; // 被点击的非顶部牌
    record.card2 = _currentTopCard; // 当前顶部牌
    record.originalPosition = card->getPosition();
    record.wasTopCard = card->isTopCard();

    // 执行交换操作
    // 交换位置
    cocos2d::Vec2 tempPos = card->getPosition();
    card->setPosition(_currentTopCard->getPosition());
    _currentTopCard->setPosition(tempPos);

    // 交换顶部牌身份
    card->setIsTopCard(true);
    _currentTopCard->setIsTopCard(false);

    // 更新当前顶部牌
    _currentTopCard = card;

    // 保存操作记录
    _undoStack.push(record);

    CCLOG("Hand cards swapped, new top card set");

    /******************7.13 **************/

}