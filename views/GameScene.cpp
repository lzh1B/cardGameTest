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
    // ��ʼ�� controller
    _controller = new GameController(); // ���죺��� "GameController Created"
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


    return true;
}
GameScene::~GameScene() {
    CC_SAFE_DELETE(_controller); // ��������� "GameController Destroyed"
}