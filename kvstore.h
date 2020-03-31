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
    Value* get(Key* k) {
        return map_->get(k);
    }
   
    Value* waitAndGet(Key* k) {
        return map_->get(k);
    }

    /**Insert the key-value pair*/
    void put(Key* k, Value* v) {
        map_->put(k, v);
    }
};
