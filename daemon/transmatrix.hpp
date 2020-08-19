class TransMatrix
{
private:
public:
    TransMatrix(double x, double y, double z, double rx, double ry, double rz);
    TransMatrix(double a, double d, double alpha, double theta);
    TransMatrix();
    ~TransMatrix();
    double entry[4][4];
    TransMatrix operator*(const TransMatrix &tm);
    TransMatrix inverse();
};