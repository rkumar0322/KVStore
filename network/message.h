#pragma once
//#include <unistd.h>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <cstdarg>
#include <iostream>
#include <exception>

class Message;
class Register;
class Directory;


enum class MsgKind { Ack, Nack, Put,Finished,
                    PutReply,GetReply,WaitandGetReply,  Get, WaitAndGet, Status,
                    Kill,   Register,  Directory };

 

class Message : public Object {
	public:		
    
    MsgKind kind_;  // the message kind

    size_t extra_sender;

    size_t sender_; // the index of the sender node

    size_t extra_target;

    size_t target_; // the index of the receiver node

    size_t id_;     // an id t unique within the node

    size_t msgsize_;
    String* type;

    char* data_;


    Message(Deserializer &d) {
        sender_ = d.read_size_t();
        printf("MAKES IT HERE %d\n", sender_);
        target_ = d.read_size_t();
        printf("MAKES IT HERE %d\n", target_);
        type = new String(d);
    }

    Message(MsgKind kind, size_t sender, size_t target, size_t id, size_t msgsize,char* data) {
        kind_ = kind;
        extra_sender = sender;
        sender_ = sender;
        extra_target = target;
        target_ = target;
        id_ = id;
        msgsize_ = msgsize;
        data_ = data;
        type = new String("Message");
    }

    void serialize(Serializer &ser) {
        ser.write_size_t(sender_);
        ser.write_size_t(target_);
        type->serialize(ser);
        ser.write_size_t(extra_target);
        ser.write_size_t(id_);
        ser.write_size_t(msgsize_);
        ser.write_chars(data_,msgsize_);
    }

    Message() {
    }

    size_t target() {
        return target_;
    }

};

class WaitAndGet : public Message {
public:
    WaitAndGet(size_t s1, size_t s2, size_t s3) {
        kind_ = MsgKind::WaitAndGet;
        sender_ = s1;
        target_ = s2;
        id_ = s3;
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

    size_t port;
    size_t idx;
    size_t sender_;
    size_t target_;
    String* ip_addr;
    String* type;

    Register(size_t sender_, size_t target_, unsigned idx, unsigned port, char* ip_addr_) {
        port = port;
        idx = idx;
        ip_addr = new String(ip_addr_);
        sender_ = sender_;
        target_ = target_;
        type = new String("REGISTER");

    }

    Register(Deserializer& d) {
        sender_ = d.read_size_t();
        printf("MAKES IT HERE %d\n", sender_);
        target_ = d.read_size_t();
        printf("MAKES IT HERE %d\n", target_);
        type = new String(d);
        port = d.read_size_t();
        printf("MAKES IT HERE %d\n", port);
        idx = d.read_size_t();
        printf("MAKES IT HERE %d\n", idx);
        ip_addr = new String(d);
        printf("MAKES IT HERE %d\n", ip_addr);
    }

    void serialize(Serializer& ser) {
        ser.write_size_t(sender_);
        printf("MAKES IT HERE %d\n", sender_);
        ser.write_size_t(target_);
        printf("MAKES IT HERE %d\n", target_);
        type->serialize(ser);
        ser.write_size_t(port);
        printf("MAKES IT HERE %d\n", port);
        ser.write_size_t(idx);
        printf("MAKES IT HERE %d\n", idx);
        ip_addr->serialize(ser);
        printf("MAKES IT HERE %d\n", ip_addr);
    }

    int sender() {return 0;}

   
};


class Directory : public Message {

    public:

    size_t client;

    size_t * ports;  // owned

    StrArray* addresses;  // owned; strings owned

    
    Directory(size_t sender, size_t target, size_t id, size_t client,
    size_t* ports, StrArray* addresses) {
        kind_ = MsgKind::Directory;
        sender_ = sender;
        target_ = target;
        id_ = id;
        client = client;
        ports = ports;
        addresses = addresses;
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

   }
*/
};