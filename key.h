//
// Created by Rahul Kumar on 3/26/20.
//

#include "object.h"
#include "string.h"

class Key : public Object {
public:
        String name;
        size_t home;

        Key(String name,size_t home) {
            this->name = name;
            this->home = home;
        }

};