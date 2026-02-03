# 学生系统后端核心模块

## How to Run

### 环境要求
- Visual Studio 2022 (Windows)
- GCC 7.0+ 支持C++17 (Linux/Mac)
- C++17 或更高版本

### 编译运行步骤

#### 方法1：使用编译脚本（推荐）

##### Windows系统
```bash
# 运行编译脚本
build.bat

# 编译完成后会生成可执行文件：
# StudentSystemCore.exe - 主程序
```

##### Linux/Mac系统
```bash
# 给脚本执行权限
chmod +x build.sh

# 运行编译脚本
./build.sh

# 编译完成后会生成可执行文件：
# ./StudentSystemCore - 主程序
```

#### 方法2：使用Visual Studio 2022
```
1. 打开Visual Studio 2022
2. 打开解决方案文件: StudentSystemCore.sln
3. 选择Release或Debug配置
4. 按F5或点击"开始调试"编译运行
```

#### 方法3：手动编译
```bash
# 编译主程序
g++ -std=c++17 -O2 backend/src/main.cpp backend/src/CourseScheduleGreedy.cpp backend/src/ClubResourceGreedy.cpp backend/src/ScorePredictDP.cpp backend/src/PathPlanDP.cpp -o StudentSystemCore
```


### 运行交互程序
```bash
# Windows
build.bat
StudentSystemCore.exe

# Linux/Mac
chmod +x build.sh && ./build.sh
./StudentSystemCore
```

### 运行演示程序
```bash
# Windows
build.bat
DemoTestCases.exe

# Linux/Mac
chmod +x build.sh && ./build.sh
./DemoTestCases
```

## Services

### 项目概述
基于C++开发的学生系统后端核心模块，适配Visual Studio 2022控制台工程。实现4个核心功能：课程表优化、社团资源分配、学生成绩预测、校园路径规划。

### 功能模块

1. **课程表优化** - 贪心算法实现无时间冲突的最优课程选择
   - 输入：学生课程偏好列表（含课程编号、时间、学分、兴趣优先级）
   - 输出：无时间冲突且优先级最高的课程组合
   - 特性：冲突检测逻辑、优先级排序、学分最大化

2. **社团资源分配** - 贪心算法实现多维度优先级资源分配
   - 输入：资源总量（场地数量、资金额度）、社团信息（优先级、历史表现分、活动规模）
   - 输出：资源分配结果
   - 分配顺序：优先级高→历史表现好→规模适配

3. **学生成绩预测** - 动态规划算法预测未来课程成绩区间
   - 输入：学生历史成绩序列、课程难度系数、学习习惯权重（出勤率、作业完成度）
   - 输出：未来1-2门课程的成绩区间
   - 特性：状态转移方程、多因子权重计算

4. **校园路径规划** - 动态规划算法计算最优路径
   - 输入：校园地图邻接矩阵（节点为教学楼/宿舍/食堂，边为路径长度+拥堵系数）
   - 输出：最短路径及总代价
   - 特性：支持自定义权重（如偏好少爬坡路径）

## 测试账号

本项目为学生系统后端核心模块，实现的是算法功能而非完整的用户系统，因此不需要测试账号。用户可以直接运行程序，使用预设的测试数据或自定义输入数据进行功能测试。

## 题目内容
请基于 C++ 开发学生系统后端核心模块，适配 Visual Studio 2022 控制台/静态库工程，要求实现以下 4 个核心功能，架构简单且满足作业级项目需求： 
1.  课程表优化：实现贪心算法，输入学生课程偏好列表（含课程编号、时间、学分、兴趣优先级），输出无时间冲突且优先级最高的课程组合，需提供冲突检测逻辑。 
2.  社团资源分配：实现贪心算法，输入资源总量（场地数量、资金额度）、社团信息（优先级、历史表现分、活动规模），按“优先级高→历史表现好→规模适配”的顺序分配资源，输出分配结果。 
3.  学生成绩预测：实现动态规划算法，输入学生历史成绩序列、课程难度系数、学习习惯权重（如出勤率、作业完成度），构建状态转移方程，预测未来 1-2 门课程的成绩区间。 
4.  校园路径规划：实现动态规划算法，输入校园地图邻接矩阵（节点为教学楼/宿舍/食堂，边为路径长度+拥堵系数），输入起点和终点，输出最短路径及总代价，支持自定义权重（如偏好少爬坡路径）。 
要求： 
- 每个功能独立封装为类（如 CourseScheduleGreedy、ClubResourceGreedy、ScorePredictDP、PathPlanDP），提供清晰的接口函数。 
- 避免复杂设计模式，使用标准库容器（vector、map），代码带详细注释，可直接在 Visual Studio 中编译运行。 
- 提供测试用例，包含输入输出示例，验证每个算法的正确性。

### 项目结构
```
label-01116/
  ├── backend/              # 后端核心代码目录
  │   ├── src/              # 源代码目录
  │   │   ├── ClubResourceGreedy.cpp    # 社团资源分配实现
  │   │   ├── ClubResourceGreedy.h      # 社团资源分配头文件
  │   │   ├── CourseScheduleGreedy.cpp  # 课程表优化实现
  │   │   ├── CourseScheduleGreedy.h    # 课程表优化头文件
  │   │   ├── DataStructures.h          # 数据结构定义
  │   │   ├── PathPlanDP.cpp            # 校园路径规划实现
  │   │   ├── PathPlanDP.h              # 校园路径规划头文件
  │   │   ├── ScorePredictDP.cpp        # 学生成绩预测实现
  │   │   ├── ScorePredictDP.h          # 学生成绩预测头文件
  │   │   └── main.cpp                  # 主程序入口
  │   └── tests/            # 测试代码目录
  │       ├── DemoTestCases.cpp         # 演示测试用例
  │       └── TestCases.cpp             # 单元测试用例
  ├── docs/                 # 文档目录
  │   ├── QUICKSTART.md                 # 快速开始指南
  │   └── project_design.md             # 项目设计文档
  ├── .gitignore            # Git忽略文件
  ├── README.md             # 项目说明文档
  ├── build.bat             # Windows编译脚本
  └── build.sh              # Linux/Mac编译脚本
```


