#pragma once
#include "cocos2d.h"
#include <vector>
#include "models/CardData.h"
#include "utils/FileNameLoadUtil.h"

class CardView : public cocos2d::Sprite {

public:
	static CardView* create(CardData cardData);
	bool initWithModel(CardData cardData);

	// 设置是否为顶部牌
	void setIsTopCard(bool isTop) { _isTopCard = isTop; }

	// 获取是否为顶部牌
	bool isTopCard() const { return _isTopCard; }

	int getPosX() { return _cardData.posX; }

	int getPosY() { return _cardData.posY; }

private:
	bool _isTopCard = false; // 是否为顶部牌
	CardData _cardData;
	std::vector<std::string> _pixResList;
	void createCard();
	std::vector<std::string> _getTextureNameForCard();

};