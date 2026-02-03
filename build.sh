#!/bin/bash

echo "========================================"
echo "    学生系统后端核心模块编译脚本"
echo "========================================"

echo "正在编译主程序(用户交互版)..."
g++ -std=c++17 -O2 -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter \
    backend/src/main.cpp \
    backend/src/CourseScheduleGreedy.cpp \
    backend/src/ClubResourceGreedy.cpp \
    backend/src/ScorePredictDP.cpp \
    backend/src/PathPlanDP.cpp \
    -o StudentSystemCore

if [ $? -eq 0 ]; then
    echo "✓ 主程序编译成功！"
    echo "可执行文件: StudentSystemCore"
else
    echo "✗ 主程序编译失败！"
    exit 1
fi

echo ""
echo "正在编译演示测试程序..."
g++ -std=c++17 -O2 -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter \
    backend/tests/DemoTestCases.cpp \
    backend/src/CourseScheduleGreedy.cpp \
    backend/src/ClubResourceGreedy.cpp \
    backend/src/ScorePredictDP.cpp \
    backend/src/PathPlanDP.cpp \
    -o DemoTestCases

if [ $? -eq 0 ]; then
    echo "✓ 演示测试程序编译成功！"
    echo "可执行文件: DemoTestCases"
else
    echo "✗ 演示测试程序编译失败！"
    exit 1
fi

echo ""
echo "正在编译单元测试程序..."
g++ -std=c++17 -O2 -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter \
    backend/tests/TestCases.cpp \
    backend/src/CourseScheduleGreedy.cpp \
    backend/src/ClubResourceGreedy.cpp \
    backend/src/ScorePredictDP.cpp \
    backend/src/PathPlanDP.cpp \
    -o UnitTestCases

if [ $? -eq 0 ]; then
    echo "✓ 单元测试程序编译成功！"
    echo "可执行文件: UnitTestCases"
else
    echo "✗ 单元测试程序编译失败！"
    exit 1
fi

echo ""
echo "正在编译性能测试程序..."
g++ -std=c++17 -O2 -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter \
    backend/tests/PerformanceTests.cpp \
    backend/src/CourseScheduleGreedy.cpp \
    backend/src/ClubResourceGreedy.cpp \
    backend/src/ScorePredictDP.cpp \
    backend/src/PathPlanDP.cpp \
    -o PerformanceTests

if [ $? -eq 0 ]; then
    echo "✓ 性能测试程序编译成功！"
    echo "可执行文件: PerformanceTests"
else
    echo "✗ 性能测试程序编译失败！"
    exit 1
fi

echo ""
echo "========================================"
echo "编译完成！可以运行以下程序："
echo "1. ./StudentSystemCore - 主程序(支持用户输入)"
echo "2. ./DemoTestCases - 演示测试(预设数据)"
echo "3. ./UnitTestCases - 单元测试(边界条件)"
echo "4. ./PerformanceTests - 性能测试(大数据量)"
echo "========================================"

# 设置执行权限
chmod +x StudentSystemCore
chmod +x DemoTestCases
chmod +x UnitTestCases
chmod +x PerformanceTests

echo "已设置可执行权限"