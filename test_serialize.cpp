#include <unistd.h>
#include <assert.h>
#include "serial.h"
#include "array.h"
#include "message.h"


void test_double() {
    double d = 8.8;
	char* buf = serial_double(d);
    double dd = deserial_double(buf);
    assert(d == dd);
    std::cout << "test_double pass" << "\n";
}

void test_int() {
    int i = 8;
	char* buf = serial_int(i);
    double ii = deserial_int(buf);
    assert(i == ii);
    std::cout << "test_int pass" << "\n";
}

void test_sizet() {
    size_t s = 1;
    char* buf = serial_sizet(s);
    size_t ss = deserial_sizet(buf);
    assert(s == ss);
    std::cout << "test_sizet pass" << "\n";
}

void test_bool() {
    bool b = 1;
	char* buf = serial_bool(b);
    bool bb = deserial_bool(buf);
    assert(b == bb);
    std::cout << "test_bool pass" << "\n";
}

void test_float() {
    float f = 1.1;
	char* buf = serial_float(f);
    float ff = deserial_float(buf);
    assert(f == ff);
    std::cout << "test_float pass" << "\n";
}



void test_StrArray() {
    String* s = new String("hello");
	String* ss = new String("world");


	StrArray* sa = new StrArray();
	sa->add(s);
	sa->add(ss);
	char* bufff = sa->serialize();
	StrArray* saa = deserial_string_array(bufff);
	assert(sa->equals(saa));
	std::cout << "test_StrArray pass" << "\n";
}


void test_DoubleArray() {
    double d = 8.8;
    DoubleArray* da = new DoubleArray();
	da->add(d);
	da->add(d);
	char* dbuf = serialize_double_array(da);
	DoubleArray* daa = deserial_double_array(dbuf);
	assert(da->equals(daa));
	std::cout << "serialize/deserialzie DoubleArray pass" << "\n";
}

void test_message() {
    size_t num1 = 1;
	size_t num2 = 2;
	size_t num3 = 3;

	Ack* ack = new Ack(num1, num2, num3);
	//size_t n = 1;
	//std::cout << buf << "\n";
	//size_t nn = deserial_sizet(buf);
	//std::cout << deserial_sizet(buf) << "\n";
	char* ackbuf = serialize_message(ack);
	
	Message* ack1 = deserial_message(ackbuf);
    //size_t sender = ack1->target_;
    assert(ack->equals(ack1));
    std::cout << "test_message pass" << "\n";
}



int main(int argc, char** argv) {

    test_double();
    test_int();
    test_sizet();
    test_bool();
    test_float();
    test_StrArray();
    test_DoubleArray();
    test_message();


	return 0;
}
