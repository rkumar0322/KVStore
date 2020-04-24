//
// Created by Rahul Kumar on 3/29/20.
//
#include <iostream>
#include <assert.h>
#include <stdio.h>
#include "helpers/serial.h"
#include "helpers/helper.h"
#include "helpers/object.h"
#include "helpers/string.h"
#include "KDStore/keyvalue.h"
#include "network/thread.h"
#include "helpers/array.h"
#include "KDStore/map.h"
#include "KDStore/newmap.h"
#include "network/thread.h"
#include <time.h>
#include "arg.h"
#include "network/message.h"
#include "network/network_ifc.h"
#include "network/network_pseudo.h"
#include "network/network_ip.h"
#include "DataFrame/schema.h"
#include "DataFrame/column.h"
#include "DataFrame/row.h"
#include "application/reader.h"
#include "application/writer.h"
#include "DataFrame/dataframe.h"
#include "KDStore/kdstore.h"
#include "application/application.h"
#include "application/wordcount.h"
//#include "application/linus.h"
#include <time.h>

Args arg;



void serialize_stringarr();
void serialize_string();
void toyexample();
void toyexample2();
void testwritesandreads();
void serialize_intarr();
void serialize_doubarr();
void serialize_floatarr();
void serialize_boolarr();
void serialize_column();
void test_schema_serialize();
void test_char_write();
void serialize_cols();
void serialize_chunks();

int main() {
    testwritesandreads();
    serialize_string();
    test_char_write();
    serialize_stringarr();
    serialize_intarr();
    serialize_doubarr();
    serialize_floatarr();
    serialize_boolarr();
    serialize_column();
    test_schema_serialize();
    serialize_chunks();
    serialize_cols();
    printf("ALL TESTS PASS\n");
}

void serialize_intarr() {
    IntArray* s = new IntArray();
    int a = 1;
    int b = 2;
    int c = 3;
    s->add(a);
    s->add(b);
    s->add(c);
    Serializer ser;
    s->serialize(ser);
    Deserializer dser(ser.data_,ser.length_);
    IntArray* s2 = new IntArray(dser);
    assert(a==s2->get(0));
    delete s;
    delete s2;
}

void serialize_doubarr() {
    DoubleArray* s = new DoubleArray();
    double a = 1.0;
    double b = 2.0;
    double c = 3.0;
    s->add(a);
    s->add(b);
    s->add(c);
    Serializer ser;
    s->serialize(ser);
    Deserializer dser(ser.data_,ser.length_);
    DoubleArray* s2 = new DoubleArray(dser);
    assert(a==s2->get(0));
    delete s; delete s2;
}


void serialize_floatarr() {
    FloatArray* s = new FloatArray();
    float a = 1.0;
    float b = 2.0;
    float c = 3.0;
    s->add(a);
    s->add(b);
    s->add(c);
    Serializer ser;
    s->serialize(ser);
    Deserializer dser(ser.data_,ser.length_);
    FloatArray* s2 = new FloatArray(dser);
    assert(a==s2->get(0));
    delete s; delete s2;
}

void serialize_boolarr() {
    BoolArray* s = new BoolArray();
    bool a = true;
    bool b = false;
    bool c = true;
    s->add(a);
    s->add(b);
    s->add(c);
    Serializer ser;
    s->serialize(ser);
    Deserializer dser(ser.data_,ser.length_);
    BoolArray* s2 = new BoolArray(dser);
    assert(a==s2->get(0));
    delete s; delete s2;
}

void serialize_column() {
    DoubleColumn* s = new DoubleColumn();
    double a = 1.0;
    double b = 2.0;
    double c = 3.0;
    s->push_back(a);
    s->push_back(b);
    s->push_back(c);
    Serializer ser;
    s->serialize(ser);
    Deserializer dser(ser.data_,ser.length_);
    DoubleColumn* s2 = new DoubleColumn(dser);
    assert(a==s2->get(0));
    delete s; delete s2;
}

void serialize_string() {
    String s("Rahul");
    Serializer ser;
    s.serialize(ser);
    Deserializer dser(ser.data_,ser.length_);
    String s1(dser);
    assert(s.equals(&s1));
}

void serialize_stringarr() {
    String* a = new String("Rahul");
    String* b  = new String("Rahul");
    String* c = new String("Rahul");
    StrArray* d = new StrArray();
    d->add(a);
    d->add(b);
    d->add(c);
    Serializer s1;
    d->serialize(s1);
    Deserializer d1(s1.data_,s1.length_);
    StrArray* d2 = new StrArray(d1);
    assert(d2->get(0)->equals(d2->get(0)));
}

void serialize_chunks() {
    String* a = new String("Rahul");
    String* b  = new String("Rahul");
    String* c = new String("Rahul");
    StringChunk* d = new StringChunk();
    d->push_back(a);
    d->push_back(b);
    d->push_back(c);
    Serializer s1;
    d->serialize(s1);
    Deserializer d1(s1.data_,s1.length_);
    StringChunk* d2 = new StringChunk(d1);
    assert(d2->get(0)->equals(d2->get(0)));
}

void serialize_cols() {
    String* a = new String("Rahul");
    String* b  = new String("Rahul");
    String* c = new String("Rahul");
    StringColumn* d = new StringColumn();
    d->push_back(a);
    d->push_back(b);
    d->push_back(c);
    Serializer s1;
    d->serialize(s1);
    Deserializer d1(s1.data_,s1.length_);
    StringColumn* d2 = new StringColumn(d1);
    printf("PRINTS THIS OUT %s\n",d2->get(0)->cstr_);
    assert(d2->get(0)->equals(d2->get(0)));
}

void testwritesandreads() {
    Serializer s;
    size_t a = 1;
    s.write_size_t(a);
    Deserializer d(s.data_,s.length_);
    assert(a == d.read_size_t());
}

void test_schema_serialize() {
    Schema* s = new Schema("SSSSSSSSSS");
    Serializer s1;
    s->serialize(s1);
    Deserializer d(s1.data_,s1.length_);
    Schema* s2 = new Schema(d);
    assert(s2->column_types->equals(s->column_types));
    delete s2;
    delete s;
}

void test_char_write() {
    char* a = "Rahul";
    Serializer s;
    s.write_chars(a,5);
    assert(s.length_ == 5);
    Deserializer d(s.data_,s.length_);
    char* b = d.read_chars(5);
    delete[] b;
}
