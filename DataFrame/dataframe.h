//
// Created by Rahul Kumar on 2/13/20.
//



class KDStore;
#include "schema.h"
#include "column.h"
#include "row.h"
#include "../helpers/serial.h"
#include <stdio.h>
#include <thread>
#include "../KDStore/keyvalue.h"

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
    Schema* s;      //schema of the dataframe
    size_t num;    //current number of columns

    /** Create a data frame with the same columns as the given df but with no rows or rownmaes */
    DataFrame(DataFrame& df):
            Object(df) {
        s = df.s;
        num = df.num;
        data = new Column*[num];
        memcpy(data, df.data, num);
    }
    /** Create a data frame from a schema and columns-> All columns are created
      * empty. */
    DataFrame(Schema& schema) {
        s = new Schema();
        s->column_num = schema.column_num;
        s->row_num = schema.row_num;
        s->column_cap = schema.column_cap;
        s->row_cap = schema.row_cap;
        s->column_types = new String(schema.column_types->cstr_);
        data = new Column*[s->column_num];
        num = s->column_num;
	    for(size_t i = 0; i < s->column_num; i++) {
		    if(s->col_type(i) == 'S') {
			    data[i] = new StringColumn();
		    }
		    if(s->col_type(i) == 'B') {
		        data[i] = new BoolColumn();
		    }
		    if(s->col_type(i) == 'I') {
		        data[i] = new IntColumn();
		    }
		    if(s->col_type(i) == 'F') {
		        data[i] = new FloatColumn();
		    }
            if(s->col_type(i) == 'D') {
                data[i] = new DoubleColumn();
            }
	    }
    }

    /**Deconstructor*/
    ~DataFrame() {
        delete s;
        for (int i = 0; i < num;i++) {
            if(s->col_type(i) == 'D') {
                DoubleColumn* d = data[i]->as_double();
                delete d;
            }
        }
        delete[] data;
    }

    DataFrame(Deserializer &d) {
        num = d.read_size_t();
        s = new Schema(d);
        data = new Column*[s->column_num];
        for(size_t i = 0; i < s->column_num; i++) {
            if(s->col_type(i) == 'S') {
                data[i] = new StringColumn(d);
            }
            if(s->col_type(i) == 'B') {
                data[i] = new BoolColumn(d);
            }
            if(s->col_type(i) == 'I') {
                data[i] = new IntColumn(d);
            }
            if(s->col_type(i) == 'F') {
                data[i] = new FloatColumn(d);
            }
            if(s->col_type(i) == 'D') {
                DoubleColumn* dc = new DoubleColumn(d);
                data[i] = dc;
            }
        }
    }



    /** Serialize a dataframe into a serialzier*/
    void serialize(Serializer &ser) {
        ser.write_size_t(num);
        s->serialize(ser);
        for(size_t i = 0; i < s->column_num; i++) {
            if(s->col_type(i) == 'S') {
                StringColumn* d = (StringColumn*)data[i];
                d->serialize(ser);
            }
            if(s->col_type(i) == 'B') {
                BoolColumn* d = (BoolColumn*)data[i];
                d->serialize(ser);
            }
            if(s->col_type(i) == 'I') {
                IntColumn* d = (IntColumn*)data[i];
                d->serialize(ser);
            }
            if(s->col_type(i) == 'F') {
                FloatColumn* d = (FloatColumn*)data[i];
                d->serialize(ser);
            }
            if(s->col_type(i) == 'D') {
                data[i]->as_double()->serialize(ser);
            }
        }
    }

    /** Returns the dataframe's schema. Modifying the schema after a dataframe
      * has been created in undefined. */
    Schema& get_schema() {
        return *s;
    }

    /** Adds a column this dataframe, updates the schema, the new column
      * is external, and appears as the last column of the dataframe, the
      * name is optional and external. A nullptr colum is undefined. */
    void add_column(Column* col) {
        if (col != nullptr) {
            data[s->column_num] = col;
            s->add_column(col->get_type());
            num++;
        } else {
            exit(1);
        }
    }

    /** Return the value at the given column and row. Accessing rows or
     *  columns out of bounds, or request the wrong type is undefined.*/
    int get_int(size_t col, size_t row) {
        if(s->column_types->cstr_[col] == 'I') {
            IntColumn* i = data[col]->as_int();
            return i->get(row);
        } else {
            exit(1);
        }
    }

    bool get_bool(size_t col, size_t row) {
        if(s->column_types->cstr_[col] == 'B') {
            BoolColumn* i = data[col]->as_bool();
            return i->get(row);
        } else {
            exit(1);
        }
    }

    float get_float(size_t col, size_t row) {
        if(s->column_types->cstr_[col] == 'F') {
            FloatColumn* i = data[col]->as_float();
            return i->get(row);
        } else {
            exit(1);
        }
    }

    String* get_string(size_t col, size_t row) {
        if(s->column_types->cstr_[col] == 'S') {
            StringColumn* i = data[col]->as_string();
            return i->get(row);
        } else {
            exit(1);
        }
    }

    double get_double(size_t col, size_t row) {
        if(col < s->column_num && data[col]->get_type() == 'D') {
            DoubleColumn* i = (DoubleColumn*)data[col];
            return i->get(row);
        } else {
            exit(1);
        }
    }

    /** Set the value at the given column and row to the given value.
      * If the column is not  of the right type or the indices are out of
      * bound, the result is undefined. */
    void set(size_t col, size_t row, int val) {
        if(col < s->column_num && data[col]->get_type() == 'I') {
            if(row < s->row_num) {
                data[col]->as_int()->set(row, val);
            }
        } else {
            exit(1);
        }
    }

    void set(size_t col, size_t row, bool val) {
        if(col < s->column_num && data[col]->get_type() == 'B') {
            if(row < s->row_num) {
                data[col]->as_bool()->set(row, val);
            }
        } else {
            exit(1);
        }
    }

    void set(size_t col, size_t row, float val) {
        if(col < s->column_num && data[col]->get_type() == 'F') {
            if(row < s->row_num) {
                data[col]->as_float()->set(row, val);
            }
        } else {
            exit(1);
        }
    }

    void set(size_t col, size_t row, String val) {
        if(col < s->column_num && data[col]->get_type() == 'S') {
            if(row < s->row_num) {
                data[col]->as_string()->set(row, val);
            }
        } else {
            exit(1);
        }
    }

    void set(size_t col, size_t row, double val) {
        if(col < s->column_num && data[col]->get_type() == 'D') {
            if(row < s->row_num) {
                data[col]->as_double()->set(row, val);
            }
        } else {
            exit(1);
        }
    }

    /** Add a row at the end of this dataframe. The row is expected to have
     *  the right schema and be filled with values, otherwise undedined.  */
    void add_row(Row& row) {
        for (size_t i = 0; i < row.num;i++) {
            if (s->column_types->cstr_[i] == 'S') {
                //data[i]->as_string()->push_back(row.get_string(i));
            } else if (s->column_types->cstr_[i] == 'I') {
                data[i]->as_int()->push_back(row.get_int(i));
            } else if (s->column_types->cstr_[i] == 'B') {
                data[i]->as_bool()->push_back(row.get_bool(i));
            } else if (s->column_types->cstr_[i] == 'F') {
                data[i]->as_float()->push_back(row.get_float(i));
            } else if (s->column_types->cstr_[i] == 'D') {
                data[i]->as_double()->push_back(row.get_double(i));
            } else {
                exit(1);
            }
	}
	    s->row_num++;
    }

    /** The number of rows in the dataframe. */
    size_t nrows() {
        return s->length();
    }

    /** The number of columns in the dataframe.*/
    size_t ncols() {
        return s->width();
    }
    static DataFrame* fromArray(Key* k, KDStore* kv, size_t num, double* vals);
    static DataFrame* fromScalar(Key* k, KDStore* kv, double val);
};