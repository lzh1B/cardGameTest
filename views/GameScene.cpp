// GameScene.cpp
#include "GameScene.h"
#include "ui/CocosGUI.h"
#include "LevelSelectScene.h"
#include "utils/JsonUtils.h"

USING_NS_CC;

Scene* GameScene::createScene(int levelNumber) {
    auto scene = Scene::create();        // �����ճ���
    // �� JsonUtils ��ȡ����
    auto playfieldCards = JsonUtils::parsePlayfield(levelNumber);

    auto stackCards = JsonUtils::parseStack(levelNumber);
    GameController* controller = new GameController(playfieldCards,stackCards);

    auto layer = GameScene::create();   // ���� CREATE_FUNC �ṩ�� create()

    // ���� controller ����������
    layer->setController(controller);
    layer->_levelNumber = levelNumber;  // ���ùؿ���
    log("level number is : xxxxx %d", layer->_levelNumber);
    layer->createUI();
    scene->addChild(layer);             // ��� GameScene �㵽������
    return scene;
}

bool GameScene::init() {
    if (!Scene::init()) {
        return false;
    }
    return true;
}
//���ƽ���
void GameScene::createUI() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

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
    this->addChild(homeButton, 10);

    // ==== ���� Playfield ��Ƭ���� ====
    auto cards = _controller->getPlayfieldCards(); // ��������ʵ�� getPlayfieldCards()
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

    // ==== ��Ӵ��������� ====

   // Top Layer �������
    auto topTouchListener = EventListenerTouchOneByOne::create();
    topTouchListener->onTouchBegan = [this](Touch* touch, Event* event) {
        Vec2 touchLocation = touch->getLocation();
        if (_topLayer->getBoundingBox().containsPoint(touchLocation)) {
            CCLOG("Touched Top Layer");
            return true; // �̵��¼�
        }
        return false;
        };
    topTouchListener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(topTouchListener, _topLayer);

    // Bottom Layer �������
    auto bottomTouchListener = EventListenerTouchOneByOne::create();
    bottomTouchListener->onTouchBegan = [this](Touch* touch, Event* event) {
        Vec2 touchLocation = touch->getLocation();
        if (_bottomLayer->getBoundingBox().containsPoint(touchLocation)) {
            CCLOG("Touched Bottom Layer");
            return true; // �̵��¼�
        }
        return false;
        };
    bottomTouchListener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(bottomTouchListener, _bottomLayer);
}
GameScene::~GameScene() {
    CC_SAFE_DELETE(_controller); // ��������� "GameController Destroyed"
}
// GameScene.cpp
void GameScene::setController(GameController* controller) {
    //CC_SAFE_DELETE(_controller); // ��ȫɾ���ɶ���
    _controller = controller;
}