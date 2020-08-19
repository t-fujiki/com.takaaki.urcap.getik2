class IKSolver
{
private:
    int ur;
    double x;
    double y;
    double z;
    double rx;
    double ry;
    double rz;

    double offset_x;
    double offset_y;
    double offset_z;
    double offset_rx;
    double offset_ry;
    double offset_rz;

    double theta[7];

public:
    IKSolver(int ur, double x, double y, double z, double rx, double ry, double rz);
    ~IKSolver();
    void setOffset(double x, double y, double z, double rx, double ry, double rz);
    void solve(int num);
    double *getAngle();
    int getPattern(double *angles);
};
