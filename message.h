#pragma once
#include "object.h"
#include "string.h"
//#include <unistd.h>
#include <stdio.h>
//#include <sys/socket.h>
#include <stdlib.h>
//#include <netinet/in.h>
#include <string>
#include "array.h"
//#include <arpa/inet.h>


enum class MsgKind { Ack, Nack, Put,
                    Reply,  Get, WaitAndGet, Status,
                    Kill,   Register,  Directory };

 

class Message : public Object {
	public:		
    
    MsgKind kind_;  // the message kind

    size_t sender_; // the index of the sender node

    size_t target_; // the index of the receiver node

    size_t id_;     // an id t unique within the node

    Message(Deserializer &d) {
        sender_ = d.read_size_t();
        target_ = d.read_size_t();
        id_ = d.read_size_t();

    }

    Message() {

    }

    size_t target() {
        return target_;
    }

};

 

class Ack : public Message {


    public:
    
    Ack(size_t s1, size_t s2, size_t s3) {
        kind_ = MsgKind::Ack;
        sender_ = s1;
        target_ = s2;
        id_ = s3;
    }

    bool equals(Object *o) {
        if (o == nullptr) {
            return false;
        }
        Ack* ack1 = dynamic_cast<Ack*> (o);
        if (sender_ != ack1->sender_) {
            return false;
        }
        if (target_ != ack1->target_) {
            return false;
        }
        if (id_ != ack1->id_) {
            return false;
        }
        return true;
    }

};

 

class Status : public Message {


    public:

    StrArray* msg_; // owned
    
    
    Status(size_t s1, size_t s2, size_t s3, StrArray* s) {
        kind_ = MsgKind::Status;
        sender_ = s1;
        target_ = s2;
        id_ = s3;
        msg_ = s; 
    }

/*
    char* serialize() {
        size_t kind = 7; 
        size_t sender = sender_;
        size_t target = target_;
        size_t id = id_;    
        char* kind_code = reinterpret_cast<char*>(&kind);
        char* sender_code = reinterpret_cast<char*>(&sender);
        char* target_code = reinterpret_cast<char*>(&target);
        char* id_code = reinterpret_cast<char*>(&id);
        char* msg_buf = msg_->serialize();
        size_t total = 4*sizeof(size_t) + sizeof(msg_buf);
        char* buf = new char[total];
        strcat(buf, kind_code);
        strcat(buf, " ");
        strcat(buf, sender_code);
        strcat(buf, " ");
        strcat(buf, target_code);
        strcat(buf, " ");
        strcat(buf, id_code);
        strcat(buf, " msg: ");
        strcat(buf, msg_buf);
        return buf; 
    }
    */

};

 

class Register : public Message {
public:

    struct sockaddr_in client;
    size_t port;

    Register(unsigned idx, unsigned port) {
        port = port;
    }

    int sender() {return 0;}

   
};

 

class Directory : public Message {

    public:

    size_t client;

    size_t * ports;  // owned

    StrArray* addresses;  // owned; strings owned

    
    Directory(size_t s1, size_t s2, size_t s3, size_t s4, 
    size_t* s5, StrArray* s) {
        kind_ = MsgKind::Directory;
        sender_ = s1;
        target_ = s2;
        id_ = s3;
        client = s4;
        ports = s5;
        addresses = s; 
    }

    Directory(size_t* ports, String** addresses1, size_t arr_addr) {
        addresses = new StrArray();
        addresses->arr = addresses1;
        addresses->len = arr_addr;
        addresses->capacity_ = arr_addr * 2;
        ports = ports;
    }

    void log() {

    }

    /*
    char* serialize() {
        size_t kind = 10; 
        size_t sender = sender_;
        size_t target = target_;
        size_t id = id_;    
        size_t portss = *ports;
        char* kind_code = reinterpret_cast<char*>(&kind);
        char* sender_code = reinterpret_cast<char*>(&sender);
        char* target_code = reinterpret_cast<char*>(&target);
        char* id_code = reinterpret_cast<char*>(&id);
        char* client_code = reinterpret_cast<char*>(&client);
        char* ports_code = reinterpret_cast<char*>(ports);
        char* addresses_code = addresses->serialize();
        size_t total = 4*sizeof(size_t) + sizeof(addresses_code);
        char* buf = new char[total];
        strcat(buf, kind_code);
        strcat(buf, " ");
        strcat(buf, sender_code);
        strcat(buf, " ");
        strcat(buf, target_code);
        strcat(buf, " ");
        strcat(buf, id_code);
        strcat(buf, " ");
        strcat(buf, client_code);
        strcat(buf, " ");
        strcat(buf, ports_code);
        strcat(buf, " address: ");
        strcat(buf, addresses_code);
        return buf; 

   }*/

};
