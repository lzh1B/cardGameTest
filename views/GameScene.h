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

private:
    int _levelNumber = 0;
    cocos2d::LayerColor* _topLayer = nullptr;
    cocos2d::LayerColor* _bottomLayer = nullptr;

    GameController* _controller; // 控制器
};