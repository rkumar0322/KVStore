#pragma once

#include "object.h"
#include "map.h"


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
};
