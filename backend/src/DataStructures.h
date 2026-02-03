#pragma once
#include <string>
#include <vector>

/**
 * 课程信息结构体
 * 用于课程表优化模块
 */
struct Course {
    int courseId;           // 课程编号
    std::string courseName; // 课程名称
    int startTime;          // 开始时间（分钟，从0点开始计算）
    int endTime;            // 结束时间（分钟）
    int dayOfWeek;          // 星期几（1-7，1为周一）
    int credits;            // 学分
    int priority;           // 兴趣优先级（1-10，10为最高）

    Course() : courseId(0), startTime(0), endTime(0), dayOfWeek(1), credits(0), priority(1) {}
    
    Course(int id, const std::string& name, int start, int end, int day, int cred, int prio)
        : courseId(id), courseName(name), startTime(start), endTime(end), 
          dayOfWeek(day), credits(cred), priority(prio) {}
};

/**
 * 社团信息结构体
 * 用于社团资源分配模块
 */
struct Club {
    int clubId;             // 社团编号
    std::string clubName;   // 社团名称
    int priority;           // 优先级（1-10，10为最高）
    double historyScore;    // 历史表现分（0-100）
    int activityScale;      // 活动规模（人数）
    int venueNeeded;        // 需要场地数
    double fundNeeded;      // 需要资金额度

    Club() : clubId(0), priority(1), historyScore(0.0), activityScale(0), venueNeeded(0), fundNeeded(0.0) {}
    
    Club(int id, const std::string& name, int prio, double history, int scale, int venue, double fund)
        : clubId(id), clubName(name), priority(prio), historyScore(history), 
          activityScale(scale), venueNeeded(venue), fundNeeded(fund) {}
};

/**
 * 成绩记录结构体
 * 用于学生成绩预测模块
 */
struct ScoreRecord {
    int courseId;           // 课程编号
    std::string courseName; // 课程名称
    double score;           // 成绩（0-100）
    double difficulty;      // 课程难度系数（0-1，1为最难）
    double attendance;      // 出勤率（0-1）
    double homeworkRate;    // 作业完成度（0-1）

    ScoreRecord() : courseId(0), score(0.0), difficulty(0.5), attendance(1.0), homeworkRate(1.0) {}
    
    ScoreRecord(int id, const std::string& name, double sc, double diff, double att, double hw)
        : courseId(id), courseName(name), score(sc), difficulty(diff), attendance(att), homeworkRate(hw) {}
};

/**
 * 校园节点信息结构体
 * 用于校园路径规划模块
 */
struct CampusNode {
    int nodeId;             // 节点编号
    std::string nodeName;   // 节点名称（如"教学楼A"）
    std::string nodeType;   // 节点类型（教学楼/宿舍/食堂等）
    double slopeFactor;     // 坡度因子（影响路径权重）

    CampusNode() : nodeId(0), slopeFactor(1.0) {}
    
    CampusNode(int id, const std::string& name, const std::string& type, double slope = 1.0)
        : nodeId(id), nodeName(name), nodeType(type), slopeFactor(slope) {}
};

/**
 * 路径边信息结构体
 */
struct PathEdge {
    int fromNode;           // 起始节点
    int toNode;             // 目标节点
    double distance;        // 距离（米）
    double congestionFactor; // 拥堵系数（1.0为正常，>1.0为拥堵）

    PathEdge() : fromNode(0), toNode(0), distance(0.0), congestionFactor(1.0) {}
    
    PathEdge(int from, int to, double dist, double congestion = 1.0)
        : fromNode(from), toNode(to), distance(dist), congestionFactor(congestion) {}
};

/**
 * 资源分配结果结构体
 */
struct AllocationResult {
    Club club;              // 分配的社团
    int allocatedVenues;    // 分配的场地数
    double allocatedFunds;  // 分配的资金
    bool isFullyAllocated;  // 是否完全满足需求

    AllocationResult() : allocatedVenues(0), allocatedFunds(0.0), isFullyAllocated(false) {}
};

/**
 * 路径规划结果结构体
 */
struct PathResult {
    std::vector<int> path;  // 路径节点序列
    double totalCost;       // 总代价
    double totalDistance;   // 总距离
    bool pathFound;         // 是否找到路径

    PathResult() : totalCost(0.0), totalDistance(0.0), pathFound(false) {}
};

/**
 * 自定义异常类
 */
class StudentSystemException : public std::exception {
private:
    std::string message;
    std::string errorCode;

public:
    StudentSystemException(const std::string& msg, const std::string& code = "UNKNOWN")
        : message(msg), errorCode(code) {}
    
    const char* what() const noexcept override {
        return message.c_str();
    }
    
    const std::string& getErrorCode() const noexcept {
        return errorCode;
    }
};

/**
 * 输入验证异常
 */
class InvalidInputException : public StudentSystemException {
public:
    InvalidInputException(const std::string& msg) 
        : StudentSystemException("输入验证失败: " + msg, "INVALID_INPUT") {}
};

/**
 * 算法执行异常
 */
class AlgorithmException : public StudentSystemException {
public:
    AlgorithmException(const std::string& msg) 
        : StudentSystemException("算法执行失败: " + msg, "ALGORITHM_ERROR") {}
};

/**
 * 数据处理异常
 */
class DataProcessingException : public StudentSystemException {
public:
    DataProcessingException(const std::string& msg) 
        : StudentSystemException("数据处理失败: " + msg, "DATA_ERROR") {}
};