#ifndef PROYECTO_REQUEST_H
#define PROYECTO_REQUEST_H

#include "record.h"

struct request {
    // Mensaje enviado por el servidor o por los clientes
    long mtype; // Cuando lo envía un cliente, es REQUEST_MSG. Cuando lo envía el servidor, es el pid del cliente.
    int client; // PID del cliente.
    int req_type; // Tipo de pedido, ADD o GET para agregar u obtener un registro.
    struct record rec; // El registro a guardar, o el registro leído de la base de datos.
    int status; // Resultado del pedido, OK o NOT_FOUND si no se encontró un registro buscado.
};

#endif //PROYECTO_REQUEST_H
