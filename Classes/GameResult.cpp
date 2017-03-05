//
//  GameResult.cpp
//  mockups
//
//  Created by 榎田直哉 on 2015/12/30.
//
//

#include "GameResult.h"

#include "Constants.h"

USING_NS_CC;

// ---------- 定数
static  cocos2d::Color3B BG_COLOR{ cocos2d::Color3B(27,186,202)};

// ---------- クラスメソッド
// create関数
GameResult* GameResult::create(bool finished,int score, int maxScore)
{
    GameResult* p = new GameResult();
    if (p && p->init(finished,score, maxScore)) { p->autorelease(); }
    else { CC_SAFE_DELETE(p); }
    
    return p;
}

// ---------- インスタンスメソッド
// コンストラクタ
GameResult::GameResult() {}

// デストラクタ
GameResult::~GameResult() {}

// 初期化
bool GameResult::init(bool finished, int score, int maxScore)
{
    if (!Sprite::init()) return false;

    Size winSize { Director::getInstance()->getWinSize()};
    this->setTextureRect(cocos2d::Rect(0,0,winSize.width,400));
    this->setContentSize(Size(winSize.width, 400));
    this->setAnchorPoint(cocos2d::Point::ANCHOR_MIDDLE);
    this->setCascadeColorEnabled(false);
    this->setCascadeOpacityEnabled(true);
    this->setColor(BG_COLOR);
   
    // タイトル
    cocos2d::Label* title { cocos2d::Label::createWithSystemFont((finished)?"Finish!":"Game Over!", FONT_NAME, 50) };
    title->setColor(cocos2d::Color3B::WHITE);
    title->setAnchorPoint(Point::ANCHOR_MIDDLE);
    title->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height - 60));
    this->addChild(title);
    
    // 得点
    cocos2d::Label* scoreLabel {cocos2d::Label::createWithSystemFont("score:" + cocos2d::StringUtils::format("%d",score), FONT_NAME, 50)};
    scoreLabel->setColor(cocos2d::Color3B(13,86,95));
    scoreLabel->setAnchorPoint(Point::ANCHOR_MIDDLE);
    scoreLabel->setPosition(this->getContentSize()/2);
    scoreLabel->setPositionY(scoreLabel->getPositionY() + scoreLabel->getContentSize().height/2 + 5);
    this->addChild(scoreLabel);

    // 最高得点
    cocos2d::Label* maxScoreLabel { cocos2d::Label::createWithSystemFont("best:" + cocos2d:: StringUtils::format("%d",maxScore), FONT_NAME, 50)};
    maxScoreLabel->setColor(cocos2d::Color3B(13,86,95));
    maxScoreLabel->setAnchorPoint(Point::ANCHOR_MIDDLE);
    maxScoreLabel->setPosition(this->getContentSize()/2);
    maxScoreLabel->setPositionY(maxScoreLabel->getPositionY() - maxScoreLabel->getContentSize().height/2 - 5);
    this->addChild(maxScoreLabel);

    // メニューを作成
    Menu* menu { Menu::create() };
    menu->setContentSize(this->getContentSize());
    menu->setPosition(Point::ZERO);
    this->addChild(menu);
    
    // リトライボタン
    MenuItemSprite* retryButton { MenuItemSprite::create(Sprite::create("retry.png"), Sprite::create("retry.png"), CC_CALLBACK_1(GameResult::onTouchRetryButton, this)) };
    retryButton->setAnchorPoint(Point::ANCHOR_MIDDLE);
    retryButton->setPosition(Vec2(this->getContentSize().width/2, 50));
    retryButton->setScale(6);
    menu->addChild(retryButton);
    this->retryButton = retryButton;
    
    return true;
}

// ホームボタン選択時
void GameResult::onTouchHomeButton(Ref* _)
{
    if (!this->onTouchHomeCallBack)return;
    
    this->onTouchHomeCallBack();
}

// リトライボタン選択時
void GameResult::onTouchRetryButton(Ref* _)
{
    if (!this->onTouchRetryCallBack)return;
    
    this->onTouchRetryCallBack();
}

// ホームボタン選択時のコールバック設定
void GameResult::setOnTouchHomeCallBack(const CallBack& cb)
{
    this->onTouchHomeCallBack = cb;
   
}

// リトライボタン選択時のコールバック設定
void GameResult::setOnTouchRetryCallBack(const CallBack& cb)
{
    this->onTouchRetryCallBack = cb;
}

