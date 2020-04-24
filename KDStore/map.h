#pragma once

//A map stores a list of keys and a list of values.
//Key and value at the same index in the list are paired.
class Map: public Object {
public:
    Object** keys_;
    Object** values_;
    size_t len;
    size_t cap;

    //construct two lists with cap 10.
    Map() {
        keys_ = new Object*[10];
        values_ = new Object*[10];
        len = 0;
        cap = 10;
    }

    //deconstruct
    virtual ~Map() {
    }

    //overwrites equal method
    bool equals(Object* o) {
        if(o == nullptr) {
            return false;
        }
        Map* m = dynamic_cast<Map*>(o);
        if(m->len == this->len) {
            for(size_t i = 0; i < len; i++) {
                if(!m->keys_[i]->equals(this->keys_[i])
                   || !m->values_[i]->equals(this->
                        values_[i])) {
                    return false;
                }
            }
            return true;
        }
        else {
            return false;
        }
    }

    //put key value pair into the map
    //update size or list if neccessary
    //return true if key is founded, false if key is new
    bool put_(Object* k, Object* v) {
        //size_t index;
        for(size_t i = 0; i < len; i++) {
            if(keys_[i]->equals(k)) {
                values_[i] = v;
                return true;
            }
        }
        if(len == cap) {
            Object** newkeys_ = new Object*[cap*2];
            Object** newvalues_ = new Object*[cap*2];
            for(size_t i = 0; i < len; i++) {
                newkeys_[i] = keys_[i];
                newvalues_[i] = values_[i];
            }
            newkeys_[len] = k;
            newvalues_[len] = v;
            delete[] keys_;
            delete[] values_;
            keys_ = newkeys_;
            values_ = newvalues_;
            len += 1;
            cap *= 2;
            return false;
        }
        else {
            keys_[len] = k;
            values_[len] = v;
            len += 1;
            return false;
        }
    }

    //get the value of the given key
    //return nullptr if doesn't find key
    Object* get_(Object* k) {
        for(size_t i = 0; i < len; i++) {
            if(k->equals(keys_[i])) {
                return values_[i];
            }
        } 
        return nullptr;
    }

};