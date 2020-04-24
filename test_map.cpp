#include <iostream>
#include <assert.h>
#include <stdio.h>
#include "arg.h"
#include "KDStore/map.h"

void test_map() {
    Map* m1 = new Map();
    Map* m2 = new Map();
    assert(m1->equals(m2));
    String* k1 = new String("hello");
    char c1[] = "world";
    char c2[] = "John";
    char* cp1 = c1;
    char* cp2 = c2;
    Value* v1 = new Value(c1);
    Value* v2 = new Value(c2);

    assert(!m1->put_(k1, v1));
    assert(!m1->equals(m2));
    m2->put_(k1, v1);
    assert(m1->equals(m2));
    assert(m1->put_(k1, v2));
    assert(!m1->equals(m2));
    std::cout << "test_map passed" << "\n";
}

void test_map2() {
    Map* m1 = new Map();
    Map* m2 = new Map();
    assert(m1->equals(m2));
    String* k1 = new String("hello");
    char c[] = "world";
    char* c1 = c;
    Value* v1 = new Value(c1);

    m1->put_(k1, v1);
    assert(m1->get_(k1)->equals(v1));
    std::cout << "test_map2 passed" << "\n";

}

int main(int argc, char** argv) {
    test_map();
    test_map2();
    return 0;
}





