#include "../helpers/helper.h"


class Application: public Object
{
public:
    KDStore kv;
    size_t index;

    Application(size_t idx) : kv(idx) {
        index = idx;

    }

    Application(size_t idx,NetworkIfc& ifc):kv(ifc,idx) {
        index = idx;
    }

    virtual void run_() {

    }

    size_t this_node() {
        return index;
    }


};

class Trivial : public Application {
public:
    Trivial(size_t idx,NetworkIfc& ifc) :Application(idx,ifc) {}
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

class Demo : public Application {
public:
    Demo(size_t idx): Application(idx) {}
    Demo(size_t idx,NetworkIfc& ifc) :Application(idx,ifc) {}

    void run_() override {
        switch(this_node()) {
            case 0:   producer();     break;
            case 1:   counter();      break;
            case 2:   summarizer();
        }
    }

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

    void counter() {
        Key main("main",0);
        Key verify("verif",0);
        Key check("ck",0);
        DataFrame* v = kv.waitAndGet(main);
        printf("DAT OBTAINED\n");
        size_t sum = 0;
        for (size_t i = 0; i < 100*1000; ++i) sum += v->get_double(0,i);
        p("The sum is  ").pln(sum);
        DataFrame::fromScalar(&verify, &kv, sum);
    }

    void summarizer() {
        Key main("main",0);
        Key verify("verif",0);
        Key check("ck",0);
        DataFrame* result = kv.waitAndGet(verify);
        printf("DAT OBTAINED\n");
        DataFrame* expected = kv.waitAndGet(check);
        printf("DAT OBTAINED\n");
        pln(expected->get_double(0,0)==result->get_double(0,0) ? "SUCCESS":"FAILURE");
    }
};