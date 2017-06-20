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
    Database(std::string filename) {
        this->filename = filename;
        std::ifstream file(filename);
        if (file.is_open()) {
            // Si el archivo existía, leo la base de datos y la guardo en el map
            std::string line;
            while (std::getline(file, line)) {
                std::vector<std::string> result;
                std::stringstream lineStream(line);
                std::string field;

                while (std::getline(lineStream, field, ',')) {
                    result.push_back(field);
                }
                if (!lineStream && field.empty()) {
                    result.push_back("");
                }
                struct record rec;
                strncpy(rec.name, result[0].c_str(), NAME_LENGTH);
                strncpy(rec.address, result[1].c_str(), ADDRESS_LENGTH);
                strncpy(rec.phone, result[2].c_str(), PHONE_LENGTH);

                data[rec.name] = rec;
            }
        }
    }

    struct record get(std::string name) {
        std::map<std::string, struct record>::iterator it = data.find(name);
        if (it == data.end()) {
            throw NOT_FOUND;
        } else {
            return it->second;
        }
    }

    void add(struct record rec) {
        data[rec.name] = rec;
    }

    void save() {
        std::ofstream file(filename);
        std::map<std::string, struct record>::iterator it;
        for (it = data.begin(); it != data.end(); ++it) {
            struct record rec = it->second;
            file << rec.name << "," << rec.address << "," << rec.phone << std::endl;
        }
    }
};


#endif //PROYECTO_DATABASE_H
