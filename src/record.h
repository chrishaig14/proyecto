#ifndef PROYECTO_RECORD_H
#define PROYECTO_RECORD_H

#include "constants.h"

struct record {
    char name[NAME_LENGTH];
    char address[ADDRESS_LENGTH];
    char phone[PHONE_LENGTH];
};

#endif //PROYECTO_RECORD_H