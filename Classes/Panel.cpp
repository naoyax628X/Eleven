//
//  Panel.cpp
//  mockups
//
//  Created by 榎田直哉 on 2015/12/27.
//
//

#include "Panel.h"

#include "SelectFrame.h"
#include "Constants.h"

USING_NS_CC;

using namespace std;

// ---------- クラスメソッド
// create関数
Panel* Panel::create(int number, const cocos2d::Size& size)
{
    Panel* p = new Panel();
    if (p && p->init(number,size)) { p->autorelease(); }
    else { CC_SAFE_DELETE(p); }
    
    return p;
}

// ---------- インスタンスメソッド
// コンストラクタ
Panel::Panel() {}

// デストラクタ
Panel::~Panel() {}

// 初期化
bool Panel::init(int number, const cocos2d::Size &size)
{
    if(!Sprite::init())return false;
    this->setTextureRect(cocos2d::Rect(0, 0, size.width, size.height));
    this->setContentSize(size);
    this->setAnchorPoint(cocos2d::Point::ANCHOR_MIDDLE);
    this->setCascadeColorEnabled(false);
    this->setCascadeOpacityEnabled(true);
    this->setColor((number < 0)?cocos2d::Color3B::GRAY:this->getColor(number));

    if (number < 0)return true;
    
    // 数字
    cocos2d::Label* numLabel { cocos2d::Label::createWithSystemFont(cocos2d::StringUtils::format("%d",number), FONT_NAME, 60)};
    numLabel->setColor(cocos2d::Color3B::WHITE);
    numLabel->setAnchorPoint(Vec2(0.5, 0.5));
    numLabel->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
    this->addChild(numLabel);
    this->number = number;
    
    return true;
}

// 番号を取得
int Panel::getNumber()const
{
    return this->number;
}

// 番号色を取得
cocos2d::Color3B Panel::getColor(int number)const
{
    map<int , cocos2d::Color3B> colors
    {
        {1,cocos2d::Color3B(242,159,41)},
        {2,cocos2d::Color3B(233,121,42)},
        {3,cocos2d::Color3B(238,88,44)},
        {4,cocos2d::Color3B(236,49,50)},
        {5,cocos2d::Color3B(230,29,98)},
        {6,cocos2d::Color3B(206,33,141)},
        {7,cocos2d::Color3B(163,65,152)},
        {8,cocos2d::Color3B(114,59,154)},
        {9,cocos2d::Color3B(2,201,116)},
        {10,cocos2d::Color3B(12,175,206)},
        {11,cocos2d::Color3B(191,24,80)},
        {12,cocos2d::Color3B(255,203,1)},
    };
    
    if (colors.count(number) == 0)return cocos2d::Color3B::ORANGE;
    
    return colors.at(number);
}

// 選択時
void Panel::didSelected()
{
    SelectFrame *frame{SelectFrame::create(Size(this->getContentSize().width + 26, this->getContentSize().height + 26))};
    frame->setPosition(Vec2(this->getContentSize().width/2,this->getContentSize().height/2));
    frame->setTag(FRAME_TAG);
    this->addChild(frame);
}

// 指定座標を通って移動
void Panel::move(const vector<int>&indexes, const vector<cocos2d::Point>& poses,const CallBack& cb)
{
    this->stopAllActions();
    this->removeChildByTag(FRAME_TAG);
    
    int gridWidth{4};
    
    // カーブとなる箇所
    vector<int>curves;;
    for (int i = 0; i < indexes.size(); i++)
    {
        if (i < 2) continue;
        if (indexes.at(i)%gridWidth == indexes.at(i-1)%gridWidth && indexes.at(i-1)/gridWidth == indexes.at(i-2)/gridWidth)
        {
            curves.push_back(i-1);
            continue;
        }
        if (indexes.at(i)/gridWidth == indexes.at(i-1)/gridWidth && indexes.at(i-1)%gridWidth == indexes.at(i-2)%gridWidth)
        {
            curves.push_back(i-1);
            continue;
        }
    }
    
    Vector<cocos2d::FiniteTimeAction*>actions;
    float t{MOVE_UNIT_DURATION / 2 / (indexes.size() - 1)  + MOVE_UNIT_DURATION};
    
    for (int i = 0; i < poses.size(); i++)
    {
        cocos2d::Point pos{poses.at(i)};
        
        // カーブ
        if (find(curves.begin(), curves.end(), i) != curves.end())
        {
            Point start{(pos + poses.at(i-1))/2};
            Point end{(pos + poses.at(i+1))/2};
            
            ccBezierConfig config;
            config.controlPoint_1 = (start + pos)/2;
            config.controlPoint_2 = (end + pos)/2;
            config.endPosition    = end;
            actions.pushBack(cocos2d::MoveTo::create(t/3, start));
            actions.pushBack(cocos2d::BezierTo::create(t * 2/3, config));
        }
        // 直線
        else
        {
            bool afterCurve{find(curves.begin(), curves.end(), i-1) != curves.end()};
            actions.pushBack(cocos2d::MoveTo::create((afterCurve)?t/2:t, pos));
        }
    }
    actions.pushBack(CallFunc::create(cb));
    
    this->runAction(Sequence::create(actions));
}

// 削除
void Panel::remove(Panel::FlipDirection direction)
{
    if (direction == FlipDirection::VERTICAL)
    {
         this->runAction(Sequence::create(ScaleTo::create(FRIP_DURATION, 1, 0),RemoveSelf::create(), nullptr));
    }
    else
    {
         this->runAction(Sequence::create(ScaleTo::create(FRIP_DURATION, 0, 1),RemoveSelf::create(), nullptr));
    }
}
