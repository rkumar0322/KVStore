#include <assert.h>
#include "application/application.h"


void test_kvstore() {
    size_t SZ = 1000*1000;
    double* vals = new double[SZ];
    double sum = 0;
    KDStore kv(0);
    for (size_t i = 0; i < SZ; ++i) sum += vals[i] = i;
    Key key("triv",0);
    DataFrame* df = DataFrame::fromArray(&key, &kv, SZ, vals);
    assert(df->get_double(0,1) == 1);
    delete df;
    delete[] vals;
    std::cout << "fromArray pass" << "\n";
}

void test_kvstore2() {
    double val = 1000*1000;
    KDStore kv(0);
    Key key("verif",0);
    DataFrame* df = DataFrame::fromScalar(&key, &kv, val);
    assert(df->get_double(0,0) == 1000*1000);
    delete df;
    std::cout << "fromScalar pass" << "\n";
}

void test_DataFrameSerialize() {
    Trivial d(0);
    d.run_();
    printf("THE TEST PASSES\n");
}

int main(int argc, char** argv) {
    test_DataFrameSerialize();
    return 0;
}
