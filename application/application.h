#include "../helpers/object.h"
#include "../KDStore/kdstore.h"

/** The user-level interface which is used for reading,
 * storing, retrieving and writing data.
 * It contains a KDStore which keep tracks of all nodes,
 * and a number which represents its index.
 * 
 * 
 **/ 
class Application: public Object
{
public:
    KDStore kv;
    size_t index;

    /**Constructor*/
    Application(size_t idx) : kv(idx) {
        index = idx;

    }

    Application(NetworkIfc& ifc, size_t idx):kv(ifc,idx) {
        index = idx;
    }

    /** run the application*/
    virtual void run_() {

    }

    /** return the index of the current application*/
    size_t this_node() {
        return index;
    }


};

/** An example class for testing dataframe inside application*/
class Trivial : public Application {
public:
    Trivial(NetworkIfc& ifc, size_t idx) :Application(ifc,idx) {}
    Trivial(size_t idx) : Application(idx) { }
    void run_() {
        size_t SZ = 1000*1000;
        double* vals = new double[SZ];
        double sum = 0;
        for (size_t i = 0; i < SZ; ++i) sum += vals[i] = i;
        Key key("triv",0);
        DataFrame* df = DataFrame::fromArray(&key, &kv, SZ, vals);
        assert(df->get_double(0,1) == 1);
        DataFrame* df2 = kv.get(key);
        for (size_t i = 0; i < SZ; ++i) sum -= df2->get_double(0,i);
        assert(sum==0);
        delete df2;
        delete df;
        delete[] vals;
        printf("All tests pass\n");
    }
};

/** A Demo class for testing different nodes running in the same application 
 * Use 3 nodes: producer, counter, summerizer 
*/
class Demo : public Application {
public:

    /**Constructor*/
    Demo(size_t idx): Application(idx) {}

    /**Pass in a NetworkIfc to initialize the network inside application*/
    Demo(NetworkIfc& ifc, size_t idx) :Application(ifc,idx) {}

    void run_() override {
        switch(this_node()) {
            case 0:   producer();     break;
            case 1:   counter();      break;
            case 2:   summarizer();
        }
    }

    /**create a large dataframe with data and a small
     * dataframe with the sum.
     */
    void producer() {
        Key main("main",0);
        Key verify("verif",0);
        Key check("ck",0);
        size_t SZ = 100*1000;
        double* vals = new double[SZ];
        double sum = 0;
        for (size_t i = 0; i < SZ; ++i) sum += vals[i] = i;
        delete DataFrame::fromArray(&main, &kv, SZ, vals);
        delete DataFrame::fromScalar(&check, &kv, sum);
        delete[] vals;
    }

    /** get the dataframe and sum it*/
    void counter() {
        Key main("main",0);
        Key verify("verif",0);
        Key check("ck",0);
        DataFrame* v = kv.waitAndGet(main);
        size_t sum = 0;
        for (size_t i = 0; i < 100*1000; ++i) sum += v->get_double(0,i);
        p("The sum is  ").pln(sum);
        DataFrame::fromScalar(&verify, &kv, sum);
    }

    /** get both sums from producer and counter, and output the result*/
    void summarizer() {
        Key main("main",0);
        Key verify("verif",0);
        Key check("ck",0);
        DataFrame* result = kv.waitAndGet(verify);
        DataFrame* expected = kv.waitAndGet(check);
        pln(expected->get_double(0,0)==result->get_double(0,0) ? "SUCCESS":"FAILURE");
    }
};