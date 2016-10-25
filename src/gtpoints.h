#ifndef SRC_LDA_C_GTPOINTS_H
#define SRC_LDA_C_GTPOINTS_H
//----------------------------------------------------------------------------------
#include "utility.h"
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
struct point {
    double x;
    double y;
};
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
struct GtPointsOptions {
    int P;
    string srctopics;
    string output_dir;
    GtPointsOptions();
};
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
class GtPoints {
private:
    double At(int i, const vector<double> & delts_pcts, const double alpha[]);
    GtPointsOptions options;
    gsl_rng * RANDOM_NUMBER;
public:
    void run();
    void load();
    void load_saved(string file_name);
    void save();
    double map(int id, double x);
    void set(gsl_rng * RANDOM_NUMBER, vector<vector<double>>* delts, vector<vector<double>>* delts_pcts, int p);
    double At_x(double g_y, int i, const vector<double> &delts, const vector<double> &delts_pcts);
    double At_one(int i,  const vector<double> & delts, const vector<double> & delts_pcts);
    double At_zero(int i, const vector<double> & delta_p);
    GtPoints(GtPointsOptions options);
    GtPoints();
    int P;
    int B;
    vector<vector<double>>* deltas;
    vector<vector<double>>* deltas_pcts;
    vector<vector<point>> src_points;
};
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
#endif //SRC_LDA_C_GTPOINTS_H
