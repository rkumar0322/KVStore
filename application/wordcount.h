
/****************************************************************************
 * Calculate a word count for given file:
 *   1) read the data (single node)
 *   2) produce word counts per homed chunks, in parallel
 *   3) combine the results
 **********************************************************author: pmaj ****/
class WordCount: public Application {
public:
    static const size_t BUFSIZE = 1024;
    Key in;
    KeyBuff kbuf;
    SIMap all;

    WordCount(size_t idx, NetworkIfc & net):
            Application(idx, net), in("data",index), kbuf(new Key("wc-map-",0)) {}

    /** The master nodes reads the input, then all of the nodes count. */
    void run_() override {
        printf("INDICES: %d\n",index);
        if (index == 0) {
            FileReader fr;
            delete DataFrame::fromVisitor(&in, &kv, "S", fr);
            printf("DONE WITH FROM VISITOR\n");
        }
        local_count();
        reduce();

    }

    /** Returns a key for given node.  These keys are homed on master node
     *  which then joins them one by one. */
    Key* mk_key(size_t idx) {
        Key * k = kbuf.c(idx).get();
        //LOG("Created key " << k->c_str());
        return k;
    }

    /** Compute word counts on the local node and build a data frame. */
    void local_count() {
        DataFrame* words = (kv.waitAndGet(in));
        //printf("%d\n",words->s->row_num);
        //printf("%s\n",words->s->column_types->cstr_);
        //printf("STRING IS: %s\n",words->get_string(0,0)->cstr_);
        printf("NODE %d RECEIVED THEIR DATA\n",index);
        //p("Node ").p(index).pln(": starting local count...");
        SIMap map;
        Adder add(map);
        words->local_map(add);
        printf("Map Size %d\n", map.size());
        delete words;
        Summer cnt(map);
        delete DataFrame::fromVisitor(mk_key(index), &kv, "SI", cnt);
        printf("FINISHES LOCAL COUNT %d\n",index);
    }

    /** Merge the data frames of all nodes */
    void reduce() {
        if (index != 0) return;
        //pln("Node 0: reducing counts...");
        SIMap map;
        Key* own = mk_key(0);
        printf("THE KEY: %s\n",own->key_->cstr_);
        merge(kv.get(*own), map);
        for (size_t i = 1; i < arg.num_nodes; ++i) { // merge other nodes
            Key* ok = mk_key(i);
            printf("THE KEY: %s\n",ok->key_->cstr_);
            printf("MERGE1\n");
            printf("THE KEY: %d\n",ok->nodeidx);
            merge(kv.waitAndGet(*ok), map);
            delete ok;
        }
        printf("FINISHED\n");
        printf("Map Size %d\n", map.size());
        //p("Different words: ").pln(map.size());
        delete own;
    }

    void merge(DataFrame* df, SIMap& m) {
        printf("Map Size %d\n", m.size());
        printf("DF Size %d\n", df->nrows());
        Adder add(m);
        df->map(add);
        printf("Map Size %d\n", m.size());
        printf("DF Size %d\n", df->nrows());
        delete df;
    }
}; // WordcountDemo