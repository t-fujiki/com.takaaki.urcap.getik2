#pragma once

class DHParam
{
private:
public:
    DHParam(int ur);
    ~DHParam();

    double a[7];
    double d[7];
    double alpha[7];
};
