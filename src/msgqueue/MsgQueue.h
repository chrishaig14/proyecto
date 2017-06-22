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

    int id;

public:
    MsgQueue(const std::string &archivo, const char letra);

    ~MsgQueue();

    int write(const T &dato) const;

    int read(const int tipo, T *buffer) const;

    int destroy() const;
};

template<class T>
MsgQueue<T>::MsgQueue(const std::string &archivo, const char letra) {
    key_t clave = ftok(archivo.c_str(), letra);
    if (clave == -1) {
        perror("Error en ftok");
    }

    this->id = msgget(clave, 0777 | IPC_CREAT);
    if (this->id == -1) {
        perror("Error en msgget");
    }
}

template<class T>
MsgQueue<T>::~MsgQueue() {
}

template<class T>
int MsgQueue<T>::destroy() const {
    int resultado = msgctl(this->id, IPC_RMID, NULL);
    if (resultado == -1) {
        perror("Error al eliminar cola de mensajes");
    }
    return resultado;
}

template<class T>
int MsgQueue<T>::write(const T &dato) const {
    int resultado = msgsnd(this->id, static_cast<const void *>(&dato), sizeof(T) - sizeof(long), 0);
    if (resultado == -1) {
        perror("Error al enviar mensaje");
    }
    return resultado;
}

template<class T>
int MsgQueue<T>::read(const int tipo, T *buffer) const {
    int resultado = msgrcv(this->id, static_cast<void *>(buffer), sizeof(T) - sizeof(long), tipo, 0);
    return resultado;
}

#endif /* COLA_H_ */
