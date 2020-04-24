//
// Created by Rahul Kumar on 3/31/20.
//

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
    Lock general_lock;   // lock for the current node
    bool process_over;   //check for process status 
    bool app_done;       //check for application status 
    MessageQueue* put_queue;  // A list of put messages received from put method 
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
        Value v1 = internal_get(k);
        Deserializer d(v1.val_, v1.size_);
        return new DataFrame(d);
    }

    /** helper of get method
     * @return the Value of the key
    */
    Value internal_get(Key k) {
        if (k.nodeidx == index_) {
            Object* o = get_(&k);
            Value* v = (Value*)o;
            Value v1 = *v;
            return v1;
        } else {
            get_lock.lock();
            Serializer s1;
            k.serialize(s1);
            Message* m = new Message(MsgKind::Get,index_,k.nodeidx,0,s1.length_,s1.data_);
            ifc_->send_m(m);
            while(get_queue->peek() == nullptr) {
                get_lock.wait();
            }
            Message* m1 = get_queue->pop();
            Deserializer d1(m1->data_,m1->msgsize_);
            Value v(d1);
            get_lock.unlock();
            get_lock.notify_all();
            return v;
        }
    }

    /** get the value of the given key, blocking other threads from accessing it. 
     * @return the Dataframe* of the key
    */
    DataFrame* waitAndGet(Key k) {
        Value v = internal_wg(k);
        Deserializer d(v.val_, v.size_);
        return new DataFrame(d);
    }

    /** helper of waitandGet method.
     * @return the Value of the key
    */
    Value internal_wg(Key k) {
        if (k.nodeidx == index_) {
            printf("GETS STUCK node %d\n",index_);
            general_lock.lock();
            printf("LOCKED BY: CONSUMER %d",index_);
            //assert(get_(&k) != nullptr);
            while(get_(&k) == nullptr) {
                general_lock.wait();
            }
            printf("GETS UNSTUCK node %d\n",index_);
            Object* o = get_(&k);
            Value* v = (Value*)o;
            Value v1 = *v;
            Deserializer d(v->val_, v->size_);
            general_lock.unlock();
            general_lock.notify_all();
            printf("UNLOCKED BY: CONSUMER %d",index_);
            return v1;
        } else {
            wg_lock.lock();
            Serializer s1;
            k.serialize(s1);
            Message* m = new Message(MsgKind::WaitAndGet,index_,k.nodeidx,0,s1.length_,s1.data_);
            ifc_->send_m(m);
            while(waitandget_queue->peek() == nullptr) {
                wg_lock.wait();
            }
            Message* m1 = waitandget_queue->pop();
            Value v(m1->data_, m1->msgsize_);
            wg_lock.unlock();
            wg_lock.notify_all();
            return v;
        }
    }

    /**
     *
     * @param df
     * @param node
     * @return
     */
    DataFrame* get_chunked_df(DataFrame* df,size_t node) {
        DataFrame* df2 = new DataFrame(*df->s);
        for (int j = 0; j < df->num;j++) {
            if (df->s->col_type(j) == 'S') {
                StringColumn* s1 = new StringColumn();
                StringColumn* s = df->data[j]->as_string();
                for (int x = node; x < s->chunks_;x= x+arg.num_nodes) {
                    StringChunk* s2 = s->get_chunk(x);
                    s1->add_chunk(s2);
                    s1->len += s2->arr->len;
                }
                df2->add_string_column(s1);
                df2->s->row_num = s1 ->len;
            } else if (df->s->col_type(j) == 'I') {
                IntColumn* s1 = new IntColumn();
                IntColumn* s = df->data[j]->as_int();
                for (int x = node; x < s->chunks_;x= x+arg.num_nodes) {
                    IntChunk* s2 = s->get_chunk(x);
                    s1->add_chunk(s2);
                    s1->len += s2->arr->len;
                    printf("THE COUNT OF THIS IS: %d\n",s2->get(0));
                }
                df2->add_int_column(s1);
                df2->s->row_num = s1 ->len;
            }
        }
        return df2;
    }

    /**
     * Method that given a dataframe, chunk the dataframe accordingly based on the number of rows per chunk.
     * @param df to be split up
     * @return A list of dataframes that were derived based on how many rows a chunk has to have.
     */
    DataFrame** get_chunked_dfs(DataFrame* df) {
        DataFrame** dfs = new DataFrame*[arg.num_nodes];
        for (int i = 0; i < arg.num_nodes;i++) {
            dfs[i] = get_chunked_df(df,i);
        }
        return dfs;
    }

    void distributed_put(Key k, DataFrame* df) {
        printf("GETS TO THIS POINT\n");
        DataFrame** dfs = get_chunked_dfs(df);
        for (int i = 0; i < arg.num_nodes;i++) {
            //printf("GETS TO THIS POINT\n");
            //printf("KEY IS: %s\n",k.key_->cstr_);
            Serializer s;
            //printf("KEY IS: %s\n",k.key_->cstr_);
            DataFrame* df1 = dfs[i];
            //printf("KEY IS: %s\n",k.key_->cstr_);
            df1->serialize(s);
            Deserializer d(s.data_,s.length_);
            DataFrame* df2 = new DataFrame(d);
            //printf("%s\n",df2->get_string(0,0)->cstr_);
            //printf("GETS TO THIS POINT\n");
            //exit(0)
            Value *v = new Value(s);
            Key *copy = new Key(k.key_->cstr_, i);
            //printf("%s\n",copy->key_->cstr_);
            internal_put(copy,v);
            //printf("Node %d has their data\n",i);
        }
        //printf("FINISHES THE PUT FROM NODE %d\n",index_);
    }

    /** insert the key-value pair, blocking other threads from inserting at the same time 
     * @return void 
    */
    void put(Key k, DataFrame* df) {
        printf("GETS TO THIS POINT %d\n",k.nodeidx);
        Serializer s;
        df->serialize(s);
        Value *v = new Value(s);
        Key *copy = new Key(k.key_->cstr_, k.nodeidx);
        internal_put(copy,v);
    }

    /** helper of put method, check if the key-value pair is inside the current node.
     * @return void
    */
    void internal_put(Key* k, Value* v) {
        if (k->nodeidx == index_) {
            general_lock.lock();
            printf("PUT WAS LOCKED BY: CONSUMER %d\n",index_);
            put_(k, v);
            printf("DATA WAS PUT IN THIS KEY %s\n",k->key_->cstr_);
            printf("DATA WAS PUT IN THE STORE LOCALLY %d\n",index_);
            general_lock.unlock();
            general_lock.notify_all();
            printf("PUT WAS UNLOCKED BY: CONSUMER %d\n",index_);
        } else {
            put_lock.lock();
            printf("MESSAGES SENT SUCCESSFULLY\n");
            KVPair p(*k,*v);
            Serializer s1;
            p.serialize(s1);
            Message* m = new Message(MsgKind::Put,index_,k->nodeidx,0,s1.length_,s1.data_);
            ifc_->send_m(m);
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
                printf("MAKES it TO THE OTHER NODE\n");
                Deserializer d(m->data_, m->msgsize_);
                KVPair k1(d);
                put_(&k1.k, &k1.v);
                printf("DATA WAS PUT IN THE STORE\n");
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

DataFrame* DataFrame::fromVisitor(Key* k, KDStore* kv,char* schema, Writer& w) {
    Schema s(schema);
    DataFrame* ret = new DataFrame(s);
    Row r(s);
    while(!w.done()) {
        w.visit(r);
        //printf("GETS HERE WRITERS\n");
        ret->add_row(r);
    }
    if (w.distributed()) {
        kv->distributed_put(*k,ret);
    } else {
        kv->put(*k,ret);
    }
    return ret;
}
