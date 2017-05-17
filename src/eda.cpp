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
#include "eda.h"
#include <algorithm>
#include <chrono>
//----------------------------------------------------------------------------------
using namespace std;
using namespace std::chrono;
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
EdaOptions::EdaOptions() {
    K = 100;
    I = 1000;
    P = 3;
    burn = 100;
    corpus = "sampled_input_200_c.dat";
    edatopics = "bias_topics_c.dat";
    output_dir = ".";
    model = src;
    use_key = false;
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void Eda::Load_corpus(){

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
void Eda::Load_topics(){

    for (int i=0; i<K; i++) {
        topic_labels.push_back(to_string(i));
    }

    cout << currentDateTime() << "...EDA.Load_concepts - begin\n";

    ifstream fin;
    fin.open(options.edatopics, ios_base::in);
    vector<int> counts;
    for (string line; getline(fin, line);) {
        istringstream fstring(line);
        string label;
        fstring >> label;
        topic_labels.push_back(label);

        string word;
        int count;

        unordered_map<int, double> phi_explicit_t;
        phi_explicit.push_back(phi_explicit_t);
        counts.push_back(0);

        while (fstring >> word >> count) {
            auto iter = word_id.find(word);
            if (!(iter == word_id.end())) {
                phi_explicit.back()[iter->second] = (double)count;
                counts.back() += count;
            }
        }
    }

    for (int i=0; i<phi_explicit.size(); i++) {
        for (auto itr = phi_explicit[i].begin(); itr != phi_explicit[i].end(); ++itr) {
            phi_explicit[i][itr->first] =  ((double)itr->second) / ((double)counts[i]);
        }
    }

    B = phi_explicit.size();
    cout << currentDateTime() << "...EDA.Load_concepts - end\n";
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void Eda::Display_stats(int iter) {
    double avg_eda = stats.cnt_model > 0 ? stats.tot_model / ((double)stats.cnt_model) : 0;
    double avg_reg = stats.cnt_reg > 0 ? stats.tot_reg / ((double)stats.cnt_reg) : 0;
    cout << currentDateTime() << "...EDA.gibbs - end iter...iteration time " << stats.iteration_time << endl;
    cout << currentDateTime() << "...EDA.gibbs - end iter...avg iteration time " << ((double)stats.tot_iteration_time) / ((double)iter+1)  << endl;
    cout << currentDateTime() << "...EDA.gibbs - end iter...avg eda " << avg_eda << endl;
    cout << currentDateTime() << "...EDA.gibbs - end iter...avg reg " << avg_reg << endl;
    if (options.use_key) {
        cout << currentDateTime() << "...SrcLDA.gibbs - end iter...accuracy "
             << ((((double) stats.assign_correct) / ((double) stats.assign_total)) * 100.0) << endl;
        cout << currentDateTime() << "...SrcLDA.gibbs - end iter...correct " << stats.assign_correct << endl;
        cout << currentDateTime() << "...SrcLDA.gibbs - end iter...total " << stats.assign_total << endl;
    }
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void Eda::Hide_topic(int t) {
    visible_topics.erase(std::remove(visible_topics.begin(), visible_topics.end(), t), visible_topics.end());
    hidden[t] = true;
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void Eda::load() {

    cout << currentDateTime() << "...EDA.load - Start\n";

    I = options.I;
    P = options.P;
    K = options.model == bijective ? 0 : options.K;
    token_count = 0;

    cout << currentDateTime() << "...EDA.load - Load corpus\n";

    Load_corpus();


    beta = ((double)200) / ((double)V);

    cout << currentDateTime() << "...EDA.load - Load deltas\n";

    Load_topics();
    T = B + K;
    cout << currentDateTime() << "...EDA.load - Init counts\n";

    Init_random();
    Update_n();

    hidden = new bool[T];

    for (int i=0; i<T; i++) {
        hidden[i] = false;
        visible_topics.push_back(i);
    }

    alpha = ((double)50) / ((double)visible_topics.size());

    pr = new double[T];

    cout << currentDateTime() << "...EDA.load - Done\n";
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void Eda::save() {
    Calculate_theta();
    Calculate_phi_latent();

    Write_distributions();
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void Eda::prune(int iter, int n_prune, int start_y){

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
            cout << currentDateTime() << "...EDA.gibbs - pruning topic " << i << endl;
            Hide_topic(i);
        }
    }
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void Eda::gibbs() {

    cout << currentDateTime() << "...EDA.gibbs - Start\n";

    int burn = this->options.burn;
    int start_y = T-K;
    int n_prune = ceil(((double)start_y) / ((double)(I-burn)));

    stats.tot_iteration_time = 0;

    for (int iter=0; iter < I; iter++) {

        stats.assign_correct = 0;
        stats.assign_total = 0;

        auto start = high_resolution_clock::now();

        stats.tot_reg = 0;
        stats.cnt_reg = 0;
        stats.tot_model = 0;
        stats.cnt_model = 0;
        stats.iteration_time = 0;

        cout << currentDateTime() << "...EDA.gibbs - begin iter " << iter << "...topics " << visible_topics.size() << endl;

        if (iter % 100 == 0 && iter > 0) {
            Calculate_theta();
            Calculate_phi_latent();
            Write_distributions();
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

    cout << currentDateTime() << "...EDA.gibbs - Done\n";
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void Eda::Init_random(){
    corpus_t = new vector<int>[D];
    for (int doc=0; doc<D; doc++) {
        vector<int> topics;
        for (int token=0; token<corpus[doc].size(); token++) {
            int t = Rand(RANDOM_NUMBER, 0, T-1);
            topics.push_back(t);
        }
        corpus_t[doc] = topics;
    }
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void Eda::Update_n(){
    n_w = new int*[K];
    n_w_dot = new int[T];
    n_d_dot = new int[T];
    n_d = new int*[T];
    for (int i=0; i<K; i++) {
        n_w[i] = new int[V];
        for (int j=0; j<V; j++) {
            n_w[i][j] = 0;
        }
    }
    for (int i=0; i<T; i++) {
        n_w_dot[i] = 0;
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
                n_w[t][w]++;
            }
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
void Eda::Populate_prob(int i, int t, int word, int doc, int start) {
    if (t < K) {
        pr[i] = (((double) n_w[t][word] + beta) / (((double) n_w_dot[t]) + ((double) V) * beta)) *
               (((double) n_d[t][doc] + alpha) / (((double) (corpus[doc].size() - 1)) + ((double) visible_topics.size()) * alpha));

        stats.cnt_reg++;
        stats.tot_reg += pr[i];
    }
    else {

        int b = t-K;
        auto iter = phi_explicit[b].find(word);
        if (iter == phi_explicit[b].end()) {
            pr[i] = (beta / (((double) n_w_dot[t]) + ((double) V) * beta)) *
                    (alpha / (((double) (corpus[doc].size() - 1)) + ((double) visible_topics.size()) * alpha));
        }
        else{
            pr[i] = iter->second * (((double) n_d[t][doc] + alpha) / (((double) (corpus[doc].size() - 1)) + ((double) visible_topics.size()) * alpha));
        }

        stats.cnt_model++;
        stats.tot_model += pr[i];
    }

    if (i > start) {
        pr[i] += pr[i-1];
    }
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
int Eda::Pop_sample(int word, int doc) {


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
int Eda::Sample(int doc, int token){

    int topic = corpus_t[doc][token];
    int w = corpus[doc][token];

    n_d[topic][doc] = Max(n_d[topic][doc]-1, 0);

    if (topic < K) {
        n_w[topic][w] = Max(n_w[topic][w] - 1, 0);
    }
    n_w_dot[topic] = Max(n_w_dot[topic]-1, 0);

    if (n_d[topic][doc] == 0) {
        n_d_dot[topic]--;
    }

    if (n_w_dot[topic] == 0 && visible_topics.size() > K && !hidden[topic] && !(options.model == bijective)) {
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
        n_w[topic][w]++;
    }
    n_w_dot[topic]++;
    return topic;
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void Eda::Calculate_theta(){
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
void Eda::Calculate_phi_latent(){
    for (int i=0; i<phi_latent.size(); i++) {
        phi_latent[i].clear();
    }
    phi_latent.clear();
    for (int t=0; t<K; t++) {
        vector<double> phi_t(V);
        for (int w=0; w<V; w++) {
            phi_t[w] = (((double)n_w[t][w]) + beta)/(((double)n_w_dot[t]) + (((double)V)*beta));
        }
        phi_latent.push_back(phi_t);
    }
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void Eda::Write_distributions() {
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

    vector<int> idx(T);

    ofstream phi_out_latent;
    phi_out_latent.open(options.output_dir + "/phi.dat");
    for (int t=0; t<K; t++) {
        if (hidden[t]) { continue; }
        phi_out_latent << t;
        for (int w=0; w<V; w++) {
            phi_out_latent << " " << phi_latent[t][w];
        }
        phi_out_latent << endl;
    }
    phi_out_latent.close();

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
            int cur = corpus_t[doc][t];
            string topic = options.display.labels ? topic_labels[cur] : to_string(cur);
            if (t==0) {
                t_out << topic;
            }
            else {
                t_out << " " << topic;
            }
        }
        t_out << endl;
    }
    t_out.close();
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
Eda::Eda(EdaOptions options) {
    this->options = options;
    RANDOM_NUMBER = gsl_rng_alloc(gsl_rng_taus);
    time_t t;
    time(&t);
    long seed = (long) t;
    gsl_rng_set(RANDOM_NUMBER, (long) seed);
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
Eda::~Eda() {

    for (int i=0; i<K; i++) {
        delete[] n_w[i];
    }

    for (int i=0; i<T; i++) {
        delete[] n_d[i];
    }

    delete[] n_w;
    delete[] n_d;
    delete[] n_w_dot;
    delete[] n_d_dot;

    delete[] corpus_t;
    delete[] hidden;
    delete[] pr;

    gsl_rng_free (RANDOM_NUMBER);
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------