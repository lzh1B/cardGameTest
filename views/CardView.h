#pragma once
#include "cocos2d.h"
#include <vector>
#include "models/CardData.h"
#include "utils/FileNameLoadUtil.h"

class CardView : public cocos2d::Sprite {

public:
	static CardView* create(CardData cardData);
	bool initWithModel(CardData cardData);

private:
	CardData _cardData;
	std::vector<std::string> _pixResList;
	void createCard();
	std::vector<std::string> _getTextureNameForCard();

};