#include "views/CardView.h"
USING_NS_CC;

CardView* CardView::create(CardData cardData) {
    CardView* pRet = new(std::nothrow) CardView();
    if (pRet && pRet->initWithModel(cardData)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);

    return nullptr;
}
bool CardView::initWithModel(CardData cardDatd) {
    if (!Sprite::init()) {
        return false;
    }
	_cardData = cardDatd;
	_pixResList = _getTextureNameForCard();
    createCard();
    return true;
}
std::vector<std::string> CardView::_getTextureNameForCard() {
	// ���ݿ�������������������
	// ʵ����Ŀ��Ӧ���ݿ������ͷ��ض�ӦͼƬ·��

	return FileNameLoadUtil::fileNameLoad(_cardData);
}
//��������
void CardView::createCard() {
	//������Ⱦ����
	const float CARD_WIDTH = 182.0f;
	const float CARD_HEIGHT = 282.0f;
	this->setContentSize(Size(CARD_WIDTH, CARD_HEIGHT));
	//���Ʊ���
	if (!_pixResList.empty()) {
		auto background = Sprite::create(_pixResList[0]);
		if (background) {
			background->setAnchorPoint(Vec2::ZERO);
			background->setPosition(0, 0); // ���ĵ�
			background->setContentSize(Size(CARD_WIDTH, CARD_HEIGHT));
			this->addChild(background, 0);
		}
	}

	// 2. ���ƻ�ɫ�����Ͻǣ�
	if (_pixResList.size() > 1) {
		auto suitSprite = Sprite::create(_pixResList[1]);
		if (suitSprite) {
			suitSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
			suitSprite->setPosition(15, 245); // ���Ͻ�λ��
			this->addChild(suitSprite, 1);
		}
	}

	// 3. ���ƴ��棨�м䣩
	if (_pixResList.size() > 2) {
		auto bigFace = Sprite::create(_pixResList[2]);
		if (bigFace) {
			bigFace->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			bigFace->setPosition(CARD_WIDTH / 2, CARD_HEIGHT / 2 - 30); // ����λ��
			this->addChild(bigFace, 2);
		}
	}

	// 4. ����С���棨���Ͻǣ�
	if (_pixResList.size() > 3) {
		auto smallFace = Sprite::create(_pixResList[3]);
		if (smallFace) {
			smallFace->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
			smallFace->setPosition(CARD_WIDTH - 25, 245); // ���Ͻ�λ��
			this->addChild(smallFace, 3);
		}
	}
}