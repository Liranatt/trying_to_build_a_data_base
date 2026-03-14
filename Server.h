//
// Created by Ksenia on 12/03/2026.
//

#ifndef SERVER_H
#define SERVER_H

#include "KVStore.h"
#include <string>

class Server {
private:
    int port;
    int server_fd; //file descriptor of the main server
    KVStore& db;

    void handle_client(int client_socket);

public:
    Server(int port, KVStore& db);
    ~Server();

    void start(); // main listening loop
};


#endif //SERVER_H