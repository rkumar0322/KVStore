#include <iostream>
#include <assert.h>
#include <stdio.h>
#pragma once

#define LOG(...) fprintf(stderr, "(" __FILE__ ") " __VA_ARGS__);

class Serializer {
public:
    char* data_;
    size_t length_ = 0;
    size_t cap_;
    Serializer() {
        data_ = new char[1024];
        cap_ = 1024;
    }

    void write_size_t(size_t v) {
        if (length_+ sizeof(size_t) > cap_) {
            char* newstr = new char[cap_ * 2];
            memcpy(newstr,data_,length_);
            delete[] data_;
            data_ = newstr;
            cap_ = cap_ * 2;
        }
        memcpy(data_+length_,&v, sizeof(size_t));
        length_ += sizeof(size_t);
    }
    void write_chars(char* v, size_t len) {
        if (length_+ len > cap_) {
            char* newstr = new char[length_ * 2];
            memcpy(newstr,data_,length_);
            delete[] data_;
            data_ = newstr;
            cap_ = cap_ * 2;
        }
        memcpy(data_+ length_,v, len);
        length_ += len;
    }
    void write_float(float v) {
        if (length_+ sizeof(size_t) > cap_) {
            char* newstr = new char[length_ * 2];
            memcpy(newstr,data_,length_);
            delete[] data_;
            data_ = newstr;
            cap_ = cap_ * 2;
        }
        memcpy(data_ + length_,&v, sizeof(float));
        length_ += sizeof(float);
    }
    void write_double(double v) {
        if (length_+ sizeof(double) > cap_) {
            char* newstr = new char[length_ * 2];
            memcpy(newstr,data_,length_);
            delete[] data_;
            data_ = newstr;
            cap_ = cap_ * 2;
        }
        memcpy(data_ + length_,&v, sizeof(double));
        length_ += sizeof(double);
    }
    void write_intarr(int* v, size_t len) {
        if (length_+ (sizeof(int)*len) > cap_) {
            char* newstr = new char[length_ * 2];
            memcpy(newstr,data_,length_);
            delete[] data_;
            data_ = newstr;
            cap_ = cap_ * 2;
        }
        memcpy(data_ + length_, v, len * sizeof(int));
        length_ += len * sizeof(int);
    }
    void write_floatarr(float* v, size_t len) {
        if (length_+ (sizeof(float)*len) > cap_) {
            char* newstr = new char[length_ * 2];
            memcpy(newstr,data_,length_);
            delete[] data_;
            data_ = newstr;
            cap_ = cap_ * 2;
        }
        memcpy(data_ + length_, v, len * sizeof(float));
        length_ += len * sizeof(float);
    }
    void write_doublearr(double* v, size_t len) {
        if (length_+ (sizeof(double)*len) > cap_) {
            char* newstr = new char[length_ * 2];
            memcpy(newstr,data_,length_);
            delete[] data_;
            data_ = newstr;
            cap_ = cap_ * 2;
        }
        memcpy(data_ + length_, v, len * sizeof(double));
        length_ += len * sizeof(double);
    }
    void write_boolarr(bool* v, size_t len) {
        if (length_+ (sizeof(bool)*len) > cap_) {
            char* newstr = new char[length_ * 2];
            memcpy(newstr,data_,length_);
            delete[] data_;
            data_ = newstr;
            cap_ = cap_ * 2;
        }
        memcpy(data_ + length_, v, len * sizeof(bool));
        length_ += len * sizeof(bool);
    }

};

class Deserializer {
public:
    char* data_;
    size_t length_;
    size_t cap_len;
    Deserializer(char* data, size_t length) {
        data_ = data;
        cap_len = length;
        length_ = 0;
    }
    size_t read_size_t() {
        size_t v;
        memcpy(&v,data_+length_, sizeof(size_t));
        length_ += sizeof(size_t);
        return v;
    }
    char* read_chars(size_t len) {
        char *x = new char[len];
        memcpy(x, data_ + length_, len * sizeof(char));
        length_ += len * sizeof(char);
        return x;
    }
    float read_float() {
        float v;
        memcpy(&v,data_+length_, sizeof(float));
        length_ += sizeof(float);
        return v;
    }
    double read_double() {
        double v;
        memcpy(&v,data_+length_, sizeof(double));
        length_ += sizeof(double);
        return v;
    }

    int read_int() {
        int v;
        memcpy(&v,data_+length_, sizeof(int));
        length_ += sizeof(int);
        return v;
    }
    bool read_bool() {
        bool v;
        memcpy(&v,data_+length_, sizeof(bool));
        length_ += sizeof(bool);
        return v;
    }

    int* read_int_arr(size_t len) {
        int* x = new int[len];
        memcpy(x, data_ + length_, len * sizeof(int));
        length_ += len * sizeof(int);
        return x;
    }
    bool* read_bool_arr(size_t len) {
        bool *x = new bool[len];
        memcpy(x, data_ + length_, len * sizeof(bool));
        length_ += len * sizeof(bool);
        return x;
    }
    double* read_double_arr(size_t len) {
        double *x = new double[len];
        memcpy(x, data_ + length_, len * sizeof(double));
        length_ += len * sizeof(double);
        return x;
    }
    float* read_float_arr(size_t len) {
        float *x = new float[len];
        memcpy(x, data_ + length_, len * sizeof(float));
        length_ += len * sizeof(float);
        return x;
    }

};

