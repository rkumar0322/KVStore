//
// Created by Rahul Kumar on 4/6/20.
//

//
// Created by Rahul Kumar on 4/6/20.
//
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


NetworkIfc* initialize() {
    if (arg.pseudo) {
        //LOG("Initializing pseudo network\n");
        return new NetworkPseudo();
    } else {
        //LOG("Initializing IP network...\n");
        NetworkIp* res = new NetworkIp();
        if (arg.index == 0) {
            res->server_init(arg.index,arg.port);
        } else {
            res->client_init(arg.index,arg.port,arg.master_ip,arg.master_port);
        }
        return res;
    }
}



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
/**
 * pick the qpplication that is inended to be run.
 * @param i : the node of the application
 * @param net : the Network Object
 * @return the application that is intended to be run
 */
Application* pick(size_t i,NetworkIfc& net) {
    if (strcmp(arg.app,"demo") == 0) {
        return new Demo(i,net);
    } else if (strcmp(arg.app,"wc") == 0) {
        return new WordCount(i,net);
    } else if (strcmp(arg.app,"triv") == 0) {
        return new Trivial(i,net);
    } else if (strcmp(arg.app,"linus") == 0) {
        //return new Linus(net, i);
    } else {
        exit(1);
    }
}

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
/**
 * The eau2 system
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char** argv) {
    arg.parse(argc,argv);
    printf("makes it here\n");
    NetworkIfc* network = initialize();
    assert(arg.num_nodes != 0 && "Cannot have empty cloud.");
    try {
        if (arg.pseudo) {
            Producer** p = new Producer*[arg.num_nodes];
            Consumer** c = new Consumer*[arg.num_nodes];
            for (int i = 0; i < arg.num_nodes; i++) {
                Application* a1 = pick(i,*network);
                Producer* p1 = new Producer(a1);
                Consumer* c1 = new Consumer(a1);
                c[i] = c1;
                p[i] = p1;
            }
            for (int i = 0; i < arg.num_nodes; i++) {
                p[i]->start();
            }
            for (int i = 0; i < arg.num_nodes; i++) {
                c[i]->start();
            }
            for (int i = 0; i < arg.num_nodes; i++) {
                p[i]->join();
            }
            for (int i = 0; i < arg.num_nodes; i++) {
                c[i]->join();
            }
        } else {
            {
                Application* app = pick(arg.index,*network);
                Producer* producers = new Producer(app);
                Consumer* consumers = new Consumer(app);
                producers->start();
                consumers->start();
                producers->join();
                consumers->join();
            }
        }
    } catch (std::exception const &e) {
    } catch(...) {
        LOG("ERROR: Unknown");
    }
    delete network;
}