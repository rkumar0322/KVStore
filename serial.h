#include <iostream>
#include <assert.h>
#include <stdio.h>
#include "string.h"
#include "message.h"
#include "array.h"
#pragma once

#define LOG(...) fprintf(stderr, "(" __FILE__ ") " __VA_ARGS__);


char* serial_double(double num) {
    char* buf = new char[sizeof(double)];
    memcpy(buf, &num, sizeof(double));
    return buf;
}

double deserial_double(char* buf) {
	double d = 0.0;
    memcpy(&d, buf, sizeof(double));
    return d;
}

char* serial_int(int num) {
    char* buf = new char[sizeof(int)];
    memcpy(buf, &num, sizeof(int));
    return buf;
}

int deserial_int(char* buf) {
	int s = 0;
    memcpy(&s, buf, sizeof(int));
    return s;
}

char* serial_sizet(size_t num) {
    char* buf = new char[sizeof(size_t)];
    memcpy(buf, &num, sizeof(size_t));
    return buf;
}

size_t deserial_sizet(char* buf) {
    size_t s = 0;
    memcpy(&s, buf, sizeof(size_t));
    return s;
}

char* serial_float(float f) {
    char* buf = new char[sizeof(float)];
    memcpy(buf, &f, sizeof(float));
    return buf;
}

float deserial_float(char* buf) {
	float f = 0.0;
    memcpy(&f, buf, sizeof(float));
    return f;
}

char* serial_bool(bool b) {
    char* buf = new char[sizeof(bool)];
    memcpy(buf, &b, sizeof(bool));
    return buf;
}

bool deserial_bool(char* buf) {
	bool b = 0;
    memcpy(&b, buf, sizeof(bool));
    return b;
}


char* serial_string(String* s) {
	char* buf = new char[s->size_];
    memcpy(buf, s->cstr_, s->size_);
	return buf;
}


String* deserial_string(char* buf) {
    char* c = new char[strlen(buf)];
    memcpy(c, buf, strlen(buf));
	String* s = new String(c, strlen(buf));
	return s;
}

char* serialize_double_array(DoubleArray* da) {
        char* buf = new char[da->len*sizeof(double)];
        for(size_t i = 0; i < da->len; i++) {
            double d = da->get(i);
            memcpy(buf + i*sizeof(double), &d, sizeof(double));
        }
        return buf;
}

DoubleArray* deserial_double_array(char* buf) {
    DoubleArray* da = new DoubleArray();
    double d;
    size_t index = 0;
    while(index+8 < strlen(buf)) {
        d = deserial_double(buf + index);
        da->add(d);
        index += sizeof(double);
    }

    return da;
}

char* serialize_string_array(StrArray* sa) {
    size_t total = 0;
    size_t index = 0;
    for(size_t i = 0; i < sa->len; i++) {
        total += sa->get(i)->size_;
    }
    char* buf = new char[total+ sa->len];
    for(size_t i = 0; i < sa->len; i++) {
        String* s = sa->get(i);
        memcpy(buf+index, s->cstr_, s->size_);
        index += s->size_;
        buf[index] = '0';
        index ++;
    }
    return buf;
}

StrArray* deserial_string_array(char* buf) {
	StrArray* sa = new StrArray();
    String* s;
    char* tokens = strtok(buf, "0");
    
    while(tokens != NULL) {
        s = new String(tokens, strlen(tokens));
        sa->add(s);
        tokens = strtok(NULL, "0");
    } 
    return sa;
}

char* serialize_message(Message* m) {
        size_t kind = 1;
        size_t sender = m->sender_;
        size_t target = m->target_;
        size_t id = m->id_;    
        char* kind_code = serial_sizet(kind);
        //std::cout << kind_code << "\n";
        char* sender_code = reinterpret_cast<char*>(&sender);
        char* target_code = reinterpret_cast<char*>(&target);
        char* id_code = serial_sizet(id);
        size_t total = 4*sizeof(size_t) + 3;
        //std::cout << total << "\n";
        char* buf = new char[total];
        strcat(buf, kind_code);
        strcat(buf, " ");
        strcat(buf, sender_code);
        strcat(buf, " ");
        strcat(buf, target_code);
        strcat(buf, " ");
        strcat(buf, id_code);
        return buf; 
    }


Message* deserial_message(char* buf) {
    
    Message* m;
    //std::cout << buf << "\n";
    char* tokens = strtok(buf, " ");
    //std::cout << tokens << "\n";
    size_t t = deserial_sizet(tokens);
    //std::cout << t << "\n";
    //return nullptr;
    tokens = strtok(NULL, " ");
    size_t sender = deserial_sizet(tokens);
    //std::cout << sender << "\n";
    tokens = strtok(NULL, " ");
    size_t target = deserial_sizet(tokens);
    tokens = strtok(NULL, " ");
    size_t id = deserial_sizet(tokens);
    
   
    if(t == 1) {
        std::cout << "hello" << "\n";
        m = new Ack(sender, target, id);
    }
    
   /* 
    else if (t == 7) {
        tokens = strtok(NULL, " msg: ");
        StrArray* msg = deserial:wq_string_array(tokens);
        m = new Status(sender, target, id, msg);        
    }
    else if (t == 10) {
        tokens = strtok(NULL, " ");
        size_t client = *reinterpret_cast<size_t*>(tokens);
        tokens = strtok(NULL, " ");
        size_t* ports = reinterpret_cast<size_t*>(tokens);
        tokens = strtok(NULL, " address: ");
        StrArray* addr = deserial_string_array(tokens);
        m = new Directory(sender, target, 
        id, client, ports, addr);
    }
    else {
        return NULL;
    }
    */
     
    return m;
 
}


