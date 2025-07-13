#include "DeskCardManager.h"
#include "controllers/GameController.h"
#include "views/GameScene.h"
DeskCardManager::DeskCardManager(GameController* controller)
    : _controller(controller) {
}

void DeskCardManager::handleCardTouch(CardView* card) {
    CardView* currentTopCard = _controller->getCurrentTopCard();
    if (!currentTopCard) {
        CCLOG("No top card in hand stack");
        return;
    }

    int diff = std::abs(card->getCardFace() - currentTopCard->getCardFace());
    if (diff != 1) {
        CCLOG("Difference is not 1: %d", diff);
        return;
    }

    OperationRecord record;
    record.type = OperationType::DESK_TO_HAND;
    record.card1 = card;
    record.card2 = currentTopCard;
    record.originalPosition = card->getPosition();
    record.wasTopCard = false;
    record.wasVisible = card->isVisible();

    // 执行操作
    card->setPosition(currentTopCard->getPosition());
    card->setIsTopCard(true);

    currentTopCard->setVisible(false);
    currentTopCard->setIsTopCard(false);

    _controller->setCurrentTopCard(card);
    _controller->getUndoManager()->pushRecord(record);

    // 通知场景更新卡片归属
    if (_controller->getGameScene()) {
        _controller->getGameScene()->moveCardToBottomLayer(card);
    }
}

void DeskCardManager::undoOperation(const OperationRecord& record) {
    // 恢复桌面牌位置和状态
    record.card1->setPosition(record.originalPosition);
    record.card1->setIsTopCard(record.wasTopCard);
    record.card1->setVisible(record.wasVisible);

    // 恢复原顶部牌
    record.card2->setVisible(true);
    record.card2->setIsTopCard(true);

    // 通知场景更新卡片归属
    if (_controller->getGameScene()) {
        _controller->getGameScene()->moveCardToTopLayer(record.card1);
    }
}