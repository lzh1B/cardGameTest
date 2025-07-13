// JsonUtils.h
#pragma once
#include "cocos2d.h"
#include <vector>
#include <string>
#include "models/CardData.h"


class JsonUtils {
public:
    // ���� playfield ����
    static std::vector<CardData> parsePlayfield(int levelNumber);

    // ���� stack ����
    static std::vector<CardData> parseStack(int levelNumber);

    // ��ӡ��Ƭ���ݣ�֧��ǰ׺���֣�
    static void printCards(const std::vector<CardData>& cards, const std::string& prefix = "");
};