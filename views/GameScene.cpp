// GameScene.cpp
#include "GameScene.h"
#include "ui/CocosGUI.h"
#include "LevelSelectScene.h"
#include "utils/JsonUtils.h"

USING_NS_CC;

Scene* GameScene::createScene(int levelNumber) {
    auto scene = Scene::create();        // 创建空场景
    // 从 JsonUtils 获取数据
    auto playfieldCards = JsonUtils::parsePlayfield(levelNumber);

    auto stackCards = JsonUtils::parseStack(levelNumber);
    GameController* controller = new GameController(playfieldCards,stackCards);

    auto layer = GameScene::create();   // 调用 CREATE_FUNC 提供的 create()

    // 构造 controller 并保存数据
    layer->setController(controller);
    layer->_levelNumber = levelNumber;  // 设置关卡号
    log("level number is : xxxxx %d", layer->_levelNumber);
    layer->createUI();
    scene->addChild(layer);             // 添加 GameScene 层到场景中
    return scene;
}

bool GameScene::init() {
    if (!Scene::init()) {
        return false;
    }
    return true;
}
//绘制界面
void GameScene::createUI() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    const float totalRatio = 150 + 58;
    float topHeight = visibleSize.height * (150 / totalRatio);
    float bottomHeight = visibleSize.height * (58 / totalRatio);
    // 创建顶部区域
    _topLayer = LayerColor::create(Color4B(200, 230, 255, 255), visibleSize.width, topHeight);
    _topLayer->setPosition(origin.x, origin.y + bottomHeight);
    this->addChild(_topLayer, -1);

    // 创建底部区域
    _bottomLayer = LayerColor::create(Color4B(230, 255, 220, 255), visibleSize.width, bottomHeight);
    _bottomLayer->setPosition(origin.x, origin.y);
    this->addChild(_bottomLayer, 0);


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
    this->addChild(homeButton, 10);

    // ==== 创建 Playfield 卡片精灵 ====
    auto cards = _controller->getPlayfieldCards(); // 假设你已实现 getPlayfieldCards()
    for (const auto& cardData : *cards) {
        auto cardSprite = Sprite::create("res/card_general.png");
        if (cardSprite) {
            cardSprite->setPosition(cardData.posX, cardData.posY+500);
            this->addChild(cardSprite);
            _playfieldSprites.push_back(cardSprite);
        }
        else {
            CCLOG("Error: Failed to create card sprite at (%d, %d)", cardData.posX, cardData.posY);
        }
    }

    // ==== 添加触摸监听器 ====

   // Top Layer 点击监听
    auto topTouchListener = EventListenerTouchOneByOne::create();
    topTouchListener->onTouchBegan = [this](Touch* touch, Event* event) {
        Vec2 touchLocation = touch->getLocation();
        if (_topLayer->getBoundingBox().containsPoint(touchLocation)) {
            CCLOG("Touched Top Layer");
            return true; // 吞掉事件
        }
        return false;
        };
    topTouchListener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(topTouchListener, _topLayer);

    // Bottom Layer 点击监听
    auto bottomTouchListener = EventListenerTouchOneByOne::create();
    bottomTouchListener->onTouchBegan = [this](Touch* touch, Event* event) {
        Vec2 touchLocation = touch->getLocation();
        if (_bottomLayer->getBoundingBox().containsPoint(touchLocation)) {
            CCLOG("Touched Bottom Layer");
            return true; // 吞掉事件
        }
        return false;
        };
    bottomTouchListener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(bottomTouchListener, _bottomLayer);
}
GameScene::~GameScene() {
    CC_SAFE_DELETE(_controller); // 析构：输出 "GameController Destroyed"
}
// GameScene.cpp
void GameScene::setController(GameController* controller) {
    //CC_SAFE_DELETE(_controller); // 安全删除旧对象
    _controller = controller;
}