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

void test_key() {
    String* k1 = new String("hello");
    size_t s1 = 0;
    Key* key1 = new Key(k1, s1);
    Serializer ser;
    key1->serialize(ser);
    Deserializer dser(ser);
    Key* key2 = new Key(dser);
    assert(key1->equals(key2));

    std::cout << "test_key passed" << "\n";
}

void test_value() {
    char c1[] = "world";
    char c2[] = "John";
    char* cp1 = c1;
    char* cp2 = c2;
    Value* v1 = new Value(c1);
    Serializer ser;
    v1->serialize(ser);
    Deserializer dser(ser);
    Value* v2 = new Value(dser);
    assert(v1->equals(v2));
    std::cout << "test_value passed" << "\n";

}

void test_KVPair() {
    String* k1 = new String("hello");
    Key* key1 = new Key(k1, 0);
    char c1[] = "world";
    char* cp1 = c1;
    Value* v1 = new Value(c1);
    KVPair* kv = new KVPair(*key1, *v1);
    Serializer ser;
    kv->serialize(ser);
    Deserializer dser(ser);
    KVPair* kv2 = new KVPair(dser);
    assert(kv->equals(kv2));
    std::cout << "test_KVPair passed" << "\n";

}

int main() {
    test_key();
    test_value();
    test_KVPair();
    return 0;
}
