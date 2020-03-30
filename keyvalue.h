#pragma once

#include "object.h"


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

