#include "HandCardManager.h"
#include "controllers/GameController.h"

HandCardManager::HandCardManager(GameController* controller)
    : _controller(controller) {
}

void HandCardManager::handleCardTouch(CardView* card) {
    if (card->isTopCard()) {
        CCLOG("Controller: Hand card touched this is Top Card!!!, Position: (%.0f, %.0f)",
            card->getPositionX(), card->getPositionY());
        return;
    }

    CardView* currentTopCard = _controller->getCurrentTopCard();
    if (!currentTopCard) {
        CCLOG("No top card in hand stack");
        return;
    }

    OperationRecord record;
    record.type = OperationType::HAND_SWAP;
    record.card1 = card;
    record.card2 = currentTopCard;
    record.originalPosition = card->getPosition();
    record.wasTopCard = card->isTopCard();
    record.wasVisible = card->isVisible();

    // 执行交换操作
    cocos2d::Vec2 tempPos = card->getPosition();
    card->setPosition(currentTopCard->getPosition());
    currentTopCard->setPosition(tempPos);

    card->setIsTopCard(true);
    currentTopCard->setIsTopCard(false);

    // 确保原顶部牌可见
    currentTopCard->setVisible(true);

    _controller->setCurrentTopCard(card);
    _controller->getUndoManager()->pushRecord(record);
}

void HandCardManager::undoOperation(const OperationRecord& record) {
    // 交换回位置
    cocos2d::Vec2 tempPos = record.card1->getPosition();
    record.card1->setPosition(record.card2->getPosition());
    record.card2->setPosition(tempPos);

    // 恢复顶部牌身份
    record.card1->setIsTopCard(record.wasTopCard);
    record.card2->setIsTopCard(!record.wasTopCard);

    // 恢复可见性
    record.card1->setVisible(record.wasVisible);
    record.card2->setVisible(true);

    // 更新当前顶部牌
    if (record.card1->isTopCard()) {
        _controller->setCurrentTopCard(record.card1);
    }
    else {
        _controller->setCurrentTopCard(record.card2);
    }
}