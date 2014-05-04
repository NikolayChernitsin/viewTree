#ifndef GEOMVECTOR_H
#define GEOMVECTOR_H

class geomVector
{
public:
    geomVector(double x0, double y0, double x1, double y1);
    //geomVector(geomVector vect);
    void reset();
    void rot(double alpha);
    double *getVector();
    double getRotation();
    void cut(double dxy);

private:
    double x;
    double y;
};

double gradToRad(double alpha);
double radToGrad(double alpha);
const int X = 0;
const int Y = 1;

#endif // GEOMVECTOR_H
