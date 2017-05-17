#ifndef SRC_LDA_C_LDA_H
#define SRC_LDA_C_LDA_H

//----------------------------------------------------------------------------------
#include <gsl/gsl_randist.h>
#include <vector>
#include "utility.h"
#include "gtpoints.h"
#include "srclda.h"
#include <unordered_map>
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
struct LdaOptions {
public:
    int K;
    int I;
    string corpus;
    string output_dir;
    DisplayOptions display;
    LdaOptions();
};
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
class Lda {
protected:
    string Clean(double d);
    Stats stats;
    vector<vector<int>> corpus;
    vector<int>* corpus_t;
    double* pr;
    int** n_w;
    int** n_d;
    int* n_d_dot;
    int* n_w_dot;
    unordered_map<string, int> word_id;
    unordered_map<int, string> id_word;
    LdaOptions options;
    void Load_corpus();
    void Init_random();
    void Update_n();
    int Sample(int doc, int token);
    void Calculate_theta();
    void Calculate_phi();
    void Display_stats(int iter);
    void Populate_prob(int i, int t, int word, int doc, int start);
    virtual int Pop_sample(int word, int doc);
public:
    gsl_rng * RANDOM_NUMBER;
    double alpha;
    double beta;
    int K, D, V, I;
    vector<vector<double>> theta;
    vector<vector<double>> phi;
    void Write_distributions();
    Lda(LdaOptions options);
    void load();
    virtual void gibbs();
    void save();
    ~Lda();
};
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
#endif //SRC_LDA_C_LDA_H
