//
//  SelectFrame.hpp
//  mockups
//
//  Created by 榎田直哉 on 2015/12/30.
//
//

#ifndef __SelectFrame_H__
#define __SelectFrame_H__

#include "ui/UIScale9Sprite.h"
//
// 選択枠
//
class SelectFrame final : public cocos2d::Sprite
{
// ---------- クラスメソッド
public:
    static SelectFrame* create(const cocos2d::Size& size); // create関数

// ---------- インスタンスメソッド
public:
    SelectFrame();                          // コンストラクタ
    ~SelectFrame();                         // デストラクタ
    bool init(const cocos2d::Size &size); // 初期化
};

#endif // __SelectFrame_H__
