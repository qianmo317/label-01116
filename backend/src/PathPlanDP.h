#pragma once
#include "DataStructures.h"
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <climits>
#include <cmath>

using namespace std;

/**
 * 校园路径规划类 - 动态规划算法实现
 * 功能：基于校园地图邻接矩阵，计算考虑拥堵和坡度因素的最优路径
 */
class PathPlanDP {
private:
    vector<vector<double>> originalGraph;    // 原始邻接矩阵
    vector<vector<double>> weightedGraph;    // 加权后的邻接矩阵
    vector<CampusNode> nodes;                // 校园节点信息
    vector<vector<int>> pathMatrix;          // 路径重构矩阵
    
    static const double INF;                 // 无穷大值
    static const int MAX_NODES = 100;        // 最大节点数

public:
    PathPlanDP();
    ~PathPlanDP();

    /**
     * 路径规划主函数
     * @param graph 校园地图邻接矩阵
     * @param nodeInfo 节点信息列表
     * @param start 起点编号
     * @param end 终点编号
     * @param slopeWeight 坡度权重系数（默认1.0）
     * @param congestionWeight 拥堵权重系数（默认1.0）
     * @return 路径规划结果（路径和总代价）
     */
    PathResult findOptimalPath(vector<vector<double>>& graph, 
                              vector<CampusNode>& nodeInfo,
                              int start, 
                              int end, 
                              double slopeWeight = 1.0,
                              double congestionWeight = 1.0);

    /**
     * 更新路径权重（考虑坡度和拥堵因素）
     * @param graph 原始邻接矩阵
     * @param nodeInfo 节点信息
     * @param slopeWeight 坡度权重
     * @param congestionWeight 拥堵权重
     */
    void updatePathWeights(vector<vector<double>>& graph, 
                          vector<CampusNode>& nodeInfo,
                          double slopeWeight, 
                          double congestionWeight);

    /**
     * 打印路径规划结果
     * @param result 路径规划结果
     * @param nodeInfo 节点信息
     */
    void printPathResult(const PathResult& result, const vector<CampusNode>& nodeInfo);

    /**
     * 获取两点间的直线距离估算
     * @param node1 节点1
     * @param node2 节点2
     * @return 估算距离
     */
    static double getEstimatedDistance(const CampusNode& node1, const CampusNode& node2);

    /**
     * 验证路径的有效性
     * @param path 路径
     * @param graph 邻接矩阵
     * @return true表示路径有效
     */
    bool validatePath(const vector<int>& path, const vector<vector<double>>& graph);

private:
    /**
     * Floyd-Warshall算法实现（DP核心）
     * @param graph 邻接矩阵
     * @return 最短距离矩阵
     */
    vector<vector<double>> floydWarshall(const vector<vector<double>>& graph);

    /**
     * 重构最短路径
     * @param start 起点
     * @param end 终点
     * @return 路径节点序列
     */
    vector<int> reconstructPath(int start, int end);

    /**
     * 计算路径总代价
     * @param path 路径节点序列
     * @param graph 邻接矩阵
     * @return 总代价
     */
    double calculatePathCost(const vector<int>& path, const vector<vector<double>>& graph);

    /**
     * 计算坡度影响因子
     * @param node1 起始节点
     * @param node2 目标节点
     * @param slopeWeight 坡度权重
     * @return 坡度影响因子
     */
    double calculateSlopeFactor(const CampusNode& node1, const CampusNode& node2, double slopeWeight);

    /**
     * 初始化路径矩阵
     * @param size 矩阵大小
     */
    void initializePathMatrix(int size);

    /**
     * 打印邻接矩阵（调试用）
     * @param graph 邻接矩阵
     * @param title 标题
     */
    void printGraph(const vector<vector<double>>& graph, const string& title);

    /**
     * 获取节点类型的权重系数
     * @param nodeType 节点类型
     * @return 权重系数
     */
    double getNodeTypeWeight(const string& nodeType);
};