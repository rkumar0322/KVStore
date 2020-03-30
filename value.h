#include "object.h"
#include "dataframe.h"

class Value:public Object {
public:
    char* data;

    Value(DataFrame* df) {
        Serializer ser;
        data = df->serialize(ser);
    }
};