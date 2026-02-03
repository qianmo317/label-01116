#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <iomanip>
#include <cassert>
#include "../src/CourseScheduleGreedy.h"
#include "../src/ClubResourceGreedy.h"
#include "../src/ScorePredictDP.h"
#include "../src/PathPlanDP.h"

/**
 * 性能测试模块
 * 测试各个算法在大数据量下的性能表现
 */

class PerformanceTestSuite {
private:
    std::mt19937 rng;
    
public:
    PerformanceTestSuite() : rng(std::chrono::steady_clock::now().time_since_epoch().count()) {}
    
    void runAllTests();
    void testCourseSchedulePerformance();
    void testClubResourcePerformance();
    void testScorePredictPerformance();
    void testPathPlanPerformance();
    
private:
    // 数据生成器
    std::vector<Course> generateRandomCourses(int count);
    std::vector<Club> generateRandomClubs(int count);
    std::vector<ScoreRecord> generateRandomScoreHistory(int count);
    std::vector<std::vector<double>> generateRandomGraph(int nodeCount);
    std::vector<CampusNode> generateRandomNodes(int count);
    
    // 性能测量工具
    template<typename Func>
    double measureExecutionTime(Func&& func);
    
    void printPerformanceResult(const std::string& testName, int dataSize, double timeMs, const std::string& complexity);
};

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "    学生系统后端核心模块性能测试" << std::endl;
    std::cout << "========================================" << std::endl;
    
    PerformanceTestSuite testSuite;
    testSuite.runAllTests();
    
    return 0;
}

void PerformanceTestSuite::runAllTests() {
    std::cout << "\n开始执行性能测试..." << std::endl;
    
    testCourseSchedulePerformance();
    testClubResourcePerformance();
    testScorePredictPerformance();
    testPathPlanPerformance();
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "        性能测试完成" << std::endl;
    std::cout << "========================================" << std::endl;
}

void PerformanceTestSuite::testCourseSchedulePerformance() {
    std::cout << "\n=== 课程表优化性能测试 ===" << std::endl;
    std::cout << "算法类型: 贪心算法" << std::endl;
    std::cout << "理论复杂度: 时间O(n²), 空间O(n)" << std::endl;
    
    CourseScheduleGreedy scheduler;
    std::vector<int> testSizes = {10, 50, 100, 500, 1000};
    
    for (int size : testSizes) {
        try {
            auto courses = generateRandomCourses(size);
            
            double timeMs = measureExecutionTime([&]() {
                scheduler.optimizeSchedule(courses);
            });
            
            printPerformanceResult("课程表优化", size, timeMs, "O(n²)");
            
        } catch (const std::exception& e) {
            std::cout << "测试失败 (n=" << size << "): " << e.what() << std::endl;
        }
    }
}

void PerformanceTestSuite::testClubResourcePerformance() {
    std::cout << "\n=== 社团资源分配性能测试 ===" << std::endl;
    std::cout << "算法类型: 贪心算法" << std::endl;
    std::cout << "理论复杂度: 时间O(n log n), 空间O(n)" << std::endl;
    
    ClubResourceGreedy allocator;
    std::vector<int> testSizes = {10, 50, 100, 500, 1000, 5000};
    
    for (int size : testSizes) {
        try {
            auto clubs = generateRandomClubs(size);
            int venues = size / 10 + 1;
            double funds = size * 5.0;
            
            double timeMs = measureExecutionTime([&]() {
                allocator.allocateResources(clubs, venues, funds);
            });
            
            printPerformanceResult("社团资源分配", size, timeMs, "O(n log n)");
            
        } catch (const std::exception& e) {
            std::cout << "测试失败 (n=" << size << "): " << e.what() << std::endl;
        }
    }
}

void PerformanceTestSuite::testScorePredictPerformance() {
    std::cout << "\n=== 学生成绩预测性能测试 ===" << std::endl;
    std::cout << "算法类型: 动态规划" << std::endl;
    std::cout << "理论复杂度: 时间O(n²), 空间O(n²)" << std::endl;
    
    ScorePredictDP predictor;
    std::vector<int> testSizes = {5, 10, 20, 50, 100};
    
    for (int size : testSizes) {
        try {
            auto history = generateRandomScoreHistory(size);
            
            double timeMs = measureExecutionTime([&]() {
                predictor.predictScoreRange(history, 0.8, 0.9, 0.85);
            });
            
            printPerformanceResult("学生成绩预测", size, timeMs, "O(n²)");
            
        } catch (const std::exception& e) {
            std::cout << "测试失败 (n=" << size << "): " << e.what() << std::endl;
        }
    }
}

void PerformanceTestSuite::testPathPlanPerformance() {
    std::cout << "\n=== 校园路径规划性能测试 ===" << std::endl;
    std::cout << "算法类型: 动态规划 (Floyd-Warshall)" << std::endl;
    std::cout << "理论复杂度: 时间O(n³), 空间O(n²)" << std::endl;
    
    PathPlanDP planner;
    std::vector<int> testSizes = {5, 10, 20, 50, 100};
    
    for (int size : testSizes) {
        try {
            auto graph = generateRandomGraph(size);
            auto nodes = generateRandomNodes(size);
            
            double timeMs = measureExecutionTime([&]() {
                planner.findOptimalPath(graph, nodes, 0, size-1);
            });
            
            printPerformanceResult("校园路径规划", size, timeMs, "O(n³)");
            
        } catch (const std::exception& e) {
            std::cout << "测试失败 (n=" << size << "): " << e.what() << std::endl;
        }
    }
}

std::vector<Course> PerformanceTestSuite::generateRandomCourses(int count) {
    std::vector<Course> courses;
    std::uniform_int_distribution<int> idDist(1000, 9999);
    std::uniform_int_distribution<int> timeDist(480, 1080); // 8:00-18:00
    std::uniform_int_distribution<int> dayDist(1, 7);
    std::uniform_int_distribution<int> creditDist(1, 6);
    std::uniform_int_distribution<int> priorityDist(1, 10);
    std::uniform_int_distribution<int> durationDist(60, 180); // 1-3小时
    
    for (int i = 0; i < count; i++) {
        int startTime = timeDist(rng);
        int duration = durationDist(rng);
        int endTime = startTime + duration;
        
        courses.emplace_back(
            idDist(rng),
            "课程" + std::to_string(i + 1),
            startTime,
            endTime,
            dayDist(rng),
            creditDist(rng),
            priorityDist(rng)
        );
    }
    
    return courses;
}

std::vector<Club> PerformanceTestSuite::generateRandomClubs(int count) {
    std::vector<Club> clubs;
    std::uniform_int_distribution<int> idDist(2000, 9999);
    std::uniform_int_distribution<int> priorityDist(1, 10);
    std::uniform_real_distribution<double> scoreDist(60.0, 100.0);
    std::uniform_int_distribution<int> scaleDist(10, 100);
    std::uniform_int_distribution<int> venueDist(1, 5);
    std::uniform_real_distribution<double> fundDist(1.0, 20.0);
    
    for (int i = 0; i < count; i++) {
        clubs.emplace_back(
            idDist(rng),
            "社团" + std::to_string(i + 1),
            priorityDist(rng),
            scoreDist(rng),
            scaleDist(rng),
            venueDist(rng),
            fundDist(rng)
        );
    }
    
    return clubs;
}

std::vector<ScoreRecord> PerformanceTestSuite::generateRandomScoreHistory(int count) {
    std::vector<ScoreRecord> history;
    std::uniform_int_distribution<int> idDist(3000, 9999);
    std::uniform_real_distribution<double> scoreDist(60.0, 95.0);
    std::uniform_real_distribution<double> difficultyDist(0.3, 1.0);
    std::uniform_real_distribution<double> rateDist(0.7, 1.0);
    
    for (int i = 0; i < count; i++) {
        history.emplace_back(
            idDist(rng),
            "历史课程" + std::to_string(i + 1),
            scoreDist(rng),
            difficultyDist(rng),
            rateDist(rng),
            rateDist(rng)
        );
    }
    
    return history;
}

std::vector<std::vector<double>> PerformanceTestSuite::generateRandomGraph(int nodeCount) {
    std::vector<std::vector<double>> graph(nodeCount, std::vector<double>(nodeCount, 0.0));
    std::uniform_real_distribution<double> distDist(50.0, 500.0);
    std::uniform_real_distribution<double> probDist(0.0, 1.0);
    
    for (int i = 0; i < nodeCount; i++) {
        for (int j = i + 1; j < nodeCount; j++) {
            // 30%的概率有连接
            if (probDist(rng) < 0.3) {
                double distance = distDist(rng);
                graph[i][j] = distance;
                graph[j][i] = distance;
            }
        }
    }
    
    // 确保图连通
    for (int i = 1; i < nodeCount; i++) {
        if (graph[i-1][i] == 0.0) {
            double distance = distDist(rng);
            graph[i-1][i] = distance;
            graph[i][i-1] = distance;
        }
    }
    
    return graph;
}

std::vector<CampusNode> PerformanceTestSuite::generateRandomNodes(int count) {
    std::vector<CampusNode> nodes;
    std::uniform_real_distribution<double> slopeDist(0.5, 2.0);
    std::vector<std::string> types = {"教学楼", "宿舍", "食堂", "图书馆", "体育馆"};
    std::uniform_int_distribution<int> typeDist(0, types.size() - 1);
    
    for (int i = 0; i < count; i++) {
        nodes.emplace_back(
            i,
            "节点" + std::to_string(i),
            types[typeDist(rng)],
            slopeDist(rng)
        );
    }
    
    return nodes;
}

template<typename Func>
double PerformanceTestSuite::measureExecutionTime(Func&& func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    return duration.count() / 1000.0; // 转换为毫秒
}

void PerformanceTestSuite::printPerformanceResult(const std::string& testName, int dataSize, double timeMs, const std::string& complexity) {
    std::cout << std::setw(15) << testName 
              << " | n=" << std::setw(5) << dataSize 
              << " | 时间: " << std::setw(8) << std::fixed << std::setprecision(2) << timeMs << "ms"
              << " | 复杂度: " << complexity << std::endl;
}