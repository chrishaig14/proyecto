#ifndef PROYECTO_DATABASE_H
#define PROYECTO_DATABASE_H

#include <iostream>
#include <map>
#include <fstream>
#include "../record.h"
#include <vector>
#include <sstream>
#include <cstring>

class Database {
    std::map<std::string, struct record> data;
    std::string filename;
public:
    Database(std::string filename);

    struct record get(std::string name);

    void add(struct record rec);

    void save();
};


#endif //PROYECTO_DATABASE_H
