// GameScene.cpp
#include "GameScene.h"
#include "ui/CocosGUI.h"
#include "LevelSelectScene.h"
#include "utils/JsonUtils.h"
#include "CardView.h"

USING_NS_CC;

Scene* GameScene::createScene(int levelNumber) {
    auto scene = Scene::create();
    auto playfieldCards = JsonUtils::parsePlayfield(levelNumber);
    auto stackCards = JsonUtils::parseStack(levelNumber);
    GameController* controller = new GameController(playfieldCards, stackCards);

    auto layer = GameScene::create();
    layer->setController(controller);
    layer->_levelNumber = levelNumber;
    layer->createUI();

    scene->addChild(layer);
    return scene;
}

bool GameScene::init() {
    if (!Scene::init()) {
        return false;
    }
    return true;
}

void GameScene::createUI() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 计算顶部和底部高度
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
    auto homeButton = ui::Button::create("home_normal.png", "home_selected.png");
    homeButton->setPosition(Vec2(visibleSize.width - 120, visibleSize.height - 120));
    homeButton->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            this->onHomeButtonClicked();
        }
        });
    this->addChild(homeButton, 10);

    // 创建卡片精灵
    this->createCardSprites();

    // 注册触摸事件监听器
    this->registerTouchEventHandlers();
}

// 创建卡片精灵的独立函数
void GameScene::createCardSprites() {
    // Playfield 卡片
    auto deskcards = _controller->getPlayfieldCards();
    for (const auto& cardData : *deskcards) {
        auto cardSprite = CardView::create(cardData);
        if (cardSprite) {
            cardSprite->setPosition(cardData.posX, cardData.posY + 500);
            this->addChild(cardSprite);
            _playfieldSprites.push_back(cardSprite);
        }
        else {
            CCLOG("Error: Failed to create card sprite at (%d, %d)", cardData.posX, cardData.posY);
        }
    }

    // Stack 卡片
    auto handcards = _controller->getStackCards();
    for (const auto& cardData : *handcards) {
        auto cardSprite = CardView::create(cardData);
        if (cardSprite) {
            cardSprite->setPosition(cardData.posX - 100, cardData.posY - 50);
            this->addChild(cardSprite);
            _playfieldSprites.push_back(cardSprite);
        }
        else {
            CCLOG("Error: Failed to create card sprite at (%d, %d)", cardData.posX, cardData.posY);
        }
    }
}

// 注册触摸事件处理器的独立函数
void GameScene::registerTouchEventHandlers() {
    // 顶部图层触摸事件
    auto topTouchListener = EventListenerTouchOneByOne::create();
    topTouchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTopLayerTouched, this);
    topTouchListener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(topTouchListener, _topLayer);

    // 底部图层触摸事件
    auto bottomTouchListener = EventListenerTouchOneByOne::create();
    bottomTouchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onBottomLayerTouched, this);
    bottomTouchListener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(bottomTouchListener, _bottomLayer);
}

// 顶部图层触摸处理函数
bool GameScene::onTopLayerTouched(Touch* touch, Event* event) {
    Vec2 touchLocation = touch->getLocation();
    if (_topLayer->getBoundingBox().containsPoint(touchLocation)) {
        CCLOG("Touched Top Layer");
        // 添加顶部图层特有的处理逻辑
        this->handleTopLayerTouch(touchLocation);
        return true; // 吞掉事件
    }
    return false;
}

// 底部图层触摸处理函数
bool GameScene::onBottomLayerTouched(Touch* touch, Event* event) {
    Vec2 touchLocation = touch->getLocation();
    if (_bottomLayer->getBoundingBox().containsPoint(touchLocation)) {
        CCLOG("Touched Bottom Layer");
        // 添加底部图层特有的处理逻辑
        this->handleBottomLayerTouch(touchLocation);
        return true; // 吞掉事件
    }
    return false;
}

// 顶部图层具体业务逻辑
void GameScene::handleTopLayerTouch(const Vec2& position) {
    // 实现顶部图层的具体业务逻辑
    // 例如：处理卡片操作、游戏控制等
    CCLOG("Top layer touch at position: (%.1f, %.1f)", position.x, position.y);
}

// 底部图层具体业务逻辑
void GameScene::handleBottomLayerTouch(const Vec2& position) {
    // 实现底部图层的具体业务逻辑
    // 例如：处理堆叠卡片、特殊操作等
    CCLOG("Bottom layer touch at position: (%.1f, %.1f)", position.x, position.y);
}

// Home 按钮点击处理
void GameScene::onHomeButtonClicked() {
    CCLOG("Current Level Number: %d", this->_levelNumber);
    CCLOG("Back to menu");

    auto scene = LevelSelectScene::createScene();
    auto transition = TransitionFade::create(0.5f, scene);
    Director::getInstance()->replaceScene(transition);
}

GameScene::~GameScene() {
    CC_SAFE_DELETE(_topLayer);
    CC_SAFE_DELETE(_bottomLayer);
    CC_SAFE_DELETE(_controller);
}

void GameScene::setController(GameController* controller) {
    _controller = controller;
}