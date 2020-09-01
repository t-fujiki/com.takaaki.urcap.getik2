#include <cstdlib>
#include <cmath>

#include "DHParam.hpp"
#include "ur.hpp"

DHParam::DHParam(int ur)
{

    if (ur == UR::UR3)
    {
        a.push_back(0);
        a.push_back(-0.24365);
        a.push_back(-0.21325);
        a.push_back(0);
        a.push_back(0);
        a.push_back(0);

        d.push_back(0.1519);
        d.push_back(0);
        d.push_back(0);
        d.push_back(0.11235);
        d.push_back(0.08535);
        d.push_back(0.0819);
    }
    else if (ur == UR::UR5)
    {
        a.push_back(0);
        a.push_back(-0.425);
        a.push_back(-0.39225);
        a.push_back(0);
        a.push_back(0);
        a.push_back(0);

        d.push_back(0.089159);
        d.push_back(0);
        d.push_back(0);
        d.push_back(0.10915);
        d.push_back(0.09465);
        d.push_back(0.0823);
    }
    else if (ur == UR::UR10)
    {
        a.push_back(0);
        a.push_back(-0.612);
        a.push_back(-0.5723);
        a.push_back(0);
        a.push_back(0);
        a.push_back(0);

        d.push_back(0.1273);
        d.push_back(0);
        d.push_back(0);
        d.push_back(0.163941);
        d.push_back(0.1157);
        d.push_back(0.0922);
    }
    else if (ur == UR::UR3e)
    {
        a.push_back(0);
        a.push_back(-0.24355);
        a.push_back(-0.2132);
        a.push_back(0);
        a.push_back(0);
        a.push_back(0);

        d.push_back(0.15185);
        d.push_back(0);
        d.push_back(0);
        d.push_back(0.13105);
        d.push_back(0.08535);
        d.push_back(0.0921);
    }
    else if (ur == UR::UR5e)
    {
        a.push_back(0);
        a.push_back(-0.425);
        a.push_back(-0.3922);
        a.push_back(0);
        a.push_back(0);
        a.push_back(0);

        d.push_back(0.1625);
        d.push_back(0);
        d.push_back(0);
        d.push_back(0.1333);
        d.push_back(0.0997);
        d.push_back(0.0996);
    }
    else if (ur == UR::UR10e)
    {
        a.push_back(0);
        a.push_back(-0.6127);
        a.push_back(-0.57155);
        a.push_back(0);
        a.push_back(0);
        a.push_back(0);

        d.push_back(0.1807);
        d.push_back(0);
        d.push_back(0);
        d.push_back(0.17415);
        d.push_back(0.11985);
        d.push_back(0.11655);
    }
    else if (ur == UR::UR16e)
    {
        a.push_back(0);
        a.push_back(-0.4784);
        a.push_back(-0.36);
        a.push_back(0);
        a.push_back(0);
        a.push_back(0);

        d.push_back(0.1807);
        d.push_back(0);
        d.push_back(0);
        d.push_back(0.17415);
        d.push_back(0.11985);
        d.push_back(0.11655);
    }

    alpha.push_back(M_PI / 2);
    alpha.push_back(0);
    alpha.push_back(0);
    alpha.push_back(M_PI / 2);
    alpha.push_back(-M_PI / 2);
    alpha.push_back(0);
}

DHParam::~DHParam()
{
}
