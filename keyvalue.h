#pragma once

#include "object.h"
#include "string.h"

/**A key is part of kvstore which contains a string and a node index*/
class Key: public Object {
	public:
		String* key_;
		size_t nodeidx;

		/**Constructor*/
		Key(String* key, size_t s) {
			this->key_ = key;
			this->nodeidx = s;
		}
		Key(Deserializer &dser) {
		    key_ = new String(dser);
		    nodeidx = dser.read_size_t();
		}

		void serialize(Serializer &ser) {
		    key_->serialize(ser);
		    ser.write_size_t(nodeidx);
		}

		/**Deconstructor*/
		~Key() {
			delete key_;
		}

		/**overwrites equals*/
		bool equals(Object* o) {
			Key* k = dynamic_cast<Key*>(o);
			if(k == nullptr) {
				return false;
			}
			return key_->equals(k->key_) && 
				(nodeidx == k->nodeidx);
		}
};



/**A Value is part of kvstore which stores the char* representation of dataframe after serialized.*/
class Value: public Object {
	public:
		char* val_;
		size_t size_;

		/**Constructor*/
		Value() {
			val_ = nullptr;
		}

		Value(Deserializer &dser) {
            size_ = dser.read_size_t();
		    val_= dser.read_chars(size_);
		}

		
		Value(const char* buf) {
			val_ = new char[strlen(buf)];
			memcpy(val_, buf, strlen(buf));
		}

		Value(Serializer &ser) {
            size_ = ser.length_;
		    val_ = ser.data_;
		}

		/**Deconstructor*/
		~Value() {
			delete[] val_;
		}

		/**Overwrites equals*/
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

class KVPair : public Object {
public:
    Key* k;
    Value* v;

    KVPair(Key* k, Value* v) {
        k = k;
        v = v;
    }

    KVPair(Deserializer &d) {
        k = new Key(d);
        v = new Value(d);
    }

    void serialize(Serializer &s) {
        k->serialize(s);
        v->serialize(s);
    }
};
