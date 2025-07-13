
#pragma once

#include <vector>
#include <stack>
#include "models/CardData.h"
#include "managers/GameManager.h"
#include "views/CardView.h"
class GameScene;
/******************7.13 **************/
// ��������ö��
enum class OperationType {
    HAND_SWAP,      // ���ƽ�������
    DESK_TO_HAND    // �����Ƶ����Ʋ���
};

// ������¼�ṹ
struct OperationRecord {
    OperationType type;
    CardView* card1;    // �����漰�Ŀ���1
    CardView* card2;    // �����漰�Ŀ���2 (DESK_TO_HANDʱΪ������)
    cocos2d::Vec2 originalPosition; // ԭʼλ��
    bool wasTopCard;    // ����ǰ�Ƿ��Ƕ�����
};
/******************7.13 **************/////////////////////

class GameController {
public:
    explicit GameController(GameScene* scene, std::vector<CardData>* playfield, std::vector<CardData>* stack);
    ~GameController();

    void logPlayfield() const;
    void logStack() const;

    std::vector<CardData>* getPlayfieldCards(){ return _gameManager->getPlayfieldCards(); }
    std::vector<CardData>* getStackCards(){ return _gameManager->getStackCards(); }

    //�¼�����
  
    // �����ز���
    void handleUndo();

    // �������濨Ƭ����
    void handleDeskCardTouch(CardView* card);

    // �������ƿ�Ƭ����
    void handleHandCardTouch(CardView* card);

    // ��ȡ��ǰ������
    CardView* getCurrentTopCard() { return _currentTopCard; }
private:
    GameManager* _gameManager = nullptr;

    /******************7.13 **************/
    
    GameScene* _gameScene = nullptr; // ָ����Ϸ����������
    CardView* _currentTopCard = nullptr; // ��ǰ�������Ķ�����
    std::stack<OperationRecord> _undoStack; // ������ʷջ
public:
    // ��� setCurrentTopCard ����
    void setCurrentTopCard(CardView* card) { _currentTopCard = card; }
    /******************7.13 **************/////////////////////

};