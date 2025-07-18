# 冒泡排序可视化程序

这是一个使用 Dear ImGui 实现的冒泡排序可视化程序，可以直观地展示冒泡排序算法的执行过程。

## 功能特点

- 🎯 **实时可视化**：以柱状图形式展示排序过程中数组元素的变化
- 🎮 **交互控制**：支持开始、暂停、单步执行等操作
- 📊 **统计信息**：显示排序步骤数、交换次数等统计数据
- 🎨 **颜色标识**：
  - 红色：正在比较的元素
  - 绿色：已排序完成的元素
  - 蓝色：未排序的元素
- ⚙️ **参数调节**：可调节数组大小和排序速度

## 项目结构

```
my-test/
├── main.cpp                           # 主程序源代码
├── BubbleSortVisualization.vcxproj    # Visual Studio 项目文件
├── BubbleSortVisualization.vcxproj.filters # VS 项目过滤器
├── build.bat                          # 编译批处理脚本
├── .vscode/
│   ├── tasks.json                     # VS Code 任务配置
│   └── c_cpp_properties.json          # C++ 智能感知配置
├── Debug/                             # 编译输出目录
└── README.md                          # 本文件
```

## 编译要求

- Windows 10/11
- Visual Studio 2022 (Community/Professional/Enterprise)
- DirectX 11 支持
- C++17 或更高版本

## 编译方法

### 方法一：使用批处理文件（推荐）

1. 打开 "x64 Native Tools Command Prompt for VS 2022"
2. 切换到项目目录：
   ```cmd
   cd /d "d:\Git\Project\Information-Security\Data-Structure\design\imgui\my-test"
   ```
3. 运行编译脚本：
   ```cmd
   build.bat
   ```

### 方法二：使用 Visual Studio

1. 用 Visual Studio 2022 打开 `BubbleSortVisualization.vcxproj`
2. 选择 Debug|x64 配置
3. 按 F5 或点击"调试"→"开始执行"

### 方法三：使用 VS Code

1. 在 VS Code 中打开项目根目录（imgui 文件夹）
2. 按 `Ctrl+Shift+P` 打开命令面板
3. 运行 "Tasks: Run Task"
4. 选择 "Build Bubble Sort Visualization"

### 方法四：使用 MSBuild

1. 打开 "x64 Native Tools Command Prompt for VS 2022"
2. 切换到项目目录
3. 运行：
   ```cmd
   msbuild BubbleSortVisualization.vcxproj /p:Configuration=Debug /p:Platform=x64
   ```

## 使用说明

1. 启动程序后会看到两个窗口：
   - **控制面板**：包含各种控制按钮和统计信息
   - **排序可视化**：显示数组的柱状图表示

2. **控制操作**：
   - 调节数组大小（5-50个元素）
   - 调节排序速度（10-1000毫秒间隔）
   - 点击"重置数组"生成新的随机数组
   - 点击"开始排序"开始可视化
   - 在排序过程中可以"暂停"或"单步执行"

3. **观察要点**：
   - 红色柱子表示当前正在比较的两个元素
   - 绿色柱子表示已经排序完成的元素
   - 蓝色柱子表示尚未排序的元素
   - 统计面板显示当前的循环进度和操作次数

## 技术实现

- **UI 框架**：Dear ImGui
- **图形 API**：DirectX 11
- **平台后端**：Win32
- **算法实现**：逐步执行的冒泡排序，支持状态保存和恢复

## 扩展建议

可以在此基础上添加：
- 其他排序算法（快排、归并排序等）
- 音效反馈
- 性能对比
- 算法复杂度分析
- 导出排序过程为视频

## 故障排除

如果编译失败：
1. 确保已安装 Visual Studio 2022 并包含 C++ 开发工具
2. 确保系统安装了 Windows 10 SDK
3. 检查 DirectX 11 是否正常工作
4. 如果是路径问题，检查项目文件中的相对路径是否正确
