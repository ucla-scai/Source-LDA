#ifndef SRC_LDA_C_CONCEPTLDA_H
#define SRC_LDA_C_CONCEPTLDA_H

//----------------------------------------------------------------------------------
#include <gsl/gsl_randist.h>
#include <vector>
#include "utility.h"
#include "lda.h"
#include "srclda.h"
#include <unordered_set>
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
struct ConceptLdaOptions {
public:
    int K;
    int I;
    int P;
    int burn;
    string corpus;
    string concepttopics;
    int C;
    DisplayOptions display;
    string output_dir;
    Model model;
    string key;
    bool use_key;
    ConceptLdaOptions();
};
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
class ConceptLda {
protected:
    vector<vector<int>> key_t;
    int corr;
    int out_of;
    Stats stats;
    vector<vector<int>> corpus;
    vector<int>* corpus_t;
    bool* hidden;
    vector<int> visible_topics;
    vector<string> topic_labels;
    vector<unordered_set<int>> concepts;
    double* pr;
    int** n_w_phi;
    int** n_w_psi;
    int** n_d;
    int* n_d_dot;
    int* n_w_dot_phi;
    int* n_w_dot_psi;
    unordered_map<string, int> word_id;
    unordered_map<int, string> id_word;
    ConceptLdaOptions options;
    void Load_corpus();
    void Init_random();
    void Update_n();
    int Sample(int doc, int token);
    void Calculate_theta();
    void Calculate_phi();
    void Calculate_psi();
    void Load_concepts();
    void prune(int iter, int n_prune, int start_y);
    virtual void Hide_topic(int t);
    void Display_stats(int iter);
    void Populate_prob(int i, int t, int word, int doc, int start);
    virtual int Pop_sample(int word, int doc);
public:
    int token_count;
    gsl_rng * RANDOM_NUMBER;
    double alpha;
    double beta_phi;
    double beta_psi;
    int T, K, D, V, B, I, P, V_c, C;
    vector<vector<double>> theta;
    vector<vector<double>> phi;
    vector<vector<double>> psi;
    void Write_distributions();
    string Clean(double d);
    ConceptLda(ConceptLdaOptions options);
    void load();
    virtual void gibbs();
    void save();
    ~ConceptLda();
};
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
#endif //SRC_LDA_C_CONCEPTLDA_H
