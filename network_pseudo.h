#include "thread.h"
#include "map.h"
#include "message.h"
#include <set>

class MessArrayQ : public Array {
public:
    MessArrayQ(size_t cap): Array(cap) {}
    Message* pop() {
        Message* result = dynamic_cast<Message *> (get(len - 1));
        arr[--len] = nullptr;
        return result;
    }
};

class MessageQueue : public Object {
public:
    MessArrayQ q_;
    Lock lock_;

    MessageQueue() : q_(2) {}

    void push(Message* msg) {
        lock_.lock();
        q_.add(msg);
        lock_.notify_all();
        lock_.unlock();
    }

    Message* pop() {
        lock_.lock();
        while (q_.size() == 0) lock_.wait();
        Message* result = q_.pop();
        lock_.unlock();
        return result;
    }

};


class String_size_t_Map : public Map {
public:
    Lock lock_;

    void set_u(String& k, size_t v) {
        lock_.lock();
        put(&k,(Object *) v);
        lock_.unlock();
    }

    size_t get(String k) {
        lock_.lock();
        size_t res = (size_t) get(k);
        lock_.unlock();
        return res;
    }
};

class MQArray : public Array {
public:
    MQArray (size_t cap) : Array(cap) {};
    MessageQueue * get(size_t i) {return dynamic_cast<MessageQueue*>(get(i));}
};

class NetworkPseudo : public NetworkIfc {
public:
    String_size_t_Map threads_;
    MQArray qs_;
    size_t index_;

    NetworkPseudo(size_t num_nodes) : qs_(1){
        for (size_t i = 0; i < num_nodes;i++) {
            qs_.add(new MessageQueue());
        }
    }

    void register_node(size_t idx) override {
        String* tid = Thread::thread_id();
        threads_.set_u(*tid,idx);
        delete tid;
    }

    void send_m(Message* msg) override {
        qs_.get(msg->target_)->push(msg);
    }

    Message* recv_m() override {
        String* tid = Thread::thread_id();
        size_t i = threads_.get(*tid);
        delete tid;
        return qs_.get(i)->pop();
    }

    size_t index() {
        return index_;
    }


};