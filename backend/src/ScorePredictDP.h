#pragma once
#include "DataStructures.h"
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

/**
 * 学生成绩预测类 - 动态规划算法实现
 * 功能：基于历史成绩和学习习惯，预测未来课程的成绩区间
 */
class ScorePredictDP {
private:
    vector<ScoreRecord> historyRecords;      // 历史成绩记录
    vector<vector<double>> dpTable;          // DP状态表
    map<string, double> transitionWeights;   // 状态转移权重
    
    // 成绩区间定义
    static const int SCORE_RANGES = 5;       // 成绩区间数量（优秀、良好、中等、及格、不及格）
    static const double RANGE_BOUNDARIES[6]; // 区间边界

public:
    ScorePredictDP();
    ~ScorePredictDP();

    /**
     * 成绩预测主函数
     * @param history 历史成绩记录
     * @param difficulty 新课程难度系数（0-1）
     * @param attendance 预期出勤率（0-1）
     * @param homeworkRate 预期作业完成度（0-1）
     * @return 预测成绩区间（最低分，最高分）
     */
    pair<double, double> predictScoreRange(vector<ScoreRecord>& history, 
                                         double difficulty, 
                                         double attendance, 
                                         double homeworkRate);

    /**
     * 预测多门课程的成绩
     * @param history 历史成绩记录
     * @param futureCourses 未来课程信息（难度、出勤率、作业完成度）
     * @return 每门课程的预测成绩区间
     */
    vector<pair<double, double>> predictMultipleCourses(vector<ScoreRecord>& history,
                                                       vector<tuple<double, double, double>>& futureCourses);

    /**
     * 计算状态转移概率
     * @param prev 前一门课程记录
     * @param curr 当前课程参数
     * @return 转移概率矩阵
     */
    vector<vector<double>> calculateTransitionMatrix(const ScoreRecord& prev, 
                                                   double difficulty, 
                                                   double attendance, 
                                                   double homeworkRate);

    /**
     * 打印预测结果
     * @param range 预测成绩区间
     * @param difficulty 课程难度
     * @param attendance 出勤率
     * @param homeworkRate 作业完成度
     */
    void printPredictionResult(const pair<double, double>& range, 
                             double difficulty, 
                             double attendance, 
                             double homeworkRate);

    /**
     * 获取成绩区间索引
     * @param score 成绩
     * @return 区间索引（0-4）
     */
    static int getScoreRangeIndex(double score);

    /**
     * 获取区间名称
     * @param index 区间索引
     * @return 区间名称
     */
    static string getScoreRangeName(int index);

private:
    /**
     * 初始化DP表
     * @param historySize 历史记录数量
     */
    void initializeDPTable(int historySize);

    /**
     * 计算学习能力因子
     * @param history 历史成绩记录
     * @return 学习能力因子（0-2，1为平均水平）
     */
    double calculateLearningAbility(const vector<ScoreRecord>& history);

    /**
     * 计算课程适应度
     * @param history 历史成绩记录
     * @param difficulty 新课程难度
     * @return 适应度系数（0-2）
     */
    double calculateCourseAdaptability(const vector<ScoreRecord>& history, double difficulty);

    /**
     * 计算习惯影响因子
     * @param attendance 出勤率
     * @param homeworkRate 作业完成度
     * @return 习惯影响因子（0-2）
     */
    double calculateHabitFactor(double attendance, double homeworkRate);

    /**
     * 执行动态规划计算
     * @param transitionMatrix 状态转移矩阵
     * @return 最终状态概率分布
     */
    vector<double> performDPCalculation(const vector<vector<double>>& transitionMatrix);

    /**
     * 根据概率分布计算预测区间
     * @param probabilities 状态概率分布
     * @return 预测成绩区间
     */
    pair<double, double> calculatePredictionRange(const vector<double>& probabilities);

    /**
     * 验证预测结果的合理性
     * @param range 预测区间
     * @param difficulty 课程难度
     * @return true表示验证通过
     */
    bool validatePrediction(const pair<double, double>& range, double difficulty);
};