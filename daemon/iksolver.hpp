class IKSolver
{
private:
    int ur;
    Pose tcp_pose;
    Pose tcp_offset;

    double theta[7];

    double delta_a[7], delta_d[7], delta_alpha[7], delta_theta[7];

public:
    IKSolver(int ur, Pose tcp_pose, Pose tcp_offset);
    ~IKSolver();
    void setCalibration(double *delta_a, double *delta_d, double *delta_alpha, double *delta_theta);
    void solve(int num);
    double *getAngle();
    int getPattern(double *angles);
};
