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
#include "conceptlda.h"
#include <algorithm>
#include <chrono>
//----------------------------------------------------------------------------------
using namespace std;
using namespace std::chrono;
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
ConceptLdaOptions::ConceptLdaOptions() {
    K = 100;
    I = 1000;
    P = 7;
    burn = 100;
    corpus = "dat/sampled_input_200_c.dat";
    concepttopics = "dat/src_topics_c.dat";
    output_dir = ".";
    model = src;
    use_key = false;
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void ConceptLda::Load_corpus(){

    if (options.use_key) {
        ifstream fin_t;
        fin_t.open(options.key, ios_base::in);
        for (string line; getline(fin_t, line);) {
            istringstream fstring(line);
            int t_id;
            vector<int> doc_key;
            while (fstring >> t_id) {
                doc_key.push_back(t_id);
            }
            ground_truth.push_back(doc_key);
        }
        fin_t.close();
    }

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
void ConceptLda::Display_stats(int iter) {
    cout << currentDateTime() << "...ConceptLDA.gibbs - end iter...iteration time " << stats.iteration_time << endl;
    cout << currentDateTime() << "...ConceptLDA.gibbs - end iter...avg iteration time " << ((double)stats.tot_iteration_time) / ((double)iter+1)  << endl;
    if (options.use_key) {
        cout << currentDateTime() << "...SrcLDA.gibbs - end iter...accuracy "
             << ((((double) stats.assign_correct) / ((double) stats.assign_total)) * 100.0) << endl;
        cout << currentDateTime() << "...SrcLDA.gibbs - end iter...correct " << stats.assign_correct << endl;
        cout << currentDateTime() << "...SrcLDA.gibbs - end iter...total " << stats.assign_total << endl;
    }
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void ConceptLda::Hide_topic(int t) {
    visible_topics.erase(std::remove(visible_topics.begin(), visible_topics.end(), t), visible_topics.end());
    hidden[t] = true;
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void ConceptLda::Load_concepts(){

    cout << currentDateTime() << "...ConceptLDA.Load_concepts - begin\n";

    unordered_set<int> c_vocab;
    C = options.C;

    ifstream fin;
    fin.open(options.concepttopics, ios_base::in);
    for (string line; getline(fin, line);) {
        istringstream fstring(line);
        string label;
        fstring >> label;
        topic_labels.push_back(label);

        unordered_set<int> concept_set;
        concepts.push_back(concept_set);
        string word;
        int count;

        while (fstring >> word >> count) {
            auto iter = word_id.find(word);
            if (!(iter == word_id.end())) {
                concepts.back().insert(iter->second);
                c_vocab.insert(iter->second);
            }
            if (concepts.back().size() >= C) {
                break;
            }
        }
    }

    B = concepts.size();
    V_c = c_vocab.size();

    cout << currentDateTime() << "...ConceptLDA.Load_concepts - end\n";
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void ConceptLda::load() {

    cout << currentDateTime() << "...ConceptLDA.load - Start\n";

    I = options.I;
    P = options.P;

    cout << currentDateTime() << "...ConceptLDA.load - Load corpus\n";

    Load_corpus();

    alpha = ((double)50) / ((double)K);
    beta_phi = ((double)200) / ((double)V);

    cout << currentDateTime() << "...ConceptLDA.load - Load deltas\n";

    Load_concepts();

    beta_psi = ((double)200) / ((double)V_c);

    K = options.model == bijective ? 0 : options.K;
    T = B + K;
    cout << currentDateTime() << "...ConceptLDA.load - Init counts\n";

    Init_random();

    hidden = new bool[T];

    for (int i=0; i<T; i++) {
        hidden[i] = false;
        visible_topics.push_back(i);
    }

    Update_n();

    cout << currentDateTime() << "...ConceptLDA.load - Set cache\n";

    pr = new double[T];

    cout << currentDateTime() << "...ConceptLDA.load - Done\n";
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void ConceptLda::save() {
    Calculate_theta();
    Calculate_phi();
    Calculate_psi();
    Write_distributions();
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void ConceptLda::prune(int iter, int n_prune, int start_y){

    int burn = this->options.burn;

    if (visible_topics.size() <= K) {
        return;
    }

    double m = ((double)(start_y))/((double)(burn-(3*I/4)));
    double b = start_y;

    int cur_y = visible_topics.size()-K;
    int des_y = m*((double)(iter-burn)) + b;

    if (cur_y <= des_y) {
        return;
    }

    int to_prune = cur_y - des_y;

    std::map<int, vector<int>> ordered;
    for (int i=0; i<T; i++) {
        if (hidden[i]) {
            continue;
        }
        int number_assign = 0;
        for (int j=0; j<D; j++) {
            number_assign += n_d[i][j];
        }
        ordered[number_assign].push_back(i);
    }

    bool keep[T];
    for (int i=0; i<T; i++) {
        keep[i] = false;
    }
    int idx = 0;
    for (auto o_itr = ordered.begin(); o_itr != ordered.end(); ++o_itr) {
        for (int i=0; i<o_itr->second.size(); i++) {
            keep[o_itr->second[i]] = idx >= to_prune;
            idx++;
        }
    }
    for (int i=0; i<T; i++) {
        if (hidden[i] || visible_topics.size() <= K) {
            continue;
        }
        if (!keep[i]) {
            cout << currentDateTime() << "...ConceptLDA.gibbs - pruning topic " << i << endl;
            Hide_topic(i);
        }
    }
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void ConceptLda::gibbs() {

    cout << currentDateTime() << "...ConceptLDA.gibbs - Start\n";

    int burn = this->options.burn;
    int start_y = T-K;
    int n_prune = ceil(((double)start_y) / ((double)(I-burn)));

    stats.tot_iteration_time = 0;

    for (int iter=0; iter <= I; iter++) {

        stats.assign_correct = 0;
        stats.assign_total = 0;

        auto start = high_resolution_clock::now();

        stats.iteration_time = 0;

        cout << currentDateTime() << "...ConceptLDA.gibbs - begin iter " << iter << "...topics " << visible_topics.size() << endl;

        if (iter % 100 == 0 && iter > 0) {
            save();
        }

        if (iter % burn == 0 && iter > 0 && options.model != bijective) {
            prune(iter, n_prune, start_y);
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

    cout << currentDateTime() << "...ConceptLDA.gibbs - Done\n";
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void ConceptLda::Init_random(){
    corpus_t = new vector<int>[D];
    for (int doc=0; doc<D; doc++) {
        vector<int> topics;
        for (int token=0; token<corpus[doc].size(); token++) {
            int w = corpus[doc][token];
            int t = Rand(RANDOM_NUMBER, 0, T-1);
            topics.push_back(t);
        }
        corpus_t[doc] = topics;
    }
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void ConceptLda::Update_n(){
    n_w_phi = new int*[K];
    n_w_psi = new int*[B];
    n_w_dot_phi = new int[K];
    n_w_dot_psi = new int[B];
    n_d_dot = new int[T];
    n_d = new int*[T];

    for (int i=0; i<K; i++) {
        n_w_dot_phi[i] = 0;
        n_w_phi[i] = new int[V];
        for (int j=0; j<V; j++) {
            n_w_phi[i][j] = 0;
        }
    }

    for (int i=0; i<B; i++) {
        n_w_dot_psi[i] = 0;
        n_w_psi[i] = new int[V];
        for (int j=0; j<V; j++) {
            n_w_psi[i][j] = 0;
        }
    }

    for (int i=0; i<T; i++) {
        n_d_dot[i] = 0;
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
            if (t < K) {
                n_w_phi[t][w]++;
                n_w_dot_phi[t]++;
            }
            else {
                n_w_psi[t-K][w]++;
                n_w_dot_psi[t-K]++;
            }

            n_d[t][doc]++;
        }
        for (auto itr = topics_in_doc.begin(); itr != topics_in_doc.end(); ++itr) {
            n_d_dot[(*itr)]++;
        }
    }
    for (int i=0; i<K; i++) {
        if (n_w_dot_phi[i] == 0) {
            Hide_topic(i);
        }
    }
    for (int i=0; i<B; i++) {
        if (n_w_dot_psi[i] == 0 && options.model != bijective) {
            cout << currentDateTime() << "...removing topic " << (i+K) << endl;
            Hide_topic(i+K);
        }
    }
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void ConceptLda::Populate_prob(int i, int t, int word, int doc, int start) {
    if (t < K) {
        pr[i] = (((double) n_w_phi[t][word] + beta_phi) / (((double) n_w_dot_phi[t]) + ((double) V) * beta_phi)) *
               (((double) n_d[t][doc] + alpha) / (((double) (corpus[doc].size() - 1)) + ((double) visible_topics.size()) * alpha));
    }
    else {
        int b = t-K;

        auto iter = concepts[b].find(word);
        if (iter == concepts[b].end()) {
            pr[i] = (beta_psi / (((double) n_w_dot_psi[b]) + ((double) V) * beta_psi)) *
                    (alpha / (((double) (corpus[doc].size() - 1)) + ((double) visible_topics.size()) * alpha));
        }
        else {
            pr[i] = (((double) n_w_psi[b][word] + beta_psi) / (((double) n_w_dot_psi[b]) + ((double) V) * beta_psi)) *
                    (((double) n_d[t][doc] + alpha) / (((double) (corpus[doc].size() - 1)) + ((double) visible_topics.size()) * alpha));
        }
    }

    if (i > start) {
        pr[i] += pr[i-1];
    }
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
int ConceptLda::Pop_sample(int word, int doc) {

    for (int i=0; i<visible_topics.size(); i++) {
        int t = visible_topics[i];
        Populate_prob(i, t, word, doc, 0);
    }

    double scale = pr[visible_topics.size()-1] * gsl_rng_uniform(RANDOM_NUMBER);

    int topic = 0;

    if (pr[0] <= scale) {
        int low = 0;
        int high = visible_topics.size()-1;
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
int ConceptLda::Sample(int doc, int token){

    int topic = corpus_t[doc][token];
    int w = corpus[doc][token];

    n_d[topic][doc] = Max(n_d[topic][doc]-1, 0);
    if (topic < K) {
        n_w_phi[topic][w] = Max(n_w_phi[topic][w] - 1, 0);
        n_w_dot_phi[topic] = Max(n_w_dot_phi[topic] - 1, 0);
    }
    else {
        n_w_psi[topic-K][w] = Max(n_w_psi[topic-K][w] - 1, 0);
        n_w_dot_psi[topic-K] = Max(n_w_dot_psi[topic-K] - 1, 0);
    }

    if (n_d[topic][doc] == 0) {
        n_d_dot[topic]--;
    }

    if (topic < K && n_w_dot_phi[topic] == 0 && visible_topics.size() > K && !hidden[topic] && options.model != bijective) {
        cout << currentDateTime() << "...removing topic " << topic << endl;
        Hide_topic(topic);
    }
    else if (topic >= K && n_w_dot_psi[topic-K] == 0 && visible_topics.size() > K && !hidden[topic] && options.model != bijective) {
        cout << currentDateTime() << "...removing topic " << topic << endl;
        Hide_topic(topic);
    }

    alpha = ((double)50) / ((double)visible_topics.size());
    topic = Pop_sample(w, doc);
    topic = visible_topics[topic];

    stats.assign_total++;
    int offset = options.model == bijective ? 0 : K;
    if (options.use_key && ground_truth[doc][token] + offset == topic) {
        stats.assign_correct++;
    }
    if (n_d[topic][doc] == 0) {
        n_d_dot[topic]++;
    }

    n_d[topic][doc]++;

    if (topic < K) {
        n_w_phi[topic][w]++;
        n_w_dot_phi[topic]++;
    }
    else {
        n_w_psi[topic-K][w]++;
        n_w_dot_psi[topic-K]++;
    }

    return topic;
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void ConceptLda::Calculate_theta(){
    for (int i=0; i<theta.size(); i++) {
        theta[i].clear();
    }
    theta.clear();

    int topic_count = visible_topics.size();

    for (int doc=0; doc<D; doc++) {
        vector<double> theta_d(T);
        for (int t=0; t<T; t++) {
            theta_d[t] = (((double)n_d[t][doc]) + alpha) / (((double)corpus[doc].size()) + (((double)topic_count)*alpha));
        }
        theta.push_back(theta_d);
    }
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void ConceptLda::Calculate_psi(){
    for (int i=0; i<psi.size(); i++) {
        psi[i].clear();
    }
    psi.clear();
    for (int t=0; t<B; t++) {
        vector<double> psi_t(V);

        for (int w=0; w<V; w++) {
            psi_t[w] = (((double) n_w_psi[t][w]) + beta_psi)/(((double) n_w_dot_psi[t]) + (((double)V)* beta_psi));
        }
        psi.push_back(psi_t);
    }
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void ConceptLda::Calculate_phi(){
    for (int i=0; i<phi.size(); i++) {
        phi[i].clear();
    }
    phi.clear();
    for (int t=0; t<K; t++) {
        vector<double> phi_t(V);
        for (int w=0; w<V; w++) {
            phi_t[w] = (((double) n_w_phi[t][w]) + beta_phi)/(((double) n_w_dot_phi[t]) + (((double)V)* beta_phi));
        }
        phi.push_back(phi_t);
    }
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
string ConceptLda::Clean(double d) {

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
void ConceptLda::Write_distributions() {

    int top = options.display.top ? options.display.n : T;
    vector<int> idx(T);

    ofstream theta_out;
    theta_out.open(options.output_dir + "/theta.dat");

    for (int doc=0; doc<D; doc++) {
        theta_out << doc;
        for (int t=0; t<T; t++) {
            theta_out << " " << theta[doc][t];
        }
        theta_out << endl;
    }

    theta_out.close();

    ofstream phi_out;
    phi_out.open(options.output_dir + "/phi.dat");

    for (int t=0; t<K; t++) {
        if (hidden[t]) { continue; }
        phi_out << t;
        for (int w=0; w<V; w++) {
            phi_out << " " << phi[t][w];
        }
        phi_out << endl;
    }

    phi_out.close();

    ofstream psi_out;
    psi_out.open(options.output_dir + "/psi.dat");
    for (int t=0; t<B; t++) {
        if (hidden[t+K]) { continue; }
        string topic = options.display.labels ? topic_labels[t] : to_string(t);
        psi_out << topic;
        if (options.display.top) {
            vector<double> psi_t(psi[t]);
            Sort(psi_t, idx);
            for (int w=0; w<options.display.n; w++) {
                string word = options.display.labels ? id_word[idx[w]] : to_string(idx[w]);
                psi_out << " " << word << ":" << Clean(psi_t[w]);
            }
        }
        else {
            for (int w=0; w<V; w++) {
                psi_out << " " << Clean(psi[t][w]);
            }
        }
        psi_out << endl;
    }
    psi_out.close();

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
ConceptLda::ConceptLda(ConceptLdaOptions options) {
    this->options = options;
    RANDOM_NUMBER = gsl_rng_alloc(gsl_rng_taus);
    time_t t;
    time(&t);
    long seed = (long) t;
    gsl_rng_set(RANDOM_NUMBER, (long) seed);
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
ConceptLda::~ConceptLda() {

    for (int i=0; i<T; i++) {
        delete[] n_d[i];
    }
    for (int i=0; i<K; i++) {
        delete[] n_w_phi[i];
    }
    for (int i=0; i<B; i++) {
        delete[] n_w_psi[i];
    }
    delete[] n_w_phi;
    delete[] n_w_psi;
    delete[] n_d;
    delete[] n_w_dot_phi;
    delete[] n_w_dot_psi;
    delete[] n_d_dot;

    delete[] corpus_t;
    delete[] hidden;
    delete[] pr;

    gsl_rng_free (RANDOM_NUMBER);
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------