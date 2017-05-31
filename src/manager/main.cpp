//
// Created by chris on 30/05/17.
//

#include <iostream>
#include <map>
#include "../msgqueue/Cola.h"

#define REQUEST_MSG 1
#define ADD 1
#define GET 2

struct record {
    char name[61];
    char address[120];
    char phone[13];
};

class Database {
    std::map<std::string, struct record> data;
public:
    struct record get(std::string name) {
        return data[name];
    }

    void add(struct record rec) {
        data[rec.name] = rec;
    }
};


struct request {
    long mtype;
    int client;
    int req_type;
    struct record rec;
};

struct request handle_request(Database &db, struct request req) {

    if (req.req_type == ADD) {
        std::cout << "ADD " << req.rec.name << " , " << req.rec.address << " , " << req.rec.phone << std::endl;
        db.add(req.rec);
    }
    if (req.req_type == GET) {
        std::cout << "GET " << req.rec.name << std::endl;
        req.rec = db.get(req.rec.name);
    }
    req.mtype = req.client;
    return req;
}

int main() {
    Cola<struct request> msg_queue("/bin/bash", 'a');
    Database db;
    while (true) {
        struct request req;
        std::cout << "[server] waiting for request" << std::endl;
        msg_queue.leer(REQUEST_MSG, &req);
        std::cout << "[server] read " << req.mtype << std::endl;
        struct request response = handle_request(db, req);
        msg_queue.escribir(response);
    }
}