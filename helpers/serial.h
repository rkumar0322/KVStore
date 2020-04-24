#include <iostream>
#include <assert.h>
#include <cstring>
#include <stdio.h>
#pragma once

#define LOG(...) fprintf(stderr, "(" __FILE__ ") " __VA_ARGS__);

/**
 * A Utility where primitive data is compressed. into a char buffer.
 */
class Serializer{
public:
    char* data_;
    size_t length_ = 0;
    size_t cap_;
    Serializer() {
        data_ = new char[100000];
        cap_ = 100000;
    }
    ~Serializer() {
    }

    /**
     * writes a size_t into the buffer.
     * @param v a size_t
     */
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

    /**
     * writes a char* into the buffer.
     * @param v a char*
     */
    void write_chars(char* v, size_t len) {
        if (length_+ len > cap_) {
            char* newstr = new char[length_ + len];
            memcpy(newstr,data_,length_);
            delete[] data_;
            data_ = newstr;
            cap_ = cap_ * 2;
        }
        memcpy(data_+ length_,v, len);
        length_ += len;
    }

    /**
     * writes a float into the buffer.
     * @param v a float
     */
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

    /**
     * writes a double into the buffer.
     * @param v a double
     */
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

    /**
     * writes a int array into the buffer.
     * @param v a int array
     */
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

    /**
     * writes a float array into the buffer.
     * @param v a float array
     */
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

    /**
     * writes a double array into the buffer.
     * @param v a double
     */
    void write_doublearr(double* v, size_t len) {
        if (length_+ (sizeof(double)*len) > cap_) {
            size_t offset = len * 8;
            char* newstr = new char[(length_ + (sizeof(double)*len))* 2];
            memcpy(newstr,data_,length_);
            delete[] data_;
            data_ = newstr;
            cap_ = cap_ * 2;
        }
        memcpy(data_ + length_, v, len * sizeof(double));
        length_ += len * sizeof(double);
    }

    /**
     * writes a bool into the buffer.
     * @param v a bool
     */
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

    /**
     * return this buffer
     */
    char* build() {
        return data_;
    }

    /**
     * size of this buffer
     * @return the size of this buffer
     */
    size_t size() {
        return length_;
    }

};

/**
 * A utility class to decompress data into the respectie original forms from a char buffer.
 */
class Deserializer {
public:
    char* data_;
    size_t length_;
    size_t cap_len;
    Deserializer(char* data, size_t length) {
        data_ = new char[length];
        memcpy(data_,data,length);
        cap_len = length;
        length_ = 0;
    }

    Deserializer(Serializer &s) {
        data_ = s.data_;
        length_ = 0;
        cap_len = s.length_;

    }

    ~Deserializer() {
        delete[] data_;
    }

    /**
     * reads a size_t into memory..
     */
    size_t read_size_t() {
        size_t v;
        memcpy(&v,data_+length_, sizeof(size_t));
        length_ += sizeof(size_t);
        return v;
    }

    /**
     * reads a char* into memory..
     */
    char* read_chars(size_t len) {
        char* x = new char[len];
        memcpy(x, data_ + length_, len);
        length_ += len;
        return x;
    }

    /**
     * reads a f into memory.
     */
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

