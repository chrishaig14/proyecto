//
// Created by chris on 30/05/17.
//

#include <iostream>
#include <unistd.h>
#include <cstring>
#include "../msgqueue/Cola.h"
#include "../constantes.h"
#include "../record.h"
#include "../request.h"

std::string getField(std::string name, size_t len) {
    std::string line;
    std::cout << name << ":";
    do {
        std::getline(std::cin, line);
    } while (line.size() > len);
    return line;
}

void handle_add(Cola<struct request> &msg_queue) {
    struct request req;
    req.mtype = REQUEST_MSG;
    req.req_type = ADD;
    req.client = getpid();
    struct record rec;

    std::string name = getField("Name", NAME_LENGTH);

    strncpy(rec.name, name.c_str(), NAME_LENGTH);

    std::string address = getField("Address", ADDRESS_LENGTH);

    strncpy(rec.address, address.c_str(), ADDRESS_LENGTH);

    std::string phone = getField("Phone", PHONE_LENGTH);

    strncpy(rec.phone, phone.c_str(), PHONE_LENGTH);

    req.rec = rec;

    msg_queue.escribir(req);
    std::cout << "Waiting for server's response ..." << std::endl;
    msg_queue.leer(getpid(), &req);
    if (req.status == OK) {
        std::cout << "* OK" << std::endl;
    }
}

void handle_get(Cola<struct request> &msg_queue) {
    struct request req;
    req.mtype = REQUEST_MSG;
    req.req_type = GET;
    req.client = getpid();
    std::string line;

    std::string name = getField("Name", NAME_LENGTH);

    strncpy(req.rec.name, name.c_str(), NAME_LENGTH);

    msg_queue.escribir(req);
    std::cout << "Waiting for server's response ..." << std::endl;
    msg_queue.leer(getpid(), &req);
    std::cout << std::endl;
    if (req.status == OK) {
        std::cout << "* Name:" << req.rec.name << std::endl;
        std::cout << "* Address:" << req.rec.address << std::endl;
        std::cout << "* Phone:" << req.rec.phone << std::endl;
    } else {
        if (req.status == NOT_FOUND) {
            std::cout << "* Not found" << std::endl;
        }
    }
}


int main() {
    Cola<struct request> msg_queue(KEY_PATH, MSG_QUEUE);
    char c = '0';
    while (true) {
        std::cout << std::endl;
        std::cout << "1. Add record" << std::endl;
        std::cout << "2. Get record by name" << std::endl;
        c = (char) getchar();
        while (c != OPTION_ADD && c != OPTION_GET && c != OPTION_QUIT) {
            c = (char) getchar();
        }
        if (c == OPTION_QUIT) {
            break;
        }
        std::cin.ignore();
        if (c == OPTION_ADD) {
            handle_add(msg_queue);
        }
        if (c == OPTION_GET) {
            handle_get(msg_queue);
        }

    }
}