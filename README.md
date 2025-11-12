# TwoDDraw - 2D图形绘制应用程序

一个基于 Qt6 的 2D 图形绘制应用程序,支持绘制多种基本图形和几何形状。

## 📋 项目简介

TwoDDraw 是一个使用 C++ 和 Qt6 框架开发的 2D 图形绘制工具,提供了直观的用户界面和丰富的图形绘制功能。

## 🏗️ 项目结构

```
TwoDDraw/
├── CMakeLists.txt          # CMake 构建配置文件
├── readme.md               # 项目说明文档
├── src/                    # 源代码目录
│   ├── main.cpp           # 程序入口
│   ├── core/              # 核心功能模块
│   │   ├── Shape.h/cpp            # 图形基类
│   │   ├── LineShape.h/cpp        # 线型图形基类
│   │   ├── AreaShape.h/cpp        # 区域图形基类
│   │   └── shapes/                # 具体图形实现
│   │       ├── Segment.h/cpp      # 线段
│   │       ├── Polyline.h/cpp     # 折线
│   │       ├── Polygon.h/cpp      # 多边形
│   │       ├── Triangle.h/cpp     # 三角形
│   │       ├── Rectangle.h/cpp    # 矩形
│   │       ├── Circle.h/cpp       # 圆形
│   │       └── Ellipse.h/cpp      # 椭圆
│   └── ui/                # 用户界面模块
│       ├── MainWindow.h/cpp       # 主窗口
│       └── Canvas.h/cpp           # 画布组件
└── build/                 # 构建输出目录
```

## ✨ 功能特性

### 支持的图形类型

- **线型图形**
  - 线段 (Segment)
  - 折线 (Polyline)

- **区域图形**
  - 三角形 (Triangle)
  - 矩形 (Rectangle)
  - 多边形 (Polygon)
  - 圆形 (Circle)
  - 椭圆 (Ellipse)

## 🔧 技术栈

- **编程语言**: C++17
- **GUI 框架**: Qt 6.7.2
- **构建工具**: CMake 3.20+
- **编译器**: MinGW 11.2.0 (GCC)

## 📦 环境要求

### 必需软件

1. **Qt 6.7.2 或更高版本**
   - 需要 Qt Widgets 模块
   - 推荐使用 Qt 官方安装器

2. **CMake 3.20 或更高版本**

3. **C++ 编译器**
   - Windows: MinGW 11.2.0+
   - Linux: GCC 11.2.0+ 或 Clang
   - macOS: Xcode Command Line Tools

## 🚀 本地构建步骤

### Windows (MinGW)

```bash
# 1. 克隆或下载项目
cd TwoDDraw

# 2. 创建构建目录
mkdir build
cd build

# 3. 配置 CMake (指定 Qt 路径)
cmake .. -DCMAKE_PREFIX_PATH="D:/AppData/QT/6.7.2/mingw_64"

# 4. 编译项目
cmake --build .

# 5. 运行程序
./TwoDDraw.exe
```

### Linux

```bash
# 1. 克隆或下载项目
cd TwoDDraw

# 2. 创建构建目录
mkdir build
cd build

# 3. 配置 CMake
cmake .. -DCMAKE_PREFIX_PATH="/path/to/Qt/6.7.2/gcc_64"

# 4. 编译项目
make -j$(nproc)

# 5. 运行程序
./TwoDDraw
```

### macOS

```bash
# 1. 克隆或下载项目
cd TwoDDraw

# 2. 创建构建目录
mkdir build
cd build

# 3. 配置 CMake
cmake .. -DCMAKE_PREFIX_PATH="/path/to/Qt/6.7.2/macos"

# 4. 编译项目
make -j$(sysctl -n hw.ncpu)

# 5. 运行程序
./TwoDDraw
```

## ⚙️ CMake 配置选项

在 `CMakeLists.txt` 中已配置以下选项:

- `CMAKE_CXX_STANDARD`: C++17
- `CMAKE_AUTOMOC`: ON (自动 MOC 处理)
- `CMAKE_AUTORCC`: ON (自动资源编译)
- `CMAKE_AUTOUIC`: ON (自动 UI 编译)

## 🔍 Qt 路径配置

如果 CMake 无法自动找到 Qt,请手动指定 Qt 安装路径:

```bash
# Windows
cmake .. -DCMAKE_PREFIX_PATH="D:/AppData/QT/6.7.2/mingw_64"

# Linux
cmake .. -DCMAKE_PREFIX_PATH="/home/user/Qt/6.7.2/gcc_64"

# macOS
cmake .. -DCMAKE_PREFIX_PATH="/Users/user/Qt/6.7.2/macos"
```

## 🐛 常见问题

### 问题 1: CMake 找不到 Qt6

**解决方案**: 设置 `CMAKE_PREFIX_PATH` 环境变量或在 CMake 命令中指定

```bash
cmake .. -DCMAKE_PREFIX_PATH="<Qt安装路径>"
```

### 问题 2: 编译器版本不兼容

**解决方案**: 确保使用的编译器版本支持 C++17 标准

### 问题 3: MOC 文件生成失败

**解决方案**: 清理构建目录并重新构建

```bash
cd build
cmake --build . --clean-first
```

## 📝 开发说明

### 添加新图形

1. 在 `src/core/shapes/` 创建新的图形类
2. 继承 `LineShape` 或 `AreaShape`
3. 实现必要的虚函数
4. 在 `CMakeLists.txt` 中添加新文件

### 代码风格

- 使用 4 空格缩进
- 类名使用 PascalCase
- 函数名和变量名使用 camelCase
- 头文件使用 `#pragma once`

## 👥 贡献

欢迎提交 Issue 和 Pull Request!

## 📧 联系方式

如有问题或建议,请提交 Issue 反馈。

---

**注意**: 请确保在构建前正确安装并配置 Qt 6.7.2 及相关依赖。
