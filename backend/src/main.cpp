#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <limits>
#include <cfloat>
#include <climits>
#include <functional>
#include "CourseScheduleGreedy.h"
#include "ClubResourceGreedy.h"
#include "ScorePredictDP.h"
#include "PathPlanDP.h"

using namespace std;

/**
 * 学生系统后端核心模块主程序
 * 支持用户交互式输入数据
 */

void runCourseScheduleOptimization();
void runClubResourceAllocation();
void runScorePrediction();
void runPathPlanning();
void printMainMenu();
void printSeparator(const string& title);

// 输入辅助函数
vector<Course> inputCourseData();
vector<Club> inputClubData();
vector<ScoreRecord> inputScoreHistory();
vector<vector<double>> inputCampusGraph(vector<CampusNode>& nodes);
vector<CampusNode> inputCampusNodes();

// 数据验证函数
bool validateTimeInput(int time);
bool validateDayInput(int day);
bool validatePriorityInput(int priority);
bool validateScoreInput(double score);
bool validateRateInput(double rate);

// 输入清理函数
void clearInputBuffer();
int getIntInput(const string& prompt, int minVal = INT_MIN, int maxVal = INT_MAX);
double getDoubleInput(const string& prompt, double minVal = -1e9, double maxVal = 1e9);
string getStringInput(const string& prompt);

int main() {
    cout << "========================================" << endl;
    cout << "    学生系统后端核心模块" << endl;
    cout << "========================================" << endl;
    cout << "本程序提供4个核心算法模块的功能：" << endl;
    cout << "1. 课程表优化 (贪心算法)" << endl;
    cout << "2. 社团资源分配 (贪心算法)" << endl;
    cout << "3. 学生成绩预测 (动态规划)" << endl;
    cout << "4. 校园路径规划 (动态规划)" << endl;
    cout << "========================================" << endl;

    int choice;
    do {
        printMainMenu();
        choice = getIntInput("请选择要使用的功能 (1-5): ", 1, 5);

        switch (choice) {
            case 1:
                runCourseScheduleOptimization();
                break;
            case 2:
                runClubResourceAllocation();
                break;
            case 3:
                runScorePrediction();
                break;
            case 4:
                runPathPlanning();
                break;
            case 5:
                cout << "\n感谢使用学生系统后端核心模块！" << endl;
                break;
        }

        if (choice != 5) {
            cout << "\n按回车键继续...";
            cin.ignore();
            cin.get();
        }

    } while (choice != 5);

    return 0;
}

void printMainMenu() {
    cout << "\n========== 主菜单 ==========" << endl;
    cout << "1. 课程表优化" << endl;
    cout << "2. 社团资源分配" << endl;
    cout << "3. 学生成绩预测" << endl;
    cout << "4. 校园路径规划" << endl;
    cout << "5. 退出程序" << endl;
    cout << "===========================" << endl;
}

void printSeparator(const string& title) {
    cout << "\n" << string(50, '=') << endl;
    cout << "    " << title << endl;
    cout << string(50, '=') << endl;
}

void runCourseScheduleOptimization() {
    printSeparator("课程表优化");
    
    CourseScheduleGreedy scheduler;
    
    cout << "课程表优化功能说明：" << endl;
    cout << "- 根据课程优先级、学分、时间安排生成最优课程表" << endl;
    cout << "- 自动检测并避免时间冲突" << endl;
    cout << "- 时间格式：24小时制分钟数（如480=8:00, 540=9:00）" << endl;
    cout << "- 星期：1-7（1=周一，7=周日）" << endl;
    cout << "- 优先级：1-10（10为最高优先级）" << endl;
    
    // 获取用户输入的课程数据
    vector<Course> courses = inputCourseData();
    
    if (courses.empty()) {
        cout << "未输入任何课程数据，返回主菜单。" << endl;
        return;
    }
    
    // 执行课程表优化
    vector<Course> optimizedSchedule = scheduler.optimizeSchedule(courses);
    
    // 打印结果
    scheduler.printScheduleResult(optimizedSchedule);
}

void runClubResourceAllocation() {
    printSeparator("社团资源分配");
    
    ClubResourceGreedy allocator;
    
    cout << "社团资源分配功能说明：" << endl;
    cout << "- 根据社团优先级、历史表现、活动规模分配有限资源" << endl;
    cout << "- 资源包括：场地数量、资金额度" << endl;
    cout << "- 优先级：1-10（10为最高优先级）" << endl;
    cout << "- 历史表现：0-100分" << endl;
    
    // 获取资源总量
    cout << "\n=== 设置资源总量 ===" << endl;
    int totalVenues = getIntInput("请输入总场地数量: ", 0, 100);
    double totalFunds = getDoubleInput("请输入总资金额度(万元): ", 0.0, 1000.0);
    
    // 获取用户输入的社团数据
    vector<Club> clubs = inputClubData();
    
    if (clubs.empty()) {
        cout << "未输入任何社团数据，返回主菜单。" << endl;
        return;
    }
    
    // 执行资源分配
    vector<AllocationResult> results = allocator.allocateResources(clubs, totalVenues, totalFunds);
    
    // 打印结果
    allocator.printAllocationResult(results);
}

void runScorePrediction() {
    printSeparator("学生成绩预测");
    
    ScorePredictDP predictor;
    
    cout << "学生成绩预测功能说明：" << endl;
    cout << "- 基于历史成绩数据预测未来课程成绩区间" << endl;
    cout << "- 考虑课程难度、出勤率、作业完成度等因素" << endl;
    cout << "- 成绩：0-100分" << endl;
    cout << "- 难度系数：0-1（1为最难）" << endl;
    cout << "- 出勤率/作业完成度：0-1（1为100%）" << endl;
    
    // 获取历史成绩数据
    vector<ScoreRecord> history = inputScoreHistory();
    
    if (history.empty()) {
        cout << "未输入历史成绩数据，将使用默认预测。" << endl;
    }
    
    // 获取新课程参数
    cout << "\n=== 设置新课程参数 ===" << endl;
    string courseName = getStringInput("请输入新课程名称: ");
    double difficulty = getDoubleInput("请输入课程难度系数(0-1): ", 0.0, 1.0);
    double attendance = getDoubleInput("请输入预期出勤率(0-1): ", 0.0, 1.0);
    double homeworkRate = getDoubleInput("请输入预期作业完成度(0-1): ", 0.0, 1.0);
    
    cout << "\n新课程: " << courseName << endl;
    
    // 执行成绩预测
    pair<double, double> prediction = predictor.predictScoreRange(history, difficulty, attendance, homeworkRate);
    
    // 打印结果
    predictor.printPredictionResult(prediction, difficulty, attendance, homeworkRate);
}

void runPathPlanning() {
    printSeparator("校园路径规划");
    
    PathPlanDP planner;
    
    cout << "校园路径规划功能说明：" << endl;
    cout << "- 基于校园地图计算最优路径" << endl;
    cout << "- 考虑距离、坡度、拥堵等因素" << endl;
    cout << "- 支持自定义权重偏好" << endl;
    
    // 获取校园节点数据
    vector<CampusNode> nodes = inputCampusNodes();
    
    if (nodes.size() < 2) {
        cout << "至少需要2个节点才能进行路径规划，返回主菜单。" << endl;
        return;
    }
    
    // 获取邻接矩阵
    vector<vector<double>> campusGraph = inputCampusGraph(nodes);
    
    // 获取起点和终点
    cout << "\n=== 设置路径规划参数 ===" << endl;
    cout << "可用节点列表：" << endl;
    for (size_t i = 0; i < nodes.size(); i++) {
        cout << i << ": " << nodes[i].nodeName << " (" << nodes[i].nodeType << ")" << endl;
    }
    
    int startNode = getIntInput("请输入起点编号: ", 0, (int)nodes.size() - 1);
    int endNode = getIntInput("请输入终点编号: ", 0, (int)nodes.size() - 1);
    
    double slopeWeight = getDoubleInput("请输入坡度权重(1.0为标准): ", 0.1, 5.0);
    double congestionWeight = getDoubleInput("请输入拥堵权重(1.0为标准): ", 0.1, 5.0);
    
    // 执行路径规划
    PathResult result = planner.findOptimalPath(campusGraph, nodes, startNode, endNode, slopeWeight, congestionWeight);
    
    // 打印结果
    planner.printPathResult(result, nodes);
}

// ==================== 输入辅助函数实现 ====================

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int getIntInput(const string& prompt, int minVal, int maxVal) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            if (value >= minVal && value <= maxVal) {
                clearInputBuffer();
                return value;
            } else {
                cout << "输入超出范围 [" << minVal << ", " << maxVal << "]，请重新输入。" << endl;
            }
        } else {
            cout << "输入格式错误，请输入整数。" << endl;
            clearInputBuffer();
        }
    }
}

double getDoubleInput(const string& prompt, double minVal, double maxVal) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            if (value >= minVal && value <= maxVal) {
                clearInputBuffer();
                return value;
            } else {
                cout << "输入超出范围 [" << fixed << setprecision(2) << minVal 
                     << ", " << maxVal << "]，请重新输入。" << endl;
            }
        } else {
            cout << "输入格式错误，请输入数字。" << endl;
            clearInputBuffer();
        }
    }
}

string getStringInput(const string& prompt) {
    string value;
    cout << prompt;
    getline(cin, value);
    return value;
}

bool validateTimeInput(int time) {
    return time >= 0 && time < 1440;  // 0-1439分钟（24小时）
}

bool validateDayInput(int day) {
    return day >= 1 && day <= 7;
}

bool validatePriorityInput(int priority) {
    return priority >= 1 && priority <= 10;
}

bool validateScoreInput(double score) {
    return score >= 0.0 && score <= 100.0;
}

bool validateRateInput(double rate) {
    return rate >= 0.0 && rate <= 1.0;
}

vector<Course> inputCourseData() {
    vector<Course> courses;
    
    cout << "\n=== 输入课程数据 ===" << endl;
    int courseCount = getIntInput("请输入课程数量: ", 1, 20);
    
    for (int i = 0; i < courseCount; i++) {
        cout << "\n--- 输入第 " << (i + 1) << " 门课程 ---" << endl;
        
        int courseId = getIntInput("课程编号: ", 1, 9999);
        
        // 清空输入缓冲区，避免读取到之前的换行符
        clearInputBuffer();
        
        string courseName = getStringInput("课程名称: ");
        
        cout << "时间输入说明：使用24小时制分钟数" << endl;
        cout << "例如：8:00=480, 9:30=570, 10:00=600, 14:00=840" << endl;
        
        int startTime = getIntInput("开始时间(分钟): ", 0, 1439);
        int endTime = getIntInput("结束时间(分钟): ", startTime + 1, 1439);
        
        int dayOfWeek = getIntInput("星期几(1-7): ", 1, 7);
        int credits = getIntInput("学分: ", 1, 10);
        int priority = getIntInput("优先级(1-10): ", 1, 10);
        
        courses.emplace_back(courseId, courseName, startTime, endTime, dayOfWeek, credits, priority);
        
        cout << "✓ 课程 \"" << courseName << "\" 添加成功" << endl;
    }
    
    return courses;
}

vector<Club> inputClubData() {
    vector<Club> clubs;
    
    cout << "\n=== 输入社团数据 ===" << endl;
    int clubCount = getIntInput("请输入社团数量: ", 1, 20);
    
    for (int i = 0; i < clubCount; i++) {
        cout << "\n--- 输入第 " << (i + 1) << " 个社团 ---" << endl;
        
        int clubId = getIntInput("社团编号: ", 1, 9999);
        string clubName = getStringInput("社团名称: ");
        int priority = getIntInput("优先级(1-10): ", 1, 10);
        double historyScore = getDoubleInput("历史表现分(0-100): ", 0.0, 100.0);
        int activityScale = getIntInput("活动规模(人数): ", 1, 500);
        int venueNeeded = getIntInput("需要场地数: ", 0, 10);
        double fundNeeded = getDoubleInput("需要资金(万元): ", 0.0, 100.0);
        
        clubs.emplace_back(clubId, clubName, priority, historyScore, activityScale, venueNeeded, fundNeeded);
        
        cout << "✓ 社团 \"" << clubName << "\" 添加成功" << endl;
    }
    
    return clubs;
}

vector<ScoreRecord> inputScoreHistory() {
    vector<ScoreRecord> history;
    
    cout << "\n=== 输入历史成绩数据 ===" << endl;
    int recordCount = getIntInput("请输入历史成绩记录数量(0表示无历史记录): ", 0, 20);
    
    if (recordCount == 0) {
        return history;
    }
    
    for (int i = 0; i < recordCount; i++) {
        cout << "\n--- 输入第 " << (i + 1) << " 门课程成绩 ---" << endl;
        
        int courseId = getIntInput("课程编号: ", 1, 9999);
        string courseName = getStringInput("课程名称: ");
        double score = getDoubleInput("成绩(0-100): ", 0.0, 100.0);
        double difficulty = getDoubleInput("课程难度系数(0-1): ", 0.0, 1.0);
        double attendance = getDoubleInput("出勤率(0-1): ", 0.0, 1.0);
        double homeworkRate = getDoubleInput("作业完成度(0-1): ", 0.0, 1.0);
        
        history.emplace_back(courseId, courseName, score, difficulty, attendance, homeworkRate);
        
        cout << "✓ 成绩记录 \"" << courseName << "\" 添加成功" << endl;
    }
    
    return history;
}

vector<CampusNode> inputCampusNodes() {
    vector<CampusNode> nodes;
    
    cout << "\n=== 输入校园节点数据 ===" << endl;
    int nodeCount = getIntInput("请输入节点数量: ", 2, 20);
    
    cout << "\n节点类型选项：教学楼、宿舍、食堂、图书馆、体育馆、停车场、入口等" << endl;
    
    for (int i = 0; i < nodeCount; i++) {
        cout << "\n--- 输入第 " << (i + 1) << " 个节点 ---" << endl;
        
        string nodeName = getStringInput("节点名称: ");
        string nodeType = getStringInput("节点类型: ");
        double slopeFactor = getDoubleInput("坡度因子(0.5-3.0): ", 0.5, 3.0);
        
        nodes.emplace_back(i, nodeName, nodeType, slopeFactor);
        
        cout << "✓ 节点 \"" << nodeName << "\" 添加成功" << endl;
    }
    
    return nodes;
}

vector<vector<double>> inputCampusGraph(vector<CampusNode>& nodes) {
    int n = nodes.size();
    vector<vector<double>> graph(n, vector<double>(n, 0.0));
    
    cout << "\n=== 输入节点连接关系 ===" << endl;
    cout << "请输入节点间的距离(米)，输入0表示不连通" << endl;
    
    // 显示节点列表
    cout << "\n节点列表：" << endl;
    for (size_t i = 0; i < nodes.size(); i++) {
        cout << i << ": " << nodes[i].nodeName << endl;
    }
    
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            cout << "\n节点 " << i << "(" << nodes[i].nodeName << ") 到 节点 " 
                 << j << "(" << nodes[j].nodeName << ") 的距离: ";
            
            double distance = getDoubleInput("", 0.0, 10000.0);
            
            if (distance > 0) {
                graph[i][j] = distance;
                graph[j][i] = distance;  // 双向连接
            }
        }
    }
    
    // 验证连通性
    cout << "\n检查图的连通性..." << endl;
    bool hasConnection = false;
    for (int i = 0; i < n && !hasConnection; i++) {
        for (int j = 0; j < n && !hasConnection; j++) {
            if (i != j && graph[i][j] > 0) {
                hasConnection = true;
            }
        }
    }
    
    if (!hasConnection) {
        cout << "警告：图中没有任何连接，路径规划可能失败。" << endl;
    } else {
        cout << "✓ 图连接关系设置完成" << endl;
    }
    
    return graph;
}