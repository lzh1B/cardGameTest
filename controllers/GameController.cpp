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
    // ������ӳ����߼�
    /******************7.13 **************/


    if (_undoStack.empty()) {
        CCLOG("Nothing to undo");
        return;
    }

    OperationRecord record = _undoStack.top();
    _undoStack.pop();

    switch (record.type) {
    case OperationType::HAND_SWAP: {
        // ������λ��
        cocos2d::Vec2 tempPos = record.card1->getPosition();
        record.card1->setPosition(record.card2->getPosition());
        record.card2->setPosition(tempPos);

        // �ָ����������
        record.card1->setIsTopCard(record.wasTopCard);
        record.card2->setIsTopCard(!record.wasTopCard);

        // ���µ�ǰ������
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
        // �ָ�������λ�ú�״̬
        record.card1->setPosition(record.originalPosition);
        record.card1->setIsTopCard(false);
        record.card1->setVisible(true);

        // �ָ�ԭ������
        record.card2->setVisible(true);
        record.card2->setIsTopCard(true);

        // ���µ�ǰ������
        _currentTopCard = record.card2;

        // ֪ͨ�������¿�Ƭ����
        if (_gameScene) {
            _gameScene->moveCardToTopLayer(record.card1);
        }

        CCLOG("Undo: Desk card moved back to original position");
        break;
        }
    }


    /******************7.13 **************/////////////////////
}
// �������濨Ƭ����
void GameController::handleDeskCardTouch(CardView* card) {
    CCLOG("Controller: Desk card touched , Position: (%.0f, %.0f)",
        card->getPositionX(), card->getPositionY());

    /******************7.13 **************/

    // ȷ���ж�����
    if (!_currentTopCard) {
        CCLOG("No top card in hand stack");
        return;
    }

    // ����ֵ�Ƿ�Ϊ1
    int diff = std::abs(card->getCardFace() - _currentTopCard->getCardFace());
    if (diff != 1) {
        CCLOG("Difference is not 1: %d", diff);
        return;
    }

    // ����������¼
    OperationRecord record;
    record.type = OperationType::DESK_TO_HAND;
    record.card1 = card; // ������
    record.card2 = _currentTopCard; // ԭ������
    record.originalPosition = card->getPosition();
    record.wasTopCard = false;

    // ִ�в���
    // �������ƶ���������λ��
    card->setPosition(_currentTopCard->getPosition());
    card->setIsTopCard(true);

    // ԭ���������ز�ʧȥ���
    _currentTopCard->setVisible(false);
    _currentTopCard->setIsTopCard(false);

    // ���µ�ǰ������
    _currentTopCard = card;

    // ���������¼
    _undoStack.push(record);

    // ֪ͨ�������¿�Ƭ����
    if (_gameScene) {
        _gameScene->moveCardToBottomLayer(card);
    }

    CCLOG("Desk card moved to hand top position");

    /******************7.13 **************/////////////////////

}

// �������ƿ�Ƭ����
void GameController::handleHandCardTouch(CardView* card) {
    if (card->isTopCard()) {
        CCLOG("Controller: Hand card touched this is Top Card!!!, Position: (%.0f, %.0f)",
            card->getPositionX(), card->getPositionY());
        return;
    }

    /******************7.13 **************/////////////////////

    CCLOG("Controller: Bottom card touched: Position: (%.0f, %.0f)",
        card->getPositionX(), card->getPositionY());

    // ȷ���ж�����
    if (!_currentTopCard) {
        CCLOG("No top card in hand stack");
        return;
    }

    // ����������¼
    OperationRecord record;
    record.type = OperationType::HAND_SWAP;
    record.card1 = card; // ������ķǶ�����
    record.card2 = _currentTopCard; // ��ǰ������
    record.originalPosition = card->getPosition();
    record.wasTopCard = card->isTopCard();

    // ִ�н�������
    // ����λ��
    cocos2d::Vec2 tempPos = card->getPosition();
    card->setPosition(_currentTopCard->getPosition());
    _currentTopCard->setPosition(tempPos);

    // �������������
    card->setIsTopCard(true);
    _currentTopCard->setIsTopCard(false);

    // ���µ�ǰ������
    _currentTopCard = card;

    // ���������¼
    _undoStack.push(record);

    CCLOG("Hand cards swapped, new top card set");

    /******************7.13 **************/

}