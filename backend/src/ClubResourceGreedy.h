#pragma once
#include "DataStructures.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>

using namespace std;

/**
 * 社团资源分配类 - 贪心算法实现
 * 功能：根据多维度优先级为社团分配有限的场地和资金资源
 */
class ClubResourceGreedy {
private:
    vector<AllocationResult> allocationResults;  // 分配结果列表
    int totalVenues;                             // 总场地数
    double totalFunds;                           // 总资金
    int remainingVenues;                         // 剩余场地数
    double remainingFunds;                       // 剩余资金

public:
    ClubResourceGreedy();
    ~ClubResourceGreedy();

    /**
     * 资源分配主函数
     * @param clubs 社团信息列表
     * @param venues 总场地数量
     * @param funds 总资金额度
     * @return 分配结果列表
     */
    vector<AllocationResult> allocateResources(vector<Club>& clubs, int venues, double funds);

    /**
     * 检查是否可以为社团分配资源
     * @param club 社团信息
     * @param availableVenues 可用场地数
     * @param availableFunds 可用资金
     * @return true表示可以分配，false表示资源不足
     */
    bool canAllocate(const Club& club, int availableVenues, double availableFunds);

    /**
     * 社团优先级比较函数（用于排序）
     * 排序规则：优先级高 -> 历史表现好 -> 规模适配（规模适中优先）
     */
    static bool compareClubs(const Club& a, const Club& b);

    /**
     * 打印资源分配结果
     * @param results 分配结果列表
     */
    void printAllocationResult(const vector<AllocationResult>& results);

    /**
     * 获取资源利用率统计
     */
    void printResourceUtilization();

    /**
     * 清空分配结果
     */
    void clearAllocation();

private:
    /**
     * 计算社团的综合评分
     * @param club 社团信息
     * @return 综合评分
     */
    double calculateClubScore(const Club& club);

    /**
     * 计算规模适配度得分
     * @param activityScale 活动规模
     * @return 适配度得分（规模适中得分更高）
     */
    double calculateScaleScore(int activityScale);

    /**
     * 尝试部分分配资源
     * @param club 社团信息
     * @param availableVenues 可用场地数
     * @param availableFunds 可用资金
     * @return 分配结果
     */
    AllocationResult tryPartialAllocation(const Club& club, int availableVenues, double availableFunds);

    /**
     * 验证分配结果的合理性
     * @param results 分配结果列表
     * @return true表示验证通过
     */
    bool validateAllocation(const vector<AllocationResult>& results);
};