//
// Created by Rahul Kumar on 2/13/20.
//
#pragma once
#include "schema.h"
#include "column.h"
#include "math.h"

#ifndef SUBMISSION_ROWER_H
#define SUBMISSION_ROWER_H

#endif //SUBMISSION_ROWER_H

/*************************************************************************
 * Row::
 *
 * This class represents a single row of data constructed according to a
 * dataframe's schema. The purpose of this class is to make it easier to add
 * read/write complete rows. Internally a dataframe hold data in columns.
 * Rows have pointer equality.
 */
class Row : public Object {
 public:
	 Schema* s;
	 Column** data;
	 char* types;
	 size_t num;
	 size_t index;
	
  /** Build a row following a schema. */
  Row(Schema& scm) {
	  s = new Schema(scm.column_types);
	  types = scm.column_types;
	  num = scm.column_num;
	  data = new Column*[num];
	  for(size_t i = 0; i < num; i++) {
		  if (types[i] == 'S') {
			  data[i] = new StringColumn();
		  }
		  else if (types[i] == 'I') {
                          data[i] = new IntColumn();
                  }
		  else if (types[i] == 'B') {
                          data[i] = new BoolColumn();
                  }
		  else if (types[i] == 'F') {
                          data[i] = new FloatColumn();
                  }
	  }
  }
 
  /** Setters: set the given column with the given value. Setting a column with
    * a value of the wrong type is undefined. */
  void set(size_t col, int val) {
	  if(types[col] == 'I') {
		  data[col]->as_int()->set(0, val);
	  }
  }

  void set(size_t col, float val) {
	  if(types[col] == 'F') {
          data[col]->as_float()->set(0, val);
          }
  }

  void set(size_t col, bool val) {
	  if(types[col] == 'B') {
          data[col]->as_bool()->set(0, val);
          }
  }

  /** The string is external. */
  void set(size_t col, String* val) {
	  if(types[col] == 'S') {
          data[col]->as_string()->set(0, val);
      }
  }
 
  /** Set/get the index of this row (ie. its position in the dataframe. This is
   *  only used for informational purposes, unused otherwise */
  void set_idx(size_t idx) {
	  index = idx;
  }

  size_t get_idx() {
	  return index;
  }
 
  /** Getters: get the value at the given column. If the column is not
    * of the requested type, the result is undefined. */
  int get_int(size_t col) {
	  if(types[col] == 'I') {
		  return data[col]->as_int()->get(0);
          }
  }

  bool get_bool(size_t col) {
	  if(types[col] == 'B') {
                  return data[col]->as_bool()->get(0);
          }
  }

  float get_float(size_t col) {
	  if(types[col] == 'F') {
                  return data[col]->as_float()->get(0);
          }
  }

  String* get_string(size_t col) {
	  if(types[col] == 'S') {
                  return data[col]->as_string()->get(0);
          }
  }
 
  /** Number of fields in the row. */
  size_t width() {
	  return num;
  }
 
   /** Type of the field at the given position. An idx >= width is  undefined. */
  char col_type(size_t idx) {
	  return types[idx];
  }

};


