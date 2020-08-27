class FKSolver
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

    double delta_a[7], delta_d[7], delta_alpha[7], delta_theta[7];

    double jacobian[6][6];

public:
    FKSolver(int ur);
    ~FKSolver();
    void setOffset(double x, double y, double z, double rx, double ry, double rz);
    void setCalibration(double *delta_a, double *delta_d, double *delta_alpha, double *delta_theta);
    void solve(double *theta);
    double getX();
    double getY();
    double getZ();
    double getRx();
    double getRy();
    double getRz();
    void calcJacobian();
};
