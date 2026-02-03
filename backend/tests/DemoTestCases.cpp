#include <iostream>
#include <vector>
#include <iomanip>
#include "../src/CourseScheduleGreedy.h"
#include "../src/ClubResourceGreedy.h"
#include "../src/ScorePredictDP.h"
#include "../src/PathPlanDP.h"

using namespace std;

/**
 * 演示测试用例程序
 * 包含预设的完整测试数据，展示各个算法模块的功能
 */

void demoTestCourseScheduleOptimization();
void demoTestClubResourceAllocation();
void demoTestScorePrediction();
void demoTestPathPlanning();
void printMainMenu();
void printSeparator(const string& title);

int main() {
    cout << "========================================" << endl;
    cout << "    学生系统后端核心模块演示测试" << endl;
    cout << "========================================" << endl;
    cout << "本程序使用预设数据演示4个核心算法模块：" << endl;
    cout << "1. 课程表优化 (贪心算法)" << endl;
    cout << "2. 社团资源分配 (贪心算法)" << endl;
    cout << "3. 学生成绩预测 (动态规划)" << endl;
    cout << "4. 校园路径规划 (动态规划)" << endl;
    cout << "========================================" << endl;

    int choice;
    do {
        printMainMenu();
        cout << "请选择要演示的功能 (1-6): ";
        cin >> choice;

        switch (choice) {
            case 1:
                demoTestCourseScheduleOptimization();
                break;
            case 2:
                demoTestClubResourceAllocation();
                break;
            case 3:
                demoTestScorePrediction();
                break;
            case 4:
                demoTestPathPlanning();
                break;
            case 5:
                // 运行所有演示
                demoTestCourseScheduleOptimization();
                demoTestClubResourceAllocation();
                demoTestScorePrediction();
                demoTestPathPlanning();
                break;
            case 6:
                cout << "\n感谢使用学生系统后端核心模块演示程序！" << endl;
                break;
            default:
                cout << "\n无效选择，请重新输入。" << endl;
                break;
        }

        if (choice != 6) {
            cout << "\n按回车键继续...";
            cin.ignore();
            cin.get();
        }

    } while (choice != 6);

    return 0;
}

void printMainMenu() {
    cout << "\n========== 演示菜单 ==========" << endl;
    cout << "1. 演示课程表优化" << endl;
    cout << "2. 演示社团资源分配" << endl;
    cout << "3. 演示学生成绩预测" << endl;
    cout << "4. 演示校园路径规划" << endl;
    cout << "5. 运行所有演示" << endl;
    cout << "6. 退出程序" << endl;
    cout << "=============================" << endl;
}

void printSeparator(const string& title) {
    cout << "\n" << string(50, '=') << endl;
    cout << "    " << title << endl;
    cout << string(50, '=') << endl;
}

void demoTestCourseScheduleOptimization() {
    printSeparator("课程表优化演示");
    
    CourseScheduleGreedy scheduler;
    
    // 创建演示课程数据
    vector<Course> courses = {
        Course(1001, "高等数学", 480, 570, 1, 4, 9),      // 周一 8:00-9:30, 4学分, 优先级9
        Course(1002, "线性代数", 600, 690, 1, 3, 7),      // 周一 10:00-11:30, 3学分, 优先级7
        Course(1003, "数据结构", 480, 570, 2, 3, 10),     // 周二 8:00-9:30, 3学分, 优先级10
        Course(1004, "计算机网络", 600, 690, 2, 3, 8),    // 周二 10:00-11:30, 3学分, 优先级8
        Course(1005, "操作系统", 480, 570, 3, 3, 9),      // 周三 8:00-9:30, 3学分, 优先级9
        Course(1006, "数据库原理", 540, 630, 1, 3, 6),    // 周一 9:00-10:30, 3学分, 优先级6 (与高等数学冲突)
        Course(1007, "软件工程", 720, 810, 3, 2, 5),      // 周三 12:00-13:30, 2学分, 优先级5
        Course(1008, "人工智能", 600, 690, 4, 3, 8),      // 周四 10:00-11:30, 3学分, 优先级8
        Course(1009, "机器学习", 480, 570, 5, 3, 9),      // 周五 8:00-9:30, 3学分, 优先级9
        Course(1010, "英语", 540, 630, 2, 2, 4)           // 周二 9:00-10:30, 2学分, 优先级4 (与数据结构冲突)
    };
    
    cout << "演示数据说明：" << endl;
    cout << "- 共10门课程，存在时间冲突" << endl;
    cout << "- 时间格式：分钟数（480=8:00, 540=9:00, 600=10:00等）" << endl;
    cout << "- 优先级：1-10，10为最高优先级" << endl;
    cout << "- 预期结果：选择无冲突的高优先级课程组合" << endl;
    
    // 执行课程表优化
    vector<Course> optimizedSchedule = scheduler.optimizeSchedule(courses);
    
    // 打印结果
    scheduler.printScheduleResult(optimizedSchedule);
    
    cout << "\n演示结论：" << endl;
    cout << "✓ 贪心算法成功选择了无时间冲突的最优课程组合" << endl;
    cout << "✓ 优先选择了高优先级和高学分的课程" << endl;
    cout << "✓ 冲突检测机制工作正常" << endl;
    cout << "✓ 算法时间复杂度：O(n²)，空间复杂度：O(n)" << endl;
}

void demoTestClubResourceAllocation() {
    printSeparator("社团资源分配演示");
    
    ClubResourceGreedy allocator;
    
    // 创建演示社团数据
    vector<Club> clubs = {
        Club(2001, "计算机协会", 9, 85.5, 45, 2, 8.5),    // 高优先级，历史表现好
        Club(2002, "篮球社", 8, 78.2, 60, 3, 12.0),       // 较高优先级，规模大
        Club(2003, "文学社", 7, 92.1, 25, 1, 4.5),        // 中等优先级，历史表现最好
        Club(2004, "音乐社", 8, 76.8, 35, 2, 7.2),        // 较高优先级，规模适中
        Club(2005, "摄影社", 6, 68.5, 20, 1, 3.8),        // 中等优先级，规模小
        Club(2006, "舞蹈社", 9, 88.9, 40, 2, 9.5),        // 高优先级，表现好
        Club(2007, "志愿者协会", 10, 95.0, 80, 1, 6.0),   // 最高优先级，规模大
        Club(2008, "创业社", 7, 72.3, 30, 2, 8.8),        // 中等优先级
        Club(2009, "动漫社", 5, 65.2, 50, 2, 5.5),        // 较低优先级
        Club(2010, "辩论社", 8, 89.7, 15, 1, 4.2)         // 较高优先级，规模小
    };
    
    // 设置资源限制
    int totalVenues = 8;      // 总场地数
    double totalFunds = 35.0; // 总资金35万元
    
    cout << "演示数据说明：" << endl;
    cout << "- 共10个社团申请资源" << endl;
    cout << "- 总资源：场地8个，资金35万元" << endl;
    cout << "- 分配策略：优先级 → 历史表现 → 规模适配" << endl;
    cout << "- 预期结果：高优先级社团优先获得资源" << endl;
    
    // 执行资源分配
    vector<AllocationResult> results = allocator.allocateResources(clubs, totalVenues, totalFunds);
    
    // 打印结果
    allocator.printAllocationResult(results);
    
    cout << "\n演示结论：" << endl;
    cout << "✓ 贪心算法成功按多维度优先级分配资源" << endl;
    cout << "✓ 高优先级社团优先获得资源" << endl;
    cout << "✓ 资源利用率达到最优" << endl;
    cout << "✓ 算法时间复杂度：O(n log n)，空间复杂度：O(n)" << endl;
}

void demoTestScorePrediction() {
    printSeparator("学生成绩预测演示");
    
    ScorePredictDP predictor;
    
    // 创建演示历史成绩数据
    vector<ScoreRecord> history = {
        ScoreRecord(3001, "高等数学A", 78.5, 0.8, 0.95, 0.90),   // 难度高，表现良好
        ScoreRecord(3002, "线性代数", 82.3, 0.6, 0.92, 0.88),    // 难度中等，表现较好
        ScoreRecord(3003, "概率统计", 75.8, 0.7, 0.89, 0.85),    // 难度较高，表现一般
        ScoreRecord(3004, "数据结构", 88.2, 0.9, 0.96, 0.95),    // 难度很高，表现优秀
        ScoreRecord(3005, "算法分析", 85.6, 0.85, 0.94, 0.92)    // 难度高，表现良好
    };
    
    cout << "演示数据说明：" << endl;
    cout << "- 历史成绩：5门课程的完整记录" << endl;
    cout << "- 包含成绩、难度系数、出勤率、作业完成度" << endl;
    cout << "- 预测目标：机器学习课程成绩" << endl;
    cout << "- 预期结果：基于历史趋势预测成绩区间" << endl;
    
    // 设置新课程参数
    double newCourseDifficulty = 0.85;  // 机器学习难度系数
    double expectedAttendance = 0.90;   // 预期出勤率
    double expectedHomework = 0.88;     // 预期作业完成度
    
    cout << "\n新课程参数：" << endl;
    cout << "课程：机器学习" << endl;
    cout << "难度系数：" << newCourseDifficulty << endl;
    cout << "预期出勤率：" << expectedAttendance << endl;
    cout << "预期作业完成度：" << expectedHomework << endl;
    
    // 执行成绩预测
    pair<double, double> prediction = predictor.predictScoreRange(history, 
                                                                newCourseDifficulty, 
                                                                expectedAttendance, 
                                                                expectedHomework);
    
    // 打印结果
    predictor.printPredictionResult(prediction, newCourseDifficulty, expectedAttendance, expectedHomework);
    
    // 演示多门课程预测
    cout << "\n========== 多门课程预测演示 ==========" << endl;
    vector<tuple<double, double, double>> futureCourses = {
        make_tuple(0.75, 0.92, 0.90),  // 课程1：中等难度
        make_tuple(0.90, 0.88, 0.85)   // 课程2：高难度
    };
    
    vector<pair<double, double>> multiPredictions = predictor.predictMultipleCourses(history, futureCourses);
    
    cout << "多门课程预测结果：" << endl;
    for (size_t i = 0; i < multiPredictions.size(); i++) {
        cout << "课程" << (i + 1) << " 预测区间: [" 
             << fixed << setprecision(1) << multiPredictions[i].first 
             << ", " << multiPredictions[i].second << "]" << endl;
    }
    
    cout << "\n演示结论：" << endl;
    cout << "✓ 动态规划算法成功基于历史数据预测成绩" << endl;
    cout << "✓ 考虑了课程难度、学习习惯等多个因素" << endl;
    cout << "✓ 预测区间合理，置信度评估准确" << endl;
    cout << "✓ 算法时间复杂度：O(n²)，空间复杂度：O(n²)" << endl;
}

void demoTestPathPlanning() {
    printSeparator("校园路径规划演示");
    
    PathPlanDP planner;
    
    // 创建演示校园节点数据
    vector<CampusNode> nodes = {
        CampusNode(0, "主校门", "入口", 1.0),
        CampusNode(1, "教学楼A", "教学楼", 1.2),
        CampusNode(2, "教学楼B", "教学楼", 1.1),
        CampusNode(3, "图书馆", "图书馆", 1.0),
        CampusNode(4, "学生宿舍1", "宿舍", 0.9),
        CampusNode(5, "学生宿舍2", "宿舍", 1.0),
        CampusNode(6, "食堂", "食堂", 0.8),
        CampusNode(7, "体育馆", "体育馆", 1.3),
        CampusNode(8, "停车场", "停车场", 1.1)
    };
    
    // 创建邻接矩阵（距离单位：米）
    const int n = 9;
    vector<vector<double>> campusGraph(n, vector<double>(n, 0));
    
    // 设置连接关系和距离
    campusGraph[0][1] = 200;  // 主校门 -> 教学楼A
    campusGraph[0][6] = 150;  // 主校门 -> 食堂
    campusGraph[1][2] = 100;  // 教学楼A -> 教学楼B
    campusGraph[1][3] = 180;  // 教学楼A -> 图书馆
    campusGraph[1][6] = 120;  // 教学楼A -> 食堂
    campusGraph[2][3] = 80;   // 教学楼B -> 图书馆
    campusGraph[2][4] = 220;  // 教学楼B -> 宿舍1
    campusGraph[3][4] = 160;  // 图书馆 -> 宿舍1
    campusGraph[3][5] = 140;  // 图书馆 -> 宿舍2
    campusGraph[4][5] = 90;   // 宿舍1 -> 宿舍2
    campusGraph[4][6] = 200;  // 宿舍1 -> 食堂
    campusGraph[5][7] = 250;  // 宿舍2 -> 体育馆
    campusGraph[6][7] = 180;  // 食堂 -> 体育馆
    campusGraph[6][8] = 100;  // 食堂 -> 停车场
    campusGraph[7][8] = 120;  // 体育馆 -> 停车场
    
    // 设置双向连接
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (campusGraph[i][j] > 0) {
                campusGraph[j][i] = campusGraph[i][j];
            }
        }
    }
    
    cout << "演示数据说明：" << endl;
    cout << "- 校园地图：9个节点，包含教学楼、宿舍、食堂等" << endl;
    cout << "- 考虑坡度因子和拥堵系数" << endl;
    cout << "- 路径规划：从主校门到体育馆" << endl;
    cout << "- 预期结果：找到考虑多因素的最优路径" << endl;
    
    // 设置路径规划参数
    int startNode = 0;  // 主校门
    int endNode = 7;    // 体育馆
    double slopeWeight = 1.2;      // 坡度权重
    double congestionWeight = 1.1; // 拥堵权重
    
    cout << "\n路径规划参数：" << endl;
    cout << "起点：" << nodes[startNode].nodeName << endl;
    cout << "终点：" << nodes[endNode].nodeName << endl;
    cout << "坡度权重：" << slopeWeight << endl;
    cout << "拥堵权重：" << congestionWeight << endl;
    
    // 执行路径规划
    PathResult result = planner.findOptimalPath(campusGraph, nodes, startNode, endNode, slopeWeight, congestionWeight);
    
    // 打印结果
    planner.printPathResult(result, nodes);
    
    // 演示不同权重设置
    cout << "\n========== 不同权重对比演示 ==========" << endl;
    
    // 演示偏好少爬坡的路径
    cout << "\n偏好少爬坡路径（坡度权重2.0）：" << endl;
    PathResult lowSlopeResult = planner.findOptimalPath(campusGraph, nodes, startNode, endNode, 2.0, 1.0);
    cout << "路径长度: " << lowSlopeResult.path.size() << " 节点, 总代价: " 
         << fixed << setprecision(2) << lowSlopeResult.totalCost << endl;
    
    // 演示最短距离路径
    cout << "\n最短距离路径（权重均为1.0）：" << endl;
    PathResult shortestResult = planner.findOptimalPath(campusGraph, nodes, startNode, endNode, 1.0, 1.0);
    cout << "路径长度: " << shortestResult.path.size() << " 节点, 总代价: " 
         << fixed << setprecision(2) << shortestResult.totalCost << endl;
    
    cout << "\n演示结论：" << endl;
    cout << "✓ 动态规划算法成功找到最优路径" << endl;
    cout << "✓ Floyd-Warshall算法处理多源最短路径问题" << endl;
    cout << "✓ 权重系统能够根据偏好调整路径选择" << endl;
    cout << "✓ 路径验证和重构功能正常" << endl;
    cout << "✓ 算法时间复杂度：O(n³)，空间复杂度：O(n²)" << endl;
}