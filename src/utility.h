#ifndef SRC_LDA_C_UTILITY_H
#define SRC_LDA_C_UTILITY_H
//----------------------------------------------------------------------------------
#include <string>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_sf_gamma.h>
#include <math.h>
#include <cfloat>
#include <stdexcept>
#include <cmath>
#include <vector>
#include <algorithm>
#include <sstream>
//----------------------------------------------------------------------------------
using namespace std;
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
#ifdef EPS
const double epsilon = EPS;
#else
const double epsilon = DBL_EPSILON;
#endif
const double epsilon_min = 4.94065645841247e-324;
const double sqrt_2_pi = sqrt(2.0*M_PI);
const double sqrt_2 = sqrt(2.0);
const double HALF_LOG_TWO_PI = log(2.0 * M_PI) / 2.0;
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
enum Perplexity {
    none, imp, lr, heinrich
};
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
enum Model {
    bijective, mix, src
};
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
struct DisplayOptions {
public:
    bool labels;
    bool truncated;
    bool top;
    int n;
    int d;
    DisplayOptions();
};
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
struct Stats {
    double tot_model;
    int cnt_model;
    double tot_reg;
    int cnt_reg;
    long iteration_time;
    long tot_iteration_time;
    int assign_correct;
    int assign_total;
};
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
inline void split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
inline double logGammaStirling(double z) {
    int shift = 0;
    while (z < 2) {
        z++;
        shift++;
    }

    double result = HALF_LOG_TWO_PI + (z - 0.5) * log(z) - z +
    1/(12 * z) - 1 / (360 * z * z * z) + 1 / (1260 * z * z * z * z * z);

    while (shift > 0) {
        shift--;
        z--;
        result -= log(z);
    }

    return result;
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
inline double logsumexp(const vector<double>& nums) {
    double max_exp = nums[0], sum = 0.0;

    for (int i = 1 ; i < nums.size() ; i++)
        if (nums[i] > max_exp)
            max_exp = nums[i];

    for (int i = 0; i < nums.size() ; i++)
        sum += exp(nums[i] - max_exp);

    return log(sum) + max_exp;
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
inline double gammaln(double x) {
    return gsl_sf_lngamma(x);
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
inline double Normal(double x, double mu, double sigma) {
    double x_minus_mu_squared = (x - mu)*(x - mu);
    double two_sigma_squared = (sigma*sigma)*2.0;
    double exponent = -1.0 * x_minus_mu_squared / two_sigma_squared;
    double e_exponent = exp(exponent);
    double sqrt_two_sigma_squared_pi = sqrt(two_sigma_squared * M_PI);
    return (1.0 / sqrt_two_sigma_squared_pi) * e_exponent;
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
template <class T> inline void heapify(vector<T>&  arr, vector<int>& idx, int n, int i)
{
    int largest = i;
    int l = 2*i + 1;
    int r = 2*i + 2;

    if (l < n && arr[l] > arr[largest]) { largest = l; }
    if (r < n && arr[r] > arr[largest]) { largest = r; }

    if (largest != i)
    {
        swap(arr[i], arr[largest]);
        swap(idx[i], idx[largest]);
        heapify(arr, idx, n, largest);
    }
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
inline bool File_exists (const std::string& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
template <class T> inline void Sort(vector<T>& arr, vector<int>& idx) {

    int n = arr.size();

    idx.resize(n);

    for (int i=0; i<n; i++) {
        idx[i] = i;
    }

    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, idx, n, i);
    }

    for (int i=n-1; i>=0; i--)
    {
        swap(arr[0], arr[i]);
        swap(idx[0], idx[i]);
        heapify(arr, idx, i, 0);
    }

    reverse(idx.begin(), idx.end());
    reverse(arr.begin(), arr.end());
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
template <class T> inline T Min(T a, T b) {
    if (a < b) { return a; } else { return b; }
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
template <class T> inline T Max(T a, T b) {
    if (a > b) { return a; } else { return b; }
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
template <class T> inline T Max(T a, T b, T c, T d) {
    T max = Max(a, b);
    max = Max(max, c);
    return Max(max, d);
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
inline int Rand(const gsl_rng * RANDOM_NUMBER, int left, int right){
    return gsl_rng_uniform_int(RANDOM_NUMBER, (right+1) - left) + left;
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
inline double nextUniform(const gsl_rng * RANDOM_NUMBER){
    int start = 0;
    int end = INT32_MAX-1;
    return ((double)Rand(RANDOM_NUMBER, start, end)) / ((double)end);
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
inline const string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    return buf;
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
inline double KL(const double p[], const double q[], int n) {
    double kl = 0;
    for (int i=0; i<n; i++)
    {
        double p_i = p[i] == 0.0 ? epsilon_min : p[i];
        double q_i = q[i] == 0.0 ? epsilon_min : q[i];
        kl += p_i * (log(p_i) - log(q_i));
        if (!std::isfinite(kl))
        {
            throw std::invalid_argument( "invalid value" );
        }
    }
    return kl;
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
inline double JS(const double p[], const double q[], int n) {
    double m[n];
    for(int i=0; i<n; i++)
    {
        m[i] = 0.5 * (p[i] + q[i]);
    }
    return 0.5 * KL(p, m, n) + 0.5 * KL(q, m, n);
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
#endif //SRC_LDA_C_UTILITY_H
