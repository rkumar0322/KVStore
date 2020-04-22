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
	 Schema s;
	 Column** data;
	 char* types;
	 size_t num;
	 size_t index;
	
  /** Build a row following a schema. */
  Row(Schema& scm) : s(scm.column_types->cstr_) {
	  types = scm.column_types->cstr_;
	  num = scm.column_num;
	  data = new Column*[num];
	  for(size_t i = 0; i < num; i++) {
		  if (types[i] == 'S') {
			  data[i] = new StringColumn();
		  }
		  else if (types[i] == 'I') {
                          data[i] = new IntColumn();
                          data[i]->as_bool()->push_back(1);
                  }
		  else if (types[i] == 'B') {
                          data[i] = new BoolColumn();
                          data[i]->as_bool()->push_back(true);
                  }
		  else if (types[i] == 'F') {
                          data[i] = new FloatColumn();
                          data[i]->as_float()->push_back(1.000);
                  }
          else if (types[i] == 'D') {
                          data[i] = new DoubleColumn();
                          data[i]->as_double()->push_back(1.000);
                  }
	  }
  }

  ~Row() {
      for (int i = 0; i < num;i++) {
          if(s.col_type(i) == 'D') {
              DoubleColumn* d = data[i]->as_double();
              data[i] = nullptr;
              delete d;
          }
      }
      delete[] data;
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
  
  void set(size_t col, double val) {
	  if(types[col] == 'D') {
          data[col]->as_double()->set(0, val);
	  }
  }

  /** The string is external. */
  void set(size_t col, String val) {
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
      else {
          exit(1);
      }
  }

  bool get_bool(size_t col) {
	  if(types[col] == 'B') {
                  return data[col]->as_bool()->get(0);
          }
      else {
          exit(1);
      }
  }

  float get_float(size_t col) {
	  if(types[col] == 'F') {
                  return data[col]->as_float()->get(0);
          }
      else {
          exit(1);
      }
  }
  
  double get_double(size_t col) {
	  if(types[col] == 'D') {
                  return data[col]->as_double()->get(0);
          }
      else {
          exit(1);
      }
  }

  String* get_string(size_t col) {
	  if(types[col] == 'S') {
                  return data[col]->as_string()->get(0);
          }
      else {
          exit(1);
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


