//
//  Grid.cpp
//  mockups
//
//  Created by 榎田直哉 on 2015/12/28.
//
//


#include "Grid.h"

USING_NS_CC;

// ---------- クラスメソッド
// create関数
Grid* Grid::create(const cocos2d::Size& size)
{
    Grid* p = new Grid();
    if (p && p->init(size)) { p->autorelease(); }
    else { CC_SAFE_DELETE(p); }
    
    return p;
}

// ---------- インスタンスメソッド
// コンストラクタ
Grid::Grid() {}

// デストラクタ
Grid::~Grid() {}

// 初期化
bool Grid::init(const cocos2d::Size& size)
{
    Sprite *sprite { Sprite::create("rect.png") };
    
   
    if(!cocos2d::Sprite::init())return false;
    this->setTextureRect(cocos2d::Rect(0,0,size.width,size.height));
 //   if (!cocos2d::ui::Scale9Sprite::init(sprite, cocos2d::Rect(14, 14, 36, 36), cocos2d::Rect::ZERO)) return false;
 //   if (!cocos2d::ui::Scale9Sprite::initWithSpriteFrameName("rect.png", cocos2d::Rect(14, 14, 36, 36))) return false;
    this->setContentSize(size);
    this->setAnchorPoint(cocos2d::Point::ANCHOR_MIDDLE);
    this->setCascadeColorEnabled(false);
    this->setCascadeOpacityEnabled(true);
    this->setColor(Color3B(51,51,51));
    
    return true;
}

// 確定済みかどうかを返却
bool Grid::isDone()const
{
    return this->done;
}

// 確定済みかどうかを設定
void Grid::setDone(bool done)
{
    this->done = done;
}

// コストを取得
int Grid::getCost()const
{
    return this->cost;
}

// コストを設定
void Grid::setCost(int cost)
{
    this->cost = cost;
}

// 隣接indexを取得
const vector<int>& Grid::getEdgeTos()const
{
    return this->edge_tos;
}

// 隣接indexを設定
void Grid::setEdgetTos(const vector<int>& edgeTos)
{
    this->edge_tos = edgeTos;
}

 // 隣接コストを取得
const vector<int>& Grid::getCostTos()const
{
    
    return  this->cost_tos;
}

// 隣接コストを設定
void Grid::setCostTos(const vector<int>& costs)
{
    this->cost_tos = costs;
}

// どこから来たかを取得
int Grid::getEdgeFrom()const
{
    return this->edge_from;
}

// どこから来たかを設定
void Grid::setEdgeFrom(int from)
{
    this->edge_from = from;
}
