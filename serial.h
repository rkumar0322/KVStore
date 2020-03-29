#include <iostream>
#include <assert.h>
#include <stdio.h>
#include "string.h"
#include "message.h"
#include "array.h"
#pragma once

#define LOG(...) fprintf(stderr, "(" __FILE__ ") " __VA_ARGS__);


char* serial_double(double num) {
    char* buf = reinterpret_cast<char*>(&num);
    return buf;
}

double deserial_double(char* buf) {
	return *reinterpret_cast<double*>(buf);
}

char* serial_float(float f) {
    char* buf = reinterpret_cast<char*>(&f);
	return buf;
}

float deserial_float(char* buf) {
	return *reinterpret_cast<float*>(buf);
}


char* serial_int(int num) {
    int i = num;
    char* buf = reinterpret_cast<char*>(&i);
	return buf;
}

int deserial_int(char* buf) {
	return *reinterpret_cast<int*>(buf);
}

char* serial_bool(bool b) {
    char* buf = reinterpret_cast<char*>(&b);
	return buf;
}

bool deserial_bool(char* buf) {
	return *reinterpret_cast<bool*>(buf);
}

char* serial_sizet(size_t num) {
    char* buf = reinterpret_cast<char*>(&num);
    return buf;
}


size_t deserial_sizet(char* buf) {
    //char* copy = buf;
    return *reinterpret_cast<size_t*>(buf);
    //return s;
}


char* serial_string(String* s) {
	char* buf = s->cstr_;
	return buf;
}


String* deserial_string(char* buf) {
	String* s = new String(buf);
	return s;
}

char* serialize_double_array(DoubleArray* da) {
        size_t total = 0;
        char** list = new char*[da->len];
        for(size_t i = 0; i < da->len; i++) {
                total += 8;
                double d = da->get(i);
                list[i] = reinterpret_cast<char*>(&d);
        }
        char* buf = new char[total + da->len];
        for(size_t i = 0; i < da->len; i++) {
                strcat(buf, list[i]);
                strcat(buf, " ");
        }
        return buf;
    }

DoubleArray* deserial_double_array(char* buf) {
    DoubleArray* da = new DoubleArray();
    double d;
    char* tokens = strtok(buf, " ");
    while (tokens != NULL) {
        d = deserial_double(tokens);
        da->add(d);
        tokens = strtok(NULL, " ");
    }
    return da;
}

StrArray* deserial_string_array(char* buf) {
	StrArray* sa = new StrArray();
	char* tokens = strtok(buf, " ");
	while (tokens != NULL) {
		sa->add(new String(tokens));
		tokens = strtok(NULL, " ");
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


