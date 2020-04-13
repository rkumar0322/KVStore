#include "object.h"
#include "kdstore.h"
#include "keyvalue.h"


class Application: public Object 
{
public:
    KDStore* kv;
    size_t index;

    Application(size_t idx) {
        kv = new KDStore();
        index = idx;
    };

    Application(NetworkIfc* ifc, size_t idx) {
        kv = new KDStore(ifc, idx);
        index = idx;
    };
    Application(KDStore* kd, size_t idx) {
        kv = kd;
        index = idx;
    }

    ~Application() {
        delete kv;
    }

    virtual void run_() {

    }

    size_t this_node() {
        return index;
    }


};

class Trivial : public Application {
 public:
  Trivial(size_t idx) : Application(idx) { }
  void run_() {
    size_t SZ = 1000*1000;
    double* vals = new double[SZ];
    double sum = 0;
    for (size_t i = 0; i < SZ; ++i) sum += vals[i] = i;
    String* s = new String("triv");
    Key* key = new Key(s,0);
    DataFrame* df = DataFrame::fromArray(key, kv, SZ, vals);
    assert(df->get_double(0,1) == 1);
    DataFrame* df2 = kv->get(key);
    for (size_t i = 0; i < SZ; ++i) sum -= df2->get_double(0,i);
    assert(sum==0);
    delete df; delete df2;
  }
};

class Demo : public Application {
public:
  Key* main;
  Key* verify;
  Key* check;
 
  Demo(NetworkIfc* ifc, size_t idx): Application(ifc, idx) {
      String* s1 = new String("main");
      main = new Key(s1, 0);
      String* s2 = new String("verif");
      verify = new Key(s2, 0);
      String* s3 = new String("check");
      check = new Key(s3, 0);
  }

    Demo(KDStore* kv, size_t idx): Application(kv,idx) {
        String* s1 = new String("main");
        main = new Key(s1, 0);
        String* s2 = new String("verif");
        verify = new Key(s2, 0);
        String* s3 = new String("check");
        check = new Key(s3, 0);
    }
 
  void run_() override {
    switch(this_node()) {
    case 0:   producer();     break;
    case 1:   counter();      break;
    case 2:   summarizer();
   }
  }
 
  void producer() {
    size_t SZ = 100*1000;
    double* vals = new double[SZ];
    double sum = 0;
    for (size_t i = 0; i < SZ; ++i) sum += vals[i] = i;
    DataFrame::fromArray(main, kv, SZ, vals);
    DataFrame::fromScalar(check, kv, sum);
    printf("FINISHES THIS PART\n");
  }
 
  void counter() {
      printf("MAKES IT TO COUNTER\n");
    DataFrame* v = kv->waitandget(main);
      printf("GETS TO DATAFRAME\n");
    size_t sum = 0;
    for (size_t i = 0; i < 100*1000; ++i) sum += v->get_double(0,i);
    p("The sum is  ").pln(sum);
    DataFrame::fromScalar(verify, kv, sum);
  }
 
  void summarizer() {
    DataFrame* result = kv->waitandget(verify);
    DataFrame* expected = kv->waitandget(check);
    pln(expected->get_double(0,0)==result->get_double(0,0) ? "SUCCESS":"FAILURE");
  }
};