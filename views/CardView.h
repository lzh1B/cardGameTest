#pragma once
#include "cocos2d.h"
#include <vector>
#include "models/CardData.h"
#include "utils/FileNameLoadUtil.h"

class CardView : public cocos2d::Sprite {

public:
	static CardView* create(CardData cardData);
	bool initWithModel(CardData cardData);

	// �����Ƿ�Ϊ������
	void setIsTopCard(bool isTop) { _isTopCard = isTop; }

	// ��ȡ�Ƿ�Ϊ������
	bool isTopCard() const { return _isTopCard; }

	int getPosX() { return _cardData.posX; }

	int getPosY() { return _cardData.posY; }

private:
	bool _isTopCard = false; // �Ƿ�Ϊ������
	CardData _cardData;
	std::vector<std::string> _pixResList;
	void createCard();
	std::vector<std::string> _getTextureNameForCard();

};