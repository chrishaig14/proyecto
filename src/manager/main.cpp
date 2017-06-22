#include <iostream>
#include <map>
#include "../msgqueue/MsgQueue.h"
#include "../signals/SignalHandler.h"
#include "../signals/SIGINT_Handler.h"
#include "Database.h"
#include "../request.h"

struct request handle_request(Database &db, struct request req) {
    if (req.req_type == ADD) {
        // Agregar un registro a la bdd
        std::cout << "ADD Request from: " << req.client << std::endl;
        std::cout << " Record  Name: " << req.rec.name << " , Address: " << req.rec.address << " , Phone: "
                  << req.rec.phone << std::endl;
        db.add(req.rec);
        req.status = OK;
    }
    if (req.req_type == GET) {
        // Obtener un registro de la bdd
        std::cout << "GET Request from: " << req.client << std::endl;
        std::cout << "Record by Name: " << req.rec.name;
        try {
            req.rec = db.get(req.rec.name);
            req.status = OK;
            std::cout << " found" << std::endl;
        } catch (int e) {
            req.status = e;
            std::cout << " NOT found" << std::endl;
        }
    }
    req.mtype = req.client; // Poner el PID del cliente como mtype para que el cliente pueda leerlo.
    return req;
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        std::cout << "HELP: run as \"manager <csv database file>\"" << std::endl;
        return 1;
    }

    SignalHandler *signalHandler = SignalHandler::getInstance();
    SIGINT_Handler sigintHandler;
    signalHandler->registerHandler(SIGINT, &sigintHandler);

    MsgQueue<struct request> msg_queue(KEY_PATH, MSG_QUEUE);

    Database db(argv[1]); // argv[1]: archivo de la bdd

    while (sigintHandler.getGracefulQuit() == 0) {
        struct request req;
        msg_queue.read(REQUEST_MSG, &req);
        if (sigintHandler.getGracefulQuit() == 0) {
            //Si no está esta condición, cuando recibe SIGINT, toma el último request otra vez.
            struct request response = handle_request(db, req);
            msg_queue.write(response);
        }
    }
    // graceful quit -> guardar base de datos
    std::cout << "Saving database ..." << std::endl;
    db.save();
    msg_queue.destroy();
    signalHandler->destroy();
    return 0;
}