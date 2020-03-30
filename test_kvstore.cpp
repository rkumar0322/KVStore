#include <unistd.h>
#include <assert.h>
#include "serial.h"
#include "array.h"
#include "message.h"
#include "dataframe.h"
#include "kvstore.h"


void test_double() {
    size_t SZ = 10;
    double* vals = new double[SZ];
    double sum = 0;
    KV* kv = new KV();
    for (size_t i = 0; i < SZ; ++i) sum += vals[i] = i;
    std::cout << sum << "\n";
    String* s = new String("triv");
    Key* key = new Key(s, 0);
    DataFrame* df = DataFrame::fromArray(key, kv, SZ, vals);
    assert(df->get_double(0,1) == 1);
    Value* v = kv->get(key);
    //DoubleArray* da = deserial_double_array(v->val_);
    //std::cout << da->get(0) << "\n";
    //for (size_t i = 0; i < SZ; ++i) sum -= df2->get_double(0,i);
    //assert(sum==0);
    delete df; 
    //delete df2;
}

int main(int argc, char** argv) {

    test_double();

	return 0;
}
