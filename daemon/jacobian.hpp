class Jacobian
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

    double entry[6][6];
    double inv_entry[6][6];

public:
    Jacobian(int ur);
    ~Jacobian();
    void setOffset(double x, double y, double z, double rx, double ry, double rz);
    void setCalibration(double *delta_a, double *delta_d, double *delta_alpha, double *delta_theta);
    void solve(double *theta);
    void inverse();
    void calc();
};
