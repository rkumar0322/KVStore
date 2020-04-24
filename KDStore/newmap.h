#pragma once

/**  Item_ are entries in a Map, they are not exposed, are immutable, own
 *   they key, but the value is external.  author: jv */
class Items_ {
public:
    Array keys_;
    Array vals_;

    Items_() : keys_(8), vals_(8) {}

    Items_(Object *k, Object * v) : keys_(8), vals_(8) {
        keys_.add(k);
        vals_.add(v);
    }

    ~Items_() {
    }

    bool contains_(Object& k) {
        for (int i = 0; i < keys_.size(); i++)
            if (k.equals(keys_.get(i)))
                return true;
        return false;
    }

    Object* get_(Object& k) {
        for (int i = 0; i < keys_.size(); i++)
            if (k.equals(keys_.get(i)))
                return vals_.get(i);
        return nullptr;
    }

    size_t set_(Object& k, Object* v) {
        for (int i = 0; i < keys_.size(); i++)
            if (k.equals(keys_.get(i))) {
                vals_.add(v,i);
                return 0;
            }
        // The keys are owned, but the key is received as a reference, i.e. not owned so we must make a copy of it.
        keys_.add(k.clone());
        vals_.add(v);
        return 1;
    }

    size_t erase_(Object& k) {
        for (int i = 0; i < keys_.size(); i++)
            if (k.equals(keys_.get(i))) {
                keys_.remove(i);
                vals_.remove(i);
                return 1;
            }
        return 0;
    }
};

/** A generic map class from Object to Object. Subclasses are responsibly of
 * making the types more specific.  author: jv */
class NewMap : public Object {
public:
    size_t capacity_;
    // TODO this was not size of the map, but number of occupied item positions in the top level
    size_t size_ = 0;
    Items_* items_;  // owned

    NewMap() : NewMap(10) {}
    NewMap(size_t cap) {
        capacity_ = cap;
        items_ = new Items_[capacity_];
    }

    ~NewMap() { delete[] items_; }

    /** True if the key is in the map. */
    bool contains(Object& key)  { return items_[off_(key)].contains_(key); }

    /** Return the number of elements in the map. */
    size_t size()  {
        return size_;
    }

    size_t off_(Object& k) { return  k.hash() % capacity_; }

    /** Get the value.  nullptr is allowed as a value.  */
    Object* get_(Object &key) { return items_[off_(key)].get_(key); }

    /** Add item->val_ at item->key_ either by updating an existing Item_ or
     * creating a new one if not found.  */
    void set(Object &k, Object *v) {
        if (size_ >= capacity_)
            grow();
        //printf("GETS TO THIS POINT size: %d\n",size());
        size_ += items_[off_(k)].set_(k,v);
        /*
        printf("ITEM SIZES\n");
        printf("KEY SIZE IS: %d\n", items_[off_(k)].keys_.len);
        printf("VALUE SIZE IS: %d\n", items_[off_(k)].vals_.len);
         */
        assert(items_[off_(k)].keys_.len == items_[off_(k)].vals_.len);
    }

    /** Removes element with given key from the map.  Does nothing if the
        key is not present.  */
    void erase(Object& k) {
        size_ -= items_[off_(k)].erase_(k);
    }

    /** Resize the map, keeping all Item_s. */
    void grow() {
        //LOG("Growing map from capacity " << capacity_);
        NewMap newm(capacity_ * 2);
        for (size_t i = 0; i < capacity_; i++) {
            size_t sz = items_[i].keys_.size();
            for (size_t j = 0; j < sz; j++) {
                Object* k = items_[i].keys_.get(j);
                Object* v = items_[i].vals_.get(j);
                newm.set(*k,v);
                // otherwise the values would get deleted (if the array's destructor was doing its job I found later:)
                items_[i].vals_.add(nullptr,j);
            }
        }
        delete[] items_;
        items_ = newm.items_;
        capacity_ = newm.capacity_;
        //printf("DOES NOT MAKE IT PAST THIS LINE\n");
        assert(size_ == newm.size_);
        //printf("MAKES IT PAST THIS LINE:\n");
        newm.items_ = nullptr;
    }
}; // Map

class MutableString : public String {
public:
    MutableString() : String("", 0) {}
    void become(const char* v) {
        size_ = strlen(v);
        cstr_ = (char*) v;
        hash_ = hash_me();
    }
};


/***************************************************************************
 * 
 **********************************************************author:jvitek */
class Num : public Object {
public:
    size_t v = 0;
    Num() {}
    Num(size_t v) : v(v) {}
};

class SIMap : public NewMap {
public:
    SIMap () {}
    Num* get(String& key) { return dynamic_cast<Num*>(get_(key)); }
    void set(String& k, Num* v) {
        assert(v);
        NewMap::set(k, v);
    }
}; // KVMap