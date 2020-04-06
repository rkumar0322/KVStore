#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include "network_ifc.h"
#include <cstdarg>
#include <iostream>

class NodeInfo : public Object {
public:
    unsigned id;
    sockaddr_in address;
};

class NetworkIp : public NetworkIfc {
public:
    NodeInfo* nodes_;
    size_t this_node_;
    int sock;
    sockaddr_in ip_;

    ~NetworkIp() {close(sock);}
    size_t index() override  {return this_node_;}

    void server_init(unsigned idx, unsigned port) {
        this_node_ = idx;
        assert(idx== 0 && "Serverm must be 0");
        init_sock_(port);
        nodes_ = new NodeInfo[arg.num_nodes];
        for (size_t i = 0; i < arg.num_nodes;++i) nodes_[i].id = 0;
        nodes_[0].address = ip_;
        nodes_[0].id = 0;
        for (size_t i = 2; i <= arg.num_nodes;++i) {
            Register* msg = dynamic_cast<Register*>(recv_m());
            nodes_[msg->sender()].id = msg->sender();
            nodes_[msg->sender()].address.sin_family = AF_INET;
            nodes_[msg->sender()].address.sin_addr = msg->client.sin_addr;
            nodes_[msg->sender()].address.sin_port = htons(msg->port);
        }
        size_t* ports = new size_t[arg.num_nodes - 1];
        String** addresses = new String*[arg.num_nodes - 1];
        for (size_t i = 0; i < arg.num_nodes - 1;++i) {
            ports[i] = ntohs(nodes_[i+1].address.sin_port);
            addresses[i] = new String(inet_ntoa(nodes_[i+1].address.sin_addr));
        }
        Directory ipd(ports.addresses);
        for (int i = 1; i < arg.num_nodes;++i) {
            ipd.target_ = i;
            send_m(&ipd);
        }
    }

    void client_init(unsigned idx, unsigned port, char* server_adr, unsigned server_port) {
        this_node_ = idx;
        init_sock_(port);
        nodes_ = new NodeInfo[1];
        nodes_[0].id = 0;
        nodes_[0].address.sin_family = AF_INET;
        nodes_[0].address.sin_port = htons(server_port);
        if(inet_pton(AF_INET, server_adr, &nodes_[0].address.sin_addr) <= 0) {
            assert(false && "Invalid Server ip address");
        }
        Register msg(idx,port);
        send_m(&msg);
        Directory* ipd = dynamic_cast<Directory*>(recv_m());
        ipd->log();
        NodeInfo* nodes = new NodeInfo[arg.num_nodes];
        nodes[0] = nodes_[0];
        for(size_t i = 0; i < ipd->client;i++) {
            nodes[i+1].id = i+1;
            nodes[i+1].address.sin_famiily = AF_INET;
            nodes[i+1].address.sin_port = htons(ipd->ports[i]);
            if (inet_pton(AF_INET, ipd->addresses[i].c_str(),&nodes[i+1].address.sin_addr < 0) {
                FATAL_ERRPR("Invalid IP Directory for node " <<(i+1));
            }
        }
        delete[] nodes_;
        nodes_ = nodes;
        delete ipd;
    }

    void init_sock_(unsigned port) {
        assert((sock = socket(AF_INET, SOCK_STREAM,0)) >= 0);
        int opt = 1;
        assert(setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,
                &opt,sizeof(opt)) == 0);
        ip_.sin_family = AF_INET;
        ip_.sin_addr.s_addr = INADDR_ANY;
        ip_.sin_port = htons(port);
        assert(bind(sock,(sockaddr*)&ip_,sizeof(ip_)) >= 0);
        assert(listen(sock,100) >= 0);
    }


};