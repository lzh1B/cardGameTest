#pragma once
#include "cocos2d.h"
#include "OperationType.h"

class CardView;

struct OperationRecord {
    OperationType type;
    CardView* card1;    // �����漰�Ŀ���1
    CardView* card2;    // �����漰�Ŀ���2 (DESK_TO_HANDʱΪ������)
    cocos2d::Vec2 originalPosition; // ԭʼλ��
    bool wasTopCard;    // ����ǰ�Ƿ��Ƕ�����
    bool wasVisible;   // ����ǰ�Ƿ�ɼ�
};
