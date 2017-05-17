#ifndef SRC_LDA_C_EDA_H
#define SRC_LDA_C_EDA_H
//----------------------------------------------------------------------------------
#include <gsl/gsl_randist.h>
#include <vector>
#include "utility.h"
#include "srclda.h"
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
struct EdaOptions {
public:
    int K;
    int I;
    int P;
    int burn;
    string corpus;
    string edatopics;
    string output_dir = ".";
    Model model;
    DisplayOptions display;
    string key;
    bool use_key;
    EdaOptions();
};
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
class Eda {
protected:
    Stats stats;
    vector<vector<int>> corpus;
    vector<int>* corpus_t;
    bool* hidden;
    vector<int> visible_topics;
    vector<vector<int>> ground_truth;
    vector<string> topic_labels;
    double* pr;
    int** n_w;
    int** n_d;
    int* n_d_dot;
    int* n_w_dot;
    unordered_map<string, int> word_id;
    unordered_map<int, string> id_word;
    EdaOptions options;
    void Load_corpus();
    void Load_topics();
    void Init_random();
    void Update_n();
    int Sample(int doc, int token);
    void Calculate_theta();
    void Calculate_phi_latent();
    void prune(int iter, int n_prune, int start_y);
    virtual void Hide_topic(int t);
    void Display_stats(int iter);
    void Populate_prob(int i, int t, int word, int doc, int start);
    virtual int Pop_sample(int word, int doc);
public:
    int token_count;
    gsl_rng * RANDOM_NUMBER;
    double alpha;
    double beta;
    int T, K, D, V, B, I, P;
    vector<vector<double>> theta;
    vector<vector<double>> phi_latent;
    vector<unordered_map<int, double>> phi_explicit;
    void Write_distributions();
    Eda(EdaOptions options);
    void load();
    virtual void gibbs();
    void save();
    ~Eda();
};
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
#endif //SRC_LDA_C_EDA_H
