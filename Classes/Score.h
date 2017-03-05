//
//  Score.hpp
//  mockups
//
//  Created by 榎田直哉 on 2015/12/30.
//
//

#ifndef __Score_H__
#define __Score_H__

#include "ui/UIScale9Sprite.h"

//
// 得点を表示
//
class Score final : public cocos2d::Node
{
// ---------- クラスメソッド
public:
    static Score* create(); // create関数
    
// ---------- インスタンス変数
private:
    cocos2d::Label* scoreLabel{nullptr};
    
// ---------- インスタンスメソッド
public:
    Score();                 // コンストラクタ
    ~Score();                // デストラクタ
    bool init();                  // 初期化
    void updateScore(int score);  // スコアを更新
};


#endif // __Score_H__
