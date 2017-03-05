//
//  SelectFrame.cpp
//  mockups
//
//  Created by 榎田直哉 on 2015/12/30.
//
//

#include "SelectFrame.h"

USING_NS_CC;

// ---------- クラスメソッド
// create関数
SelectFrame* SelectFrame::create(const cocos2d::Size & size)
{
    SelectFrame* p = new SelectFrame();
    if (p && p->init(size)) { p->autorelease(); }
    else { CC_SAFE_DELETE(p); }
    
    return p;
}

// ---------- インスタンスメソッド
// コンストラクタ
SelectFrame::SelectFrame() {}

// デストラクタ
SelectFrame::~SelectFrame() {}

// 初期化
bool SelectFrame::init(const cocos2d::Size &size)
{
    if(!Sprite::init())return false;
    this->setTextureRect(cocos2d::Rect(0,0,size.width,size.height));
    this->setContentSize(size);
    this->setAnchorPoint(cocos2d::Point::ANCHOR_MIDDLE);
    this->setCascadeColorEnabled(false);
    this->setCascadeOpacityEnabled(true);
    this->setColor(Color3B(33,216,237));
    this->setOpacity(70);
    
    return true;
}
