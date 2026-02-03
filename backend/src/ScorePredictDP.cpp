#include "ScorePredictDP.h"

// 成绩区间边界定义：[0-60), [60-70), [70-80), [80-90), [90-100]
const double ScorePredictDP::RANGE_BOUNDARIES[6] = {0.0, 60.0, 70.0, 80.0, 90.0, 100.0};

ScorePredictDP::ScorePredictDP() {
    historyRecords.clear();
    dpTable.clear();
    
    // 初始化状态转移权重
    transitionWeights["difficulty"] = 0.4;      // 课程难度权重
    transitionWeights["attendance"] = 0.3;      // 出勤率权重
    transitionWeights["homework"] = 0.2;        // 作业完成度权重
    transitionWeights["ability"] = 0.1;         // 学习能力权重
}

ScorePredictDP::~ScorePredictDP() {
    historyRecords.clear();
    dpTable.clear();
}

pair<double, double> ScorePredictDP::predictScoreRange(vector<ScoreRecord>& history, 
                                                     double difficulty, 
                                                     double attendance, 
                                                     double homeworkRate) {
    historyRecords = history;
    
    cout << "\n=== 学生成绩预测过程 ===" << endl;
    cout << "历史成绩记录数量: " << history.size() << endl;
    cout << "新课程参数:" << endl;
    cout << "  难度系数: " << fixed << setprecision(2) << difficulty << endl;
    cout << "  预期出勤率: " << fixed << setprecision(2) << attendance << endl;
    cout << "  预期作业完成度: " << fixed << setprecision(2) << homeworkRate << endl;
    
    if (history.empty()) {
        cout << "警告：无历史成绩记录，使用默认预测" << endl;
        return make_pair(60.0, 80.0);  // 默认预测区间
    }
    
    // 显示历史成绩
    cout << "\n历史成绩分析:" << endl;
    cout << setw(6) << "编号" << setw(15) << "课程名" << setw(8) << "成绩" 
         << setw(8) << "难度" << setw(8) << "出勤" << setw(10) << "作业完成" << endl;
    cout << string(60, '-') << endl;
    
    for (const auto& record : history) {
        cout << setw(6) << record.courseId 
             << setw(15) << record.courseName 
             << setw(8) << fixed << setprecision(1) << record.score 
             << setw(8) << fixed << setprecision(2) << record.difficulty 
             << setw(8) << fixed << setprecision(2) << record.attendance 
             << setw(10) << fixed << setprecision(2) << record.homeworkRate << endl;
    }
    
    // 计算学习能力因子
    double learningAbility = calculateLearningAbility(history);
    cout << "\n学习能力因子: " << fixed << setprecision(3) << learningAbility << endl;
    
    // 计算课程适应度
    double adaptability = calculateCourseAdaptability(history, difficulty);
    cout << "课程适应度: " << fixed << setprecision(3) << adaptability << endl;
    
    // 计算习惯影响因子
    double habitFactor = calculateHabitFactor(attendance, homeworkRate);
    cout << "习惯影响因子: " << fixed << setprecision(3) << habitFactor << endl;
    
    // 初始化DP表
    initializeDPTable(history.size());
    
    // 计算状态转移矩阵（基于最近的成绩记录）
    ScoreRecord lastRecord = history.back();
    vector<vector<double>> transitionMatrix = calculateTransitionMatrix(lastRecord, difficulty, attendance, homeworkRate);
    
    cout << "\n状态转移矩阵:" << endl;
    cout << "     ";
    for (int j = 0; j < SCORE_RANGES; j++) {
        cout << setw(8) << getScoreRangeName(j);
    }
    cout << endl;
    
    for (int i = 0; i < SCORE_RANGES; i++) {
        cout << setw(5) << getScoreRangeName(i);
        for (int j = 0; j < SCORE_RANGES; j++) {
            cout << setw(8) << fixed << setprecision(3) << transitionMatrix[i][j];
        }
        cout << endl;
    }
    
    // 执行DP计算
    vector<double> finalProbabilities = performDPCalculation(transitionMatrix);
    
    cout << "\n最终状态概率分布:" << endl;
    for (int i = 0; i < SCORE_RANGES; i++) {
        cout << getScoreRangeName(i) << ": " << fixed << setprecision(3) << finalProbabilities[i] << endl;
    }
    
    // 计算预测区间
    pair<double, double> prediction = calculatePredictionRange(finalProbabilities);
    
    // 应用学习能力和适应度调整
    double adjustment = (learningAbility - 1.0) * 10.0 + (adaptability - 1.0) * 5.0 + (habitFactor - 1.0) * 8.0;
    prediction.first = max(0.0, prediction.first + adjustment);
    prediction.second = min(100.0, prediction.second + adjustment);
    
    // 验证预测结果
    if (!validatePrediction(prediction, difficulty)) {
        cout << "警告：预测结果验证失败！" << endl;
    }
    
    return prediction;
}

vector<pair<double, double>> ScorePredictDP::predictMultipleCourses(vector<ScoreRecord>& history,
                                                                  vector<tuple<double, double, double>>& futureCourses) {
    vector<pair<double, double>> predictions;
    vector<ScoreRecord> currentHistory = history;
    
    cout << "\n=== 多门课程成绩预测 ===" << endl;
    
    for (size_t i = 0; i < futureCourses.size(); i++) {
        double difficulty = get<0>(futureCourses[i]);
        double attendance = get<1>(futureCourses[i]);
        double homeworkRate = get<2>(futureCourses[i]);
        
        cout << "\n预测第 " << (i + 1) << " 门课程:" << endl;
        pair<double, double> prediction = predictScoreRange(currentHistory, difficulty, attendance, homeworkRate);
        predictions.push_back(prediction);
        
        // 将预测结果作为新的历史记录（使用预测均值）
        ScoreRecord newRecord;
        newRecord.courseId = 1000 + i;
        newRecord.courseName = "预测课程" + to_string(i + 1);
        newRecord.score = (prediction.first + prediction.second) / 2.0;
        newRecord.difficulty = difficulty;
        newRecord.attendance = attendance;
        newRecord.homeworkRate = homeworkRate;
        
        currentHistory.push_back(newRecord);
    }
    
    return predictions;
}

vector<vector<double>> ScorePredictDP::calculateTransitionMatrix(const ScoreRecord& prev, 
                                                               double difficulty, 
                                                               double attendance, 
                                                               double homeworkRate) {
    vector<vector<double>> matrix(SCORE_RANGES, vector<double>(SCORE_RANGES, 0.0));
    
    // 移除未使用的变量警告
    (void)prev; // 标记为有意未使用，保留接口完整性
    
    // 基础转移概率（倾向于保持当前水平）
    for (int i = 0; i < SCORE_RANGES; i++) {
        for (int j = 0; j < SCORE_RANGES; j++) {
            double distance = abs(i - j);
            matrix[i][j] = exp(-distance * 0.5);  // 距离越远概率越小
        }
    }
    
    // 根据各种因子调整转移概率
    double difficultyImpact = (1.0 - difficulty) * 2.0;  // 难度越高，成绩越难提升
    double attendanceImpact = attendance * 1.5;           // 出勤率影响
    double homeworkImpact = homeworkRate * 1.2;           // 作业完成度影响
    
    for (int i = 0; i < SCORE_RANGES; i++) {
        for (int j = 0; j < SCORE_RANGES; j++) {
            if (j > i) {  // 成绩提升
                matrix[i][j] *= (difficultyImpact * attendanceImpact * homeworkImpact);
            } else if (j < i) {  // 成绩下降
                matrix[i][j] *= (2.0 - difficultyImpact * attendanceImpact * homeworkImpact);
            }
        }
    }
    
    // 归一化每行
    for (int i = 0; i < SCORE_RANGES; i++) {
        double sum = 0.0;
        for (int j = 0; j < SCORE_RANGES; j++) {
            sum += matrix[i][j];
        }
        if (sum > 0) {
            for (int j = 0; j < SCORE_RANGES; j++) {
                matrix[i][j] /= sum;
            }
        }
    }
    
    return matrix;
}

void ScorePredictDP::printPredictionResult(const pair<double, double>& range, 
                                         double difficulty, 
                                         double attendance, 
                                         double homeworkRate) {
    // 标记参数为有意未使用，保留接口完整性
    (void)difficulty;
    (void)attendance; 
    (void)homeworkRate;
    
    cout << "\n=== 成绩预测结果 ===" << endl;
    cout << "预测成绩区间: [" << fixed << setprecision(1) << range.first 
         << ", " << range.second << "]" << endl;
    cout << "预测均值: " << fixed << setprecision(1) << (range.first + range.second) / 2.0 << endl;
    cout << "区间宽度: " << fixed << setprecision(1) << (range.second - range.first) << endl;
    
    // 预测等级
    double avgScore = (range.first + range.second) / 2.0;
    string grade;
    if (avgScore >= 90) grade = "优秀";
    else if (avgScore >= 80) grade = "良好";
    else if (avgScore >= 70) grade = "中等";
    else if (avgScore >= 60) grade = "及格";
    else grade = "不及格";
    
    cout << "预测等级: " << grade << endl;
    
    // 置信度评估
    double confidence = 1.0 - (range.second - range.first) / 100.0;
    cout << "预测置信度: " << fixed << setprecision(1) << confidence * 100 << "%" << endl;
    
    // 建议
    cout << "\n学习建议:" << endl;
    if (avgScore < 70) {
        cout << "- 建议加强基础知识学习" << endl;
        cout << "- 提高出勤率和作业完成度" << endl;
    } else if (avgScore < 85) {
        cout << "- 保持当前学习状态" << endl;
        cout << "- 可适当增加练习量" << endl;
    } else {
        cout << "- 学习状态良好，继续保持" << endl;
        cout << "- 可尝试挑战更高难度内容" << endl;
    }
}

int ScorePredictDP::getScoreRangeIndex(double score) {
    for (int i = 0; i < SCORE_RANGES; i++) {
        if (score >= RANGE_BOUNDARIES[i] && score < RANGE_BOUNDARIES[i + 1]) {
            return i;
        }
    }
    return SCORE_RANGES - 1;  // 100分的情况
}

string ScorePredictDP::getScoreRangeName(int index) {
    switch (index) {
        case 0: return "不及格";
        case 1: return "及格";
        case 2: return "中等";
        case 3: return "良好";
        case 4: return "优秀";
        default: return "未知";
    }
}

void ScorePredictDP::initializeDPTable(int historySize) {
    dpTable.assign(historySize + 1, vector<double>(SCORE_RANGES, 0.0));
    
    // 初始化第一行（基于第一门课程的成绩）
    if (!historyRecords.empty()) {
        int firstRange = getScoreRangeIndex(historyRecords[0].score);
        dpTable[0][firstRange] = 1.0;
    }
}

double ScorePredictDP::calculateLearningAbility(const vector<ScoreRecord>& history) {
    if (history.size() < 2) return 1.0;
    
    double totalImprovement = 0.0;
    int validTransitions = 0;
    
    for (size_t i = 1; i < history.size(); i++) {
        // 考虑难度调整后的成绩变化
        double prevAdjustedScore = history[i-1].score / history[i-1].difficulty;
        double currAdjustedScore = history[i].score / history[i].difficulty;
        
        totalImprovement += (currAdjustedScore - prevAdjustedScore);
        validTransitions++;
    }
    
    if (validTransitions == 0) return 1.0;
    
    double avgImprovement = totalImprovement / validTransitions;
    return max(0.5, min(2.0, 1.0 + avgImprovement / 20.0));  // 限制在[0.5, 2.0]范围内
}

double ScorePredictDP::calculateCourseAdaptability(const vector<ScoreRecord>& history, double difficulty) {
    if (history.empty()) return 1.0;
    
    double totalAdaptability = 0.0;
    int count = 0;
    
    for (const auto& record : history) {
        double difficultyDiff = abs(record.difficulty - difficulty);
        double adaptability = exp(-difficultyDiff * 2.0);  // 难度差异越小，适应度越高
        totalAdaptability += adaptability;
        count++;
    }
    
    return count > 0 ? totalAdaptability / count : 1.0;
}

double ScorePredictDP::calculateHabitFactor(double attendance, double homeworkRate) {
    // 出勤率和作业完成度的综合影响
    double habitScore = attendance * 0.6 + homeworkRate * 0.4;
    return max(0.5, min(2.0, habitScore * 1.5));  // 限制在[0.5, 2.0]范围内
}

vector<double> ScorePredictDP::performDPCalculation(const vector<vector<double>>& transitionMatrix) {
    if (historyRecords.empty()) {
        return vector<double>(SCORE_RANGES, 1.0 / SCORE_RANGES);  // 均匀分布
    }
    
    // 初始状态（基于最后一门课程的成绩）
    vector<double> currentState(SCORE_RANGES, 0.0);
    int lastRange = getScoreRangeIndex(historyRecords.back().score);
    currentState[lastRange] = 1.0;
    
    // 应用状态转移
    vector<double> nextState(SCORE_RANGES, 0.0);
    for (int i = 0; i < SCORE_RANGES; i++) {
        for (int j = 0; j < SCORE_RANGES; j++) {
            nextState[j] += currentState[i] * transitionMatrix[i][j];
        }
    }
    
    return nextState;
}

pair<double, double> ScorePredictDP::calculatePredictionRange(const vector<double>& probabilities) {
    double expectedValue = 0.0;
    double variance = 0.0;
    
    // 计算期望值
    for (int i = 0; i < SCORE_RANGES; i++) {
        double rangeMidpoint = (RANGE_BOUNDARIES[i] + RANGE_BOUNDARIES[i + 1]) / 2.0;
        expectedValue += probabilities[i] * rangeMidpoint;
    }
    
    // 计算方差
    for (int i = 0; i < SCORE_RANGES; i++) {
        double rangeMidpoint = (RANGE_BOUNDARIES[i] + RANGE_BOUNDARIES[i + 1]) / 2.0;
        variance += probabilities[i] * pow(rangeMidpoint - expectedValue, 2);
    }
    
    double stdDev = sqrt(variance);
    
    // 预测区间：期望值 ± 1个标准差
    double lowerBound = max(0.0, expectedValue - stdDev);
    double upperBound = min(100.0, expectedValue + stdDev);
    
    return make_pair(lowerBound, upperBound);
}

bool ScorePredictDP::validatePrediction(const pair<double, double>& range, double difficulty) {
    // 检查区间合理性
    if (range.first < 0 || range.second > 100 || range.first > range.second) {
        cout << "验证失败: 预测区间不合理" << endl;
        return false;
    }
    
    // 检查区间宽度
    double rangeWidth = range.second - range.first;
    if (rangeWidth > 50) {  // 区间过宽表示预测不确定性太高
        cout << "警告: 预测区间过宽，不确定性较高" << endl;
    }
    
    // 检查与难度的一致性
    double avgScore = (range.first + range.second) / 2.0;
    if (difficulty > 0.8 && avgScore > 90) {
        cout << "警告: 高难度课程预测成绩过高" << endl;
    }
    
    return true;
}