

class Reader {
public:

    virtual bool visit(Row& r) {return true;};

};


class Adder : public Reader {
public:
    SIMap& map_;  // String to Num map;  Num holds an int

    Adder(SIMap& map) : map_(map)  {}

    bool visit(Row& r) override {
        String* word = r.get_string(0);
        //printf("%s\n",word->cstr_);
        assert(word != nullptr);
        //printf("MAKES IT PAST\n");
        Num* num = map_.contains(*word) ? map_.get(*word) : new Num();
        assert(num != nullptr);
        int count = 0;
        if (r.s.column_types->cstr_ == "SI") {
            //printf("GETS TO THIS POINT\n");
            num->v += r.get_int(0);
        } else {
            num->v++;
        }
        num->v++;
        //printf("MAKES IT PAST %d\n",num->v);
        map_.set(*word, num);
        return false;
    }
};