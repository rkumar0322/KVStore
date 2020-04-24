//
// Created by Rahul Kumar on 3/31/20.
//


#include "../helpers/serial.h"
#include "keyvalue.h"
#include "map.h"
#include "../network/network_pseudo.h"
#include "../network/thread.h"

class DataFrame;
class NetworkPseudo;
class Map;


class KDStore : public Map {

public:
    NetworkIfc* ifc_;  // the network which this node belongs to
    size_t index_;  // the index of the node 
    int num_finishes;
    Lock get_lock; // lock for get method
    Lock wg_lock;  // lock for wait_get method
    Lock put_lock; // lock for put method 
    Lock general_lock;
    bool process_over;   //check for process status 
    bool app_done;       //check for application status 
    MessageQueue* put_queue;
    MessageQueue* get_queue;  // A list of messages received with values from get method
    MessageQueue* waitandget_queue; // A list of messages received with values from wait_get method
    int num_nodes;
    
    /*Constructor*/
    KDStore() : Map() {
    }

    KDStore(size_t idx): Map() {
        len = 0;
        NetworkPseudo a(0);
        ifc_ = &a;
        index_ = idx;
        get_queue = new MessageQueue();
        put_queue = new MessageQueue();
        waitandget_queue = new MessageQueue();
    }

    KDStore(NetworkIfc& ifc, size_t index): Map(){
        ifc_ = &ifc;
        ifc_->register_node(index);
        index_ = index;
        process_over = false;
        app_done = false;
        get_queue = new MessageQueue();
        put_queue = new MessageQueue();
        waitandget_queue = new MessageQueue();
        num_nodes == 3;
    }

    /**Deconstructor*/
    ~KDStore() {
        delete get_queue;
        delete waitandget_queue;
        delete put_queue;
        for (int i = 0; i < len;i++) {
            Key* a = (Key*)keys_[i];
            delete keys_[i];
            Value* b = (Value*)keys_[i];
            delete values_[i];
        }
        delete[] keys_;
        delete[] values_;
    }

    /** get the value of the given key
     * @return the Dataframe* of the key
    */
    DataFrame* get(Key k) {
        //check if it the key comes from the current node
        if (k.nodeidx == index_) {
            Object* o = get_(&k);
            Value* v = (Value*)o;
            Value v1 = *v;
            Deserializer d(v1.val_, v1.size_);
            return new DataFrame(d);
        } else { //if not, set a get_lock
            get_lock.lock();
            Serializer s1;
            k.serialize(s1);
            // create and send a get message 
            Message* m = new Message(MsgKind::Get,index_,k.nodeidx,0,s1.length_,s1.data_);
            ifc_->send_m(m);
            // wait until we receive the message with the value 
            while(get_queue->peek() == nullptr) {

                get_lock.wait();
            }
            //extract and deserialize the value
            Message* m1 = get_queue->pop();
            Deserializer d1(m1->data_,m1->msgsize_);
            Value v(d1);
            Deserializer d2(v.val_, v.size_);
            get_lock.unlock();
            get_lock.notify_all();
            return new DataFrame(d2);
        }
    }

    /** get the value of the given key, blocking other threads from accessing it 
     * @return the Dataframe* of the key
    */
    DataFrame* waitAndGet(Key k) {
        //check if the key comes from the current node
        if (k.nodeidx == index_) {
            //set a lock
            general_lock.lock();
            while(get_(&k) == nullptr) {
                general_lock.wait();
            }
            Object* o = get_(&k);
            Value* v = (Value*)o;
            Value v1 = *v;
            Deserializer d(v->val_, v->size_);
            general_lock.unlock();
            return new DataFrame(d);
        } else { //if the key comes from other nodes 
            wg_lock.lock();
            Serializer s1;
            k.serialize(s1);
            Message* m = new Message(MsgKind::WaitAndGet,index_,k.nodeidx,0,s1.length_,s1.data_);
            ifc_->send_m(m);
            // wait until we receive the message with the value 
            while(waitandget_queue->peek() == nullptr) {
                wg_lock.wait();
            }
            Message* m1 = waitandget_queue->pop();
            Deserializer d2(m1->data_, m1->msgsize_);
            wg_lock.unlock();
            wg_lock.notify_all();
            return new DataFrame(d2);
        }
    }

    /** insert the key-value pair, blocking other threads from inserting at the same time 
     * @return void 
    */
    void put(Key k, DataFrame* df) {
        Serializer s;
        df->serialize(s);
        //check if it the key comes from the current node
        if (k.nodeidx == index_) {
            general_lock.lock();
            Value* v = new Value(s);
            Key* copy = new Key(k.key_->cstr_,k.nodeidx);
            put_(copy, v);
            general_lock.unlock();
            general_lock.notify_all();
        } else { //if the key comes from other nodes
            put_lock.lock();
            Value v1(s);
            KVPair p(k,v1);
            Serializer s1;
            p.serialize(s1);
            Deserializer d1(s1.data_,s1.length_);
            KVPair p1(d1);
            Message* m = new Message(MsgKind::Put,index_,k.nodeidx,0,s1.length_,s1.data_);
            ifc_->send_m(m);
            // wait until the put has been successfully done 
            while(put_queue->peek() == nullptr) {
                put_lock.wait();
            }
            put_queue->pop();
            put_lock.unlock();
            put_lock.notify_all();
        }
    }

    void end_producer() {
        process_over = true;
    }

    void applicationdone() {
        app_done = true;
        general_lock.notify_all();
    }

    void run() {
        while (!process_over) {
            Message* m = ifc_->recv_m();
            if (m == nullptr) {
                continue;
            }
            while(m->sender_ == index_) {
                ifc_->send_m(m);
                m = ifc_->recv_m();
                if (m->kind_ == MsgKind::Put) {
                    break;
                } else {
                    Deserializer d(m->data_, m->msgsize_);
                    Key* k = new Key(d);
                    if(get_(k) == nullptr) {
                        continue;
                    } else {
                        Object* o = get_(k);
                        Value* v = (Value*)o;
                        Value v1 = *v;
                        Message* m3 = new Message(MsgKind::WaitandGetReply, index_, m->extra_target, 0, v1.size_,v1.val_);
                        ifc_->send_m(m3);
                        m = nullptr;
                        break;
                    }
                }
            }
            if (m == nullptr) {
                continue;
            }
            if (m->kind_ == MsgKind::WaitAndGet) {
                Deserializer d(m->data_, m->msgsize_);
                Key *k = new Key(d);
                while (get_(k) == nullptr) {
                    general_lock.wait();
                    if (app_done) {
                        general_lock.notify_all();
                        break;
                    }
                }
                if (get_(k) == nullptr) {
                    Message *m4 = new Message(MsgKind::WaitAndGet, index_, index_, 0, m->msgsize_, m->data_);
                    m4->extra_target = m->sender_;
                    ifc_->send_m(m4);
                    continue;
                }
                Object *o = get_(k);
                Value *v = (Value *) o;
                Value v1 = *v;
                Message *m3 = new Message(MsgKind::WaitandGetReply, index_, m->sender_, 0, v1.size_, v1.val_);
                ifc_->send_m(m3);
                general_lock.notify_all();
            }
            if (m->kind_ == MsgKind::Put) {
                put_lock.lock();
                Deserializer d(m->data_, m->msgsize_);
                KVPair k1(d);
                put_(&k1.k, &k1.v);
                general_lock.notify_all();
                ifc_->send_m(new Message(MsgKind::PutReply, index_, m->sender_, 0, 0, ""));
                put_lock.unlock();
            }
            if(m->kind_ == MsgKind::PutReply) {
                if (put_queue->peek() != nullptr) {
                    put_lock.wait();
                }
                put_queue->push_(m);
                put_lock.unlock();
                put_lock.notify_all();
            }
            if (m->kind_ == MsgKind::Get) {
                Deserializer d(m->data_, m->msgsize_);
                Key k(d);
                Object *o = get_(&k);
                Value *v = (Value *) o;
                Value v1 = *v;
                Serializer s;
                v1.serialize(s);
                ifc_->send_m(new Message(MsgKind::GetReply, index_, m->sender_, 0, s.length_, s.data_));
            }
            if (m->kind_ == MsgKind::GetReply) {
                get_lock.lock();
                if (get_queue->peek() != nullptr) {
                    get_lock.wait();
                }
                get_queue->push_(m);
                get_lock.unlock();
                get_lock.notify_all();
            }
            if (m->kind_ == MsgKind::WaitandGetReply) {
                if (index_ == 0) {
                }
                wg_lock.lock();
                if (get_queue->peek() != nullptr) {
                    wg_lock.wait();
                }
                waitandget_queue->push_(m);
                wg_lock.unlock();
                wg_lock.notify_all();
            }
            if (m->kind_ == MsgKind::Finished) {
                num_finishes++;
                if (num_nodes == num_finishes) {
                    for (size_t i = 1; i < num_nodes;i++) {
                        ifc_->send_m(new Message(MsgKind::Kill,0,i,0,0,""));
                    }
                    end_producer();
                }
            }
            if (m->kind_ == MsgKind::Kill) {
                end_producer();
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
    for(size_t i = 0; i < num; i++) {
        r.set(0, vals[i]);
        ret->add_row(r);
    }
    kv->put(*k, ret);
    return ret;
}

/** Read a double into a dataframe with a single column, and store that dataframe in a kvstore
    @return the dataframe of doubles*/
DataFrame* DataFrame::fromScalar(Key* k, KDStore* kv, double val) {
    Schema s("D");
    DataFrame* ret = new DataFrame(s);
    Row r(s);
    r.set(0, val);
    ret->add_row(r);
    kv->put(*k, ret);
    return ret;
}