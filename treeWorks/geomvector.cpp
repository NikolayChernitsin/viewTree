#include "geomvector.h"
#include <math.h>

geomVector::geomVector(double x0, double y0, double x1, double y1)
{
    x = x1 - x0;
    y = y1 - y0;
}

void geomVector::rot(double alpha)
{
    double tempX = x;
    x = x * cos(gradToRad(alpha)) - y * sin(gradToRad(alpha));
    y = tempX * sin(gradToRad(alpha)) + y * cos(gradToRad(alpha));
}

double *geomVector::getVector()
{
    double ret[2];
    ret[0] = x;
    ret[1] = y;
    return ret;
}

double gradToRad(double alpha)
{
    return alpha * M_PI / 180;
}

double radToGrad(double alpha)
{
    return alpha * 180 / M_PI;
}

double geomVector::getRotation()
{
    double cosa = y / sqrt(x * x + y * y);
    return radToGrad(acos(cosa));
}

void geomVector::reset()
{
    rot(-getRotation());
}

void geomVector::cut(double dxy)
{
    double norm = sqrt(x * x + y * y);
    x /= norm;
    y /= norm;
    x *= dxy;
    y *= dxy;
}
