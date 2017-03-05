//
//  Panel.hpp
//  mockups
//
//  Created by 榎田直哉 on 2015/12/27.
//
//

#ifndef __Panel_H__
#define __Panel_H__


#include "ui/UIScale9Sprite.h"

//
// パネル
//
class Panel final : public cocos2d::Sprite
{
// ---------- 定数
public:
    
    static const int FRAME_TAG               {100};  // 選択枠のタグ
    static constexpr float FRIP_DURATION     {0.10}; // 回転アニメーション時間
    static constexpr float MOVE_UNIT_DURATION{0.06}; // グリッド単位の移動アニメーション時間
    
    using  CallBack = std::function<void()>;              // コールバック
    
    enum class FlipDirection
    {
        VERTICAL,
        HORIZONTAL,
    };
    
// ---------- クラスメソッド
public:
    static Panel* create(int number,const cocos2d::Size& size); // create関数
   
// ---------- インスタンス変数
private:
    int number{0};
    
// ---------- インスタンスメソッド
public:
    Panel();                                            // コンストラクタ
    ~Panel();                                           // デストラクタ
    bool init(int number, const cocos2d::Size&size); // 初期化
    
    int getNumber()const;                               // 番号を取得
    cocos2d::Color3B getColor(int number)const;         // 番号色を取得
    
    void didSelected();                                                                                  // 選択時
    void move(const std::vector<int>&indexes, const std::vector<cocos2d::Point>&poses, const CallBack &cb); // 指定座標を通って移動
    void remove(FlipDirection direction);                                                                                       // 削除
    
    
    void appendCubicBezier(int startPoint, std::vector<cocos2d::Vec2>& verts, const cocos2d::Vec2& from, const cocos2d::Vec2& control1, const cocos2d::Vec2& control2, const cocos2d::Vec2& to, uint32_t segments);
    Node* createRoundedRectMaskNode(cocos2d::Size size, float radius, int cornerSegments);
};

#endif // __Panel_H__
