
#pragma once

#include <vector>
#include <stack>
#include "models/CardData.h"
#include "managers/GameManager.h"
#include "views/CardView.h"
class GameScene;
/******************7.13 **************/
// 操作类型枚举
enum class OperationType {
    HAND_SWAP,      // 手牌交换操作
    DESK_TO_HAND    // 桌面牌到手牌操作
};

// 操作记录结构
struct OperationRecord {
    OperationType type;
    CardView* card1;    // 操作涉及的卡牌1
    CardView* card2;    // 操作涉及的卡牌2 (DESK_TO_HAND时为顶部牌)
    cocos2d::Vec2 originalPosition; // 原始位置
    bool wasTopCard;    // 操作前是否是顶部牌
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

    //事件处理
  
    // 处理撤回操作
    void handleUndo();

    // 处理桌面卡片触摸
    void handleDeskCardTouch(CardView* card);

    // 处理手牌卡片触摸
    void handleHandCardTouch(CardView* card);

    // 获取当前顶部牌
    CardView* getCurrentTopCard() { return _currentTopCard; }
private:
    GameManager* _gameManager = nullptr;

    /******************7.13 **************/
    
    GameScene* _gameScene = nullptr; // 指向游戏场景的引用
    CardView* _currentTopCard = nullptr; // 当前手牌区的顶部牌
    std::stack<OperationRecord> _undoStack; // 操作历史栈
public:
    // 添加 setCurrentTopCard 方法
    void setCurrentTopCard(CardView* card) { _currentTopCard = card; }
    /******************7.13 **************/////////////////////

};