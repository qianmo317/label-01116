#include "PathPlanDP.h"
#include <functional>

const double PathPlanDP::INF = 1e9;

PathPlanDP::PathPlanDP() {
    originalGraph.clear();
    weightedGraph.clear();
    nodes.clear();
    pathMatrix.clear();
}

PathPlanDP::~PathPlanDP() {
    originalGraph.clear();
    weightedGraph.clear();
    nodes.clear();
    pathMatrix.clear();
}

PathResult PathPlanDP::findOptimalPath(vector<vector<double>>& graph, 
                                      vector<CampusNode>& nodeInfo,
                                      int start, 
                                      int end, 
                                      double slopeWeight,
                                      double congestionWeight) {
    PathResult result;
    originalGraph = graph;
    nodes = nodeInfo;
    
    cout << "\n=== 校园路径规划过程 ===" << endl;
    cout << "起点: " << start << " (" << (start < nodeInfo.size() ? nodeInfo[start].nodeName : "未知") << ")" << endl;
    cout << "终点: " << end << " (" << (end < nodeInfo.size() ? nodeInfo[end].nodeName : "未知") << ")" << endl;
    cout << "坡度权重: " << fixed << setprecision(2) << slopeWeight << endl;
    cout << "拥堵权重: " << fixed << setprecision(2) << congestionWeight << endl;
    
    // 验证输入参数
    if (start < 0 || end < 0 || start >= graph.size() || end >= graph.size()) {
        cout << "错误：起点或终点编号无效" << endl;
        result.pathFound = false;
        return result;
    }
    
    if (start == end) {
        cout << "起点和终点相同，无需规划路径" << endl;
        result.path = {start};
        result.totalCost = 0.0;
        result.totalDistance = 0.0;
        result.pathFound = true;
        return result;
    }
    
    // 显示节点信息
    cout << "\n校园节点信息:" << endl;
    cout << setw(6) << "编号" << setw(15) << "节点名" << setw(12) << "类型" << setw(10) << "坡度因子" << endl;
    cout << string(50, '-') << endl;
    
    for (size_t i = 0; i < nodeInfo.size(); i++) {
        cout << setw(6) << i 
             << setw(15) << nodeInfo[i].nodeName 
             << setw(12) << nodeInfo[i].nodeType 
             << setw(10) << fixed << setprecision(2) << nodeInfo[i].slopeFactor << endl;
    }
    
    // 更新路径权重
    updatePathWeights(graph, nodeInfo, slopeWeight, congestionWeight);
    
    // 初始化路径矩阵
    initializePathMatrix(graph.size());
    
    // 执行Floyd-Warshall算法（DP核心）
    cout << "\n执行Floyd-Warshall动态规划算法..." << endl;
    vector<vector<double>> distMatrix = floydWarshall(weightedGraph);
    
    // 检查是否存在路径
    if (distMatrix[start][end] >= INF) {
        cout << "无法找到从起点到终点的路径" << endl;
        result.pathFound = false;
        return result;
    }
    
    // 重构路径
    result.path = reconstructPath(start, end);
    result.totalCost = distMatrix[start][end];
    result.totalDistance = calculatePathCost(result.path, originalGraph);
    result.pathFound = true;
    
    // 验证路径
    if (!validatePath(result.path, originalGraph)) {
        cout << "警告：路径验证失败！" << endl;
    }
    
    return result;
}

void PathPlanDP::updatePathWeights(vector<vector<double>>& graph, 
                                  vector<CampusNode>& nodeInfo,
                                  double slopeWeight, 
                                  double congestionWeight) {
    int n = graph.size();
    weightedGraph.assign(n, vector<double>(n, INF));
    
    cout << "\n更新路径权重..." << endl;
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                weightedGraph[i][j] = 0.0;
            } else if (graph[i][j] > 0 && graph[i][j] < INF) {
                // 基础距离
                double baseDistance = graph[i][j];
                
                // 坡度影响因子
                double slopeFactor = 1.0;
                if (i < nodeInfo.size() && j < nodeInfo.size()) {
                    slopeFactor = calculateSlopeFactor(nodeInfo[i], nodeInfo[j], slopeWeight);
                }
                
                // 拥堵影响因子（这里简化为随机因子，实际应用中可以是实时数据）
                double congestionFactor = 1.0 + (congestionWeight - 1.0) * 0.2;  // 简化的拥堵模型
                
                // 节点类型权重
                double nodeTypeWeight = 1.0;
                if (j < nodeInfo.size()) {
                    nodeTypeWeight = getNodeTypeWeight(nodeInfo[j].nodeType);
                }
                
                // 综合权重计算
                weightedGraph[i][j] = baseDistance * slopeFactor * congestionFactor * nodeTypeWeight;
            }
        }
    }
    
    cout << "权重更新完成" << endl;
}

void PathPlanDP::printPathResult(const PathResult& result, const vector<CampusNode>& nodeInfo) {
    cout << "\n=== 校园路径规划结果 ===" << endl;
    
    if (!result.pathFound) {
        cout << "未找到有效路径" << endl;
        return;
    }
    
    cout << "路径长度: " << result.path.size() << " 个节点" << endl;
    cout << "总距离: " << fixed << setprecision(1) << result.totalDistance << " 米" << endl;
    cout << "总代价: " << fixed << setprecision(2) << result.totalCost << endl;
    
    cout << "\n详细路径:" << endl;
    cout << setw(6) << "步骤" << setw(6) << "节点" << setw(15) << "节点名" 
         << setw(12) << "类型" << setw(10) << "距离" << setw(12) << "累计距离" << endl;
    cout << string(70, '-') << endl;
    
    double cumulativeDistance = 0.0;
    for (size_t i = 0; i < result.path.size(); i++) {
        int nodeId = result.path[i];
        double segmentDistance = 0.0;
        
        if (i > 0) {
            int prevNode = result.path[i - 1];
            if (prevNode < originalGraph.size() && nodeId < originalGraph.size()) {
                segmentDistance = originalGraph[prevNode][nodeId];
                cumulativeDistance += segmentDistance;
            }
        }
        
        cout << setw(6) << (i + 1) 
             << setw(6) << nodeId;
        
        if (nodeId < nodeInfo.size()) {
            cout << setw(15) << nodeInfo[nodeId].nodeName 
                 << setw(12) << nodeInfo[nodeId].nodeType;
        } else {
            cout << setw(15) << "未知" << setw(12) << "未知";
        }
        
        cout << setw(10) << fixed << setprecision(1) << segmentDistance 
             << setw(12) << fixed << setprecision(1) << cumulativeDistance << endl;
    }
    
    // 路径特征分析
    cout << "\n路径特征分析:" << endl;
    
    // 统计经过的节点类型
    map<string, int> nodeTypeCount;
    for (int nodeId : result.path) {
        if (nodeId < nodeInfo.size()) {
            nodeTypeCount[nodeInfo[nodeId].nodeType]++;
        }
    }
    
    cout << "经过的节点类型:" << endl;
    for (const auto& pair : nodeTypeCount) {
        cout << "  " << pair.first << ": " << pair.second << " 个" << endl;
    }
    
    // 平均坡度
    double totalSlopeFactor = 0.0;
    int slopeCount = 0;
    for (int nodeId : result.path) {
        if (nodeId < nodeInfo.size()) {
            totalSlopeFactor += nodeInfo[nodeId].slopeFactor;
            slopeCount++;
        }
    }
    
    if (slopeCount > 0) {
        cout << "平均坡度因子: " << fixed << setprecision(2) << totalSlopeFactor / slopeCount << endl;
    }
    
    // 效率评估
    double efficiency = result.totalDistance / result.totalCost;
    cout << "路径效率: " << fixed << setprecision(2) << efficiency << " (距离/代价比)" << endl;
}

vector<vector<double>> PathPlanDP::floydWarshall(const vector<vector<double>>& graph) {
    int n = graph.size();
    vector<vector<double>> dist = graph;
    
    // 初始化路径矩阵
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j && dist[i][j] < INF) {
                pathMatrix[i][j] = i;
            } else {
                pathMatrix[i][j] = -1;
            }
        }
    }
    
    // Floyd-Warshall核心算法（三重循环DP）
    for (int k = 0; k < n; k++) {
        cout << "处理中间节点 " << k;
        if (k < nodes.size()) {
            cout << " (" << nodes[k].nodeName << ")";
        }
        cout << "..." << endl;
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    pathMatrix[i][j] = pathMatrix[k][j];
                }
            }
        }
    }
    
    return dist;
}

vector<int> PathPlanDP::reconstructPath(int start, int end) {
    vector<int> path;
    
    if (pathMatrix[start][end] == -1) {
        return path;  // 无路径
    }
    
    // 递归重构路径
    function<void(int, int)> buildPath = [&](int i, int j) {
        if (pathMatrix[i][j] == i) {
            path.push_back(i);
        } else {
            buildPath(i, pathMatrix[i][j]);
        }
    };
    
    buildPath(start, end);
    path.push_back(end);
    
    return path;
}

double PathPlanDP::calculatePathCost(const vector<int>& path, const vector<vector<double>>& graph) {
    double totalCost = 0.0;
    
    for (size_t i = 1; i < path.size(); i++) {
        int from = path[i - 1];
        int to = path[i];
        
        if (from < graph.size() && to < graph.size()) {
            totalCost += graph[from][to];
        }
    }
    
    return totalCost;
}

double PathPlanDP::calculateSlopeFactor(const CampusNode& node1, const CampusNode& node2, double slopeWeight) {
    // 计算坡度差异影响
    double slopeDiff = abs(node1.slopeFactor - node2.slopeFactor);
    
    // 上坡比下坡更困难
    double slopeDirection = (node2.slopeFactor > node1.slopeFactor) ? 1.2 : 0.9;
    
    // 综合坡度影响因子
    double slopeFactor = 1.0 + slopeDiff * slopeWeight * slopeDirection;
    
    return max(0.5, min(3.0, slopeFactor));  // 限制在合理范围内
}

void PathPlanDP::initializePathMatrix(int size) {
    pathMatrix.assign(size, vector<int>(size, -1));
}

bool PathPlanDP::validatePath(const vector<int>& path, const vector<vector<double>>& graph) {
    if (path.size() < 2) {
        return path.size() == 1;  // 单节点路径有效
    }
    
    // 检查路径连通性
    for (size_t i = 1; i < path.size(); i++) {
        int from = path[i - 1];
        int to = path[i];
        
        if (from < 0 || to < 0 || from >= graph.size() || to >= graph.size()) {
            cout << "路径验证失败: 节点编号超出范围" << endl;
            return false;
        }
        
        if (graph[from][to] <= 0 || graph[from][to] >= INF) {
            cout << "路径验证失败: 节点 " << from << " 到 " << to << " 不连通" << endl;
            return false;
        }
    }
    
    return true;
}

void PathPlanDP::printGraph(const vector<vector<double>>& graph, const string& title) {
    cout << "\n" << title << ":" << endl;
    int n = min((int)graph.size(), 10);  // 只显示前10个节点
    
    cout << setw(6) << "";
    for (int j = 0; j < n; j++) {
        cout << setw(8) << j;
    }
    cout << endl;
    
    for (int i = 0; i < n; i++) {
        cout << setw(6) << i;
        for (int j = 0; j < n; j++) {
            if (graph[i][j] >= INF) {
                cout << setw(8) << "∞";
            } else {
                cout << setw(8) << fixed << setprecision(1) << graph[i][j];
            }
        }
        cout << endl;
    }
}

double PathPlanDP::getNodeTypeWeight(const string& nodeType) {
    // 不同节点类型的权重系数
    if (nodeType == "教学楼") return 1.0;
    else if (nodeType == "宿舍") return 1.1;
    else if (nodeType == "食堂") return 0.9;
    else if (nodeType == "图书馆") return 1.0;
    else if (nodeType == "体育馆") return 1.2;
    else if (nodeType == "停车场") return 1.3;
    else return 1.0;  // 默认权重
}

double PathPlanDP::getEstimatedDistance(const CampusNode& node1, const CampusNode& node2) {
    // 这里简化为基于节点编号的估算，实际应用中应使用坐标计算
    return abs(node1.nodeId - node2.nodeId) * 100.0;  // 假设每个编号差代表100米
}