#ifndef COLA_H_
#define COLA_H_

#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <string>

template<class T>
class MsgQueue {
private:
    key_t clave;
    int id;

public:
    MsgQueue(const std::string &archivo, const char letra);

    ~MsgQueue();

    int escribir(const T &dato) const;

    int leer(const int tipo, T *buffer) const;

    int destruir() const;
};

template<class T>
MsgQueue<T>::MsgQueue(const std::string &archivo, const char letra) {
    this->clave = ftok(archivo.c_str(), letra);
    if (this->clave == -1)
        perror("Error en ftok");

    this->id = msgget(this->clave, 0777 | IPC_CREAT);
    if (this->id == -1)
        perror("Error en msgget");
}

template<class T>
MsgQueue<T>::~MsgQueue() {
}

template<class T>
int MsgQueue<T>::destruir() const {
    int resultado = msgctl(this->id, IPC_RMID, NULL);
    return resultado;
}

template<class T>
int MsgQueue<T>::escribir(const T &dato) const {
    int resultado = msgsnd(this->id, static_cast<const void *>(&dato), sizeof(T) - sizeof(long), 0);
    return resultado;
}

template<class T>
int MsgQueue<T>::leer(const int tipo, T *buffer) const {
    int resultado = msgrcv(this->id, static_cast<void *>(buffer), sizeof(T) - sizeof(long), tipo, 0);
    return resultado;
}

#endif /* COLA_H_ */
