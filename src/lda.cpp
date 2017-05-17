#define _XOPEN_SOURCE 600

#include <iostream>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <math.h>
#include <iomanip>
#include <limits>
#include <set>
#include <map>
#include "lda.h"
#include <algorithm>
#include <chrono>
//----------------------------------------------------------------------------------
using namespace std;
using namespace std::chrono;
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
LdaOptions::LdaOptions() {
    K = 100;
    I = 1000;
    corpus = "sampled_input_200_c.dat";
    output_dir = ".";
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void Lda::Load_corpus(){

    ifstream fin;
    int id = 0;
    fin.open(options.corpus, ios_base::in);

    unordered_set<string> vocab;

    for (string line; getline(fin, line);) {
        istringstream fstring(line);
        vector<int> document;
        string word;
        int token;
        while (fstring >> word) {
            unordered_set<string>::const_iterator iter = vocab.find(word);
            if (iter == vocab.end()) {
                vocab.insert(word);
                word_id[word] = id;
                id_word[id] = word;
                id++;
            }
            token = word_id[word];
            document.push_back(token);
        }
        corpus.push_back(document);
    }

    D = corpus.size();
    V = vocab.size();
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void Lda::Display_stats(int iter) {
    cout << currentDateTime() << "...LDA.gibbs - end iter...iteration time " << stats.iteration_time << endl;
    cout << currentDateTime() << "...LDA.gibbs - end iter...avg iteration time " << ((double)stats.tot_iteration_time) / ((double)iter+1)  << endl;
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void Lda::load() {

    cout << currentDateTime() << "...LDA.load - Start\n";

    I = options.I;

    cout << currentDateTime() << "...LDA.load - Load corpus\n";

    Load_corpus();

    alpha = ((double)50) / ((double)K);
    beta = ((double)200) / ((double)V);

    cout << currentDateTime() << "...LDA.load - Load deltas\n";

    K = options.K;
    cout << currentDateTime() << "...LDA.load - Init counts\n";
    Init_random();
    Update_n();
    pr = new double[K];

    cout << currentDateTime() << "...LDA.load - Done\n";
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void Lda::save() {
    Calculate_theta();
    Calculate_phi();
    Write_distributions();
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void Lda::gibbs() {

    cout << currentDateTime() << "...LDA.gibbs - Start\n";
    stats.tot_iteration_time = 0;

    for (int iter=0; iter < I; iter++) {

        auto start = high_resolution_clock::now();
        stats.iteration_time = 0;

        cout << currentDateTime() << "...LDA.gibbs - begin iter " << iter << endl;

        if (iter % 100 == 0 && iter > 0) {
            Calculate_theta();
            Calculate_phi();
            Write_distributions();
        }

        for (int doc=0; doc<D; doc++) {
            for (int token=0; token<corpus[doc].size(); token++) {
                corpus_t[doc][token] = Sample(doc, token);
            }
        }
        stats.iteration_time = duration_cast<milliseconds>(high_resolution_clock::now()-start).count();
        stats.tot_iteration_time += stats.iteration_time;
        Display_stats(iter);
    }

    cout << currentDateTime() << "...LDA.gibbs - Done\n";
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void Lda::Init_random(){
    corpus_t = new vector<int>[D];
    for (int doc=0; doc<D; doc++) {
        vector<int> topics;
        for (int token=0; token<corpus[doc].size(); token++) {
            int t = Rand(RANDOM_NUMBER, 0, K-1);
            topics.push_back(t);
        }
        corpus_t[doc] = topics;
    }
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void Lda::Update_n(){
    n_w = new int*[K];
    n_w_dot = new int[K];
    n_d_dot = new int[K];
    n_d = new int*[K];
    for (int i=0; i<K; i++) {
        n_w_dot[i] = 0;
        n_w[i] = new int[V];
        n_d_dot[i] = 0;
        for (int j=0; j<V; j++) {
            n_w[i][j] = 0;
        }
        n_d[i] = new int[D];
        for (int j=0; j<D; j++) {
            n_d[i][j] = 0;
        }
    }

    for (int doc=0; doc<D; doc++) {
        unordered_set<int> topics_in_doc;
        for (int token = 0; token < corpus[doc].size(); token++) {
            int t = corpus_t[doc][token];
            int w = corpus[doc][token];
            topics_in_doc.insert(t);
            n_w[t][w]++;
            n_d[t][doc]++;
            n_w_dot[t]++;
        }
        for (auto itr = topics_in_doc.begin(); itr != topics_in_doc.end(); ++itr) {
            n_d_dot[(*itr)]++;
        }
    }
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void Lda::Populate_prob(int i, int t, int word, int doc, int start) {

    pr[i] = (((double) n_w[t][word] + beta) / (((double) n_w_dot[t]) + ((double) V) * beta)) *
            (((double) n_d[t][doc] + alpha) / (((double) (corpus[doc].size() - 1)) + ((double) K) * alpha));

    if (i > start) {
        pr[i] += pr[i-1];
    }
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
int Lda::Pop_sample(int word, int doc) {

    for (int i=0; i<K; i++) {
        int t = i;
        Populate_prob(i, t, word, doc, 0);
    }

    double scale = pr[K-1] * gsl_rng_uniform(RANDOM_NUMBER);

    int topic = 0;

    if (pr[0] <= scale) {
        int low = 0;
        int high = K-1;
        while (low <= high) {
            if (low == high - 1) { topic = high; break; }
            int mid = (low + high) / 2;
            if (pr[mid] > scale) high = mid;
            else low = mid;
        }
    }

    return topic;
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
int Lda::Sample(int doc, int token){

    int topic = corpus_t[doc][token];
    int w = corpus[doc][token];

    n_d[topic][doc] = Max(n_d[topic][doc]-1, 0);
    n_w[topic][w] = Max(n_w[topic][w]-1, 0);
    n_w_dot[topic] = Max(n_w_dot[topic]-1, 0);

    if (n_d[topic][doc] == 0) {
        n_d_dot[topic]--;
    }

    alpha = ((double)50) / ((double)K);
    topic = Pop_sample(w, doc);

    if (n_d[topic][doc] == 0) {
        n_d_dot[topic]++;
    }

    n_d[topic][doc]++;
    n_w[topic][w]++;
    n_w_dot[topic]++;
    return topic;
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void Lda::Calculate_theta(){
    for (int i=0; i<theta.size(); i++) {
        theta[i].clear();
    }
    theta.clear();

    int topic_count = K;

    for (int doc=0; doc<D; doc++) {
        vector<double> theta_d(K);
        for (int t=0; t<K; t++) {
            theta_d[t] = (((double)n_d[t][doc]) + alpha) / (((double)corpus[doc].size()) + (((double)topic_count)*alpha));
        }
        theta.push_back(theta_d);
    }
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void Lda::Calculate_phi(){
    for (int i=0; i<phi.size(); i++) {
        phi[i].clear();
    }
    phi.clear();
    for (int t=0; t<K; t++) {
        vector<double> phi_t(V);
        for (int w=0; w<V; w++) {
            phi_t[w] = (((double)n_w[t][w]) + beta)/(((double)n_w_dot[t]) + (((double)V)*beta));
        }
        phi.push_back(phi_t);
    }
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
string Lda::Clean(double d) {

    std::stringstream ss(stringstream::in | stringstream::out);
    if (options.display.truncated) {
        ss << setprecision(options.display.d) << d;
    }
    else {
        ss << d;
    }
    string s = ss.str();
    return s;
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void Lda::Write_distributions() {

    int top = options.display.top ? options.display.n : K;
    vector<int> idx(K);

    ofstream theta_out;
    theta_out.open(options.output_dir + "/theta.dat");
    for (int doc=0; doc<D; doc++) {
        theta_out << doc;
        for (int t=0; t<K; t++) {
            theta_out << " " << theta[doc][t];
        }
        theta_out << endl;
    }
    theta_out.close();

    ofstream phi_out;
    phi_out.open(options.output_dir + "/phi.dat");
    for (int t=0; t<K; t++) {
        phi_out << t;
        if (options.display.top) {
            vector<double> phi_t(phi[t]);
            Sort(phi_t, idx);
            for (int w=0; w<Min(options.display.n, V); w++) {
                string word = options.display.labels ? id_word[idx[w]] : to_string(idx[w]);
                phi_out << " " << word << ":" << Clean(phi_t[w]);
            }
        }
        else {
            for (int w=0; w<V; w++) {
                phi_out << " " << Clean(phi[t][w]);
            }
        }
        phi_out << endl;
    }
    phi_out.close();

    ofstream v_out;
    v_out.open(options.output_dir + "/vocab.dat");
    for (auto itr = word_id.begin(); itr != word_id.end(); ++itr) {
        v_out << itr->first << " " << itr->second << endl;
    }
    v_out.close();

    ofstream t_out;
    t_out.open(options.output_dir + "/corpus_t.dat");
    for (int doc=0; doc<D; doc++) {
        for (int t=0; t<corpus_t[doc].size(); t++) {
            if (t==0) {
                t_out << corpus_t[doc][t]-100;
            }
            else {
                t_out << " " << corpus_t[doc][t]-100;
            }
        }
        t_out << endl;
    }

    t_out.close();
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
Lda::Lda(LdaOptions options) {
    this->options = options;
    RANDOM_NUMBER = gsl_rng_alloc(gsl_rng_taus);
    time_t t;
    time(&t);
    long seed = (long) t;
    gsl_rng_set(RANDOM_NUMBER, (long) seed);
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
Lda::~Lda() {

    for (int i=0; i<K; i++) {
        delete[] n_w[i];
        delete[] n_d[i];
    }
    delete[] n_w;
    delete[] n_d;
    delete[] n_w_dot;
    delete[] n_d_dot;
    delete[] corpus_t;
    delete[] pr;

    gsl_rng_free (RANDOM_NUMBER);
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------