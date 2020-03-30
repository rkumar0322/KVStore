#include <unistd.h>
#include <assert.h>
#include "serial.h"
#include "dataframe.h"
#include "kvstore.h"


void test_kvstore() {
    size_t SZ = 10;
    double* vals = new double[SZ];
    double sum = 0;
    KV* kv = new KV();
    for (size_t i = 0; i < SZ; ++i) sum += vals[i] = i;
    String* s = new String("triv");
    Key* key = new Key(s, 0);
    DataFrame* df = DataFrame::fromArray(key, kv, SZ, vals);
    assert(df->get_double(0,1) == 1);
    Value* v = kv->get(key);
    delete df; 
}

int main(int argc, char** argv) {

    test_kvstore();
    return 0;
}
