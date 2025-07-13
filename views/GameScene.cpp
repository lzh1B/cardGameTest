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

    // ���㶥���͵ײ��߶�
    const float totalRatio = 150 + 58;
    float topHeight = visibleSize.height * (150 / totalRatio);
    float bottomHeight = visibleSize.height * (58 / totalRatio);

    // ������������
    _topLayer = LayerColor::create(Color4B(200, 230, 255, 255), visibleSize.width, topHeight);
    _topLayer->setPosition(origin.x, origin.y + bottomHeight);
    this->addChild(_topLayer, -1);

    // �����ײ�����
    _bottomLayer = LayerColor::create(Color4B(230, 255, 220, 255), visibleSize.width, bottomHeight);
    _bottomLayer->setPosition(origin.x, origin.y);
    this->addChild(_bottomLayer, 0);

    // ��� Home ��ť
    auto homeButton = ui::Button::create("home_normal.png", "home_selected.png");
    homeButton->setPosition(Vec2(visibleSize.width - 120, visibleSize.height - 120));
    homeButton->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            this->onHomeButtonClicked();
        }
        });
    this->addChild(homeButton, 10);

    // ������Ƭ����
    this->createCardSprites();

    // ע�ᴥ���¼�������
    this->registerTouchEventHandlers();
}

// ������Ƭ����Ķ�������
void GameScene::createCardSprites() {
    // Playfield ��Ƭ
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

    // Stack ��Ƭ
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

// ע�ᴥ���¼��������Ķ�������
void GameScene::registerTouchEventHandlers() {
    // ����ͼ�㴥���¼�
    auto topTouchListener = EventListenerTouchOneByOne::create();
    topTouchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTopLayerTouched, this);
    topTouchListener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(topTouchListener, _topLayer);

    // �ײ�ͼ�㴥���¼�
    auto bottomTouchListener = EventListenerTouchOneByOne::create();
    bottomTouchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onBottomLayerTouched, this);
    bottomTouchListener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(bottomTouchListener, _bottomLayer);
}

// ����ͼ�㴥��������
bool GameScene::onTopLayerTouched(Touch* touch, Event* event) {
    Vec2 touchLocation = touch->getLocation();
    if (_topLayer->getBoundingBox().containsPoint(touchLocation)) {
        CCLOG("Touched Top Layer");
        // ��Ӷ���ͼ�����еĴ����߼�
        this->handleTopLayerTouch(touchLocation);
        return true; // �̵��¼�
    }
    return false;
}

// �ײ�ͼ�㴥��������
bool GameScene::onBottomLayerTouched(Touch* touch, Event* event) {
    Vec2 touchLocation = touch->getLocation();
    if (_bottomLayer->getBoundingBox().containsPoint(touchLocation)) {
        CCLOG("Touched Bottom Layer");
        // ��ӵײ�ͼ�����еĴ����߼�
        this->handleBottomLayerTouch(touchLocation);
        return true; // �̵��¼�
    }
    return false;
}

// ����ͼ�����ҵ���߼�
void GameScene::handleTopLayerTouch(const Vec2& position) {
    // ʵ�ֶ���ͼ��ľ���ҵ���߼�
    // ���磺����Ƭ��������Ϸ���Ƶ�
    CCLOG("Top layer touch at position: (%.1f, %.1f)", position.x, position.y);
}

// �ײ�ͼ�����ҵ���߼�
void GameScene::handleBottomLayerTouch(const Vec2& position) {
    // ʵ�ֵײ�ͼ��ľ���ҵ���߼�
    // ���磺����ѵ���Ƭ�����������
    CCLOG("Bottom layer touch at position: (%.1f, %.1f)", position.x, position.y);
}

// Home ��ť�������
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