//
// Created by Rahul Kumar on 3/31/20.
//

#include "dataframe.h"
#include "serial.h"
#include "keyvalue.h"
#include "map.h"
#include "network_ifc.h"
#include "network_pseudo.h"
#include "network_ip.h"
#include "thread.h"

class DataFrame;
class NetworkPseudo;
class NetworkIp;


class KDStore : public Object {

public:
    Map* map_;
    size_t len;
    NetworkPseudo* ifc;
    Lock lock_;
    /*Constructor*/
    KDStore() {
        map_ = new Map();
        len = 0;
    }

    KDStore(NetworkIfc* ifc, size_t index) {
        ifc = ifc;
        ifc->register_node(index);
        map_ = new Map();
        len = 0;
    }

    /**Deconstructor*/
    ~KDStore() {
        delete[] map_->keys_;
        delete[] map_->values_;
        delete[] map_;
    }

    DataFrame* get(Key* k) {
        Value* v = (Value*)map_->get(k);
        Deserializer d(v->val_,v->size_);
        return new DataFrame(d);
    }

    DataFrame* waitandget(Key* k) {
        printf("CREATES THE MESSAGE\n");
        Serializer ser;
        k->serialize(ser);
        MsgKind a3 = MsgKind::WaitAndGet;
        printf("CREATES THE MESSAGE1\n");
        Message* msg = new Message(a3,ifc->index(),k->nodeidx,0,
                ser.length_,ser.data_);
        printf("CREATES THE MESSAGE2\n");
        ifc->send_m(msg);
        Message* recv = ifc->recv_m();
        Deserializer dser(recv->data_,recv->msgsize_);
        Value* v = new Value(dser);
        Deserializer d(v->val_,v->size_);
        return new DataFrame(d);
    }

    void put(Key* k, DataFrame* df) {
        lock_.lock();
        Serializer s;
        df->serialize(s);
        if (k->nodeidx == ifc->index()) {
            map_->put(k, new Value(s));
        }
        Serializer s1;
        KVPair* kp = new KVPair(k,new Value(s));
        kp->serialize(s1);
        Message* msg = new Message(MsgKind::Put,ifc->index(),k->nodeidx,0,
                                   s1.length_,s1.data_);
        lock_.unlock();
    }

    void run() {
        while(true) {
            Message* msg = ifc->recv_m();
        }
    }
};

/** The consumer thread. */
class Producer : public Thread {
public:
    KDStore *box_;

    Producer(KDStore* box) {
        box_ = box;
    }

    /** Once started, the consumer will try retrieving the messages left by
     *  the producer.
     */
    virtual void run() {
        while(true) {
            printf("Making it here\n");
            printf("Making it here\n");
            Message* m = box_->ifc->recv_m();
            printf("Making it here\n");
            switch(m->kind_) {
                case MsgKind ::Put:
                    Deserializer d(m->data_,m->msgsize_);
                    KVPair* kp = new KVPair(d);
                    box_->map_->put(kp->k,kp->v);
            }
        }
    }
};


/** Read a list of doubles into a dataframe, and store that dataframe in a kvstore
    	@return the dataframe of doubles*/
DataFrame* DataFrame::fromArray(Key* k, KDStore* kv, size_t num, double* vals) {
    Schema s("D");
    DataFrame* ret = new DataFrame(s);
    Row r(s);
    DoubleArray* da = new DoubleArray();
    for(size_t i = 0; i < num; i++) {
        da->add(vals[i]);
        r.set(0, vals[i]);
        ret->add_row(r);
    }
    kv->put(k, ret);
    return ret;
}

/** Read a double into a dataframe with a single column, and store that dataframe in a kvstore
    @return the dataframe of doubles*/
DataFrame* DataFrame::fromScalar(Key* k, KDStore* kv, double val) {
    Schema s ("D");
    DataFrame* ret = new DataFrame(s);
    Row r(s);
    r.set(0, val);
    ret->add_row(r);
    kv->put(k, ret);
    return ret;
}