
#pragma once
class IntColumn;
class BoolColumn;
class FloatColumn;
class DoubleColumn;
class StringColumn;


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
    virtual DoubleColumn* as_double() {
        return nullptr;
    }
    virtual StringColumn* as_string() {
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
    virtual void push_back(double val) {
        assert(0);
    }
    virtual void push_back(String* val) {
        printf("GETS TO THIS POINT\n");
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
        } else {
            exit(1);
        }
    }
};

class IntChunk : public Object {
public:

    IntArray* arr;

    IntChunk() {
        this->arr = new IntArray();
    }
    IntChunk(int n, ...){

    }
    IntChunk(Deserializer &dser) {
        arr = new IntArray(dser);
    }
    ~IntChunk() {

    }
    int get(size_t idx) {
        return arr->get(idx);
    }
    IntChunk* as_int() {
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

/*************************************************************************
 * INTColumn::
 * Holds string pointers. The strings are external.  Nullptr is a valid
 * value.
 */
class IntColumn : public Column {
public:
    IntChunk** arr;
    size_t chunks_;
    size_t cap_;
    size_t len;
    size_t internal_chunk;

    IntColumn() {
        arr = new IntChunk*[10];
        for (int i = 0; i < 10;i++) {
            arr[i] = new IntChunk();
        }
        len = 0;
        chunks_ = 0;
        internal_chunk = 10;
        cap_ = 10 * arg.rows_per_chunk;
    }
    IntColumn(int n, ...) {

    }
    IntColumn(Deserializer &dser) {
        len = dser.read_size_t();
        chunks_ = dser.read_size_t();
        cap_ = dser.read_size_t();
        internal_chunk = dser.read_size_t();
        arr = new IntChunk*[internal_chunk];
        for (int i = 0; i < chunks_;i++) {
            printf("chunks %d\n",chunks_);
            printf("START OF DEBUGGING COLUMNS\n");
            IntChunk* a = new IntChunk(dser);
            arr[i] = a;
            printf("FINISH OF DEBUGGING COLUMNS\n");
        }
    }

    void serialize(Serializer &ser) {
        ser.write_size_t(len);
        ser.write_size_t(chunks_);
        ser.write_size_t(cap_);
        ser.write_size_t(internal_chunk);
        for (int i = 0; i < chunks_;i++) {
            IntChunk* s1 = arr[i];
            s1->serialize(ser);
        }
    }
    IntColumn* as_int() {
        return this;
    }
    /** Returns the string at idx; undefined on invalid idx.*/
    int get(size_t idx) {
        return arr[idx / arg.rows_per_chunk]->get(idx % arg.rows_per_chunk);
    }
    /** Out of bound idx is undefined. */
    void set(size_t idx, int val) {
        arr[idx / arg.rows_per_chunk]->set(idx % arg.rows_per_chunk,val);
    }
    size_t size() {
        return len;
    }
    void push_back(int val) {
        if (len >= cap_) {
            IntChunk ** newarr = new IntChunk*[internal_chunk * 2];
            int i = 0;
            for (i; i < internal_chunk;i++) {
                newarr[i] = arr[i];
            }
            for (i; i < internal_chunk * 2;i++) {
                newarr[i] = new IntChunk();
            }
            for (int i = 0; i < internal_chunk;i++) {
                delete arr[i];
            }
            delete[] arr;
            internal_chunk *= 2;
            arr = newarr;
            cap_ = cap_ * 2;
        }
        arr[len / arg.rows_per_chunk]->push_back(val);
        len += 1;
        if (len > 0) {
            chunks_ = len / arg.rows_per_chunk + 1;
        } else {
            chunks_ = 0;
        }
    }

    void add_chunk(IntChunk* s) {
        if (chunks_ >= internal_chunk) {
            IntChunk ** newarr = new IntChunk*[internal_chunk * 2];
            int i = 0;
            for (i; i < internal_chunk;i++) {
                newarr[i] = arr[i];
            }
            for (i; i < internal_chunk * 2;i++) {
                newarr[i] = new IntChunk();
            }
            for (int i = 0; i < internal_chunk;i++) {
                delete arr[i];
            }
            delete[] arr;
            internal_chunk *= 2;
            arr = newarr;
            arr[chunks_] = s;
            chunks_ += 1;
            cap_ = cap_ * 2;
        } else {
            arr[chunks_] = s;
            chunks_ += 1;
        }
    }

    IntChunk* get_chunk(size_t chunk) {
        return arr[chunk];
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
    float get(size_t idx) {
        return arr->get(idx);
    }
    FloatColumn(Deserializer &dser) {
        arr = new FloatArray(dser);
    }
    FloatColumn* as_float() {
        return this;
    }
    ~FloatColumn(){

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
        arr = new DoubleArray();
    }
    DoubleColumn(double n, ...) {

    }
    ~DoubleColumn() {
        delete arr;
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
class StringChunk : public Object {
public:
    StrArray* arr;
    StringChunk() {
        arr = new StrArray();
    }
    StringChunk(int n, ...) {

    }

    //Serializes this Chunk.
    StringChunk(Deserializer &dser) {
        //printf("SOMEHOW GETS HERE\n");
        arr = new StrArray(dser);
        //printf("SOMEHOW FINISHES HERE\n");
    }
    StringChunk* as_string() {
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
    //size of this index
    size_t size() {
        return arr->size();
    }
    //pushes a String at the end of this.
    void push_back(String* val) {
        arr->add(val);
    }

    //Serializes this column.
    void serialize(Serializer &ser) {
        arr->serialize(ser);
    }
};

/*************************************************************************
 * StringColumn::
 * Holds string pointers. The strings are external.  Nullptr is a valid
 * value.
 */
class StringColumn : public Column {
public:
    StringChunk** arr;
    size_t chunks_;
    size_t cap_;
    size_t len;
    size_t internal_chunk;

    StringColumn() {
        arr = new StringChunk*[10];
        for (int i = 0; i < 10;i++) {
            arr[i] = new StringChunk();
        }
        len = 0;
        chunks_ = 0;
        internal_chunk = 10;
        cap_ = 10 * arg.rows_per_chunk;
    }
    StringColumn(int n, ...) {

    }
    //
    StringColumn(Deserializer &dser) {
        len = dser.read_size_t();
        chunks_ = dser.read_size_t();
        cap_ = dser.read_size_t();
        internal_chunk = dser.read_size_t();
        arr = new StringChunk*[internal_chunk];
        for (int i = 0; i < chunks_;i++) {
            //printf("chunks %d\n",chunks_);
            //printf("START OF DEBUGGING COLUMNS\n");
            StringChunk* a = new StringChunk(dser);
            arr[i] = a;
            //printf("FINISH OF DEBUGGING COLUMNS\n");
        }
    }

    void serialize(Serializer &ser) {
        ser.write_size_t(len);
        ser.write_size_t(chunks_);
        ser.write_size_t(cap_);
        ser.write_size_t(internal_chunk);
        for (int i = 0; i < chunks_;i++) {
            StringChunk* s1 = arr[i];
            s1->serialize(ser);
        }
    }
    StringColumn* as_string() {
        return this;
    }
    /** Returns the string at idx; undefined on invalid idx.*/
    String* get(size_t idx) {
        return arr[idx / arg.rows_per_chunk]->get(idx % arg.rows_per_chunk);
    }
    /** Out of bound idx is undefined. This is the chunk implementation of this node.  */
    void set(size_t idx, String* val) {
        arr[idx / arg.rows_per_chunk]->set(idx % arg.rows_per_chunk,val);
    }
    size_t size() {
        return len;
    }

    //adds a value to the column at the end.
    void push_back(String* val) {
        if (len >= cap_) {
            StringChunk ** newarr = new StringChunk*[internal_chunk * 2];
            int i = 0;
            for (i; i < internal_chunk;i++) {
                newarr[i] = arr[i];
            }
            for (i; i < internal_chunk * 2;i++) {
                newarr[i] = new StringChunk();
            }
            for (int i = 0; i < internal_chunk;i++) {
                delete arr[i];
            }
            delete[] arr;
            internal_chunk *= 2;
            arr = newarr;
            cap_ = cap_ * 2;
        }
        arr[len / arg.rows_per_chunk]->push_back(val);
        len += 1;
        if (len > 0) {
            chunks_ = len / arg.rows_per_chunk + 1;
        } else {
            chunks_ = 0;
        }
    }

    //Adds a chunk of data to the end of the dataframe.
    void add_chunk(StringChunk* s) {
        if (chunks_ >= internal_chunk) {
            StringChunk ** newarr = new StringChunk*[internal_chunk * 2];
            int i = 0;
            for (i; i < internal_chunk;i++) {
                newarr[i] = arr[i];
            }
            for (i; i < internal_chunk * 2;i++) {
                newarr[i] = new StringChunk();
            }
            for (int i = 0; i < internal_chunk;i++) {
                delete arr[i];
            }
            delete[] arr;
            internal_chunk *= 2;
            arr = newarr;
            arr[chunks_] = s;
            chunks_ += 1;
            cap_ = cap_ * 2;
        } else {
            arr[chunks_] = s;
            chunks_ += 1;
        }
    }

    //Retrieves a given chunk of data based on the types of chunks in the dataframe. 
    StringChunk* get_chunk(size_t chunk) {
        return arr[chunk];
    }
};