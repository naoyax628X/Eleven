//
//  TwelveGameScene.cpp
//  mockups
//
//  Created by 榎田直哉 on 2015/12/27.
//
//

#include "TwelveGameScene.h"

#include "TwelveGameSceneData.h"
#include "AggresiveGage.h"
#include "Grid.h"
#include "Score.h"
#include "GameResult.h"
#include "Macros.h"
#include "Random.h"

USING_NS_CC;

// ---------- インスタンスメソッド
// コンストラクタ
TwelveGameScene::TwelveGameScene(){}

// デストラクタ
TwelveGameScene::~TwelveGameScene(){}

// 初期化
bool TwelveGameScene::init(TwelveGameSceneData* data)
{
    if(!cocos2d::Scene::init()) return false;
    
    // メニューを作成
    Menu* menu { Menu::create() };
    menu->setPosition(Point::ZERO);
    this->addChild(menu,ETOI(ZOrder::UI));
    this->menu = menu;
 
    this->data = data;
    
    // リトライボタン
    MenuItemSprite* retryButton { MenuItemSprite::create(Sprite::create("retry.png"), Sprite::create("retry.png")    , CC_CALLBACK_1(TwelveGameScene::onTouchRetryButton, this)) };
    retryButton->setAnchorPoint(Point::ANCHOR_MIDDLE);
    retryButton->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height - 20));
    retryButton->setScale(2);
    retryButton->setColor(Color3B(33,216,237));
    this->menu->addChild(retryButton);
    this->retryButton = retryButton;
    
    // 背景の設定
    Size winSize { Director::getInstance()->getWinSize()};
    
    LayerColor *black{LayerColor::create(Color4B(56, 56, 56, 240))};
    black->setContentSize(this->getContentSize());
    this->addChild(black);
    
    // UI要素を乗せる背景
    LayerColor *bg{LayerColor::create(Color4B::WHITE)};
    bg->setContentSize(this->getContentSize());
    bg->setOpacity(0);
    this->addChild(bg);
    this->bg = bg;
    
    // ロゴ
    cocos2d::Label* logo { cocos2d::Label::createWithSystemFont("eleven", FONT_NAME, 50)};
    logo->setColor(cocos2d::Color3B(33,216,237));
    logo->setPosition(Vec2(winSize.width/2, winSize.height - 50));
    logo->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    logo->setRotation(-10);
    this->bg->addChild(logo);
    this->logo = logo;
    
    // 得点
    Score *scoreBoard{Score::create()};
    scoreBoard->setPosition(Vec2(winSize.width, winSize.height - 20));
    this->bg->addChild(scoreBoard);
    this->scoreBoard = scoreBoard;
    this->scoreBoard->updateScore(data->getScore());

    // 最高得点
    cocos2d::Label* maxScoreLabel {cocos2d::Label::createWithSystemFont(cocos2d::StringUtils::format("best:%d",data->restoreScore()), FONT_NAME, 25)};
    maxScoreLabel->setColor(cocos2d::Color3B(33,33,33));
    maxScoreLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    maxScoreLabel->setPosition(Point(this->getContentSize().width/2, logo->getPositionY() - 45));
    maxScoreLabel->setRotation(-10);
    this->bg->addChild(maxScoreLabel);
    this->maxSore = maxScoreLabel;
    
    float panelWidth{(winSize.width -GRID_MARGIN * (MAX_GRID_WIDTH+1))/MAX_GRID_WIDTH};
    float panelHeight{panelWidth};
    this->panelSize = Size(panelWidth, panelHeight);
   
    // アグレッシブゲージ
    Size gageSize{winSize.width - GRID_MARGIN *2, BOTTOM_INDENT/3};
    AggresiveGage *gage{AggresiveGage::create(gageSize)};
    gage->setPosition(Point(this->getContentSize().width/2,BOTTOM_INDENT/2));
    this->bg->addChild(gage);
    this->aggressiveGage = gage;
    
    // タッチリスナーの追加
    EventListenerTouchOneByOne* listener { EventListenerTouchOneByOne::create() };
    listener->onTouchBegan = CC_CALLBACK_2(TwelveGameScene::onTouchBegan,this);
    listener->onTouchMoved = CC_CALLBACK_2(TwelveGameScene::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(TwelveGameScene::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(TwelveGameScene::onTouchCancelled, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

// シーン開始トランジション完了時
void TwelveGameScene::onEnterTransitionDidFinish()
{
    // グリッドの配置
    for (int index = 0 ; index < MAX_PANEL_COUNT; index++)
    {
        Grid *grid{ Grid::create(this->panelSize)};
        grid->setPosition(this->calcIndexToPos(index));
        this->bg->addChild(grid);
        this->grids.insert(index, grid);
        
    }
    // グリッド情報更新
    this->updateGridInfo();
   
    // パネルの追加
    this->addRandomPanels(3,[](){});
}

// アップデート
void TwelveGameScene::update(float dt){}

// タッチ開始時
bool TwelveGameScene::onTouchBegan(Touch *touch, Event *event)
{
    if (this->trackingTouchId != DEFAULT_TRACKING_ID) return true;
    this->trackingTouchId = touch->getID();
    
    // パネルのタッチ判定
    int index{this->calcPosToIndex(this->convertToNodeSpace(touch->getLocation()))};
    
    // 範囲外のタッチなら無視
    if (index == ETOI(PanelTouchState::NO_NEW_TOUCH))
    {
        this->onTouchCancelled(touch, event);
        return true;
    }
    
    //　一枚目の選択の場合
    if (this->selectedPanel == nullptr)
    {
        // indexにパネルがないなら無視
        if (!this->panels.at(index)) return true;
        
        // 選択パネルを保持
        this->selectedPanel = this->panels.at(index);
        this->selectedPanel->didSelected();
        return true;
    }
    
    // 選択済みパネルのindex
    int lastIndex{this->calcPosToIndex(this->selectedPanel->getPosition())};
    
    // index間の経路
    vector<int>pathIndexes{this->getPathIndexes(lastIndex, index)};
    
    // 経路がないなら無視
    if (!pathIndexes.size())
    {
        this->didFalse();
        this->selectedPanel = nullptr;
        return true;
    }
    
    // 二枚目のパネルを選択
    if (this->panels.at(index))
    {
        Panel* first{this->selectedPanel};
        Panel* second{this->panels.at(index)};
        
        if (first == second)
        {
            this->selectedPanel = nullptr;
            return true;
        }
        
        // 同じ値なら
        if (first->getNumber() == second->getNumber())
        {
            // 各パネルのindex
            int firstIndex{this->calcPosToIndex(first->getPosition())};
            int secondindex{this->calcPosToIndex(second->getPosition())};
            
            // 通る経路
            vector<Point>poses{};
            for (int idx : pathIndexes)poses.push_back(this->calcIndexToPos(idx));
            
            // 回転方向
            Panel::FlipDirection flipDiretion{this->calcFlipDirection(pathIndexes.at(pathIndexes.size()-2),secondindex)};

            // 一枚目を移動
            this->trackingTouchId = PANEL_MOVE_TRACKING_ID;
            first->move(pathIndexes, poses,[this, first,second,index, firstIndex , secondindex, flipDiretion]()
            {
              // ゲージ変動
              this->aggressiveGage->addAggresiveValue(5, [this, first,second,index,firstIndex,secondindex,flipDiretion](bool over)
              {
                  // Mapから消去
                  this->panels.erase(firstIndex);
                  this->panels.erase(secondindex);
                  
                  // 得点追加
                  this->data->addScore();
                  this->scoreBoard->updateScore(this->data->getScore());
                  
                  // 新規作成
                  this->addNextNumberPanel(first->getNumber(),index,flipDiretion);

                  if (over)
                  {
                      // パネルをシャッフル
                      this->shufflePanels([this](){this->trackingTouchId = DEFAULT_TRACKING_ID;});
                  }
                  else
                  {
                      // ランダムに配置
                      this->addRandomPanels(1,[this](){this->trackingTouchId = DEFAULT_TRACKING_ID;});
                  }
                  
                  // ゲームオーバーかどうか
                  if (this->isGameOver())
                  {
                      // 結果を表示
                      this->showGameResult(false);
                  }
                  
                  this->selectedPanel = nullptr;
                  
                  // 消去
                  second->remove(flipDiretion);
                  first->remove(flipDiretion);
              });
            });
            
        }
        // 違う値なら
        else
        {
            
            // ゲームオーバーかどうか
            if (this->isGameOver())
            {
                // 結果を表示
                this->showGameResult(false);
            }
            
            // 不正解アニメーション
            this->didFalse();
            this->selectedPanel = nullptr;
        }
    }
    // 空のindexを選択
    else
    {
        // Map変更
        this->panels.erase(lastIndex);
        this->panels.insert(index, this->selectedPanel);
        
        // パネルが動く経路
        vector<Point>poses{};
        for (int idx : pathIndexes)poses.push_back(this->calcIndexToPos(idx));
        
        // パネルを移動
        this->trackingTouchId = PANEL_MOVE_TRACKING_ID;
        this->selectedPanel->move(pathIndexes, poses,[this]()
        {
            // ゲージを更新
            this->aggressiveGage->addAggresiveValue(5, [this](bool over)
            {
                if (over)
                {
                    // パネルをシャッフル
                    this->shufflePanels([this](){ this->trackingTouchId = DEFAULT_TRACKING_ID; });
                }
                else
                {
                    // ランダムにパネル追加
                    this->addRandomPanels(2,[this](){this->trackingTouchId = DEFAULT_TRACKING_ID;});
                }
                
                // ゲームオーバーか
                if (this->isGameOver())
                {
                    // 結果を表示
                    this->showGameResult(false);
                }
                
            });
        });
        
        this->selectedPanel = nullptr;
        
        return true;
    }
    
    return true;
}

// タッチ移動時
void TwelveGameScene::onTouchMoved(Touch *touch, Event *event)
{
    if (touch->getID() != this->trackingTouchId) return;
}

// タッチ終了時
void TwelveGameScene::onTouchEnded(Touch *touch, Event *event)
{
    if (this->trackingTouchId == PANEL_MOVE_TRACKING_ID)return;
    
    if (touch->getID() != this->trackingTouchId) return;
    
    this->trackingTouchId = DEFAULT_TRACKING_ID;
}

// タッチキャンセル時
void TwelveGameScene::onTouchCancelled(Touch *touch, Event *event)
{
    if (this->trackingTouchId == PANEL_MOVE_TRACKING_ID)return;
    if (touch->getID() != this->trackingTouchId) return;
    
    this->trackingTouchId = DEFAULT_TRACKING_ID;
    
}

// indexを座標に変換
Point TwelveGameScene::calcIndexToPos(int index)const
{
    if (index >= MAX_PANEL_COUNT || index < 0) return Point::ZERO;
    
    Size panelSize{this->panelSize};
    Size winSize { Director::getInstance()->getWinSize()};
    
    int x{index%MAX_GRID_WIDTH};
    int y{index/MAX_GRID_WIDTH};
    
    float posX{GRID_MARGIN  + panelSize.width/2 + x *(panelSize.width + GRID_MARGIN) };
    float posY{BOTTOM_INDENT+(panelSize.height +GRID_MARGIN)* MAX_GRID_HEIGHT -panelSize.height/2 -(panelSize.height +GRID_MARGIN)* y};
    
    return cocos2d::Point(posX,posY);
}

// 座標をindexへ変換
int TwelveGameScene::calcPosToIndex(const cocos2d::Point& pos)const
{
    Size panelSize{this->panelSize};
    
    for (int index = 0; index < MAX_PANEL_COUNT; index++)
    {
        Point indexPos{this->calcIndexToPos(index)};
        Rect indexRect{Rect(indexPos.x -panelSize.width/2 , indexPos.y - panelSize.height/2, panelSize.width, panelSize.height)};
        if (indexRect.containsPoint(pos))
        {
            return index;
        }
    }
    
    return ETOI(PanelTouchState::NO_NEW_TOUCH);
}

// パネルの回転方向を取得
Panel::FlipDirection TwelveGameScene::calcFlipDirection(int from, int to)
{
    if ((to + ETOI(SideDlt::TOP) == from || to + ETOI(SideDlt::BOTTOM) == from))
    {
        return Panel::FlipDirection::VERTICAL;
    }
    else
    {
       return Panel::FlipDirection::HORIZONTAL;
    }
}

// ランダムにパネルを追加
void TwelveGameScene::addRandomPanels(int n, const CallBack& cb)
{
    // 空白のindex
    vector<int>blanIndexes;
    
    for (int index{0}; index < MAX_PANEL_COUNT; index++)
    {
        if (!this->panels.at(index))
        {
            blanIndexes.push_back(index);
        }
    }
    
    if ((int)blanIndexes.size() < n) n = (int)blanIndexes.size();
    if (blanIndexes.size() == 0)
    {
        return;
    };
    
    // 配置するindex
    Random::shuffle(blanIndexes.begin(), blanIndexes.end());
    
    int maxNum{(this->currentMaxNumber >= 3)?3:this->currentMaxNumber};
   
    // 配置するナンバーの配列
    vector<int>randomNums{ this->data->getRandomNums(maxNum)};
    
    for (int i = 0; i < n; i++)
    {
        int index{blanIndexes.at(i)};
        int number{randomNums.at(i)};
        
        // パネルを生成し配置
        Panel *panel{Panel::create(number, this->panelSize)};
        panel->setPosition(this->calcIndexToPos(index));
        panel->setScale(0);
        this->bg->addChild(panel);
        this->panels.insert(index, panel);
        panel->runAction(ScaleTo::create(Panel::FRIP_DURATION, 1));
    }
 
    this->runAction(Sequence::create(DelayTime::create(Panel::FRIP_DURATION),CallFunc::create(cb),nullptr));
}

// 次の番号のパネルを追加
void TwelveGameScene::addNextNumberPanel(int number, int index, Panel::FlipDirection d)
{
    // numberを一増やす
    number++;
    
    if (number > this->currentMaxNumber)
    {
        this->currentMaxNumber = number;
    }
    
    // パネルを生成し配置
    Panel *panel{Panel::create(number, this->panelSize)};
    panel->setPosition(this->calcIndexToPos(index));
    if (d == Panel::FlipDirection::VERTICAL)
    {
        panel->setScaleY(0);
    }
    else
    {
        panel->setScaleX(0);
    }
    this->bg->addChild(panel);
    this->panels.insert(index, panel);
    panel->runAction(Sequence::create(DelayTime::create(Panel::FRIP_DURATION), ScaleTo::create(Panel::FRIP_DURATION, 1,1),nullptr));
    
    // ゴール判定
    if (number == MAX_PANEL_NUMBER)
    {
        this->runAction(Sequence::create(DelayTime::create(Panel::FRIP_DURATION),CallFunc::create([this]()
        {
            // 結果を表示
            this->showGameResult(true);
        }), nullptr));
    }
}

// パネルをシャッフル
void TwelveGameScene::shufflePanels(const CallBack& cb)
{
    // インデックスをシャッフル
    vector<int>newIndexes;
    for (int i =0 ; i < MAX_PANEL_COUNT; i++)
    {
        newIndexes.push_back(i);
    }
    Random::shuffle(newIndexes.begin(),newIndexes.end());
    
    // パネルの更新
    Map<int, Panel*>newPanels{};
    
    int idx{0};
    for (auto p : this->panels)
    {
        Panel *panel{p.second};
        
        int newIndex{newIndexes.at(idx)};
        
        newPanels.insert(newIndex, panel);
        
        panel->runAction(MoveTo::create(SHUFFLE_DURATION, this->calcIndexToPos(newIndex)));
        idx++;
    }
    
    for (int k : this->panels.keys())
    {
        this->panels.erase(k);
    }
    
    this->panels = newPanels;
    
    this->runAction(Sequence::create(DelayTime::create(Panel::FRIP_DURATION),CallFunc::create(cb), nullptr));
}


// グリッドの隣接情報を更新する
void TwelveGameScene::updateGridInfo(int from, int to)
{
    for (auto g: this->grids)
    {
        Grid *grid{g.second};
        int index{g.first};
       
        // 隣接するindex
        vector<int>adjustantIndexes;
        
        // パネルが存在するindex
        vector<int>panelExistIndexes;
        for (auto p : this->panels)
        {
            // 始点か終点なら除く
            if (p.first == from || p.first == to) continue;
            
            panelExistIndexes.push_back(p.first);
        }
        
        // 上側でない
        if (index >= MAX_GRID_WIDTH &&
            find(panelExistIndexes.begin(), panelExistIndexes.end(), index + int(SideDlt::TOP)) == panelExistIndexes.end())
        {
            adjustantIndexes.push_back(index + ETOI(SideDlt::TOP));
        }
        
        // 下側でない
        if (index < (MAX_PANEL_COUNT - MAX_GRID_WIDTH)&&
            find(panelExistIndexes.begin(), panelExistIndexes.end(), index + int(SideDlt::BOTTOM)) == panelExistIndexes.end() )
        {
            adjustantIndexes.push_back(index + ETOI(SideDlt::BOTTOM));
        }
        
        // 左端でない
        if (index % MAX_GRID_WIDTH != 0 &&
            find(panelExistIndexes.begin(), panelExistIndexes.end(), index + int(SideDlt::LEFT)) == panelExistIndexes.end())
        {
            adjustantIndexes.push_back(index + ETOI(SideDlt::LEFT) );
        }
        
        // 右端でない
        if (index % MAX_GRID_WIDTH != (MAX_GRID_WIDTH - 1) &&
            find(panelExistIndexes.begin(), panelExistIndexes.end(), index + int(SideDlt::RIGHT)) == panelExistIndexes.end())
        {
            adjustantIndexes.push_back(index + ETOI(SideDlt::RIGHT));
        }

        // 隣接情報を設定
        grid->setEdgetTos(adjustantIndexes);
        
        // コストを設定
        vector<int>costTos(grid->getEdgeTos().size(),1);
        grid->setCostTos(costTos);
    }
}

// 最短経路を取得する
vector<int> TwelveGameScene::getPathIndexes(int fromIndex, int toIndex)
{
    // リセット
    for (int index : this->grids.keys())
    {
        auto g {this->grids.at(index)};
        g->setDone(false);
        g->setCost(-1);
    }
    
    // グリッドの隣接情報を更新する
    this->updateGridInfo(fromIndex,toIndex);
    
    // スタートはコスト0
    this->grids.at(toIndex)->setCost(0);
    
    while (true)
    {
        // 確定グリッドを探す
        Grid* doneGrid{nullptr};
        int doneGridIndex{-1};
        
        for ( auto g :this->grids)
        {
            if (g.second->isDone() || g.second->getCost() < 0) continue;
            if (!doneGrid || g.second->getCost() < doneGrid->getCost())
            {
                doneGrid = g.second;
                doneGridIndex = g.first;
            }
        }
        // 確定グリッドがなくなれば終了
        if (!doneGrid)break;
        
        // 確定フラグを立てる
        doneGrid->setDone(true);
        
        // 接続先グリッドの情報を更新
        for (int i = 0; i < doneGrid->getEdgeTos().size() ; i++)
        {
            int to {doneGrid->getEdgeTos().at(i)};
            int cost {doneGrid->getCost() + doneGrid->getCostTos().at(i)};
            
            if (this->grids.at(to)->getCost() < 0 || cost < this->grids.at(to)->getCost())
            {
                this->grids.at(to)->setCost(cost);
                this->grids.at(to)->setEdgeFrom(doneGridIndex);
            }
        }
    }

    // 経路となるindexの配列
    vector<int> path{};
    
    if (this->grids.at(fromIndex)->getCost() <= 0) return path;

    int idx {fromIndex};
    while (true)
    {
        Grid *grid{ this->grids.at(idx)};
        
        int edgeFrom {grid->getEdgeFrom()};
       
        path.push_back(idx);
        
        if (idx == toIndex)break;
        
        idx = edgeFrom;
    }
    
    return path;
}

// ゲームオーバーかどうか返却
bool TwelveGameScene::isGameOver()const
{
    // 空白indexがあればGameoverではない
    for (int i = 0; i < MAX_PANEL_COUNT; i++)
    {
        if (!this->panels.at(i)) return false;
    }

    // 全パネルに対して隣接を調べる
    for (int index =0; index < MAX_PANEL_COUNT; index++ )
    {
        Panel* panel{this->panels.at(index)};
        for (int adjustantIndex: this->getAdjustantIndexes(index))
        {
            Panel* adjustantPanel{this->panels.at(adjustantIndex)};
            if (!adjustantPanel)
            {
                return false;
            }
            if (panel->getNumber() == adjustantPanel->getNumber())
            {
                return false;
            }
        }
    }

    return true;
}

// 隣接したindexを返却
vector<int>TwelveGameScene::getAdjustantIndexes(int index)const
{
    // 隣接するindex
    vector<int>adjustantIndexes;
    
    // 上側でない
    if (index >= MAX_GRID_WIDTH)
    {
        adjustantIndexes.push_back(index + ETOI(SideDlt::TOP));
    }
    
    // 下側でない
    if (index < (MAX_PANEL_COUNT - MAX_GRID_WIDTH))
    {
        adjustantIndexes.push_back(index + ETOI(SideDlt::BOTTOM));
    }
    
    // 左端でない
    if (index % MAX_GRID_WIDTH != 0)
    {
        adjustantIndexes.push_back(index + ETOI(SideDlt::LEFT) );
    }
    
    // 右端でない
    if (index % MAX_GRID_WIDTH != (MAX_GRID_WIDTH - 1))
    {
        adjustantIndexes.push_back(index + ETOI(SideDlt::RIGHT));
    }
    
    return adjustantIndexes;
}

// 結果を表示
void TwelveGameScene::showGameResult(bool finished)
{
    LayerColor *bg {LayerColor::create(Color4B::BLACK)};
    bg->setContentSize(this->getContentSize());
    bg->setOpacity(0);
    bg->setPosition(Point::ZERO);
    this->bg->addChild(bg,100);
    bg->runAction(FadeTo::create(0.3, 150));
    this->retryButton->setVisible(false);
    
    GameResult *result{ GameResult::create(finished,this->data->getScore(), this->data->restoreScore())};
    result->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    result->setOnTouchRetryCallBack([this](){ this->onTouchRetryButton(nullptr);});
    
    result->setScaleY(0);
    
    float t{0.3};
    this->addChild(result);
    result->runAction(Sequence::create(ScaleTo::create(t, 1,1),CallFunc::create([this]()
    {
        // 記録を更新したなら得点を保存
        if (this->data->isBreakMaxScore())
        {
            this->data->saveScore();
        }
        this->trackingTouchId = GAME_OVER_TRACKING_ID;
        
        
    }), nullptr));
    
    for (auto g : this->grids)
    {
        g.second->runAction(MoveBy::create(t, Point(0,(g.first < MAX_GRID_WIDTH*2)?result->getContentSize().height/2:-result->getContentSize().height/3 )));
    }
    
    for (auto p : this->panels)
    {
          p.second->runAction(MoveBy::create(t, Point(0,(p.first < MAX_GRID_WIDTH*2)?result->getContentSize().height/2:-result->getContentSize().height/3 )));
    }
    
    this->aggressiveGage->runAction(MoveBy::create(t, Point(0, -result->getContentSize().height/3)));;
    this->logo->runAction(MoveBy::create(t, Point(0,result->getContentSize().height/2)));
    this->maxSore->runAction(MoveBy::create(t, Point(0,result->getContentSize().height/2)));
    this->scoreBoard->runAction(MoveBy::create(t, Point(0,result->getContentSize().height/2)));
}

// 不正解
void TwelveGameScene::didFalse()
{
    if (this->selectedPanel)
    {
        this->selectedPanel->removeChildByTag(Panel::FRAME_TAG);
    }
    Point orgPos{this->bg->getPosition()};
    float t {0.05};
    Vec2 dlt{6,6};
    this->bg->runAction(Sequence::create(MoveBy::create(t, -dlt),MoveBy::create(t, dlt*2),MoveBy::create(t, -dlt*2+dlt/3),MoveBy::create(t,dlt -dlt/5),MoveTo::create(t, orgPos),nullptr));
}

// リトライボタンを押した時
void TwelveGameScene::onTouchRetryButton(cocos2d::Ref *_)
{
   Director::getInstance()->replaceScene(TwelveGameScene::create(TwelveGameSceneData::create()));
}
