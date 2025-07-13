// GameScene.cpp
#include "GameScene.h"
#include "ui/CocosGUI.h"
#include "LevelSelectScene.h"

USING_NS_CC;

Scene* GameScene::createScene(int levelNumber) {
    auto scene = Scene::create();        // 创建空场景
    auto layer = GameScene::create();   // 调用 CREATE_FUNC 提供的 create()
    layer->_levelNumber = levelNumber;  // 设置关卡号
    log("level number is : xxxxx %d", layer->_levelNumber);
    scene->addChild(layer);             // 添加 GameScene 层到场景中
    return scene;
}

bool GameScene::init() {
    if (!Scene::init()) {
        return false;
    }


    // 添加背景色
    auto background = LayerColor::create(Color4B(200, 200, 200, 255));
    this->addChild(background);

    // 添加 Home 按钮
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
            // 创建淡入淡出动画切换效果（渐变回主菜单）
            auto scene = LevelSelectScene::createScene();
            auto transition = TransitionFade::create(0.5f, scene);

            Director::getInstance()->replaceScene(transition);
        }
        });
    this->addChild(homeButton);

    return true;
}