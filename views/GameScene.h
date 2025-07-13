#pragma once
// GameScene.h
#pragma once
#include "cocos2d.h"
#include "controllers/GameController.h"  // ���� GameController
#include "CardView.h"


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

    // ���鴥������
    void handleTopLayerSpriteTouch(CardView* sprite);
    void handleBottomLayerSpriteTouch(CardView* sprite);

    // ��ť�������
    void onHomeButtonClicked();
private:
    int _levelNumber = 0;
    cocos2d::LayerColor* _topLayer = nullptr;
    cocos2d::LayerColor* _bottomLayer = nullptr;

    GameController* _controller = nullptr; // ������

    // �ֿ��洢�����͵ײ��ľ���
    std::vector<CardView*> _topSprites;    // ����ͼ�㾫��
    std::vector<CardView*> _bottomSprites; // �ײ�ͼ�㾫��
};