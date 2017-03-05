//
//  AggresiveGage.cpp
//  mockups
//
//  Created by 榎田直哉 on 2015/12/28.
//
//

#include "AggresiveGage.h"

USING_NS_CC;

// ---------- クラスメソッド
// create関数
AggresiveGage* AggresiveGage::create(cocos2d::Size size)
{
    AggresiveGage* p = new AggresiveGage();
    if (p && p->init(size)) { p->autorelease(); }
    else { CC_SAFE_DELETE(p); }
    
    return p;
}

// ---------- インスタンスメソッド
// コンストラクタ
AggresiveGage::AggresiveGage() {}

// デストラクタ
AggresiveGage::~AggresiveGage() {}

// 初期化
bool AggresiveGage::init(cocos2d::Size size)
{
    if(!cocos2d::Sprite::init()) return false;
    this->setTextureRect(cocos2d::Rect(0,0,size.width,size.height));
    this->setContentSize(size);
    this->setAnchorPoint(cocos2d::Point::ANCHOR_MIDDLE);
    this->setCascadeColorEnabled(false);
    this->setCascadeOpacityEnabled(true);
    this->setColor(Color3B::BLACK);
    
    // ゲージを作成
    Sprite * gage { Sprite::create()};
    gage->setTextureRect(cocos2d::Rect(0,0,size.width,size.height));
    gage->setContentSize(size);
    gage->setScaleX(0);
    gage->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    gage->setCascadeColorEnabled(false);
    gage->setCascadeOpacityEnabled(true);
    gage->setColor(Color3B(160,226,242));
    gage->setPosition(0, this->getContentSize().height/2);
    this->addChild(gage);
    this->gage = gage;
    
    return true;
}

// ゲージを増やす
void AggresiveGage::addAggresiveValue(int value, CallBack cb)
{
    this->currentValue += value;
  
    float scale {(this->currentValue/AggresiveGage::MAX_AGGRESIVE_VALUE)};
    if (scale > 1) scale = 1;
    
    bool isOver{this->currentValue > AggresiveGage::MAX_AGGRESIVE_VALUE};
    
    this->gage->runAction(Sequence::create(ScaleTo::create(0.2, scale, DEFAULT_INNER_GAGE_SCALE),CallFunc::create([this,isOver]()
    {
        if (isOver)
        {
            this->gage->setScale(0,DEFAULT_INNER_GAGE_SCALE);
        }
        
    }),nullptr));
   
    if (isOver)
    {
        this->currentValue = 0;
    }
    
    cb(isOver);
}
