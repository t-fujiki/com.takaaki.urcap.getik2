#pragma once

#include <iostream>
#include <vector>
using namespace std;

class DHParam
{
private:
public:
    DHParam(int ur);
    ~DHParam();

    vector<double> a;
    vector<double> d;
    vector<double> alpha;
};
