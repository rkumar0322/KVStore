//
// Created by Rahul Kumar on 4/6/20.
//

#include "network/thread.h"
#include "application/application.h"
#include <time.h>




class NetworkUser: public Thread {
public:
    NetworkPseudo* network_;
    size_t node_;

    NetworkUser(NetworkPseudo* n, size_t node) {
        network_ = n;
        node_ = node;
    }

    void run() {
        network_->register_node(node_);

        sleep(1000);
    }

};

class Producer: public Thread {
public:
    Application* a_;

    Producer(Application* a) {
        a_ = a;
    }
    void run() {
        a_->kv.ifc_->register_node(a_->index);
        a_->kv.run();
    }
};

class Consumer: public Thread {
public:
    Application* a_;

    Consumer(Application* a) {
        a_ = a;
    }
    void run() {
        a_->kv.ifc_->register_node(a_->index);
        a_->run_();
        a_->kv.applicationdone();
    }
};

int main3() {
    MQArray* a = new MQArray(3);
    MessageQueue* b = a->get_(0);
    delete a;
    return 0;
}

int main() {
    NetworkPseudo network(3);
    Application* a1 = new Demo(network,0);
    Producer* p1 = new Producer(a1);
    Consumer* c1 = new Consumer(a1);
    Application* a2 = new Demo(network,1);
    Producer* p2 = new Producer(a2);
    Consumer* c2 = new Consumer(a2);
    Application* a3 = new Demo(network,2);
    Producer* p3 = new Producer(a3);
    Consumer* c3 = new Consumer(a3);
    p1->start();
    p2->start();
    p3->start();
    c1->start();
    c2->start();
    c3->start();
    p1->join();
    p2->join();
    p3->join();
    c1->join();
    c2->join();
    c3->join();
    delete p1;
    delete c1;
    delete a1;
    delete p2;
    delete c2;
    delete a2;
}

void basic_example() {
    NetworkPseudo* network = new NetworkPseudo(3);
    NetworkUser* a = new NetworkUser(network, 0);
    NetworkUser* b = new NetworkUser(network, 1);
    NetworkUser* c = new NetworkUser(network, 2);
    a->start();
    b->start();
    c->start();
    a->join();
    b->join();
    c->join();
    printf("FINISHES THE PROCESS: nodes in the newtork: %d\n",network->threads_->len);
    delete a;
    delete b;
    delete c;
    delete network;
}