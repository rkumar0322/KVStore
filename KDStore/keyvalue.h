#pragma once

#include "../helpers/object.h"
#include "../helpers/string.h"

/**A key is part of kvstore which contains a string and a node index*/
class Key: public Object {
	public:
		String* key_;
		size_t nodeidx;

		Key() {}

		/**Constructor*/
		Key(String* key, size_t s) {
			this->key_ = &key;
			this->nodeidx = s;
		}

		Key(const char* key, size_t s) {
		    key_ = new String(key);
		    nodeidx = s;
		}
		Key(Deserializer &dser) {
            nodeidx = dser.read_size_t();
		    key_ = new String(dser);
		}

		void serialize(Serializer &ser) {
		    ser.write_size_t(nodeidx);
            key_->serialize(ser);
		}

		/**Deconstructor*/
		~Key() {
		}

		/**overwrites equals*/
		bool equals(Object* o) {
			Key* k = (Key*)o;
			if(k == nullptr) {
				return false;
			}
			String* s = k->key_;
			return key_->equals(s) &&
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
			val_ = new char[10];
		}

		Value(Deserializer &dser) {
            size_ = dser.read_size_t();
		    val_= dser.read_chars(size_);
		}

		
		Value(const char* buf) {
			val_ = new char[strlen(buf)];
			size_ = strlen(buf);
			memcpy(val_, buf, strlen(buf));
		}

		 Value(Serializer ser) {
            size_ = ser.length_;
		    val_ = ser.data_;
		}

		Value(char* buf, size_t size) {
            size_ = size;
            val_ = buf;
		}

		void serialize(Serializer &ser) {
		    ser.write_size_t(size_);
		    ser.write_chars(val_,size_);
		}

		/**Deconstructor*/
		~Value() {
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
    Key k;
    Value v;

    KVPair(Key k_, Value v_) {
        k = k_;
        v = v_;
    }

    KVPair(Deserializer &d) {
        Key* k1 = new Key(d);
        Value* v1 = new Value(d);
        v = *v1;
        k = *k1;
    }

    void serialize(Serializer &s) {
        k.serialize(s);
        v.serialize(s);
    }

};
