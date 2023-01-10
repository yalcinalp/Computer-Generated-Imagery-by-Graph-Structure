#pragma once

#include <cmath>

struct IdPair
{
    int vertexId0;
    int vertexId1;
};

struct Double3
{
    double x, y, z;
    static double Distance(Double3 left, Double3 right);
};

inline
double Double3::Distance(Double3 left, Double3 right)
{
    double first = (left.x - right.x);
    double second = (left.y - right.y);
    double third = (left.z - right.z);

    return sqrt(pow(first, 2) + pow(second, 2) + pow(third, 2));
}
