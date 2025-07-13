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

    // 确保当前顶部牌是有效的
    if (!currentTopCard->isTopCard()) {
        CCLOG("Warning: Current top card is not marked as top!");
        currentTopCard->setIsTopCard(true); // 强制修复状态
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

    // 关键：确保只有一张顶部牌
    card->setIsTopCard(true);
    currentTopCard->setIsTopCard(false); // 清除原顶部牌状态

    // 确保原顶部牌可见
    currentTopCard->setVisible(true);

    // 更新控制器中的顶部牌引用
    _controller->setCurrentTopCard(card);

    // 保存操作记录
    _controller->getUndoManager()->pushRecord(record);

    CCLOG("Hand cards swapped, new top card set. Old top: %p, New top: %p",
        currentTopCard, card);
}

void HandCardManager::undoOperation(const OperationRecord& record) {
    // 交换回位置
    cocos2d::Vec2 tempPos = record.card1->getPosition();
    record.card1->setPosition(record.card2->getPosition());
    record.card2->setPosition(tempPos);

    // 恢复顶部牌身份
    record.card1->setIsTopCard(record.wasTopCard);
    record.card2->setIsTopCard(!record.wasTopCard); // 恢复原顶部牌状态

    // 恢复可见性
    record.card1->setVisible(record.wasVisible);
    record.card2->setVisible(true);

    // 更新当前顶部牌 - 确保唯一性
    if (record.card1->isTopCard()) {
        _controller->setCurrentTopCard(record.card1);
        CCLOG("Undo: Restored top card to card1: %p", record.card1);
    }
    else if (record.card2->isTopCard()) {
        _controller->setCurrentTopCard(record.card2);
        CCLOG("Undo: Restored top card to card2: %p", record.card2);
    }
    else {
        CCLOG("Error: No top card after undo operation!");
    }

    // 双重检查：确保只有一个顶部牌
    if (record.card1->isTopCard() && record.card2->isTopCard()) {
        CCLOG("Warning: Both cards marked as top after undo! Fixing...");
        record.card2->setIsTopCard(false);
    }
}