//
//  GameResult.hpp
//  mockups
//
//  Created by 榎田直哉 on 2015/12/30.
//
//

#ifndef __GameResult_H__
#define __GameResult_H__

#include "ui/UIScale9Sprite.h"

//
// ゲーム結果を表示
//
class GameResult final : public cocos2d::Sprite
{
// ---------- 定数
public:
    using  CallBack = std::function<void()>;
    
    
// ---------- インスタンス変数
private:
    cocos2d::MenuItemSprite* homeButton  { nullptr }; // ホームボタン
    cocos2d::MenuItemSprite* retryButton { nullptr }; // リトライボタン
    
    CallBack onTouchHomeCallBack         { nullptr }; // ホームボタン選択時のコールバック
    CallBack onTouchRetryCallBack        { nullptr }; // ホームボタン選択時のコールバック
    
// ---------- クラスメソッド
public:
    static GameResult* create(bool finished,int score, int maxScore); // create関数
    
// ---------- インスタンスメソッド
public:
    GameResult();                                        // コンストラクタ
    ~GameResult();                                       // デストラクタ
    bool init(bool finished, int score, int maxScore);   // 初期化
   
    void onTouchHomeButton(Ref* _);                      // ホームボタン選択時
    void setOnTouchRetryCallBack(const CallBack&cb);  // ホームボタン選択時のコールバック設定
    
    void onTouchRetryButton(Ref* _);                  // リトライボタン選択時
    void setOnTouchHomeCallBack(const CallBack&cb);   // リトライボタン選択時のコールバック設定
};


#endif // __GameResult_H__

