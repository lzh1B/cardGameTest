// GameScene.cpp
#include "GameScene.h"
#include "ui/CocosGUI.h"
#include "LevelSelectScene.h"

USING_NS_CC;

Scene* GameScene::createScene(int levelNumber) {
    auto scene = Scene::create();        // �����ճ���
    auto layer = GameScene::create();   // ���� CREATE_FUNC �ṩ�� create()
    layer->_levelNumber = levelNumber;  // ���ùؿ���
    log("level number is : xxxxx %d", layer->_levelNumber);
    scene->addChild(layer);             // ��� GameScene �㵽������
    return scene;
}

bool GameScene::init() {
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    const float totalRatio = 150 + 58;
    float topHeight = visibleSize.height * (150 / totalRatio);
    float bottomHeight = visibleSize.height * (58 / totalRatio);

    // ������������
    _topLayer = LayerColor::create(Color4B(200, 230, 255, 255), visibleSize.width, topHeight);
    _topLayer->setPosition(origin.x, origin.y + bottomHeight);
    this->addChild(_topLayer,-1);

    // �����ײ�����
    _bottomLayer = LayerColor::create(Color4B(230, 255, 220, 255), visibleSize.width, bottomHeight);
    _bottomLayer->setPosition(origin.x, origin.y);
    this->addChild(_bottomLayer,0);


    // ��� Home ��ť
    auto homeButton = ui::Button::create(
        "home_normal.png",
        "home_selected.png"
    );
    homeButton->setPosition(Vec2(Director::getInstance()->getWinSize().width - 120,
        Director::getInstance()->getWinSize().height - 120));
    homeButton->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            CCLOG("Current Level Number: %d", this->_levelNumber);
            CCLOG("Back to menu");
            // �������뵭�������л�Ч������������˵���
            auto scene = LevelSelectScene::createScene();
            auto transition = TransitionFade::create(0.5f, scene);

            Director::getInstance()->replaceScene(transition);
        }
        });
    this->addChild(homeButton,10);

    return true;
}