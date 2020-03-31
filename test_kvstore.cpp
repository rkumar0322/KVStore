#include <assert.h>
#include "serial.h"
#include "dataframe.h"
#include "kvstore.h"


void test_kvstore() {
    size_t SZ = 1000*1000;
    double* vals = new double[SZ];
    double sum = 0;
    KV* kv = new KV();
    for (size_t i = 0; i < SZ; ++i) sum += vals[i] = i;
    String* s = new String("triv");
    Key* key = new Key(s, 0);
    DataFrame* df = DataFrame::fromArray(key, kv, SZ, vals);
    assert(df->get_double(0,1) == 1);
    delete df; 
    std::cout << "fromArray pass" << "\n";
}

void test_kvstore2() {
    double val = 1000*1000;
    KV* kv = new KV();
    String* s = new String("verif");
    Key* key = new Key(s, 0);
    DataFrame* df = DataFrame::fromScalar(key, kv, val);
    assert(df->get_double(0,0) == 1000*1000);
    delete df; 
    std::cout << "fromScalar pass" << "\n";
}

void test_DataFrameSerialize() {

    size_t SZ = 1000*1000;
    double* vals = new double[SZ];
    double sum = 0;
    KV* kv = new KV();
    for (size_t i = 0; i < SZ; ++i) sum += vals[i] = i;
    String* s = new String("triv");
    Key* key = new Key(s, 0);
    DataFrame* df = DataFrame::fromArray(key, kv, SZ, vals);
    Serializer s1;
    df->serialize(s1);
    Deserializer d(s1.data_,s1.length_);
    DataFrame* df2 = new DataFrame(d);
    assert(df2->get_double(0,1) == 1000*1000);
    delete df2;
    std::cout << "Serialize/deserialize Dataframe pass" << "\n";
}

int main(int argc, char** argv) {

    test_kvstore();
    test_kvstore2();
    test_DataFrameSerialize();

    return 0;
}
