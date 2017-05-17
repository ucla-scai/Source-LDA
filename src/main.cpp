#include <iostream>
#include "srclda.h"
#include "srclda_parallel.h"
#include "eda.h"
#include "eda_parallel.h"
#include "conceptlda.h"
#include "conceptlda_parallel.h"
#include "lda.h"
#include "gtpoints.h"
//----------------------------------------------------------------------------------
using namespace std;
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
class InputParser{
public:
    InputParser (int &argc, char **argv){
        for (int i=1; i < argc; ++i) {
            vector<string> s;
            string cur = std::string(argv[i]);
            split(cur, '=', s);
            if (s.size() > 0) {
                this->keys.push_back(s.front());
                this->vals.push_back(s.back());
            }
            else {
                this->keys.push_back(cur);
            }
        }
    }

    string getCmdOption(const string &option) const{

        for (int i=0; i<this->keys.size(); i++) {
            if (this->keys[i] == option) {
                return this->vals[i];
            }
        }
        return "";
    }
    bool cmdOptionExists(const std::string &option) const{
        return std::find(this->keys.begin(), this->keys.end(), option) != this->keys.end();
    }
private:
    std::vector <std::string> keys;
    std::vector <std::string> vals;
};
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
enum Algorithm {
    src_lda, eda, ctm, lda
};
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void Display_help(const string & exe) {
    cout << "\nSource-LDA.\n\n";
    cout << "Usage:\n\t";

    cout << exe.substr(exe.rfind('/') + 1) << "\n\t";
    cout << exe.substr(exe.rfind('/') + 1) << " -h | --help\n\t";
    cout << exe.substr(exe.rfind('/') + 1) << " -g [-ks=<file>] [-out=<file>] [-P=<number>]\n\t";
    cout << exe.substr(exe.rfind('/') + 1) << "\n\t\t[-alg=<name>]\n\t\t";
    cout << "[-model=<name>]\n\t\t[-in=<file>]\n\t\t[-out=<file>]\n\t\t";
    cout << "[-P=<number>]\n\t\t[-K=<number>]\n\t\t[-ks=<file>]\n\t\t";
    cout << "[-I=<number>]\n\t\t[-C=<number>]\n\t\t";
    cout << "[-mu=<number>]\n\t\t[-sigma=<number>]\n\t\t[-perp=<name>]\n\t\t";
    cout << "[-log=<bool>]\n\t\t[-A=<number>]\n\t\t[-gt=<file>]\n\t\t";
    cout << "[-key=<file>]\n\t\t";
    cout << "[-save=<list>]\n\t\t";
    cout << "[-left=<number>]\n\t\t";
    cout << "[-right=<number>]\n\t\t";
    cout << "[-alpha=<number>]\n\t\t";
    cout << "[-raw=<bool>]\n\t\t";
    cout << "\nOptions:\n\t";
    cout << "-h --help Show this screen (also shows with no parameters)\n\t";
    cout << "-g Generate g_t points file\n\t";
    cout << "-alg=<name> Algorithm used (src|eda|ctm|lda) [default: src]\n\t";
    cout << "-model=<name> Model used (src|bijective|mix) [default: src]\n\t";
    cout << "-in=<file> Input corpus [default: input.dat]\n\t";
    cout << "-out=<file> Directory to save output [default: .]\n\t";
    cout << "-P=<file> Number of threads [default: 1]\n\t";
    cout << "-K=<file> Number of unlabeled topics [default: 100]\n\t";
    cout << "-ks=<file> Knowledge source [default: ks.dat]\n\t";
    cout << "-I=<number> Number of iterations [default: 1000]\n\t";
    cout << "-C=<number> Top number of n-grams to keep for CTM [default: 10000]\n\t";
    cout << "-mu=<number> Mu value for Source-LDA [default: 0.7]\n\t";
    cout << "-sigma=<number> Sigma value for Source-LDA [default: 0.3]\n\t";
    cout << "-perp=<name> Perplexity calculation used (none|hein|imp|lr) [default: none]\n\t";
    cout << "-log=<bool> Display log likelihood (true|false) [default: false]\n\t";
    cout << "-A=<number> Approximation steps for Source-LDA [default: 5]\n\t";
    cout << "-gt=<file> File containg gt point pre calculations to load/save [default: gt.dat]\n\t";
    cout << "-key=<file> Key used for classification percentage [default: key.dat]\n\t";
    cout << "-save=<list> Comma seperated list of iteration save points [default: ]\n\t";
    cout << "-left=<number> Left bound for sigma [default: 0]\n\t";
    cout << "-right=<number> Right bound for sigma [default: ]\n\t";
    cout << "-alpha=<number> Symmetric hyperparameter for theta [defalut: 50/T]\n\t";
    cout << "-raw=<false> Output entire phi and theta matrices [defalut: false]\n\n";
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
int main(int argc, char* argv[]) {

    Algorithm alg = src_lda;
    Model mod = src;
    string in = "input.dat";
    string out = ".";
    int P = 1;
    int K = 100;
    string ks = "ks.dat";
    int I = 1000;
    int C = 10000;
    double left = 0.0;
    double right = DBL_MAX;
    double mu = 0.7;
    double sigma = 0.3;
    Perplexity perp = none;
    bool log = false;
    int A = 5;
    string gt = "gt.dat";
    string key = "key.dat";
    vector<int> save;
    double alpha = 0.0;
    bool use_alpha = false;
    bool raw = false;

    InputParser input(argc, argv);
    if(argc == 1 || input.cmdOptionExists("-h") || input.cmdOptionExists("--help")){
        Display_help(argv[0]);
        return 0;
    }
    if(input.cmdOptionExists("-g")){
        string out_input = input.getCmdOption("-out");
        if (!out_input.empty()){
            out = out_input;
        }
        string ks_input = input.getCmdOption("-ks");
        if (!ks_input.empty()){
            ks = ks_input;
        }
        string p_input = input.getCmdOption("-P");
        if (!p_input.empty()){
            P = stoi(p_input);
        }

        if (!File_exists(ks)) {
            Display_help(argv[0]);
            return 0;
        }

        GtPointsOptions options;
        options.P = P;
        options.srctopics = ks;
        options.output_dir = out;
        GtPoints gtpoints(options);
        gtpoints.run();
        gtpoints.save();
        return 0;
    }
    string alg_input = input.getCmdOption("-alg");
    if (!alg_input.empty()){
        if (alg_input == "src") { alg = src_lda; }
        if (alg_input == "eda") { alg = eda; }
        if (alg_input == "ctm") { alg = ctm; }
        if (alg_input == "lda") { alg = lda; }
    }
    string mod_input = input.getCmdOption("-model");
    if (!mod_input.empty()){
        if (mod_input == "src") { mod = src; }
        if (mod_input == "bijective") { mod = bijective; }
        if (mod_input == "mix") { mod = mix; }
    }
    string in_input = input.getCmdOption("-in");
    if (!in_input.empty()){
        in = in_input;
    }
    string out_input = input.getCmdOption("-out");
    if (!out_input.empty()){
        out = out_input;
    }
    string save_input = input.getCmdOption("-save");
    if (!save_input.empty()){
        vector<string> save_str;
        split(save_input, ',', save_str);
        for (int i=0; i<save_str.size(); i++) {
            save.push_back(stoi(save_str[i]));
        }
    }
    string p_input = input.getCmdOption("-P");
    if (!p_input.empty()){
        P = stoi(p_input);
    }
    string k_input = input.getCmdOption("-K");
    if (!k_input.empty()){
        K = stoi(k_input);
    }
    string ks_input = input.getCmdOption("-ks");
    if (!ks_input.empty()){
        ks = ks_input;
    }
    string i_input = input.getCmdOption("-I");
    if (!i_input.empty()){
        I = stoi(i_input);
    }
    string c_input = input.getCmdOption("-C");
    if (!c_input.empty()){
        C = stoi(c_input);
    }
    string alpha_input = input.getCmdOption("-alpha");
    if (!alpha_input.empty()){
        alpha = stod(alpha_input);
        use_alpha = true;
    }
    string left_input = input.getCmdOption("-left");
    if (!left_input.empty()){
        left = stod(left_input);
    }
    string right_input = input.getCmdOption("-right");
    if (!right_input.empty()){
        right = stod(right_input);
    }
    string mu_input = input.getCmdOption("-mu");
    if (!mu_input.empty()){
        mu = stod(mu_input);
    }
    string sigma_input = input.getCmdOption("-sigma");
    if (!sigma_input.empty()){
        sigma = stod(sigma_input);
    }
    string perp_input = input.getCmdOption("-perp");
    if (!perp_input.empty()){
        if (perp_input == "none") { perp = none; }
        if (perp_input == "hein") { perp = heinrich; }
        if (perp_input == "imp") { perp = imp; }
        if (perp_input == "lr") { perp = lr; }
    }
    string raw_input = input.getCmdOption("-raw");
    if (!raw_input.empty()){
        if (raw_input == "true") { raw = true; }
        if (raw_input == "false") { raw = false; }
    }
    string log_input = input.getCmdOption("-log");
    if (!log_input.empty()){
        if (log_input == "true") { log = true; }
        if (log_input == "false") { log = false; }
    }
    string a_input = input.getCmdOption("-A");
    if (!a_input.empty()){
        A = stoi(a_input);
    }
    string gt_input = input.getCmdOption("-gt");
    if (!gt_input.empty()){
        gt = gt_input;
    }
    string key_input = input.getCmdOption("-key");
    if (!key_input.empty()){
        key = key_input;
    }

    cout << currentDateTime() << "..." << "<BEGIN>\n";
    if (alg == src_lda) {
        SrcLdaOptions options;
        options.A = A;
        options.corpus = in;
        options.gtpoints = gt;
        options.key = key;
        options.use_gtpoints = File_exists(gt);
        options.I = I;
        options.K = K;
        options.model = mod;
        options.mu = mu;
        options.sigma = sigma;
        options.output_dir = out;
        options.srctopics = ks;
        options.perplexity = perp;
        options.P = P;
        options.use_key = File_exists(key);
        options.show_loglike = log;
        options.left = left;
        options.right = right;
        options.alpha = alpha;
        options.use_alpha = use_alpha;
        options.display.top = !raw;
        for (int i=0; i<save.size(); i++) {
            options.save_points.insert(save[i]);
        }
        if (P > 1) {
            SrcLdaParallel topic_model(options);
            topic_model.load();
            topic_model.gibbs();
            topic_model.save();
        }
        else {
            SrcLda topic_model(options);
            topic_model.load();
            topic_model.gibbs();
            topic_model.save();
        }
    }
    if (alg == eda) {
        EdaOptions options;
        options.corpus = in;
        options.key = key;
        options.I = I;
        options.K = K;
        options.model = mod;
        options.output_dir = out;
        options.edatopics = ks;
        options.P = P;
        options.use_key = File_exists(key);
        options.display.top = !raw;
        if (P > 1) {
            EdaParallel topic_model(options);
            topic_model.load();
            topic_model.gibbs();
            topic_model.save();
        }
        else {
            Eda topic_model(options);
            topic_model.load();
            topic_model.gibbs();
            topic_model.save();
        }
    }
    if (alg == ctm) {
        ConceptLdaOptions options;
        options.corpus = in;
        options.key = key;
        options.I = I;
        options.K = K;
        options.model = mod;
        options.output_dir = out;
        options.concepttopics = ks;
        options.P = P;
        options.C = C;
        options.use_key = File_exists(key);
        options.display.top = !raw;
        if (P > 1) {
            ConceptLdaParallel topic_model(options);
            topic_model.load();
            topic_model.gibbs();
            topic_model.save();
        }
        else {
            ConceptLda topic_model(options);
            topic_model.load();
            topic_model.gibbs();
            topic_model.save();
        }
    }
    if (alg == lda) {
        LdaOptions options;
        options.corpus = in;
        options.I = I;
        options.K = K;
        options.output_dir = out;
        options.display.top = !raw;

        Lda topic_model(options);
        topic_model.load();
        topic_model.gibbs();
        topic_model.save();
    }
    cout << currentDateTime() << "..." << "<END>\n";
    return 0;
}
//----------------------------------------------------------------------------------