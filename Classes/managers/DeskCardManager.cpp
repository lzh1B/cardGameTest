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

    // ȷ����ǰ����������Ч��
    if (!currentTopCard->isTopCard()) {
        CCLOG("Warning: Current top card is not marked as top!");
        currentTopCard->setIsTopCard(true); // ǿ���޸�״̬
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
    record.wasTopCard = false; // ������ԭ�����Ƕ�����
    record.wasVisible = card->isVisible();

    // ִ�в���
    card->setPosition(currentTopCard->getPosition());
    card->setIsTopCard(true); // ��Ϊ�¶�����

    // �ؼ������ԭ������״̬
    currentTopCard->setVisible(false);
    currentTopCard->setIsTopCard(false);

    // ���¿������еĶ���������
    _controller->setCurrentTopCard(card);

    // ���������¼
    _controller->getUndoManager()->pushRecord(record);

    // ֪ͨ�������¿�Ƭ����
    if (_controller->getGameScene()) {
        _controller->getGameScene()->moveCardToBottomLayer(card);
    }

    CCLOG("Desk card moved to hand top position. New top: %p, Old top hidden: %p",
        card, currentTopCard);
}

void DeskCardManager::undoOperation(const OperationRecord& record) {
    // �ָ�������λ�ú�״̬
    record.card1->setPosition(record.originalPosition);
    record.card1->setIsTopCard(record.wasTopCard); // �ָ�Ϊ�Ƕ�����
    record.card1->setVisible(record.wasVisible);

    // �ָ�ԭ������
    record.card2->setVisible(true);
    record.card2->setIsTopCard(true); // �ָ�Ϊ������

    // ���µ�ǰ����������
    _controller->setCurrentTopCard(record.card2);

    // ֪ͨ�������¿�Ƭ����
    if (_controller->getGameScene()) {
        _controller->getGameScene()->moveCardToTopLayer(record.card1);
    }

    CCLOG("Undo: Desk card moved back. Restored top card: %p", record.card2);

    // ˫�ؼ�飺ȷ��ֻ��һ��������
    if (record.card1->isTopCard()) {
        CCLOG("Warning: Desk card still marked as top after undo! Fixing...");
        record.card1->setIsTopCard(false);
    }
}