#include "JsonUtils.h"
#include "cocos2d.h"
#include "json/document.h"
#include <sstream>
#include <map>

USING_NS_CC;

static std::map<int, std::vector<CardData>> s_cachedPlayfields;
static std::map<int, std::vector<CardData>> s_cachedStacks;

int getIntFromValue(const rapidjson::Value& obj, const std::string& key) {
    auto member = obj.FindMember(key.c_str());
    if (member != obj.MemberEnd() && member->value.IsInt()) {
        return member->value.GetInt();
    }
    CCLOG("Error: Key '%s' not found or not an integer.", key.c_str());
    return 0;
}

int getPosXFromValue(const rapidjson::Value& obj) {
    if (obj.HasMember("Position") && obj["Position"].IsObject()) {
        return getIntFromValue(obj["Position"], "x");
    }
    return 0;
}

int getPosYFromValue(const rapidjson::Value& obj) {
    if (obj.HasMember("Position") && obj["Position"].IsObject()) {
        return getIntFromValue(obj["Position"], "y");
    }
    return 0;
}

void loadAllLevelsFromJson() {
    if (!s_cachedPlayfields.empty() && !s_cachedStacks.empty()) return;

    std::string jsonPath = FileUtils::getInstance()->fullPathForFilename("levelConfig.json");
    std::string jsonData = FileUtils::getInstance()->getStringFromFile(jsonPath);

    rapidjson::Document document;
    document.Parse(jsonData.c_str());

    if (document.HasParseError()) {
        CCLOG("JSON Parse Error in levelConfig.json");
        return;
    }

    for (int i = 1; i <= 6; ++i) {
        std::string playfieldKey = "playfield" + std::to_string(i);
        if (document.HasMember(playfieldKey.c_str()) && document[playfieldKey.c_str()].IsArray()) {
            std::vector<CardData> cards;
            for (const auto& item : document[playfieldKey.c_str()].GetArray()) {
                CardData data;
                data.cardFace = getIntFromValue(item, "CardFace");
                data.cardSuit = getIntFromValue(item, "CardSuit");
                data.posX = getPosXFromValue(item);
                data.posY = getPosYFromValue(item);
                cards.push_back(data);
            }
            s_cachedPlayfields[i] = cards;
        }

        std::string stackKey = "stack" + std::to_string(i);
        if (document.HasMember(stackKey.c_str()) && document[stackKey.c_str()].IsArray()) {
            std::vector<CardData> cards;
            for (const auto& item : document[stackKey.c_str()].GetArray()) {
                CardData data;
                data.cardFace = getIntFromValue(item, "CardFace");
                data.cardSuit = getIntFromValue(item, "CardSuit");
                data.posX = getPosXFromValue(item);
                data.posY = getPosYFromValue(item);
                cards.push_back(data);
            }
            s_cachedStacks[i] = cards;
        }
    }
}

std::vector<CardData>* JsonUtils::parsePlayfield(int levelNumber) {
    loadAllLevelsFromJson();

    auto it = s_cachedPlayfields.find(levelNumber);
    if (it != s_cachedPlayfields.end()) {
        return &it->second;
    }

    CCLOG("Error: Playfield Level %d not found", levelNumber);
    return nullptr;
}

std::vector<CardData>* JsonUtils::parseStack(int levelNumber) {
    loadAllLevelsFromJson();

    auto it = s_cachedStacks.find(levelNumber);
    if (it != s_cachedStacks.end()) {
        return &it->second;
    }

    CCLOG("Error: Stack Level %d not found", levelNumber);
    return nullptr;
}