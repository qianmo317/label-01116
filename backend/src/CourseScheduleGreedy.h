#pragma once
#include "DataStructures.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <stdexcept>

/**
 * 课程表优化类 - 贪心算法实现
 * 功能：根据学生课程偏好，生成无时间冲突且优先级最高的课程组合
 */
class CourseScheduleGreedy {
private:
    std::vector<Course> selectedCourses;  // 已选择的课程列表
    int totalCredits;                     // 总学分
    double totalPriorityScore;            // 总优先级得分

    /**
     * 验证输入参数
     */
    void validateInput(const std::vector<Course>& courses);
    
    /**
     * 验证单个课程数据
     */
    void validateCourse(const Course& course);

public:
    CourseScheduleGreedy();
    ~CourseScheduleGreedy();

    /**
     * 课程表优化主函数
     * @param courses 输入的课程偏好列表
     * @return 优化后的无冲突课程组合
     * @throws InvalidInputException 输入数据无效
     * @throws AlgorithmException 算法执行失败
     */
    std::vector<Course> optimizeSchedule(std::vector<Course>& courses);

    /**
     * 检查两门课程是否存在时间冲突
     * @param a 课程A
     * @param b 课程B
     * @return true表示有冲突，false表示无冲突
     */
    bool hasTimeConflict(const Course& a, const Course& b);

    /**
     * 检查新课程与已选课程是否冲突
     * @param newCourse 待检查的新课程
     * @return true表示有冲突，false表示无冲突
     */
    bool hasConflictWithSelected(const Course& newCourse);

    /**
     * 课程优先级比较函数（用于排序）
     * 排序规则：优先级高 -> 学分高 -> 课程编号小
     */
    static bool compareCourses(const Course& a, const Course& b);

    /**
     * 打印课程表优化结果
     * @param result 优化结果课程列表
     */
    void printScheduleResult(const std::vector<Course>& result);

    /**
     * 获取总学分
     */
    int getTotalCredits() const { return totalCredits; }

    /**
     * 获取总优先级得分
     */
    double getTotalPriorityScore() const { return totalPriorityScore; }

    /**
     * 清空选择结果
     */
    void clearSelection();

private:
    /**
     * 计算课程组合的总体得分
     * @param courses 课程列表
     * @return 总体得分
     */
    double calculateTotalScore(const std::vector<Course>& courses);

    /**
     * 验证课程表的完整性和正确性
     * @param courses 课程列表
     * @return true表示验证通过
     */
    bool validateSchedule(const std::vector<Course>& courses);
};