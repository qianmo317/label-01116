#include <iostream>
#include <cassert>
#include <vector>
#include <cmath>
#include "../src/CourseScheduleGreedy.h"
#include "../src/ClubResourceGreedy.h"
#include "../src/ScorePredictDP.h"
#include "../src/PathPlanDP.h"

using namespace std;

/**
 * 单元测试模块
 * 验证各个算法模块的正确性和边界条件处理
 */

void testCourseScheduleEdgeCases();
void testClubResourceEdgeCases();
void testScorePredictEdgeCases();
void testPathPlanEdgeCases();
void runAllTests();

int main() {
    cout << "========================================" << endl;
    cout << "        单元测试执行程序" << endl;
    cout << "========================================" << endl;
    
    runAllTests();
    
    cout << "\n========================================" << endl;
    cout << "        所有测试执行完成" << endl;
    cout << "========================================" << endl;
    
    return 0;
}

void runAllTests() {
    cout << "\n开始执行单元测试..." << endl;
    
    try {
        testCourseScheduleEdgeCases();
        testClubResourceEdgeCases();
        testScorePredictEdgeCases();
        testPathPlanEdgeCases();
        
        cout << "\n✓ 所有测试用例通过！" << endl;
    } catch (const exception& e) {
        cout << "\n✗ 测试失败: " << e.what() << endl;
    }
}

void testCourseScheduleEdgeCases() {
    cout << "\n=== 课程表优化边界测试 ===" << endl;
    
    CourseScheduleGreedy scheduler;
    
    // 测试1：空课程列表
    cout << "测试1: 空课程列表..." << endl;
    vector<Course> emptyCourses;
    vector<Course> result1 = scheduler.optimizeSchedule(emptyCourses);
    assert(result1.empty());
    cout << "✓ 空课程列表测试通过" << endl;
    
    // 测试2：单门课程
    cout << "测试2: 单门课程..." << endl;
    vector<Course> singleCourse = {
        Course(1, "测试课程", 480, 570, 1, 3, 8)
    };
    vector<Course> result2 = scheduler.optimizeSchedule(singleCourse);
    assert(result2.size() == 1);
    assert(result2[0].courseId == 1);
    cout << "✓ 单门课程测试通过" << endl;
    
    // 测试3：全部冲突的课程
    cout << "测试3: 全部冲突的课程..." << endl;
    vector<Course> conflictCourses = {
        Course(1, "课程1", 480, 570, 1, 3, 10),  // 最高优先级
        Course(2, "课程2", 500, 590, 1, 3, 9),   // 与课程1冲突
        Course(3, "课程3", 520, 610, 1, 3, 8)    // 与课程1、2冲突
    };
    vector<Course> result3 = scheduler.optimizeSchedule(conflictCourses);
    assert(result3.size() == 1);
    assert(result3[0].courseId == 1);  // 应该选择优先级最高的
    cout << "✓ 全部冲突课程测试通过" << endl;
    
    // 测试4：时间边界测试
    cout << "测试4: 时间边界测试..." << endl;
    vector<Course> boundaryCourses = {
        Course(1, "课程1", 480, 570, 1, 3, 8),   // 8:00-9:30
        Course(2, "课程2", 570, 660, 1, 3, 9)    // 9:30-11:00，刚好不冲突
    };
    vector<Course> result4 = scheduler.optimizeSchedule(boundaryCourses);
    assert(result4.size() == 2);  // 两门课都应该被选中
    cout << "✓ 时间边界测试通过" << endl;
}

void testClubResourceEdgeCases() {
    cout << "\n=== 社团资源分配边界测试 ===" << endl;
    
    ClubResourceGreedy allocator;
    
    // 测试1：资源为0
    cout << "测试1: 零资源分配..." << endl;
    vector<Club> clubs1 = {
        Club(1, "社团1", 10, 90.0, 50, 1, 5.0)
    };
    vector<AllocationResult> result1 = allocator.allocateResources(clubs1, 0, 0.0);
    assert(result1.size() == 1);
    assert(result1[0].allocatedVenues == 0);
    assert(result1[0].allocatedFunds == 0.0);
    cout << "✓ 零资源分配测试通过" << endl;
    
    // 测试2：资源充足
    cout << "测试2: 资源充足分配..." << endl;
    vector<Club> clubs2 = {
        Club(1, "社团1", 8, 80.0, 30, 2, 5.0),
        Club(2, "社团2", 9, 85.0, 40, 1, 3.0)
    };
    vector<AllocationResult> result2 = allocator.allocateResources(clubs2, 10, 20.0);
    assert(result2.size() == 2);
    // 所有社团都应该得到完全分配
    for (const auto& res : result2) {
        assert(res.isFullyAllocated);
    }
    cout << "✓ 资源充足分配测试通过" << endl;
    
    // 测试3：单一资源不足
    cout << "测试3: 单一资源不足..." << endl;
    vector<Club> clubs3 = {
        Club(1, "社团1", 10, 95.0, 50, 5, 2.0)  // 需要5个场地，但只有3个
    };
    vector<AllocationResult> result3 = allocator.allocateResources(clubs3, 3, 10.0);
    assert(result3.size() == 1);
    assert(result3[0].allocatedVenues <= 3);
    cout << "✓ 单一资源不足测试通过" << endl;
}

void testScorePredictEdgeCases() {
    cout << "\n=== 成绩预测边界测试 ===" << endl;
    
    ScorePredictDP predictor;
    
    // 测试1：无历史记录
    cout << "测试1: 无历史记录预测..." << endl;
    vector<ScoreRecord> emptyHistory;
    pair<double, double> result1 = predictor.predictScoreRange(emptyHistory, 0.8, 0.9, 0.85);
    assert(result1.first >= 0 && result1.first <= 100);
    assert(result1.second >= 0 && result1.second <= 100);
    assert(result1.first <= result1.second);
    cout << "✓ 无历史记录预测测试通过" << endl;
    
    // 测试2：单一历史记录
    cout << "测试2: 单一历史记录..." << endl;
    vector<ScoreRecord> singleHistory = {
        ScoreRecord(1, "课程1", 85.0, 0.7, 0.9, 0.88)
    };
    pair<double, double> result2 = predictor.predictScoreRange(singleHistory, 0.8, 0.9, 0.85);
    assert(result2.first >= 0 && result2.first <= 100);
    assert(result2.second >= 0 && result2.second <= 100);
    assert(result2.first <= result2.second);
    cout << "✓ 单一历史记录测试通过" << endl;
    
    // 测试3：极端参数值
    cout << "测试3: 极端参数值..." << endl;
    vector<ScoreRecord> history3 = {
        ScoreRecord(1, "课程1", 95.0, 0.9, 1.0, 1.0),
        ScoreRecord(2, "课程2", 90.0, 0.8, 0.95, 0.98)
    };
    // 极高难度，极低出勤率和作业完成度
    pair<double, double> result3 = predictor.predictScoreRange(history3, 1.0, 0.1, 0.1);
    assert(result3.first >= 0 && result3.first <= 100);
    assert(result3.second >= 0 && result3.second <= 100);
    cout << "✓ 极端参数值测试通过" << endl;
    
    // 测试4：成绩区间索引
    cout << "测试4: 成绩区间索引..." << endl;
    assert(ScorePredictDP::getScoreRangeIndex(45.0) == 0);  // 不及格
    assert(ScorePredictDP::getScoreRangeIndex(65.0) == 1);  // 及格
    assert(ScorePredictDP::getScoreRangeIndex(75.0) == 2);  // 中等
    assert(ScorePredictDP::getScoreRangeIndex(85.0) == 3);  // 良好
    assert(ScorePredictDP::getScoreRangeIndex(95.0) == 4);  // 优秀
    assert(ScorePredictDP::getScoreRangeIndex(100.0) == 4); // 边界情况
    cout << "✓ 成绩区间索引测试通过" << endl;
}

void testPathPlanEdgeCases() {
    cout << "\n=== 路径规划边界测试 ===" << endl;
    
    PathPlanDP planner;
    
    // 测试1：起点终点相同
    cout << "测试1: 起点终点相同..." << endl;
    vector<vector<double>> graph1(3, vector<double>(3, 0));
    vector<CampusNode> nodes1 = {
        CampusNode(0, "节点0", "测试", 1.0),
        CampusNode(1, "节点1", "测试", 1.0),
        CampusNode(2, "节点2", "测试", 1.0)
    };
    PathResult result1 = planner.findOptimalPath(graph1, nodes1, 0, 0);
    assert(result1.pathFound);
    assert(result1.path.size() == 1);
    assert(result1.path[0] == 0);
    assert(result1.totalCost == 0.0);
    cout << "✓ 起点终点相同测试通过" << endl;
    
    // 测试2：无连通路径
    cout << "测试2: 无连通路径..." << endl;
    vector<vector<double>> graph2(3, vector<double>(3, 0));
    graph2[0][1] = 100;  // 只有0->1的连接
    graph2[1][0] = 100;
    // 节点2是孤立的
    vector<CampusNode> nodes2 = {
        CampusNode(0, "节点0", "测试", 1.0),
        CampusNode(1, "节点1", "测试", 1.0),
        CampusNode(2, "节点2", "测试", 1.0)
    };
    PathResult result2 = planner.findOptimalPath(graph2, nodes2, 0, 2);
    assert(!result2.pathFound);  // 应该找不到路径
    cout << "✓ 无连通路径测试通过" << endl;
    
    // 测试3：单边图
    cout << "测试3: 单边连接..." << endl;
    vector<vector<double>> graph3(2, vector<double>(2, 0));
    graph3[0][1] = 150;
    graph3[1][0] = 150;
    vector<CampusNode> nodes3 = {
        CampusNode(0, "起点", "测试", 1.0),
        CampusNode(1, "终点", "测试", 1.0)
    };
    PathResult result3 = planner.findOptimalPath(graph3, nodes3, 0, 1);
    assert(result3.pathFound);
    assert(result3.path.size() == 2);
    assert(result3.path[0] == 0 && result3.path[1] == 1);
    cout << "✓ 单边连接测试通过" << endl;
    
    // 测试4：路径验证功能
    cout << "测试4: 路径验证..." << endl;
    vector<int> validPath = {0, 1};
    vector<int> invalidPath = {0, 2};  // 0->2不连通
    assert(planner.validatePath(validPath, graph3));
    assert(!planner.validatePath(invalidPath, graph3));
    cout << "✓ 路径验证测试通过" << endl;
    
    // 测试5：权重影响测试
    cout << "测试5: 权重影响..." << endl;
    vector<vector<double>> graph5(3, vector<double>(3, 0));
    graph5[0][1] = 100; graph5[1][0] = 100;
    graph5[1][2] = 100; graph5[2][1] = 100;
    graph5[0][2] = 150; graph5[2][0] = 150;  // 直接路径稍长
    
    vector<CampusNode> nodes5 = {
        CampusNode(0, "起点", "测试", 1.0),
        CampusNode(1, "中转", "测试", 2.0),  // 高坡度
        CampusNode(2, "终点", "测试", 1.0)
    };
    
    // 低坡度权重：应该选择经过中转点的路径
    PathResult lowSlopeResult = planner.findOptimalPath(graph5, nodes5, 0, 2, 1.0);
    // 高坡度权重：应该选择直接路径
    PathResult highSlopeResult = planner.findOptimalPath(graph5, nodes5, 0, 2, 3.0);
    
    assert(lowSlopeResult.pathFound && highSlopeResult.pathFound);
    cout << "✓ 权重影响测试通过" << endl;
}