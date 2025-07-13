// JsonUtils.h
#pragma once
#include "cocos2d.h"
#include <vector>
#include <string>

struct CardData {
    int cardFace;
    int cardSuit;
    int posX;
    int posY;
};

class JsonUtils {
public:
    // 解析 playfield 数据
    static std::vector<CardData> parsePlayfield(int levelNumber);

    // 解析 stack 数据
    static std::vector<CardData> parseStack(int levelNumber);

    // 打印卡片数据（支持前缀区分）
    static void printCards(const std::vector<CardData>& cards, const std::string& prefix = "");
};