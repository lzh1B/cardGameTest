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


    // ��ӱ���ɫ
    auto background = LayerColor::create(Color4B(200, 200, 200, 255));
    this->addChild(background);

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
    this->addChild(homeButton);

    return true;
}