//
//  TwelveGameSceneData.cpp
//  mockups
//
//  Created by 榎田直哉 on 2015/12/27.
//
//

#include "TwelveGameSceneData.h"
#include "Random.h"

// ---------- 定数

// 最大値をキーとした番号の配列
static std::map<int, std::vector<int>>RANDOM_NUMS
{
    {3,{1,1,1,1,2,2,3}},
    {2,{1,1,2}},
    {1,{1,1,1}},
};

// 最高得点保存のためのkey
static const char* SAVE_SCORE_KEY ="save_score_key";

// ---------- インスタンスメソッド
// コンストラクタ
TwelveGameSceneData::TwelveGameSceneData(){}

// デストラクタ
TwelveGameSceneData::~TwelveGameSceneData(){}

// 初期化
bool TwelveGameSceneData::init()
{
    return true;
}

// 得点を追加
void TwelveGameSceneData::addScore(int value)
{
    this->score += value;
}

// 得点を取得
int TwelveGameSceneData::getScore()const
{
    return this->score;
}

// ランダム番号の取得
std::vector<int>TwelveGameSceneData::getRandomNums(int max)const
{
    std::vector<int>randomNums{RANDOM_NUMS.at(max)};
    
    // シャッフル
    Random::shuffle(randomNums.begin(), randomNums.end());

    return randomNums;
}

// 記録を更新したか
bool TwelveGameSceneData::isBreakMaxScore()const
{
    return this->score > this->restoreScore();
}

// スコアを保存
void TwelveGameSceneData::saveScore()
{
    cocos2d::UserDefault::getInstance()->setIntegerForKey(SAVE_SCORE_KEY, this->score);
}

// スコアを復元
int TwelveGameSceneData::restoreScore()const
{
    return cocos2d::UserDefault::getInstance()->getIntegerForKey(SAVE_SCORE_KEY, 0);
}

