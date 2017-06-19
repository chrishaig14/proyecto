//
// Created by chris on 30/05/17.
//

#include <iostream>
#include <map>
#include "../msgqueue/Cola.h"
#include "../signals/SignalHandler.h"
#include "../signals/SIGINT_Handler.h"
#include "Database.h"
#include "../request.h"

struct request handle_request(Database &db, struct request req) {

    if (req.req_type == ADD) {
        std::cout << "ADD " << req.rec.name << " , " << req.rec.address << " , " << req.rec.phone << std::endl;
        db.add(req.rec);
        req.status = OK;
    }
    if (req.req_type == GET) {
        std::cout << "GET " << req.rec.name << std::endl;
        try {
            req.rec = db.get(req.rec.name);
            req.status = OK;
        } catch (int e) {
            req.status = e;
        }
    }
    req.mtype = req.client;
    return req;
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        std::cout << "HELP: manager <csv database file>" << std::endl;
        return 1;
    }
    SignalHandler *signalHandler = SignalHandler::getInstance();
    SIGINT_Handler sigintHandler;
    signalHandler->registrarHandler(SIGINT, &sigintHandler);
    Cola<struct request> msg_queue(KEY_PATH, MSG_QUEUE);
    Database db(argv[1]);
    while (sigintHandler.getGracefulQuit() == 0) {
        struct request req;
        msg_queue.leer(REQUEST_MSG, &req);
        struct request response = handle_request(db, req);
        msg_queue.escribir(response);
    }
    // graceful quit -> guardar base de datos
    std::cout << "Saving database ..." << std::endl;
    msg_queue.destruir();
    db.save();
    signalHandler->destruir();
}