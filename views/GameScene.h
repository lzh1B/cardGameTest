#pragma once
// GameScene.h
#pragma once
#include "cocos2d.h"

class GameScene : public cocos2d::Scene {
public:
    // ������������������������� levelNumber
    static cocos2d::Scene* createScene(int levelNumber);
    // ʹ�� CREATE_FUNC ������ create() ����
    CREATE_FUNC(GameScene);
    virtual bool init() override;

private:
    int _levelNumber;
};