
#include "string.h"
#include "object.h"
#include "helper.h"
#include "array.h"
#pragma once
class IntColumn;
class BoolColumn;
class FloatColumn;
class StringColumn;
class DoubleColumn;

/**************************************************************************
 * Column ::
 * Represents one column of a data frame which holds values of a single type.
 * This abstract class defines methods overriden in subclasses. There is
 * one subclass per element type. Columns are mutable, equality is pointer
 * equality. */
class Column : public Object {
public:
    /** Type converters: Return same column under its actual type, or
     *  nullptr if of the wrong type.  */
    virtual IntColumn* as_int() {
        return nullptr;
    }
    virtual BoolColumn*  as_bool() {
        return nullptr;
    }
    virtual FloatColumn* as_float() {
        return nullptr;
    }
    virtual StringColumn* as_string() {
        return nullptr;
    }
    virtual DoubleColumn* as_double() {
        return nullptr;
    }

    /** Type appropriate push_back methods. Calling the wrong method is
      * undefined behavior. **/
    virtual void push_back(int val) {
        assert(0);
    }
    virtual void push_back(bool val) {
        assert(0);
    }
    virtual void push_back(float val) {
        assert(0);
    }
    virtual void push_back(String* val) {
        assert(0);
    }
    virtual void push_back(double val) {
        assert(0);
    }

    /** Returns the number of elements in the column. */
    virtual size_t size() {
        return 0;
    }

    /** Return the type of this column as a char: 'S', 'B', 'I' and 'F'.
     **/
    char get_type() {
        if (this->as_int() != nullptr) {
            return 'I';
        } else if (this->as_bool() != nullptr) {
            return 'B';
        } else if (this->as_string() != nullptr) {
            return 'S';
        } else if (this->as_float() != nullptr) {
            return 'F';
        } else if (this->as_double() != nullptr) {
            return 'D';
        }
    }
};


class IntColumn : public Column {
public:

    IntArray* arr;

    IntColumn() : Column() {
        this->arr = new IntArray();
    }
    IntColumn(int n, ...){

    }
    IntColumn(Deserializer &dser) {
        arr = new IntArray(dser);
    }
    int get(size_t idx) {
        return arr->get(idx);
    }
    IntColumn* as_int() {
        return this;
    }
    /** Set value at idx. An out of bound idx is undefined.  */
    void set(size_t idx, int val) {
        arr->set(val,idx);
    }
    size_t size() {
        return arr->size();
    }
    void push_back(int val) {
        arr->add(val);
    }
    void serialize(Serializer &ser) {
        arr->serialize(ser);
    }
};



class FloatColumn : public Column {
public:

    FloatArray* arr;

    FloatColumn() {
        this->arr = new FloatArray();
    }
    FloatColumn(float n, ...) {

    }
    int get(size_t idx) {
        return arr->get(idx);
    }
    FloatColumn(Deserializer &dser) {
        arr = new FloatArray(dser);
    }
    FloatColumn* as_float() {
        return this;
    }
    /** Set value at idx. An out of bound idx is undefined.  */
    void set(size_t idx, float val) {
        arr->set(val,idx);
    }
    size_t size() {
        return arr->size();
    }

    void push_back(float val) {
        arr->add(val);
    }
    void serialize(Serializer &ser) {
        arr->serialize(ser);
    }
};

class DoubleColumn : public Column {
public:

    DoubleArray* arr;

    DoubleColumn() {
        this->arr = new DoubleArray();
    }
    DoubleColumn(double n, ...) {

    }
    double get(size_t idx) {
        return arr->get(idx);
    }
    DoubleColumn* as_double() {
        return this;
    }
    DoubleColumn(Deserializer &dser) {
        arr = new DoubleArray(dser);
    }
    /** Set value at idx. An out of bound idx is undefined.  */
    void set(size_t idx, double val) {
        arr->set(val,idx);
    }
    size_t size() {
        return arr->size();
    }

    void push_back(double val) {
        arr->add(val);
    }
    void serialize(Serializer &ser) {
        arr->serialize(ser);
    }
};

class BoolColumn : public Column {
public:

    BoolArray* arr;

    BoolColumn() {
        this->arr = new BoolArray();
    }
    BoolColumn(bool n, ...) {

    }
    bool get(size_t idx) {
        return arr->get(idx);
    }
    BoolColumn* as_bool() {
        return this;
    }
    BoolColumn(Deserializer &dser) {
        arr = new BoolArray(dser);
    }
    /** Set value at idx. An out of bound idx is undefined.  */
    void set(size_t idx, bool val) {
        arr->set(val,idx);
    }
    size_t size() {
        return arr->size();
    }
    void push_back(bool val) {
        arr->add(val);
    }
    void serialize(Serializer &ser) {
        arr->serialize(ser);
    }
};

// Other primitive column classes similar...

/*************************************************************************
 * StringColumn::
 * Holds string pointers. The strings are external.  Nullptr is a valid
 * value.
 */
class StringColumn : public Column {
public:
    StrArray* arr;
    StringColumn() {
        arr = new StrArray();
    }
    StringColumn(int n, ...) {

    }
    StringColumn(Deserializer &dser) {
        arr = new StrArray(dser);
    }
    StringColumn* as_string() {
        return this;
    }
    /** Returns the string at idx; undefined on invalid idx.*/
    String* get(size_t idx) {
        return arr->get(idx);
    }
    /** Out of bound idx is undefined. */
    void set(size_t idx, String* val) {
        arr->set(val,idx);
    }
    size_t size() {
        return arr->size();
    }
    void push_back(String* val) {
        arr->add(val);
    }
    void serialize(Serializer &ser) {
        arr->serialize(ser);
    }
};