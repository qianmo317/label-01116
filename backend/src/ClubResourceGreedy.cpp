#include "ClubResourceGreedy.h"
#include <cmath>

ClubResourceGreedy::ClubResourceGreedy() : totalVenues(0), totalFunds(0.0), remainingVenues(0), remainingFunds(0.0) {
    allocationResults.clear();
}

ClubResourceGreedy::~ClubResourceGreedy() {
    allocationResults.clear();
}

vector<AllocationResult> ClubResourceGreedy::allocateResources(vector<Club>& clubs, int venues, double funds) {
    // 初始化资源
    totalVenues = venues;
    totalFunds = funds;
    remainingVenues = venues;
    remainingFunds = funds;
    clearAllocation();
    
    // 按多维度优先级排序（贪心策略的核心）
    sort(clubs.begin(), clubs.end(), compareClubs);
    
    cout << "\n=== 社团资源分配过程 ===" << endl;
    cout << "总资源: 场地 " << totalVenues << " 个, 资金 " << fixed << setprecision(2) << totalFunds << " 万元" << endl;
    
    cout << "\n按优先级排序后的社团列表：" << endl;
    cout << setw(6) << "编号" << setw(12) << "社团名" << setw(8) << "优先级" 
         << setw(10) << "历史得分" << setw(8) << "规模" << setw(8) << "需场地" << setw(10) << "需资金" << endl;
    cout << string(75, '-') << endl;
    
    for (const auto& club : clubs) {
        cout << setw(6) << club.clubId 
             << setw(12) << club.clubName 
             << setw(8) << club.priority 
             << setw(10) << fixed << setprecision(1) << club.historyScore 
             << setw(8) << club.activityScale 
             << setw(8) << club.venueNeeded 
             << setw(10) << fixed << setprecision(2) << club.fundNeeded << endl;
    }
    
    // 贪心分配过程
    cout << "\n贪心分配过程：" << endl;
    for (const auto& club : clubs) {
        AllocationResult result;
        result.club = club;
        
        if (canAllocate(club, remainingVenues, remainingFunds)) {
            // 完全分配
            result.allocatedVenues = club.venueNeeded;
            result.allocatedFunds = club.fundNeeded;
            result.isFullyAllocated = true;
            
            remainingVenues -= club.venueNeeded;
            remainingFunds -= club.fundNeeded;
            
            cout << "✓ 完全分配 " << club.clubName 
                 << ": 场地 " << club.venueNeeded << " 个, 资金 " << fixed << setprecision(2) << club.fundNeeded << " 万元" << endl;
        } else {
            // 尝试部分分配
            result = tryPartialAllocation(club, remainingVenues, remainingFunds);
            
            if (result.allocatedVenues > 0 || result.allocatedFunds > 0) {
                remainingVenues -= result.allocatedVenues;
                remainingFunds -= result.allocatedFunds;
                
                cout << "◐ 部分分配 " << club.clubName 
                     << ": 场地 " << result.allocatedVenues << "/" << club.venueNeeded 
                     << " 个, 资金 " << fixed << setprecision(2) << result.allocatedFunds << "/" << club.fundNeeded << " 万元" << endl;
            } else {
                cout << "✗ 无法分配 " << club.clubName << " (资源不足)" << endl;
            }
        }
        
        allocationResults.push_back(result);
    }
    
    // 验证结果
    if (!validateAllocation(allocationResults)) {
        cout << "警告：资源分配验证失败！" << endl;
    }
    
    return allocationResults;
}

bool ClubResourceGreedy::canAllocate(const Club& club, int availableVenues, double availableFunds) {
    return (club.venueNeeded <= availableVenues) && (club.fundNeeded <= availableFunds);
}

bool ClubResourceGreedy::compareClubs(const Club& a, const Club& b) {
    // 计算综合评分
    double scoreA = a.priority * 10.0 + a.historyScore * 0.5;
    double scoreB = b.priority * 10.0 + b.historyScore * 0.5;
    
    // 加入规模适配度评分（规模适中的社团优先）
    scoreA += (100.0 - abs(a.activityScale - 50)) * 0.1;  // 50人规模为最优
    scoreB += (100.0 - abs(b.activityScale - 50)) * 0.1;
    
    if (abs(scoreA - scoreB) > 0.01) {  // 避免浮点数比较问题
        return scoreA > scoreB;
    }
    
    // 评分相同时，按社团编号排序
    return a.clubId < b.clubId;
}

void ClubResourceGreedy::printAllocationResult(const vector<AllocationResult>& results) {
    cout << "\n=== 社团资源分配结果 ===" << endl;
    
    int fullyAllocatedCount = 0;
    int partiallyAllocatedCount = 0;
    int notAllocatedCount = 0;
    
    cout << setw(6) << "编号" << setw(12) << "社团名" << setw(10) << "分配场地" 
         << setw(12) << "分配资金" << setw(12) << "分配状态" << endl;
    cout << string(65, '-') << endl;
    
    for (const auto& result : results) {
        string status;
        if (result.isFullyAllocated) {
            status = "完全分配";
            fullyAllocatedCount++;
        } else if (result.allocatedVenues > 0 || result.allocatedFunds > 0) {
            status = "部分分配";
            partiallyAllocatedCount++;
        } else {
            status = "未分配";
            notAllocatedCount++;
        }
        
        cout << setw(6) << result.club.clubId 
             << setw(12) << result.club.clubName 
             << setw(6) << result.allocatedVenues << "/" << result.club.venueNeeded
             << setw(8) << fixed << setprecision(2) << result.allocatedFunds << "/" << result.club.fundNeeded
             << setw(12) << status << endl;
    }
    
    cout << "\n分配统计：" << endl;
    cout << "完全分配: " << fullyAllocatedCount << " 个社团" << endl;
    cout << "部分分配: " << partiallyAllocatedCount << " 个社团" << endl;
    cout << "未分配: " << notAllocatedCount << " 个社团" << endl;
    
    printResourceUtilization();
}

void ClubResourceGreedy::printResourceUtilization() {
    int usedVenues = totalVenues - remainingVenues;
    double usedFunds = totalFunds - remainingFunds;
    
    cout << "\n资源利用率：" << endl;
    cout << "场地利用率: " << fixed << setprecision(1) 
         << (totalVenues > 0 ? (double)usedVenues / totalVenues * 100 : 0) << "% (" 
         << usedVenues << "/" << totalVenues << ")" << endl;
    cout << "资金利用率: " << fixed << setprecision(1) 
         << (totalFunds > 0 ? usedFunds / totalFunds * 100 : 0) << "% (" 
         << fixed << setprecision(2) << usedFunds << "/" << totalFunds << " 万元)" << endl;
}

void ClubResourceGreedy::clearAllocation() {
    allocationResults.clear();
}

double ClubResourceGreedy::calculateClubScore(const Club& club) {
    // 综合评分公式：优先级权重60% + 历史表现权重30% + 规模适配度权重10%
    double priorityScore = club.priority * 0.6;
    double historyScore = (club.historyScore / 100.0) * 0.3;
    double scaleScore = calculateScaleScore(club.activityScale) * 0.1;
    
    return priorityScore + historyScore + scaleScore;
}

double ClubResourceGreedy::calculateScaleScore(int activityScale) {
    // 规模适配度评分：50人规模为最优（得分1.0），偏离越多得分越低
    const int optimalScale = 50;
    const int maxDeviation = 50;
    
    int deviation = abs(activityScale - optimalScale);
    if (deviation >= maxDeviation) {
        return 0.0;
    }
    
    return 1.0 - (double)deviation / maxDeviation;
}

AllocationResult ClubResourceGreedy::tryPartialAllocation(const Club& club, int availableVenues, double availableFunds) {
    AllocationResult result;
    result.club = club;
    result.isFullyAllocated = false;
    
    // 优先分配场地（场地是离散资源，更难分配）
    result.allocatedVenues = min(club.venueNeeded, availableVenues);
    
    // 然后分配资金（可以部分分配）
    result.allocatedFunds = min(club.fundNeeded, availableFunds);
    
    // 如果场地无法满足最低需求，则不分配资金
    if (result.allocatedVenues == 0 && club.venueNeeded > 0) {
        result.allocatedFunds = 0.0;
    }
    
    return result;
}

bool ClubResourceGreedy::validateAllocation(const vector<AllocationResult>& results) {
    int totalAllocatedVenues = 0;
    double totalAllocatedFunds = 0.0;
    
    // 统计总分配量
    for (const auto& result : results) {
        totalAllocatedVenues += result.allocatedVenues;
        totalAllocatedFunds += result.allocatedFunds;
        
        // 检查分配量不能超过需求量
        if (result.allocatedVenues > result.club.venueNeeded) {
            cout << "验证失败: " << result.club.clubName << " 分配场地超过需求" << endl;
            return false;
        }
        if (result.allocatedFunds > result.club.fundNeeded + 0.01) {  // 浮点数比较容差
            cout << "验证失败: " << result.club.clubName << " 分配资金超过需求" << endl;
            return false;
        }
    }
    
    // 检查总分配量不能超过总资源
    if (totalAllocatedVenues > totalVenues) {
        cout << "验证失败: 总分配场地超过可用场地" << endl;
        return false;
    }
    if (totalAllocatedFunds > totalFunds + 0.01) {  // 浮点数比较容差
        cout << "验证失败: 总分配资金超过可用资金" << endl;
        return false;
    }
    
    return true;
}