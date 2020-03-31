#pragma once

#include "object.h"
#include "map.h"

/**A key-value store, represented by a self-designed map*/
class KV: public Object {
public:
    Map* map_;
    size_t len;

    /*Constructor*/
    KV() {
        map_ = new Map();
        len = 0;
    }

    /**Deconstructor*/
    ~KV() {
        delete[] map_->keys_;
        delete[] map_->values_;
        delete[] map_;
    }

    /**Get the value of the given key*/
    virtual Object* get(Key* k) {
        return map_->get(k);
    }
   
    virtual Object* waitAndGet(Key* k) {
        return map_->get(k);
    }

    /**Insert the key-value pair*/
    virtual void put(Key* k, Object* v) {
        Value* v1 = dynamic_cast<Value*>(v);
        map_->put(k, v1);
    }
};
