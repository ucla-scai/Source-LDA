#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include "gtpoints.h"
//----------------------------------------------------------------------------------
using namespace std;
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
GtPointsOptions::GtPointsOptions() {
    P = 1;
    srctopics = "src_topics_c.dat";
    output_dir = ".";
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
GtPoints::GtPoints() { }
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
GtPoints::GtPoints(GtPointsOptions options) {
    this->options = options;
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void GtPoints::set(gsl_rng * RANDOM_NUMBER, vector<vector<double>>* delts, vector<vector<double>>* delts_pcts, int p) {
    this->RANDOM_NUMBER = RANDOM_NUMBER;
    this->P = p;
    this->deltas = delts;
    this->deltas_pcts = delts_pcts;
    this->B = deltas->size();
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
static void * Load_sample_thread(void *args) {

    pair<GtPoints*, int>* argsPtr = (pair<GtPoints*, int>*) args;
    GtPoints* sthis = argsPtr->first;
    int id = argsPtr->second;

    for (int i=0; i<sthis->B; i++) {
        if (i % sthis->P != id) { continue; }
        cout << currentDateTime() << "...GtPoints.load - sample " << i << " start\n";
        double g_y_zero = sthis->At_zero(i, (*(sthis->deltas_pcts))[i]);
        double g_y_one = sthis->At_one(i, (*(sthis->deltas))[i], (*(sthis->deltas_pcts))[i]);
        double m = (g_y_one - g_y_zero);
        double b = g_y_one - m;
        for (int times=0; times<2; times++) {
            vector<point> points;
            point f;
            f.x = 1.0;
            f.y = 1.0;
            points.push_back(f);
            for (double x = 0.95; x > 0.03; x -= 0.05) {
                double mapped_x = sthis->map(i, x);
                double at_x = sthis->At_x(mapped_x, i, (*(sthis->deltas))[i], (*(sthis->deltas_pcts))[i]);
                double x_corrected = (at_x - b) / m;
                point p;
                p.x = x_corrected;
                p.y = mapped_x;
                points.push_back(p);
            }
            point l;
            l.x = 0.0;
            l.y = 0.0;
            points.push_back(l);
            if (times == 0) {
                sthis->src_points[i] = points;
            }
            else {
                int cur_points = 0;
                int cur_src = 0;
                vector<point> merged;
                int total = sthis->src_points[i].size()+ points.size();
                int skip = 0;
                while (merged.size() != (total - skip)) {
                    while ( cur_src < sthis->src_points[i].size() && (cur_points == points.size() || points[cur_points].x == sthis->src_points[i][cur_src].x ||
                           points[cur_points].x < sthis->src_points[i][cur_src].x)) {
                        merged.push_back(sthis->src_points[i][cur_src]);
                        if (cur_points < points.size() && points[cur_points].x == sthis->src_points[i][cur_src].x) {
                            cur_points++;
                            skip++;
                        }
                        cur_src++;
                    }
                    while (cur_points < points.size() && (cur_src == sthis->src_points[i].size() || points[cur_points].x == sthis->src_points[i][cur_src].x ||
                           points[cur_points].x > sthis->src_points[i][cur_src].x)) {
                        merged.push_back(points[cur_points]);
                        if (cur_src < sthis->src_points[i].size() && points[cur_points].x == sthis->src_points[i][cur_src].x) {
                            cur_src++;
                            skip++;
                        }
                        cur_points++;
                    }
                }
                sthis->src_points[i] = merged;
            }
        }
        double error = 0.0;
        for (double x = 0.95; x > 0.03; x -= 0.05) {
            double target_y = m * x + b;
            double mapped_x = sthis->map(i, x);
            double actual_y = sthis->At_x(mapped_x, i, (*(sthis->deltas))[i], (*(sthis->deltas_pcts))[i]);
            error += abs(actual_y - target_y);
        }

        cout << currentDateTime() << "...GtPoints.load - sample " << i << " complete\n";
        cout << currentDateTime() << "...GtPoints.load - sample " << i << " error " << error << "\n";
    }
    delete argsPtr;
    pthread_exit(NULL);
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void GtPoints::save() {
    ofstream fout;
    fout.open(options.output_dir + "/gt.dat");

    for (int i=0; i<B; i++) {
        for (int j=0; j<src_points[i].size(); j++) {
            if (j == 0) {
                fout << src_points[i][j].x << " " << src_points[i][j].y;
            }
            else {
                fout << " " << src_points[i][j].x << " " << src_points[i][j].y;
            }
        }
        fout << endl;
    }
    fout.close();
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void GtPoints::load_saved(string file_name) {
    ifstream fin;
    src_points.clear();
    fin.open(file_name, ios_base::in);
    for (string line; getline(fin, line);) {
        istringstream fstring(line);
        vector<point> points;
        double x;
        double y;
        while (fstring >> x >> y) {
            point p;
            p.x = x;
            p.y = y;
            points.push_back(p);
        }
        src_points.push_back(points);
    }
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void GtPoints::run() {

    ifstream fin;
    int id = 0;
    fin.open(options.srctopics, ios_base::in);

    unordered_map<string, int> word_id;
    unordered_map<int, string> id_word;

    unordered_set<string> vocab;

    for (string line; getline(fin, line);) {
        istringstream fstring(line);
        string word;
        string topic;
        fstring >> topic;
        int count;
        while (fstring >> word >> count) {
            unordered_set<string>::const_iterator iter = vocab.find(word);
            if (iter == vocab.end()) {
                vocab.insert(word);
                word_id[word] = id;
                id_word[id] = word;
                id++;
            }
        }
        break;
    }

    fin.close();

    int V = vocab.size();

    vector<vector<double>> delts;
    vector<double> deltas_sums;
    fin.open(options.srctopics, ios_base::in);
    for (string line; getline(fin, line);) {
        istringstream fstring(line);
        string topic;
        fstring >> topic;
        string word;
        int count;
        double sum = 0.0;
        vector<double> src_words(V);
        for (int i = 0; i < V; i++) {
            src_words[i] = epsilon;
            sum += epsilon;
        }
        delts.push_back(src_words);

        while (fstring >> word >> count) {
            auto iter = word_id.find(word);
            if (!(iter == word_id.end())) {
                delts.back()[iter->second] += (double)count;
                sum += (double)count;
            }
        }
        deltas_sums.push_back(sum);
        break;
    }

    vector<vector<double>> d_pcts;
    for (int i=0; i<delts.size(); i++) {
        vector<double> d_pct;
        for (int j=0; j<delts[i].size(); j++) {
            d_pct.push_back(delts[i][j] / deltas_sums[i]);
        }
        d_pcts.push_back(d_pct);
    }

    gsl_rng * RANDOM_NUMBER = gsl_rng_alloc(gsl_rng_taus);
    time_t t;
    time(&t);
    long seed = (long) t;
    gsl_rng_set(RANDOM_NUMBER, (long) seed);
    set(RANDOM_NUMBER, &delts, &d_pcts, options.P);
    load();
    gsl_rng_free (RANDOM_NUMBER);
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void GtPoints::load(){

    cout << currentDateTime() << "...GtPoints.load - start\n";

    src_points.clear();

    for (int i=0; i<deltas_pcts->size(); i++) {
        vector<point> points;
        double d = 1.0;
        for (int j=100; j>=0; j-=5){
            point p;
            p.x = d;
            p.y = d;
            d -= 0.05;
            points.push_back(p);
        }
        src_points.push_back(points);
    }

    pthread_t threads_init[P];
    pthread_attr_t attr_init;
    void *status_init;

    pthread_attr_init(&attr_init);
    pthread_attr_setdetachstate(&attr_init, PTHREAD_CREATE_JOINABLE);

    cout << currentDateTime() << "...GtPoints.load - begin sampling\n";

    for(int i=0; i < P; i++ ){
        pair<GtPoints*, int>* args = new pair<GtPoints*, int>(std::make_pair(this, i));
        pthread_create(&threads_init[i], NULL, Load_sample_thread, (void *) args);
    }

    pthread_attr_destroy(&attr_init);
    for(int i=0; i < P; i++ ){
        pthread_join(threads_init[i], &status_init);
    }

    cout << currentDateTime() << "...GtPoints.load - end sampling\n";

    save();

    cout << currentDateTime() << "...GtPoints.load - end\n";
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
double GtPoints::At_one(int i, const vector<double> & delts, const vector<double> & delts_pcts){
    return At(i, delts_pcts, &delts[0]);
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
double GtPoints::At(int i, const vector<double> & delts_pcts, const double alpha[]){
    int k = delts_pcts.size();
    double js_divergences = 0;
    int n = 0;

    for (int i=0; i<1000; i++) {
        double theta[k];
        gsl_ran_dirichlet(this->RANDOM_NUMBER, k, alpha, theta);
        double js = JS(theta, &delts_pcts[0], k);
        js_divergences += js;
        n++;
    }
    return js_divergences / ((double)n);
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
double GtPoints::At_zero(int i, const vector<double> & delts_pcts){
    int k = delts_pcts.size();
    double ones[k];
    for (int i=0; i<k; i++) {
        ones[i] = 1.0;
    }
    return At(i, delts_pcts, ones);
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
double GtPoints::map(int id, double x) {
    int low = src_points[id].size()-1;
    int max = src_points[id].size()-1;
    int high = 0;

    while (high <= low) {
        int mid = (low + high) / 2;
        if (src_points[id][mid].x == x) {
            return src_points[id][mid].y;
        }
        if (src_points[id][low].x == x) {
            return src_points[id][low].y;
        }
        if (src_points[id][high].x == x) {
            return src_points[id][high].y;
        }
        if (low == high + 1) {
            mid = high;
        }
        if (src_points[id][mid].x > x && x > src_points[id][Min(mid+1, max)].x) {
            double x_s = src_points[id][Min(mid+1, max)].x;
            double x_l = src_points[id][mid].x;
            double y_s = src_points[id][Min(mid+1, max)].y;
            double y_l = src_points[id][mid].y;
            double x_gap = x_l - x_s;
            double y_gap = y_l - y_s;
            double frac = (x - x_s) / x_gap;
            return y_s + frac*y_gap;
        }
        else if (src_points[id][mid].x > x) {
            high = mid;
        }
        else {
            low = mid;
        }
    }
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
double GtPoints::At_x(double g_y, int i, const vector<double> &delts, const vector<double> &delts_pcts) {
    int k = delts.size();
    double alpha[k];
    for (int i=0; i<k; i++) {
        alpha[i] = pow(delts[i], g_y);
    }
    return At(i, delts_pcts, alpha);
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------