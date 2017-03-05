//
//  TwelveGameSceneData.hpp
//  mockups
//
//  Created by 榎田直哉 on 2015/12/27.
//
//

#ifndef _TwelveGameSceneData__
#define _TwelveGameSceneData__

class TwelveGameSceneData : public cocos2d::Ref
{
// ---------- 定数
private:
    static constexpr int DEFAULT_SCORE{4}; // デフォルトのスコア上昇値
   
// ---------- クラスメソッド
public:
    CREATE_FUNC(TwelveGameSceneData)
    
// ---------- インスタンス変数
private:
    int score{0};            // 得点
    
// ---------- インスタンスメソッド
public:
    TwelveGameSceneData();   // コンストラクタ
    ~TwelveGameSceneData();  // デストラクタ
    bool init();             // 初期化
    
    void addScore(int value= DEFAULT_SCORE); // 得点を追加
    int  getScore()const;                    // 得点を取得
    
    std::vector<int> getRandomNums(int max)const; // ランダム番号の取得
   
    bool isBreakMaxScore() const;            // 記録を更新したか
    
    void saveScore();                        // スコアを保存
    int restoreScore()const;                 // スコアを復元
};

#endif /* defined(_TwelveGameSceneData__) */
