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

    void setController(GameController* controller);
    void createUI();
    void createCardSprites();
    void registerTouchEventHandlers();

    // �����¼�����
    bool onTopLayerTouched(cocos2d::Touch* touch, cocos2d::Event* event);
    bool onBottomLayerTouched(cocos2d::Touch* touch, cocos2d::Event* event);

    // ����ҵ���߼�����
    void handleTopLayerTouch(const cocos2d::Vec2& position);
    void handleBottomLayerTouch(const cocos2d::Vec2& position);

    // ��ť�������
    void onHomeButtonClicked();
private:
    int _levelNumber = 0;
    cocos2d::LayerColor* _topLayer = nullptr;
    cocos2d::LayerColor* _bottomLayer = nullptr;

    GameController* _controller = nullptr; // ������

    std::vector<cocos2d::Sprite*> _playfieldSprites; // ���������� playfield ��Ƭ����
};