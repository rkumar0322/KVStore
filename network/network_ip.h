#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <cstdarg>
#include <iostream>
#include <exception>

#define ARG arg.num_nodes

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
        init_sock_(port,arg.ip_addr_str);
        nodes_ = new NodeInfo[ARG];
        for (size_t i = 0; i < ARG;++i) nodes_[i].id = 0;
        nodes_[0].address = ip_;
        nodes_[0].id = 0;
        printf("MAKESIT %s\n",inet_ntoa(ip_.sin_addr));
        printf("MAKES IT PAST HERE\n");
        for (size_t i = 2; i <= ARG;++i) {
            Register* msg = dynamic_cast<Register*>(recv_r());
            nodes_[msg->sender()].id = msg->sender();
            nodes_[msg->sender()].address.sin_family = AF_INET;
            nodes_[msg->sender()].address.sin_addr.s_addr = inet_addr(msg->ip_addr->cstr_);
            nodes_[msg->sender()].address.sin_port = htons(msg->port);
        }
        size_t* ports = new size_t[ARG - 1];
        String** addresses = new String*[ARG - 1];
        for (size_t i = 0; i < ARG - 1;++i) {
            ports[i] = ntohs(nodes_[i+1].address.sin_port);
            addresses[i] = new String(inet_ntoa(nodes_[i+1].address.sin_addr));
        }
        Directory ipd(ports,addresses,ARG - 1);
        for (int i = 1; i < ARG;++i) {
            ipd.target_ = i;
            send_m(&ipd);
        }
    }

    void client_init(unsigned idx, unsigned port, char* server_adr, unsigned server_port) {
        this_node_ = idx;
        init_sock_(port,arg.ip_addr_str);
        printf("%d\n",port);
        nodes_ = new NodeInfo[1];
        nodes_[0].id = 0;
        nodes_[0].address.sin_family = AF_INET;
        nodes_[0].address.sin_port = htons(3780);
        if(inet_pton(AF_INET, arg.master_ip, &nodes_[0].address.sin_addr) <= 0) {
            assert(false && "Invalid Server ip address");
        }
        printf("%d\n",port);
        Register msg(idx,0,idx,port,arg.ip_addr_str);
        printf("THIS PARTE\n");
        send_r(&msg);
        Directory* ipd = dynamic_cast<Directory*>(recv_m());
        ipd->log();
        NodeInfo* nodes = new NodeInfo[ARG];
        nodes[0] = nodes_[0];
        for(size_t i = 0; i < ipd->client;i++) {
            nodes[i+1].id = i+1;
            nodes[i+1].address.sin_family = AF_INET;
            nodes[i+1].address.sin_port = htons(ipd->ports[i]);
            assert(inet_pton(AF_INET, ipd->addresses[i].c_str(),&nodes[i+1].address.sin_addr) < 0 &&
                "ERROR");
        }
        delete[] nodes_;
        nodes_ = nodes;
        delete ipd;
    }

    void init_sock_(unsigned port,const char* ip_address) {
        printf("%s\n",ip_address);
        assert((sock = socket(AF_INET, SOCK_STREAM,0)) >= 0);
        int opt = 1;
        assert(setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,
                &opt,sizeof(opt)) == 0);
        ip_.sin_family = AF_INET;
        ip_.sin_addr.s_addr = inet_addr(ip_address);
        ip_.sin_port = htons(port);
        assert(bind(sock,(sockaddr*)&ip_,sizeof(ip_)) >= 0);
        printf("BINDS WELL\n");
        assert(listen(sock,100) >= 0);
    }

    void send_m(Message* msg) override {
        NodeInfo & tgt = nodes_[msg->target()];
        int conn = socket(AF_INET, SOCK_STREAM, 0);
        assert(conn >= 0 && "UNABLE to create client socket");
        assert(connect(conn,(sockaddr*)&tgt.address, sizeof(tgt.address)) < 0 && "Unable to connect to remote node");
        printf("GETS TO THIS POINT\n");
        Serializer ser;
        msg -> serialize(ser);

        char* buf = ser.build();
        size_t size = ser.size();
        send(conn, &size, sizeof(size_t),0);
        send(conn, buf, size, 0);
    }

    Message* recv_m() override {
        sockaddr_in sender;
        socklen_t addrlen = sizeof(sender);
        int req = accept(sock,(sockaddr*)&sender,&addrlen);
        printf("SOMEHOW GETS TO THIS POINt\n");
        size_t size = 0;
        assert(read(req,&size,sizeof(size_t)) == 0 && "Failure");
        char* buf = new char[size];
        int rd = 0;
        while(rd != size) {
            rd += read(req,buf+rd, size - rd);
        }
        Deserializer d(buf,size);

        Message* msg = new Message(d);
        return msg;
    }

    void send_r(Register* msg) {
        NodeInfo & tgt = nodes_[msg->target()];
        int conn = socket(AF_INET, SOCK_STREAM, 0);
        assert(conn >= 0 && "UNABLE to create client socket");
        assert(connect(conn,(sockaddr*)&tgt.address, sizeof(tgt.address)) < 0 && "Unable to connect to remote node");
        printf("GETS TO THIS POINT\n");
        Serializer ser;
        msg -> serialize(ser);

        char* buf = ser.build();
        size_t size = ser.size();
        send(conn, &size, sizeof(size_t),0);
        send(conn, buf, size, 0);
    }

    Register* recv_r() {
        sockaddr_in sender;
        socklen_t addrlen = sizeof(sender);
        int req = accept(sock,(sockaddr*)&sender,&addrlen);
        printf("SOMEHOW GETS TO THIS POINt\n");
        size_t size = 0;
        assert(read(req,&size,sizeof(size_t)) == 0 && "Failure");
        char* buf = new char[size];
        int rd = 0;
        while(rd != size) {
            rd += read(req,buf+rd, size - rd);
        }
        Deserializer d(buf,size);

        Register* msg = new Register(d);
        return msg;
    }
};