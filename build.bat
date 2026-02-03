@echo off
echo ========================================
echo     学生系统后端核心模块编译脚本
echo ========================================

echo 正在编译主程序(用户交互版)...
g++ -std=c++17 -O2 -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter ^
    src/main.cpp ^
    src/CourseScheduleGreedy.cpp ^
    src/ClubResourceGreedy.cpp ^
    src/ScorePredictDP.cpp ^
    src/PathPlanDP.cpp ^
    -o StudentSystemCore.exe

if %ERRORLEVEL% EQU 0 (
    echo ✓ 主程序编译成功！
    echo 可执行文件: StudentSystemCore.exe
) else (
    echo ✗ 主程序编译失败！
    goto :end
)

echo.
echo 正在编译演示测试程序...
g++ -std=c++17 -O2 -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter ^
    tests/DemoTestCases.cpp ^
    src/CourseScheduleGreedy.cpp ^
    src/ClubResourceGreedy.cpp ^
    src/ScorePredictDP.cpp ^
    src/PathPlanDP.cpp ^
    -o DemoTestCases.exe

if %ERRORLEVEL% EQU 0 (
    echo ✓ 演示测试程序编译成功！
    echo 可执行文件: DemoTestCases.exe
) else (
    echo ✗ 演示测试程序编译失败！
    goto :end
)

echo.
echo 正在编译单元测试程序...
g++ -std=c++17 -O2 -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter ^
    tests/TestCases.cpp ^
    src/CourseScheduleGreedy.cpp ^
    src/ClubResourceGreedy.cpp ^
    src/ScorePredictDP.cpp ^
    src/PathPlanDP.cpp ^
    -o UnitTestCases.exe

if %ERRORLEVEL% EQU 0 (
    echo ✓ 单元测试程序编译成功！
    echo 可执行文件: UnitTestCases.exe
) else (
    echo ✗ 单元测试程序编译失败！
    goto :end
)

echo.
echo 正在编译性能测试程序...
g++ -std=c++17 -O2 -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter ^
    backend/tests/PerformanceTests.cpp ^
    backend/src/CourseScheduleGreedy.cpp ^
    backend/src/ClubResourceGreedy.cpp ^
    backend/src/ScorePredictDP.cpp ^
    backend/src/PathPlanDP.cpp ^
    -o PerformanceTests.exe

if %ERRORLEVEL% EQU 0 (
    echo ✓ 性能测试程序编译成功！
    echo 可执行文件: PerformanceTests.exe
) else (
    echo ✗ 性能测试程序编译失败！
    goto :end
)

echo.
echo ========================================
echo 编译完成！可以运行以下程序：
echo 1. StudentSystemCore.exe - 主程序(支持用户输入)
echo 2. DemoTestCases.exe - 演示测试(预设数据)
echo 3. UnitTestCases.exe - 单元测试(边界条件)
echo 4. PerformanceTests.exe - 性能测试(大数据量)
echo ========================================

:end
pause