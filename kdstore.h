//
// Created by Rahul Kumar on 3/31/20.
//

#ifndef KVSTORE_KDSTORE_H
#define KVSTORE_KDSTORE_H

#include "dataframe.h"
#include "kvstore.h"
#include "serial.h"

class DataFrame;

class KDStore : public KV {

public:
    KDStore():KV() {
    }

    DataFrame* get(Key* k) {
        Value* v = map_->get(k);
        Deserializer d(v->val_,v->size_);
        return new DataFrame(d);
    }

    DataFrame* waitandget(Key* k) {
        Value* v = map_->get(k);
        Deserializer d(v->val_,v->size_);
        return new DataFrame(d);
    }

    void put(Key* k, DataFrame* df) {
        Serializer s;
        df->serialize(s);
        map_->put(k, new Value(s));
    }
};

/** Read a list of doubles into a dataframe, and store that dataframe in a kvstore
    	@return the dataframe of doubles*/
DataFrame* DataFrame::fromArray(Key* k, KDStore* kv, size_t num, double* vals) {
    Schema s("D");
    DataFrame* ret = new DataFrame(s);
    Row r(s);
    DoubleArray* da = new DoubleArray();
    for(size_t i = 0; i < num; i++) {
        da->add(vals[i]);
        r.set(0, vals[i]);
        ret->add_row(r);
    }
    kv->put(k, ret);
    return ret;
}

/** Read a double into a dataframe with a single column, and store that dataframe in a kvstore
    @return the dataframe of doubles*/
DataFrame* DataFrame::fromScalar(Key* k, KDStore* kv, double val) {
    Schema s ("D");
    DataFrame* ret = new DataFrame(s);
    Row r(s);
    r.set(0, val);
    ret->add_row(r);
    kv->put(k, ret);
    return ret;
}
#endif KVSTORE_KDSTORE_H