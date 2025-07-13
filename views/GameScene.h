#pragma once
// GameScene.h
#pragma once
#include "cocos2d.h"
#include "controllers/GameController.h"  // ���� GameController

class GameScene : public cocos2d::Scene {
public:
    // ������������������������� levelNumber
    static cocos2d::Scene* createScene(int levelNumber);
    // ʹ�� CREATE_FUNC ������ create() ����
    CREATE_FUNC(GameScene);
    virtual bool init() override;
    ~GameScene();//��������

private:
    int _levelNumber = 0;
    cocos2d::LayerColor* _topLayer = nullptr;
    cocos2d::LayerColor* _bottomLayer = nullptr;

    GameController* _controller; // ������
};