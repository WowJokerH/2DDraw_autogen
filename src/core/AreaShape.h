#pragma once
#include "Shape.h"

class AreaShape : public Shape {
public:
    static int totalCount;
    double area() const override = 0;
};
