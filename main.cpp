//
// Created by Rahul Kumar on 4/6/20.
//

#include "application.h"

int main() {
    size_t nodes = 3;
    Demo* a = new Demo(new NetworkPseudo(0),0);
    Producer* p = new Producer(a->kv);
    printf("makes it from here before run\n");
    p->run();
    p->join();
}