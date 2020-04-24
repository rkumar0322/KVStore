//
// Created by Rahul Kumar on 4/5/20.
//

#ifndef KVSTORE_NETWORK_IFC_H
#define KVSTORE_NETWORK_IFC_H

#endif //KVSTORE_NETWORK_IFC_H
class NetworkIfc : public Object {
public:
    virtual void register_node(size_t idx) {}

    virtual size_t index() {assert(false);}

    virtual void send_m(Message* msg) = 0;

    virtual Message* recv_m() = 0;
};