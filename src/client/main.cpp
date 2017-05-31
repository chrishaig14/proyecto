//
// Created by chris on 30/05/17.
//

#include <iostream>
#include <unistd.h>
#include <cstring>
#include "../msgqueue/Cola.h"

#define REQUEST_MSG 1;
#define ADD 1
#define GET 2

struct record {
    char name[61];
    char address[120];
    char phone[13];
};

struct request {
    long mtype;
    int client;
    int req_type;
    struct record rec;
};

void handle_add(Cola<struct request> &msg_queue) {
    struct request req;
    req.mtype = REQUEST_MSG;
    req.req_type = ADD;
    req.client = getpid();
    struct record rec;
    std::string line;
    std::cout << "Name:";
    do {
        std::getline(std::cin, line);
    } while (line.size() > 61);

    strncpy(rec.name, line.c_str(), 61);

    std::cout << "Address:";
    do {
        std::getline(std::cin, line);
    } while (line.size() > 120);
    strncpy(rec.address, line.c_str(), 120);

    std::cout << "Phone:";
    do {
        std::getline(std::cin, line);
    } while (line.size() > 13);
    strncpy(rec.phone, line.c_str(), 13);
    req.rec = rec;
    msg_queue.escribir(req);
    std::cout << "[client] waiting for server's response" << std::endl;
    msg_queue.leer(getpid(), &req);
    std::cout << "[client]" << req.req_type << std::endl;
}

void handle_get(Cola<struct request> &msg_queue) {
    struct request req;
    req.mtype = REQUEST_MSG;
    req.req_type = GET;
    req.client = getpid();
    std::string line;
    std::cout << "Name:";
    do {
        std::getline(std::cin, line);
    } while (line.size() > 61);

    strncpy(req.rec.name, line.c_str(), 61);
    std::cout << "GET " << req.rec.name << std::endl;
    msg_queue.escribir(req);
    std::cout << "[client] waiting for server's response" << std::endl;
    msg_queue.leer(getpid(), &req);
    std::cout << "Name:" << req.rec.name << std::endl;
    std::cout << "Address:" << req.rec.address << std::endl;
    std::cout << "Phone:" << req.rec.phone << std::endl;
}


int main() {
    Cola<struct request> msg_queue("/bin/bash", 'a');
    char c = '0';
    while (true) {
        std::cout << "1. Add record" << std::endl;
        std::cout << "2. Get record by name" << std::endl;
        c = (char) getchar();
        while (c != '1' && c != '2' && c != 'q') {
            c = (char) getchar();
        }
        if (c == 'q') {
            break;
        }
        std::cin.ignore();
        if (c == '1') {
            handle_add(msg_queue);
        }
        if (c == '2') {
            handle_get(msg_queue);
        }

    }


}