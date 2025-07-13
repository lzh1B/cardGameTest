
#pragma once

#include <vector>
#include "models/CardData.h"
#include "managers/GameManager.h"
#include "views/CardView.h"

class GameController {
public:
    explicit GameController(std::vector<CardData>* playfield, std::vector<CardData>* stack);
    ~GameController();

    void logPlayfield() const;
    void logStack() const;

    std::vector<CardData>* getPlayfieldCards(){ return _manager->getPlayfieldCards(); }
    std::vector<CardData>* getStackCards(){ return _manager->getStackCards(); }

    //事件处理
    // 
    // 处理撤回操作
    void handleUndo();

    // 处理桌面卡片触摸
    void handleDeskCardTouch(CardView* card);

    // 处理手牌卡片触摸
    void handleHandCardTouch(CardView* card);

private:
    GameManager* _manager = nullptr;
};