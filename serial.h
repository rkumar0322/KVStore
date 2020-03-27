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

char* serial_sizet(size_t num) {
        char* buf = reinterpret_cast<char*>(&num);
        return buf;
}


size_t deserial_sizet(char* buf) {
        return *reinterpret_cast<size_t*>(buf);
}


char* serial_string(String* s) {
	char* buf = s->cstr_;
	return buf;
}


String* deserial_string(char* buf) {
	String* s = new String(buf);
	return s;
}

DoubleArray* deserial_double_array(char* buf) {
    DoubleArray* da = new DoubleArray();
    double d;
    char* tokens = strtok(buf, " ");
    std::cout << tokens << "\n";
    while (tokens != NULL) {
        d = *reinterpret_cast<double*>(tokens);
        std::cout << d << "\n";
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


Message* deserial_message(char* buf) {
    
    Message* m;
    size_t t;
    char* tokens = strtok(buf, " ");
    t = *reinterpret_cast<size_t*>(tokens);
    tokens = strtok(NULL, " ");
    size_t sender = *reinterpret_cast<size_t*>(tokens);
    tokens = strtok(NULL, " ");
    size_t target = *reinterpret_cast<size_t*>(tokens);
    tokens = strtok(NULL, " ");
    size_t id = *reinterpret_cast<size_t*>(tokens);
    
    
   /* 
    if(t == 1) {
        std::cout << "hello" << "\n";
        m = new Ack(sender, target, id);
        
    }
    
    
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


