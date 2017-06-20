#ifndef PROYECTO_REQUEST_H
#define PROYECTO_REQUEST_H

#include "record.h"

struct request {
    long mtype;
    int client;
    int req_type;
    struct record rec;
    int status;
};

#endif //PROYECTO_REQUEST_H
