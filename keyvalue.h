#pragma once

#include "object.h"
#include "dataframe.h"
#include "map.h"

class Key;
class KV;
class Value;
class Map;


class Key: public Object {
	public:
		String* key_;
		size_t nodeidx;

		Key(String* key, size_t s) {
			this->key_ = key;
			this->nodeidx = s;
		}

		~Key() {
			delete key_;
		}

		bool equals(Object* o) {
			Key* k = dynamic_cast<Key*>(o);
			if(k == nullptr) {
				return false;
			}
			return key_->equals(k->key_) && 
				(nodeidx == k->nodeidx);
		}
};


class Value: public Object {
	public:
		char* val_;

		Value() {
			val_ = nullptr;
		}

		Value(const char* buf) {
			val_ = new char[strlen(buf)];
			memcpy(val_, buf, strlen(buf));
		}

		~Value() {
			delete[] val_;
		}

		bool equals(Object* o) {
			if(o == nullptr) {
				return false;
			}
			Value* v = dynamic_cast<Value*>(o);
			if(v->val_ == nullptr) {
				return false;
			}
			else {
				return strcmp(v->val_, this->val_) == 0;
			}
		}
};

//A map stores a list of keys and a list of values.
//Key and value at the same index in the list are paired.
class Map: public Object {
public:
    Key** keys_;
    Value** values_;
    size_t len;
    size_t cap;

    //construct two lists with cap 10.
    Map() {
        keys_ = new Key*[10];
        values_ = new Value*[10];
        len = 0;
        cap = 10;
    }

    //deconstruct
    ~Map() {
        delete keys_;
        delete values_;
        len = 0;
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
    bool put(Key* k, Value* v) {
        //size_t index;
        for(size_t i = 0; i < len; i++) {
            if(keys_[i]->equals(k)) {
                values_[i] = v;
                return true;
            }
        }
        if(len == cap) {
            Key** newkeys_ = new Key*[cap*2];
            Value** newvalues_ = new Value*[cap*2];
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
    Value* get(Key* k) {
        for(size_t i = 0; i < len; i++) {
            if(keys_[i]->equals(k)) {
                return values_[i];
            }
        }
        return nullptr;
    }

};

class KV: public Object {
public:
    Map* map_;
    size_t len;

    KV() {
        map_ = new Map();
        len = 0;
    }

    ~KV() {
        delete[] map_->keys_;
        delete[] map_->values_;
        delete[] map_;
    }

    Value* get(Key* k) {
        return map_->get(k);
    }

    Value* waitAndGet(Key* k) {
        return map_->get(k);
    }

    void put(Key* k, Value* v) {
        map_->put(k, v);
    }

    static DataFrame* fromArray(Key& k, KV& kv, size_t num, double* vals) {
        Schema s("D");
        DataFrame* ret = new DataFrame(s);
        Row r(s);
        DoubleArray* da = new DoubleArray();
        for(size_t i = 0; i < num; i++) {
            da->add(vals[i]);
            r.set(0, vals[i]);
            ret->add_row(r);
        }
        Serializer s1;
        ret->serialize(s1);
        char* buf = s1.data_;
        Value* v = new Value(buf);
        kv.put(&k, v);
        return ret;
    }
};

