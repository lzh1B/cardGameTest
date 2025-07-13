#pragma once
// GameScene.h
#pragma once
#include "cocos2d.h"

class GameScene : public cocos2d::Scene {
public:
    // 用这个方法创建场景，并传入 levelNumber
    static cocos2d::Scene* createScene(int levelNumber);
    // 使用 CREATE_FUNC 宏生成 create() 方法
    CREATE_FUNC(GameScene);
    virtual bool init() override;

private:
    int _levelNumber;
};