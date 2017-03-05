//
//  Score.cpp
//  mockups
//
//  Created by 榎田直哉 on 2015/12/30.
//
//

#include "Score.h"

#include "Constants.h"

USING_NS_CC;


// ---------- クラスメソッド
// create関数
Score* Score::create()
{
    Score* p = new Score();
    if (p && p->init()) { p->autorelease(); }
    else { CC_SAFE_DELETE(p); }
    
    return p;
}

// ---------- インスタンスメソッド
// コンストラクタ
Score::Score() {}

// デストラクタ
Score::~Score() {}

// 初期化
bool Score::init()
{
    if (!cocos2d::Node::init()) return false;
    this->setContentSize(Size(60, 50));
    this->setColor(Color3B::BLUE);
    
    cocos2d::Label* scoreLabel { cocos2d::Label::createWithSystemFont("0", FONT_NAME, 30)};
    scoreLabel->setColor(Color3B(33, 33, 33));
    scoreLabel->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    scoreLabel->setPosition(this->getContentSize()/2);
    this->addChild(scoreLabel);
    this->scoreLabel = scoreLabel;
    this->setAnchorPoint(Point::ANCHOR_MIDDLE_RIGHT);

    return true;
}

// スコアを更新
void Score::updateScore(int score)
{
    this->scoreLabel->setString(cocos2d::StringUtils::format("%d",score));
}
