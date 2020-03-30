#include <unistd.h>
#include <assert.h>
#include "serial.h"
#include "array.h"
#include "message.h"
#include "dataframe.h"
#include "kvstore.h"


void test_double() {
    double d = 8.8;
	char* buf = serial_double(d);
    double dd = deserial_double(buf);
    assert(d == dd);
    std::cout << "test_double pass" << "\n";
}

int main(int argc, char** argv) {

    test_double();

	return 0;
}