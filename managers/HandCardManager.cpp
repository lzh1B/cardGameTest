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

    // ִ�н�������
    cocos2d::Vec2 tempPos = card->getPosition();
    card->setPosition(currentTopCard->getPosition());
    currentTopCard->setPosition(tempPos);

    card->setIsTopCard(true);
    currentTopCard->setIsTopCard(false);

    // ȷ��ԭ�����ƿɼ�
    currentTopCard->setVisible(true);

    _controller->setCurrentTopCard(card);
    _controller->getUndoManager()->pushRecord(record);
}

void HandCardManager::undoOperation(const OperationRecord& record) {
    // ������λ��
    cocos2d::Vec2 tempPos = record.card1->getPosition();
    record.card1->setPosition(record.card2->getPosition());
    record.card2->setPosition(tempPos);

    // �ָ����������
    record.card1->setIsTopCard(record.wasTopCard);
    record.card2->setIsTopCard(!record.wasTopCard);

    // �ָ��ɼ���
    record.card1->setVisible(record.wasVisible);
    record.card2->setVisible(true);

    // ���µ�ǰ������
    if (record.card1->isTopCard()) {
        _controller->setCurrentTopCard(record.card1);
    }
    else {
        _controller->setCurrentTopCard(record.card2);
    }
}