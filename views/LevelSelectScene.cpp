// LevelSelectScene.cpp
#include "LevelSelectScene.h"
#include "ui/CocosGUI.h"
#include "utils/JsonUtils.h"
#include "GameScene.h"

USING_NS_CC;

Scene* LevelSelectScene::createScene() {
    auto scene = Scene::create();
    auto layer = LevelSelectScene::create();
    scene->addChild(layer);
    return scene;
}

bool LevelSelectScene::init() {
    if (!Scene::init()) {
        return false;
    }

    // ������Ļ�ߴ�Ϊ 1080x2080
    auto winSize = Director::getInstance()->getWinSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // ���ñ�����ɫΪǳɫ
    auto background = LayerColor::create(Color4B(100, 150, 250, 255));
    this->addChild(background);

    // ��ӱ���
    auto titleLabel = Label::createWithTTF("Level Select", "fonts/Marker Felt.ttf", 48);
    titleLabel->setPosition(Vec2(origin.x + winSize.width / 2,
        origin.y + winSize.height - 100));
    this->addChild(titleLabel, 1);

    // �������װ��
    auto star = Sprite::create("star.png");
    if (star) {
        star->setPosition(Vec2(origin.x + winSize.width / 2, origin.y + winSize.height - 300));
        this->addChild(star, 1);
    }

    // ��������ؿ���ť��3�� * 2�� = 6��
    const int cols = 3;
    const int rows = 2;
    const float buttonWidth = 160;
    const float buttonHeight = 80;
    const float spacingX = 100; // ˮƽ���
    const float spacingY = 200; // ��ֱ���

    // ���м�����ʼλ��
    float startX = origin.x + (winSize.width - (cols * buttonWidth + (cols - 1) * spacingX)) / 2+80;
    float startY = origin.y + winSize.height - 600;

    int levelIndex = 1;

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            auto button = ui::Button::create(
                "button_normal.png",
                "button_pressed.png"
            );
            button->setTitleText(std::to_string(levelIndex++));
            button->setTitleFontSize(36);
            button->setPosition(Vec2(startX + col * (buttonWidth + spacingX),
                startY - row * (buttonHeight + spacingY)));
            button->addTouchEventListener([this,levelIndex](Ref* sender, ui::Widget::TouchEventType type) {
                if (type == ui::Widget::TouchEventType::ENDED) {
                    int levelNumber = levelIndex-1;
                    CCLOG("Selected Level: %d", levelNumber);
                    auto playfieldCards = JsonUtils::parsePlayfield(levelNumber);
                    auto stackCards = JsonUtils::parseStack(levelNumber);

                    CCLOG("---------- Playfield %d Cards ----------", levelNumber);
                    JsonUtils::printCards(playfieldCards, "Playfield:");

                    CCLOG("---------- Stack %d Cards ----------", levelNumber);
                    JsonUtils::printCards(stackCards, "Stack:   ");
                    Director::getInstance()->pushScene(
                        TransitionFade::create(0.5f, GameScene::createScene(levelNumber))
                    );
                }
                });
            this->addChild(button);
        }
    }
    return true;
}