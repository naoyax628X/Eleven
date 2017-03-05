//
//  AggresiveGage.hpp
//  mockups
//
//  Created by 榎田直哉 on 2015/12/28.
//
//

#ifndef __AggresiveGage_H__
#define __AggresiveGage_H__

#include "ui/UIScale9Sprite.h"
//
// aggresiveゲージ
//
class AggresiveGage final : public cocos2d::Sprite
{
// ---------- 定数
public:
    static constexpr float MAX_AGGRESIVE_VALUE {100};
    static constexpr float DEFAULT_INNER_GAGE_SCALE{0.8};
    
    using  CallBack = std::function<void(bool)>;
    
// ---------- インスタンス変数
private:
    int currentValue            {0};          // 現在のAggresiveValue
    cocos2d::Sprite *gage       {nullptr};    // ゲージ
    CallBack cb                 {nullptr};    // コールバック
    
// ---------- クラスメソッド
public:
    static AggresiveGage* create(cocos2d::Size size); // create関数
    
// ---------- インスタンスメソッド
public:
    AggresiveGage();                                          // コンストラクタ
    ~AggresiveGage();                                         // デストラクタ
    bool init(cocos2d::Size size);                  // 初期化
    void addAggresiveValue(int value,CallBack cb);  // ゲージを増やす
};

#endif // __AggresiveGage_H__

