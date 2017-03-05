//
//  TwelveGameScene.hpp
//  mockups
//
//  Created by 榎田直哉 on 2015/12/27.
//
//

#ifndef _TwelveGameScene__
#define _TwelveGameScene__

#include "Panel.h"
#include "Macros.h"

class AggresiveGage;
class Grid;
class Score;
class TwelveGameSceneData;

class TwelveGameScene : public cocos2d::Scene
{
// ---------- 定数
private:
   
    // 隣接indexの差分
    enum class SideDlt
    {
        TOP   = -4,
        RIGHT =  1,
        BOTTOM= +4,
        LEFT  = -1,
    };
    
    // パネルのタッチ状態
    enum class PanelTouchState
    {
        NO_NEW_TOUCH = -1,
    };
    
    using  CallBack = std::function<void()>;
    
    static constexpr int MAX_PANEL_NUMBER  {11 };                               // パネルの最大ナンバー
    static constexpr int MAX_GRID_WIDTH    { 4 };                               // グリッドの横のマス数
    static constexpr int MAX_GRID_HEIGHT   { 5 };                               // グリッドの縦のマス数
    static constexpr float GRID_MARGIN     {20.f};                              // グリッド間のマージン
    static constexpr int MAX_PANEL_COUNT   {MAX_GRID_WIDTH * MAX_GRID_HEIGHT};  // 最大パネル数
    static constexpr float BOTTOM_INDENT   {60.f};                              // 画面下のスペース
    static constexpr float SHUFFLE_DURATION{0.3f};                              // シャッフル時間
    static const int DEFAULT_TRACKING_ID   {-999};                              // デフォルトトラッキングID
    static const int PANEL_MOVE_TRACKING_ID{-1000};                             // パネル移動のトラッキングID
    static const int GAME_OVER_TRACKING_ID {-1001};                             // ゲーム終了後のトラッキングID

// ---------- クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(TwelveGameScene, TwelveGameSceneData*)
    
// ---------- インスタンス変数
private:
    //
    cocos2d::Menu          * menu       { nullptr }; // メニュー
    cocos2d::MenuItemSprite* backButton { nullptr }; // 戻るボタン
    
    cocos2d::RefPtr<TwelveGameSceneData> data;                        // シーンデータ
    
    cocos2d::LayerColor      *bg              {nullptr};              // 背景
    cocos2d::Label           *logo            {nullptr};              // ロゴ
    cocos2d::Label           *maxSore         {nullptr};              // 最高得点
    Score                    *scoreBoard      {nullptr};              // 得点
    cocos2d::Map<int, Panel*>panels;                                  // パネル
    Panel                    *selectedPanel   {nullptr};              // 選択したパネル
    cocos2d::Map<int, Grid*> grids;                                   // グリッド
    AggresiveGage            *aggressiveGage  {nullptr};              // アグレッシブゲージ
    cocos2d::MenuItemSprite  *retryButton     {nullptr};              // リトライボタン
   
    int                      currentMaxNumber {1};                    // 現在の最大ナンバー
    int                      trackingTouchId  {DEFAULT_TRACKING_ID};  // トラッキング中のタッチID
   
    cocos2d::Size            panelSize        {cocos2d::Size::ZERO};  // パネルサイズ
    
// ---------- インスタンスメソッド
public:
    TwelveGameScene();                          // コンストラクタ
    ~TwelveGameScene();                         // デストラクタ
    bool init(TwelveGameSceneData* data);       // 初期化
    
    void onEnterTransitionDidFinish() override; // シーン開始トランジション完了時
    void update(float dt) override;             // アップデート
    
    // タッチのイベント
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);     // タッチ開始時
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);     // タッチ移動時
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);     // タッチ終了時
    void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event); // タッチキャンセル時
    
    void onTouchRetryButton(Ref* _);                       // リトライボタンを押した時

private:
    int calcPosToIndex(const cocos2d::Point& pos)const;     // 座標をindexへ変換
    cocos2d::Point calcIndexToPos(int index)const;          // indexを座標に変換
    Panel *getPanelAtIndex(int index)const;                 // indexのPanelを取得
    Panel::FlipDirection calcFlipDirection(int from,int to);// パネルの回転方向を取得
   
    void addRandomPanels(int n, const CallBack& cb);                      // ランダムにパネルを追加
    void addNextNumberPanel(int number,int index, Panel::FlipDirection d);// 次の番号のパネルを追加
    void shufflePanels(const CallBack& cb);                               // パネルをシャッフル
    
    std::vector<int>getAdjustantIndexes(int index)const;                  // 隣接したindexを返却
    
    bool isGameOver()const;                                                 // ゲームオーバーかどうか返却
    void showGameResult(bool finished);                                     // 結果を表示
    void didFalse();                                                        // 不正解

    void updateGridInfo(int from = -1, int to = -1);                        // グリッドの隣接情報を更新
    std::vector<int>getPathIndexes(int fromIndex, int toIndex);             // 最短経路を取得する
};

#endif /* defined(_TwelveGameScene__) */
