#include "CourseScheduleGreedy.h"

CourseScheduleGreedy::CourseScheduleGreedy() : totalCredits(0), totalPriorityScore(0.0) {
    selectedCourses.clear();
}

CourseScheduleGreedy::~CourseScheduleGreedy() {
    selectedCourses.clear();
}

void CourseScheduleGreedy::validateInput(const std::vector<Course>& courses) {
    if (courses.empty()) {
        throw InvalidInputException("课程列表不能为空");
    }
    
    if (courses.size() > 100) {
        throw InvalidInputException("课程数量不能超过100门");
    }
    
    for (const auto& course : courses) {
        validateCourse(course);
    }
}

void CourseScheduleGreedy::validateCourse(const Course& course) {
    if (course.courseId <= 0) {
        throw InvalidInputException("课程编号必须为正整数");
    }
    
    if (course.courseName.empty()) {
        throw InvalidInputException("课程名称不能为空");
    }
    
    if (course.courseName.length() > 50) {
        throw InvalidInputException("课程名称长度不能超过50个字符");
    }
    
    if (course.startTime < 0 || course.startTime >= 1440) {
        throw InvalidInputException("开始时间必须在0-1439分钟之间");
    }
    
    if (course.endTime <= course.startTime || course.endTime > 1440) {
        throw InvalidInputException("结束时间必须大于开始时间且不超过1440分钟");
    }
    
    if (course.dayOfWeek < 1 || course.dayOfWeek > 7) {
        throw InvalidInputException("星期数必须在1-7之间");
    }
    
    if (course.credits < 1 || course.credits > 10) {
        throw InvalidInputException("学分必须在1-10之间");
    }
    
    if (course.priority < 1 || course.priority > 10) {
        throw InvalidInputException("优先级必须在1-10之间");
    }
}

std::vector<Course> CourseScheduleGreedy::optimizeSchedule(std::vector<Course>& courses) {
    try {
        // 输入验证
        validateInput(courses);
        
        // 清空之前的选择
        clearSelection();
        
        // 按优先级排序（贪心策略的核心）
        std::sort(courses.begin(), courses.end(), compareCourses);
        
        std::cout << "\n=== 课程表优化过程 ===" << std::endl;
        std::cout << "按优先级排序后的课程列表：" << std::endl;
        std::cout << std::setw(6) << "编号" << std::setw(16) << "课程名" << std::setw(8) << "优先级" 
                  << std::setw(6) << "学分" << std::setw(12) << "时间段" << std::setw(8) << "星期" << std::endl;
        std::cout << std::string(62, '-') << std::endl;
        
        for (const auto& course : courses) {
            std::cout << std::setw(6) << course.courseId 
                      << std::setw(16) << course.courseName 
                      << std::setw(8) << course.priority 
                      << std::setw(6) << course.credits 
                      << std::setw(6) << course.startTime/60 << ":" << std::setfill('0') << std::setw(2) << course.startTime%60 
                      << "-" << course.endTime/60 << ":" << std::setw(2) << course.endTime%60 << std::setfill(' ')
                      << std::setw(8) << course.dayOfWeek << std::endl;
        }
        
        // 贪心选择过程
        std::cout << "\n贪心选择过程：" << std::endl;
        for (const auto& course : courses) {
            if (!hasConflictWithSelected(course)) {
                selectedCourses.push_back(course);
                totalCredits += course.credits;
                totalPriorityScore += course.priority;
                
                std::cout << "✓ 选择课程: " << course.courseName 
                          << " (优先级:" << course.priority << ", 学分:" << course.credits << ")" << std::endl;
            } else {
                std::cout << "✗ 跳过课程: " << course.courseName << " (时间冲突)" << std::endl;
            }
        }
        
        // 验证结果
        if (!validateSchedule(selectedCourses)) {
            throw AlgorithmException("课程表验证失败");
        }
        
        return selectedCourses;
        
    } catch (const InvalidInputException& e) {
        std::cerr << "输入验证错误: " << e.what() << std::endl;
        throw;
    } catch (const AlgorithmException& e) {
        std::cerr << "算法执行错误: " << e.what() << std::endl;
        throw;
    } catch (const std::exception& e) {
        std::cerr << "未知错误: " << e.what() << std::endl;
        throw AlgorithmException("课程表优化过程中发生未知错误");
    }
}

bool CourseScheduleGreedy::hasTimeConflict(const Course& a, const Course& b) {
    // 不同星期的课程不冲突
    if (a.dayOfWeek != b.dayOfWeek) {
        return false;
    }
    
    // 检查时间段是否重叠
    // 情况1：A的开始时间在B的时间段内
    if (a.startTime >= b.startTime && a.startTime < b.endTime) {
        return true;
    }
    
    // 情况2：A的结束时间在B的时间段内
    if (a.endTime > b.startTime && a.endTime <= b.endTime) {
        return true;
    }
    
    // 情况3：A完全包含B
    if (a.startTime <= b.startTime && a.endTime >= b.endTime) {
        return true;
    }
    
    return false;
}

bool CourseScheduleGreedy::hasConflictWithSelected(const Course& newCourse) {
    for (const auto& selected : selectedCourses) {
        if (hasTimeConflict(newCourse, selected)) {
            return true;
        }
    }
    return false;
}

bool CourseScheduleGreedy::compareCourses(const Course& a, const Course& b) {
    // 优先级高的排在前面
    if (a.priority != b.priority) {
        return a.priority > b.priority;
    }
    
    // 优先级相同时，学分高的排在前面
    if (a.credits != b.credits) {
        return a.credits > b.credits;
    }
    
    // 都相同时，课程编号小的排在前面
    return a.courseId < b.courseId;
}

void CourseScheduleGreedy::printScheduleResult(const std::vector<Course>& result) {
    std::cout << "\n=== 课程表优化结果 ===" << std::endl;
    std::cout << "总共选择了 " << result.size() << " 门课程" << std::endl;
    std::cout << "总学分: " << totalCredits << std::endl;
    std::cout << "总优先级得分: " << totalPriorityScore << std::endl;
    std::cout << "平均优先级: " << std::fixed << std::setprecision(2) 
              << (result.empty() ? 0.0 : totalPriorityScore / result.size()) << std::endl;
    
    std::cout << "\n详细课程表：" << std::endl;
    std::cout << std::setw(6) << "编号" << std::setw(16) << "课程名" << std::setw(8) << "优先级" 
              << std::setw(6) << "学分" << std::setw(12) << "时间段" << std::setw(8) << "星期" << std::endl;
    std::cout << std::string(62, '-') << std::endl;
    
    // 按星期和时间排序显示
    std::vector<Course> sortedResult = result;
    std::sort(sortedResult.begin(), sortedResult.end(), [](const Course& a, const Course& b) {
        if (a.dayOfWeek != b.dayOfWeek) return a.dayOfWeek < b.dayOfWeek;
        return a.startTime < b.startTime;
    });
    
    for (const auto& course : sortedResult) {
        std::cout << std::setw(6) << course.courseId 
                  << std::setw(16) << course.courseName 
                  << std::setw(8) << course.priority 
                  << std::setw(6) << course.credits 
                  << std::setw(6) << course.startTime/60 << ":" << std::setfill('0') << std::setw(2) << course.startTime%60 
                  << "-" << course.endTime/60 << ":" << std::setw(2) << course.endTime%60 << std::setfill(' ')
                  << std::setw(8) << course.dayOfWeek << std::endl;
    }
    
    std::cout << "\n冲突检测验证：" << (validateSchedule(result) ? "通过" : "失败") << std::endl;
}

void CourseScheduleGreedy::clearSelection() {
    selectedCourses.clear();
    totalCredits = 0;
    totalPriorityScore = 0.0;
}

double CourseScheduleGreedy::calculateTotalScore(const std::vector<Course>& courses) {
    double score = 0.0;
    for (const auto& course : courses) {
        // 综合考虑优先级和学分的权重
        score += course.priority * 1.0 + course.credits * 0.5;
    }
    return score;
}

bool CourseScheduleGreedy::validateSchedule(const std::vector<Course>& courses) {
    // 检查是否存在时间冲突
    for (size_t i = 0; i < courses.size(); i++) {
        for (size_t j = i + 1; j < courses.size(); j++) {
            if (hasTimeConflict(courses[i], courses[j])) {
                std::cout << "发现冲突: " << courses[i].courseName 
                          << " 与 " << courses[j].courseName << std::endl;
                return false;
            }
        }
    }
    
    // 检查课程信息的合理性
    for (const auto& course : courses) {
        try {
            validateCourse(course);
        } catch (const InvalidInputException& e) {
            std::cout << "课程验证失败: " << course.courseName << " - " << e.what() << std::endl;
            return false;
        }
    }
    
    return true;
}