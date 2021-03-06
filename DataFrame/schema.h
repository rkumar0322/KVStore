//
// Created by Rahul Kumar on 2/13/20.
//
#include "../helpers/object.h"
#include "../helpers/helper.h"
#include "../helpers/string.h"
#include "../helpers/serial.h"
#include <iostream>
#pragma once

/*************************************************************************
 * Schema::
 * A schema is a description of the contents of a data frame, the schema
 * knows the number of columns and number of rows, the type of each column,
 * optionally columns and rows can be named by strings.
 * The valid types are represented by the chars 'S', 'B', 'I' and 'F'.
 */
class Schema : public Object {
 public:
	 String* column_types;
	 size_t column_num;
	 size_t row_num;
	 size_t column_cap;
	 size_t row_cap;

  /** Copying constructor */
  Schema(Schema& from):
	  Object(from) {
		  column_num = from.column_num;
		  row_num = from.row_num;
	  }

  /** Create an empty schema **/
  Schema() {
  }

  /** Create a schema from a string of types. A string that contains
    * characters other than those identifying the four type results in
    * undefined behavior. The argument is external, a nullptr argument is
    * undefined. **/
  Schema(const char* types) {
      column_types = new String(types);
      column_num = column_types->size_;
      row_num = 0;
      column_cap =10;
      row_cap = 10;
  }

  Schema(Deserializer &dser) {
      column_num = dser.read_size_t();
      row_num = dser.read_size_t();
      column_cap = dser.read_size_t();
      row_cap = dser.read_size_t();
      column_types = new String(dser);
  }

  ~Schema() {
      delete column_types;
  }

    void serialize(Serializer &ser) {
        ser.write_size_t(column_num);
        ser.write_size_t(row_num);
        ser.write_size_t(column_cap);
        ser.write_size_t(row_cap);
        column_types->serialize(ser);
    }

  /** Add a column of the given type and name (can be nullptr), name
    * is external. Names are expectd to be unique, duplicates result
    * in undefined behavior. */
  void add_column(char typ) {
	  if(column_num >= column_cap) {
		  column_cap = column_cap * 2;
	  }
	  if(typ == *"S") {
		  column_types->cstr_[column_num] = *"S";
		  column_num ++;
	  }
	  else if (typ == *"B") {
          column_types->cstr_[column_num] = *"B";
                  column_num ++;
          }
	  else if(typ == *"I") {
          column_types->cstr_[column_num] = *"I";
                  column_num ++;
          }
	  else if(typ == *"F") {
          column_types->cstr_[column_num] = *"F";
                  column_num ++;
          }
      else if(typ == *"D") {
          column_types->cstr_[column_num] = *"D";
          column_num ++;
      }
  }

  /** Add a row with a name (possibly nullptr), name is external.  Names are
   *  expectd to be unique, duplicates result in undefined behavior. */
  void add_row() {
	  if(row_num >= row_cap) {
		  row_cap = row_cap * 2;
		  }
	  row_num ++;
  }

  /** Return type of column at idx. An idx >= width is undefined. */
  char col_type(size_t idx) {
	  return column_types->cstr_[idx];
  }

  /** The number of columns */
  size_t width() {
	  return column_num;
  }

  /** The number of rows */
  size_t length() {
      return row_num;
  }
};
