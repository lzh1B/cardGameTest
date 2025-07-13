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

    // 确保当前顶部牌是有效的
    if (!currentTopCard->isTopCard()) {
        CCLOG("Warning: Current top card is not marked as top!");
        currentTopCard->setIsTopCard(true); // 强制修复状态
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
    record.wasTopCard = false; // 桌面牌原本不是顶部牌
    record.wasVisible = card->isVisible();

    // 执行操作
    card->setPosition(currentTopCard->getPosition());
    card->setIsTopCard(true); // 成为新顶部牌

    // 关键：清除原顶部牌状态
    currentTopCard->setVisible(false);
    currentTopCard->setIsTopCard(false);

    // 更新控制器中的顶部牌引用
    _controller->setCurrentTopCard(card);

    // 保存操作记录
    _controller->getUndoManager()->pushRecord(record);

    // 通知场景更新卡片归属
    if (_controller->getGameScene()) {
        _controller->getGameScene()->moveCardToBottomLayer(card);
    }

    CCLOG("Desk card moved to hand top position. New top: %p, Old top hidden: %p",
        card, currentTopCard);
}

void DeskCardManager::undoOperation(const OperationRecord& record) {
    // 恢复桌面牌位置和状态
    record.card1->setPosition(record.originalPosition);
    record.card1->setIsTopCard(record.wasTopCard); // 恢复为非顶部牌
    record.card1->setVisible(record.wasVisible);

    // 恢复原顶部牌
    record.card2->setVisible(true);
    record.card2->setIsTopCard(true); // 恢复为顶部牌

    // 更新当前顶部牌引用
    _controller->setCurrentTopCard(record.card2);

    // 通知场景更新卡片归属
    if (_controller->getGameScene()) {
        _controller->getGameScene()->moveCardToTopLayer(record.card1);
    }

    CCLOG("Undo: Desk card moved back. Restored top card: %p", record.card2);

    // 双重检查：确保只有一个顶部牌
    if (record.card1->isTopCard()) {
        CCLOG("Warning: Desk card still marked as top after undo! Fixing...");
        record.card1->setIsTopCard(false);
    }
}