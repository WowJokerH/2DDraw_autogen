cmake_minimum_required(VERSION 3.20)
project(TwoDDraw LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
add_executable(TwoDDraw
  src/main.cpp
  src/ui/MainWindow.h
  src/ui/MainWindow.cpp
  src/ui/Canvas.h
  src/ui/Canvas.cpp
  src/core/Shape.h
  src/core/Shape.cpp
  src/core/LineShape.h
  src/core/LineShape.cpp
  src/core/AreaShape.h
  src/core/AreaShape.cpp
  src/core/shapes/Segment.h
  src/core/shapes/Segment.cpp
  src/core/shapes/Polyline.h
  src/core/shapes/Polyline.cpp
  src/core/shapes/Triangle.h
  src/core/shapes/Triangle.cpp
  src/core/shapes/Rectangle.h
  src/core/shapes/Rectangle.cpp
  src/core/shapes/Polygon.h
  src/core/shapes/Polygon.cpp
  src/core/shapes/Circle.h
  src/core/shapes/Circle.cpp
  src/core/shapes/Ellipse.h
  src/core/shapes/Ellipse.cpp
)
target_link_libraries(TwoDDraw Qt6::Widgets)
