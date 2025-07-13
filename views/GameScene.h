#pragma once
// GameScene.h
#pragma once
#include "cocos2d.h"
#include "controllers/GameController.h"  // 引入 GameController

class GameScene : public cocos2d::Scene {
public:
    // 用这个方法创建场景，并传入 levelNumber
    static cocos2d::Scene* createScene(int levelNumber);
    // 使用 CREATE_FUNC 宏生成 create() 方法
    CREATE_FUNC(GameScene);
    virtual bool init() override;
    ~GameScene();//析构函数

    void setController(GameController* controller);
    void createUI();
    void createCardSprites();
    void registerTouchEventHandlers();

    // 触摸事件处理
    bool onTopLayerTouched(cocos2d::Touch* touch, cocos2d::Event* event);
    bool onBottomLayerTouched(cocos2d::Touch* touch, cocos2d::Event* event);

    // 具体业务逻辑处理
    void handleTopLayerTouch(const cocos2d::Vec2& position);
    void handleBottomLayerTouch(const cocos2d::Vec2& position);

    // 按钮点击处理
    void onHomeButtonClicked();
private:
    int _levelNumber = 0;
    cocos2d::LayerColor* _topLayer = nullptr;
    cocos2d::LayerColor* _bottomLayer = nullptr;

    GameController* _controller = nullptr; // 控制器

    std::vector<cocos2d::Sprite*> _playfieldSprites; // 新增：保存 playfield 卡片精灵
};