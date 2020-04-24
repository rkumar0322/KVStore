#include <set>
class MessArrayQ : public Array {
public:
    MessArrayQ(size_t cap): Array(cap) {
    }

    Message* peek() {
        return dynamic_cast<Message *> (get(len - 1));
    }
    Message* pop_() {
        Object* o = pop();
        Message* v = (Message*)o;
        Message v1 = *v;
        return v;
    }
};

class MessageQueue : public Object {
public:
    MessArrayQ* q_;
    Lock lock_;

    MessageQueue(){
        q_ = new MessArrayQ(0);
    }

    ~MessageQueue() {
        delete q_;
    }

    size_t size() {
        return q_->size();
    }

    void push_(Message* msg) {
        lock_.lock();
        q_->push(msg);
        lock_.notify_all();
        lock_.unlock();
    }

    Message* peek() {
        if (q_->len == 0) {
            return nullptr;
        } else {
            return q_->peek();
        }
    }

    Message* pop() {
        lock_.lock();
        if (q_->len == 0) {
            lock_.unlock();
            return nullptr;
        } else {
            while (q_->size() == 0)  {
                lock_.wait();
            }
            Message* result = q_->pop_();
            lock_.unlock();
            lock_.notify_all();
            return result;
        }
    }

};

class Sizet_Container: public Object {
public:
    size_t size_;

    Sizet_Container(size_t size) {
        size_ = size;
    }
};


class String_size_t_Map : public Map {
public:
    Lock lock_;

    ~String_size_t_Map() override {
        for (int i = 0; i < len;i++) {
            String* a = (String*)keys_[i];
            delete keys_[i];
        }
        delete[] keys_;
        delete[] values_;
    }

    void set_u(String& k, size_t v) {
        lock_.lock();
        Sizet_Container* s = new Sizet_Container(v);
        put_(&k,(Object*)s);
        lock_.unlock();
    }

    size_t get(String k) {
        lock_.lock();
        Object* res = get_(&k);
        Sizet_Container* res1 = (Sizet_Container*)res;
        lock_.unlock();
        return res1->size_;
    }
};

class MQArray : public Array {
public:
    Lock lock;
    MQArray (size_t cap) : Array(cap) {
        for (size_t i = 0; i < cap;i++) {
            MessageQueue* s = new MessageQueue();
            arr[i] = (Object*)s;
            len+= 1;
        }
    }
    ~MQArray() {
        for (size_t i = 0; i < len;i++) {
            delete (MessageQueue*)get(i);
        }
    }
    MessageQueue* get_(size_t i) {
        lock.lock();
        Object* a = arr[i];
        lock.unlock();
        return (MessageQueue*)a;
    }
};

class NetworkPseudo : public NetworkIfc {
public:
    String_size_t_Map* threads_;
    MQArray* qs_;
    Lock lock;

    NetworkPseudo(size_t num_nodes) {
        qs_ = new MQArray(num_nodes);
        threads_ = new String_size_t_Map();
    }

    NetworkPseudo() {
        qs_ = new MQArray(arg.num_nodes);
        threads_ = new String_size_t_Map();
    }

    ~NetworkPseudo() {
        delete threads_;
        delete qs_;
    }

    void register_node(size_t idx) override {
        String* tid = Thread::thread_id();
        threads_->set_u(*tid,idx);
        if (threads_->len == (qs_->len * 2) +1) {
            lock.notify_all();
        }
    }

    void send_m(Message* msg) override {
        qs_->get_(msg->target_)->push_(msg);
    }

    Message* recv_m() override {
        lock.lock();
        if (threads_->len != (qs_->len * 2) +1) {
            lock.wait();
        }
        String* tid = Thread::thread_id();
        size_t i = threads_->get(*tid);
        delete tid;
        MessageQueue* m = (MessageQueue*)qs_->get(i);
        if (m->size() == 0) {
            lock.unlock();
            lock.notify_all();
            return nullptr;
        } else {
            Message* m1 = m->pop();
            lock.unlock();
            lock.notify_all();
            return m1;
        }
    }

    size_t index() {
        String* tid = Thread::thread_id();
        size_t i = threads_->get(*tid);
        delete tid;
        return i;
    }


};