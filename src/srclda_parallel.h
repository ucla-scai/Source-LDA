#ifndef SRC_LDA_C_SRCLDA_PARALLEL_H
#define SRC_LDA_C_SRCLDA_PARALLEL_H
//----------------------------------------------------------------------------------
#include <gsl/gsl_randist.h>
#include <vector>
#include "utility.h"
#include "srclda.h"

#if __cplusplus >= 201103
#include <atomic>
#else
#include <cstdatomic>
#endif

//----------------------------------------------------------------------------------
using namespace std;
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
class SrcLdaParallel : public SrcLda {
private:
    bool end;
    vector<int> ends;
    atomic<bool>* run;
    atomic<bool>* populated;
    int word;
    int doc;
    bool test;
    int Pop_sample(int word, int doc);
    int Pop_sample_test(int word, int doc);
    void Hide_topic(int t);
    static void * Populate_prob_thread_wrapper(void *args);
public:
    void gibbs();
    void Populate_prob_thread(int id);
    SrcLdaParallel(SrcLdaOptions options);
};
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
#endif //SRC_LDA_C_SRCLDA_PARALLEL_H
