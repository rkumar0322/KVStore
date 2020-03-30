//
// Created by Rahul Kumar on 3/29/20.
//
#include <iostream>
#include <assert.h>
#include <stdio.h>
#include "serial.h"
#include "dataframe.h"


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

int main() {
    testwritesandreads();
    serialize_string();
    serialize_stringarr();
    serialize_intarr();
    serialize_doubarr();
    serialize_floatarr();
    serialize_boolarr();
    serialize_column();
}

void toyexample() {
    char* r = "Rahul";
    char* data_ = new char[1024];
    printf("%d\n",sizeof(data_));
    memcpy(data_,r,5);
    printf("%d\n",sizeof(data_));
    size_t size = 5;
    char* g[size];
    memcpy(g,data_,5);
    printf("%d\n",g);
}
void toyexample2() {
    size_t r = 3;
    printf("%i\n",r);
    char* r1 = "Rahul";
    char* data_ = new char[1024];
    memcpy(data_,&r,sizeof(size_t));
    memcpy(data_,r1,5);
    size_t size = 5;
    char* g1[size];
    memcpy(g1,data_,5);
    printf("%s\n",g1);
    size_t g;
    memcpy(&g,data_,sizeof(size_t));
    printf("%i\n",g);
}

void serialize_stringarr() {
    StrArray* s = new StrArray();
    s->add(new String("Rahul"));
    s->add(new String("Kumar"));
    s->add(new String("Tarun"));
    Serializer ser;
    s->serialize(ser);
    Deserializer dser(ser.data_,ser.length_);
    StrArray* s2 = new StrArray(dser);
    printf("Val1: %s\n",s2->get(0)->cstr_);
    printf("Val2: %s\n",s2->get(1)->cstr_);
    printf("Val3: %s\n",s2->get(2)->cstr_);
}

void serialize_intarr() {
    IntArray* s = new IntArray();
    s->add(1);
    s->add(2);
    s->add(3);
    Serializer ser;
    s->serialize(ser);
    Deserializer dser(ser.data_,ser.length_);
    IntArray* s2 = new IntArray(dser);
    printf("Val1: %i\n",s2->get(0));
    printf("Val2: %i\n",s2->get(1));
    printf("Val3: %i\n",s2->get(2));
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
    printf("Val1: %f\n",s2->get(0));
    printf("Val2: %f\n",s2->get(1));
    printf("Val3: %f\n",s2->get(2));
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
    printf("Val1: %f\n",s2->get(0));
    printf("Val2: %f\n",s2->get(1));
    printf("Val3: %f\n",s2->get(2));
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
    printf("Val1: %d\n",s2->get(0));
    printf("Val2: %d\n",s2->get(1));
    printf("Val3: %d\n",s2->get(2));
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
    printf("Val1: %f\n",s2->get(0));
    printf("Val2: %f\n",s2->get(1));
    printf("Val3: %f\n",s2->get(2));

}


void serialize_string() {
    String* s = new String("Rahul");
    Serializer ser;
    s->serialize(ser);
    Deserializer dser(ser.data_,ser.length_);
    String* s1 = new String(dser);
    printf("My name is: %s\n", s1->cstr_);
    printf("My size is: %d\n",s1->size_);
}

void testwritesandreads() {
    Serializer s;
    size_t a = 1;
    s.write_size_t(a);
    Deserializer d(s.data_,s.length_);
    printf("final size: %d\n",d.read_size_t());
}
void test_schema_serialize() {
    Schema s = new Schema("SSSSSSSSSS");
    Serializer s1;
    s.serialize(s1);
    Deserializer d(s.data_,s.length_);
    Schema s = new Schema(d);
}
