#include "hashFuncs.h"


SimpleHash::SimpleHash(int dim, double w, int divisor) {
    this->dimension = dim;
    this->gridW = w;
    this->divisor = divisor;
    this->shifts = new vector<double>();
    this->randShifts();
}


SimpleHash::~SimpleHash() {
    delete this->shifts;
}


void SimpleHash::randShifts() { // create a disturbance for each image vector's element
    for (int i = 0; i < this->dimension; ++i) {
        this->shifts->push_back(generateDcml());
    }
}

double SimpleHash::generateDcml() const{ // return a random single disturbance
    unsigned seed = system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    uniform_real_distribution<double> distribution(0.0, this->gridW);
    return distribution(generator);
}

int SimpleHash::mod(int divident,int divisor) { //optimal modulo operation
    if(divident >= 0) {
        return divident & (divisor - 1);
    }
    return divident % divisor + divisor;
}

//optimal modulo operation ,avoids overflow when last bit is used
int SimpleHash::mod(unsigned int divident,int divisor) {
    return divident & (divisor - 1);
}

//optimal modular exponentiation algorithm
int SimpleHash::modularExp(unsigned int base, unsigned int exp, int div) {
    unsigned int factor = 1;
    unsigned int prevRes;
    for (unsigned int i = 1; i <= exp; i*=2) { // calculate mod for every term that sums up to exponent
        unsigned int nextRes;
        if(i == 1)
            nextRes = SimpleHash::mod(base,div);
        else { // take advantage of previous calculation
            unsigned int t = pow(SimpleHash::mod(prevRes,div),2);
            nextRes = SimpleHash::mod(t,div);
        }

        if((i & exp) != 0)
            factor *= nextRes;

        prevRes = nextRes;
    }


    return SimpleHash::mod(factor,div);
}

int SimpleHash::hashResult(vector<int> *pixels) {
    vector<int> a;
    /* calculate projections */
    for (int i = 0; i < this->dimension; ++i) {
        int a_i = floor(((int)pixels->at(i) - (double)this->shifts->at(i)) / this->gridW);
        a.push_back(a_i);
    }

    /* calculate hash result */
    int sum= 0;
    unsigned int m = (long)pow(2,32) - 5;
    for (int i= this->dimension-1; i >= 0; --i) {
        int a_i = a.at(i);
        int factor_1 = SimpleHash::modularExp(m,
                                              this->dimension-1-i,
                                              this->divisor);
        int factor_2 = SimpleHash::mod(a_i, this->divisor);
        sum+= SimpleHash::mod(factor_1 * factor_2, this->divisor);
    }
    return mod(sum, this->divisor);
}

AmplifiedHash::AmplifiedHash(int dim, double w, int tableSize, int numHashes) {
    subhashes = new vector<SimpleHash *>;
    this->numHashes = numHashes;
    this->tableSize = tableSize;
    for (int i = 0; i < numHashes; ++i) {
        subhashes->push_back(new SimpleHash(dim, w, pow(2,32/numHashes)));
    }
}

AmplifiedHash::~AmplifiedHash() {
    vector<SimpleHash *>::iterator it;
    for (it = this->subhashes->begin(); it < this->subhashes->end() ; ++it) {
        delete *it;
    }
    delete this->subhashes;
}

tuple<int,unsigned int> AmplifiedHash::hashResult(vector<int> * img){
    unsigned int retVal = 0;
    for (int i = 0; i < this->numHashes; ++i) {
        /* get every subhash result */
        unsigned int h_i = subhashes->at(i)->hashResult(img);
        unsigned int bitsPerHi = 32/this->numHashes;
        unsigned int shiftAm = (this->numHashes-1-i) * bitsPerHi;
        retVal |= (h_i << shiftAm);
    }
    return make_tuple((int)SimpleHash::mod(retVal,this->tableSize),retVal);
}
