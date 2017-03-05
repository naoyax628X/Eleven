//
//  Grid.hpp
//  mockups
//
//  Created by 榎田直哉 on 2015/12/28.
//
//


#ifndef __Grid_H__
#define __Grid_H__

using namespace std;

#include "ui/UIScale9Sprite.h"
//
// 最短経路計算のための情報を持ったグリッド
//
class Grid final : public cocos2d::Sprite
{
// ---------- クラスメソッド
public:
    static Grid* create(const cocos2d::Size& size); // create関数
    
// ---------- インスタンス変数
private:
    bool done           {false}; // 確定済みか
    int cost            {-1};    // コスト
    int edge_from       {-1};    // どこから来たか
    std::vector<int>edge_tos;         // 隣接index
    std::vector<int>cost_tos;         // 隣接cost
    
// ---------- インスタンスメソッド
public:
    Grid();                                       // コンストラクタ
    ~Grid();                                      // デストラクタ
    bool init(const cocos2d::Size& size);       // 初期化
    
    bool isDone()const;                           // 確定済みかどうかを返却
    void setDone(bool done);                      // 確定済みかどうかを設定
    
    int getCost()const;                           // コストを取得
    void setCost(int cost);                       // コストを設定
    
    const vector<int>& getEdgeTos()const;         // 隣接indexを取得
    void setEdgetTos(const vector<int> & edgeTos); // 隣接indexを設定
    
    const vector<int>& getCostTos()const;         // 隣接コストを設定
    void setCostTos(const vector<int>& costs);    // 隣接コストを取得
    
    void setEdgeFrom(int from);                     // どこから来たかを設定
    int  getEdgeFrom()const;                        // どこから来たかを取得
    
};

#endif // __Grid_H__
