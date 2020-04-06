#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class NodeInfo : public Object {
public:
    unsigned id;
    sockaddr_in address;
};

class NetworkIp: public NetworkIFC {
public:
    NodeInfo* nodes_;

    size_t index() override  {return this_node_;}

};