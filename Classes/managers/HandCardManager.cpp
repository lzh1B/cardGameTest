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

    // ȷ����ǰ����������Ч��
    if (!currentTopCard->isTopCard()) {
        CCLOG("Warning: Current top card is not marked as top!");
        currentTopCard->setIsTopCard(true); // ǿ���޸�״̬
    }

    OperationRecord record;
    record.type = OperationType::HAND_SWAP;
    record.card1 = card;
    record.card2 = currentTopCard;
    record.originalPosition = card->getPosition();
    record.wasTopCard = card->isTopCard();
    record.wasVisible = card->isVisible();

    // ִ�н�������
    cocos2d::Vec2 tempPos = card->getPosition();
    card->setPosition(currentTopCard->getPosition());
    currentTopCard->setPosition(tempPos);

    // �ؼ���ȷ��ֻ��һ�Ŷ�����
    card->setIsTopCard(true);
    currentTopCard->setIsTopCard(false); // ���ԭ������״̬

    // ȷ��ԭ�����ƿɼ�
    currentTopCard->setVisible(true);

    // ���¿������еĶ���������
    _controller->setCurrentTopCard(card);

    // ���������¼
    _controller->getUndoManager()->pushRecord(record);

    CCLOG("Hand cards swapped, new top card set. Old top: %p, New top: %p",
        currentTopCard, card);
}

void HandCardManager::undoOperation(const OperationRecord& record) {
    // ������λ��
    cocos2d::Vec2 tempPos = record.card1->getPosition();
    record.card1->setPosition(record.card2->getPosition());
    record.card2->setPosition(tempPos);

    // �ָ����������
    record.card1->setIsTopCard(record.wasTopCard);
    record.card2->setIsTopCard(!record.wasTopCard); // �ָ�ԭ������״̬

    // �ָ��ɼ���
    record.card1->setVisible(record.wasVisible);
    record.card2->setVisible(true);

    // ���µ�ǰ������ - ȷ��Ψһ��
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

    // ˫�ؼ�飺ȷ��ֻ��һ��������
    if (record.card1->isTopCard() && record.card2->isTopCard()) {
        CCLOG("Warning: Both cards marked as top after undo! Fixing...");
        record.card2->setIsTopCard(false);
    }
}