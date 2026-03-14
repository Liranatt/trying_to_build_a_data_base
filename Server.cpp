//
// Created by Ksenia on 12/03/2026.
//

#include "Server.h"
# include <algorithm>
#include <functional>
#include <iostream>
#include <thread>
#include <sstream>
#include <vector>
#include <unistd.h> // close(), read(), write()
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>

Server::Server(int port, KVStore &db) : port (port), db (db), server_fd(-1){
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cout << "Error creating socket" << std::endl;
        return;
    }
    struct sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);


    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cout << "bind failed" << std::endl;
        return;
    }
}


Server::~Server() {
    if (server_fd >= 0) {
        close(server_fd);
    }
}

void Server::start() {
    if (listen(server_fd, 5) < 0) {
        std::cout << "listen failed" << std::endl;
        return;
    }
    std::cout << "Server started on port " << port << std::endl;
    struct sockaddr_in client_address;
    socklen_t client_len = sizeof(client_address);
    while (true) {

        int client_socket = accept(server_fd, (struct sockaddr *)&client_address, &client_len);
        std::thread client_thread(&Server::handle_client, this, client_socket);
        client_thread.detach();
    }
}

void Server::handle_client(int client_socket) {
    char buffer[1024] = {0};

    while (true) {
        if (read(client_socket, buffer, 1024) <= 0) {
            std::cout << "read failed" << std::endl;
            break;
        }
        std::string strbuffer = (std::string)buffer;
        strbuffer.erase(std::remove(strbuffer.begin(), strbuffer.end(), '\r'), strbuffer.end());
        std::istringstream iss(strbuffer);

        std::string command;
        std::string response = "";
        iss >> command; // get the first information until space from iss

        if (command == "SET") {
            std:: string key, value;
            iss >> key >> value;
            db.set(key, value);
            response = "OK\n";
        }

        else if (command == "GET") {
            std::string key;
            iss >> key;
            auto result = db.get(key);
            if (result.has_value()) {
                response = result.value() + "\n";
            }
            else {
                response = "NOT FOUND\n";
            }

        }
        else if (command == "DEL") {
            std:: string key;
            iss >> key;
            if (db.del(key)) {
                response = "OK\n";
            }
            else {
                response = "NOT FOUND\n";
            }
        }
        else {
            response = "Command Not Found\n";
        }

        write(client_socket, response.c_str(), response.length()); // write back to the client

        // cleaning the buffer before the next message
        memset(buffer, 0, sizeof(buffer));
    }
    close(client_socket);
}
