//
// Created by Rahul Kumar on 2/13/20.
//
#include "schema.h"
#include "column.h"
#include "row.h"
#include "serial.h"
#include <stdio.h>
#include <thread>
#include "kvstore.h"

/****************************************************************************
 * DataFrame::
 *
 * A DataFrame is table composed of columns of equal length. Each column
 * holds values of the same type (I, S, B, F). A dataframe has a schema that
 * describes it.
 */
class DataFrame : public Object {
public:
    Column** data; //array of pointer of columns
    Schema s;      //schema of the dataframe
    size_t num;    //current number of columns
    size_t cap;    //current capacity of the "data" field

    /** Create a data frame with the same columns as the given df but with no rows or rownmaes */
    DataFrame(DataFrame& df):
            Object(df) {
        s = df.s;
        num = df.num;
        cap = df.cap;
        data = new Column*[num];
        memcpy(data, df.data, num);
    }
    /** Create a data frame from a schema and columns. All columns are created
      * empty. */
    DataFrame(Schema& schema):
            Object(schema) {
        s.column_num = schema.column_num;
        s.row_num = schema.row_num;
        s.column_cap = schema.column_cap;
        s.row_cap = schema.row_cap;
        s.column_types = new char(s.column_num);
        memcpy(s.column_types, schema.column_types, s.column_num);
	for(size_t i = 0; i < s.column_num; i++) {
		if(s.col_type(i) == 'S') {
			data[i] = new StringColumn();
		}
		if(s.col_type(i) == 'B') {
                        data[i] = new BoolColumn();
                }
		if(s.col_type(i) == 'I') {
                        data[i] = new IntColumn();
                }
		if(s.col_type(i) == 'F') {
                        data[i] = new FloatColumn();
                }
        if(s.col_type(i) == 'D') {
            data[i] = new FloatColumn();
        }
	}
	    }
	      
    /** Returns the dataframe's schema. Modifying the schema after a dataframe
      * has been created in undefined. */
    Schema& get_schema() {
        return s;
    }

    /** Adds a column this dataframe, updates the schema, the new column
      * is external, and appears as the last column of the dataframe, the
      * name is optional and external. A nullptr colum is undefined. */
    void add_column(Column* col) {
        if (col != nullptr) {
            data[s.column_num] = col;
            s.add_column(col->get_type());
        }
    }
    
    /** Return the value at the given column and row. Accessing rows or
     *  columns out of bounds, or request the wrong type is undefined.*/
    int get_int(size_t col, size_t row) {
        if(s.column_types[col] == 'I') {
            IntColumn* i = data[col]->as_int();
            return i->get(row);
        }
    }

    bool get_bool(size_t col, size_t row) {
        if(s.column_types[col] == 'B') {
            BoolColumn* i = data[col]->as_bool();
            return i->get(row);
        }
    }

    float get_float(size_t col, size_t row) {
        if(s.column_types[col] == 'F') {
            FloatColumn* i = data[col]->as_float();
            return i->get(row);
        }
    }

    String* get_string(size_t col, size_t row) {
        if(s.column_types[col] == 'S') {
            StringColumn* i = data[col]->as_string();
            return i->get(row);
        }
    }

    double get_double(size_t col, size_t row) {
        if(s.column_types[col] == 'D') {
            DoubleColumn* i = data[col]->as_double();
            return i->get(row);
        }
    }

    /** Set the value at the given column and row to the given value.
      * If the column is not  of the right type or the indices are out of
      * bound, the result is undefined. */
    void set(size_t col, size_t row, int val) {
        if(col < s.column_num && data[col]->get_type() == 'I') {
            if(row < s.row_num) {
                data[col]->as_int()->set(row, val);
            }
        }
    }

    void set(size_t col, size_t row, bool val) {
        if(col < s.column_num && data[col]->get_type() == 'B') {
            if(row < s.row_num) {
                data[col]->as_bool()->set(row, val);
            }
        }
    }
    
    void set(size_t col, size_t row, float val) {
        if(col < s.column_num && data[col]->get_type() == 'F') {
            if(row < s.row_num) {
                data[col]->as_float()->set(row, val);
            }
        }
    }

    void set(size_t col, size_t row, String* val) {
        if(col < s.column_num && data[col]->get_type() == 'S') {
            if(row < s.row_num) {
                data[col]->as_string()->set(row, val);
            }
        }
    }

    void set(size_t col, size_t row, double val) {
        if(col < s.column_num && data[col]->get_type() == 'D') {
            if(row < s.row_num) {
                data[col]->as_double()->set(row, val);
            }
        }
    }

    /** Add a row at the end of this dataframe. The row is expected to have
     *  the right schema and be filled with values, otherwise undedined.  */
    void add_row(Row& row) {
        for (size_t i = 0; i < row.num;i++) {
            if (data[i]->get_type() == 'S') {
                data[i]->push_back(row.get_string(i));
            } else if (data[i]->get_type() == 'I') {
                data[i]->push_back(row.get_int(i));
            } else if (data[i]->get_type() == 'B') {
                data[i]->push_back(row.get_bool(i));
            } else if (data[i]->get_type() == 'F') {
                data[i]->push_back(row.get_float(i));
            } else if (data[i]->get_type() == 'D') {
                data[i]->push_back(row.get_float(i));
            }
	}
	s.row_num++;
    }
    
    /** The number of rows in the dataframe. */
    size_t nrows() {
        return s.length();
    }

    /** The number of columns in the dataframe.*/
    size_t ncols() {
        return s.width();
    }

    

    /** Print the dataframe in SoR format to standard output. */
    void print() {
  
        for (size_t i = 0; i < s.row_num; i++) {
      
            for (size_t j = 0; j < s.column_num; j++) {
                Column* c = data[j];
                if (c->get_type() == 'B') {
                    std::cout << c->as_bool()->get(i) << " ";
                }
                else if (c->get_type() == 'I') {
                    std::cout << c->as_int()->get(i) << " ";
                }
                else if (c->get_type() == 'S') {
                    std::cout <<  c->as_string()->get(i) << " ";

                }
                else if (c->get_type() == 'F') {
                    std::cout << c->as_float()->get(i) << " ";

                }
                else if (c->get_type() == 'D') {
                    std::cout << c->as_float()->get(i) << " ";

                }
            }
            std::cout << "\n";
        }
    }

    void serialize(Serializer &ser) {
            ser.write_size_t(num);
            ser.write_size_t(cap);
            s.serialize(ser);
            for (int i = 0; i < s.column_num;i++) {
                data[i]->serialize(ser);
            }
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
    static DataFrame* fromScalar(Key* k, KV kv, double val) {
        Schema s ("D");
        DataFrame* ret = new DataFrame(s);
        Row r(s);
        r.set(0, val);
        ret->add_row(r);
        Serializer s1;
        ret->serialize(s1);
        char* buf = s1.data_;
        Value* v = new Value(buf);
        kv.put(k, v);
        return ret;
    }
};