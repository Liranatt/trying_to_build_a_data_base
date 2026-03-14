//
// Created by Ksenia on 12/03/2026.
//

#include "Server.h"

#include <functional>
#include <iostream>
#include <thread>
#include <sstream>
#include <vector>
#include <unistd.h> // close(), read(), write()
#include <sys/socket.h>
#include <netinet/in.h>

Server::Server(int port, KVStore &db) : port (port), db (db), server_fd(-1){
    //TODO: creating the socket, use socket() with SOCK_STREAM and AF_INET
    // save it inside server_fd, check if -1 returned
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cout << "Error creating socket" << std::endl;
        return;
    }
    // TODO: definening sockarr_in and the port
    // create object sockaddr_in, default it, define a family of adresses to AF_INET
    // SAVE the progress inside INADDR_ANY, and to listen to every port we will use htons(port)
    struct sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // bind the port to the socket, use bind() to connect server_fd to the adress,
    // need to check failures
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cout << "bind failed" << std::endl;
        return;
    }
}


Server::~Server() {
    // todo: clean. if close is good (greater or equal to 0) close it via server_fd.
    if (server_fd >= 0) {
        close(server_fd);
    }
}

void Server::start() {
    // todo: start listening, call listen on server_fd.
    if (listen(server_fd, 5) < 0) {
        std::cout << "listen failed" << std::endl;
        return;
    }
    std::cout << "Server started on port " << port << std::endl;
    struct sockaddr_in client_address;
    socklen_t client_len = sizeof(client_address);
    while (true) {
        // todo: accepting clients, to wait to a new client use accept()
        // it will block until someone else will try to connect
        // when someone connects it return a new client socket
        int client_socket = accept(server_fd, (struct sockaddr *)&client_address, &client_len);
        // todo: multi threading, need to create std::thread that will activate
        // handle_client and pass him client socket.
        // need tl call the new thread to run on detach() on the background independently
        std::thread client_thread(&Server::handle_client, this, client_socket);
        client_thread.detach();

    }
}

void Server::handle_client(int client_socket) {
    char buffer[1024] = {0};

    while (true) {
        // todo: reading data from the server:
        // use read() to read from the client_socket into the buffer
        // if read() = 0 then the client disconnected, if read() < 0 then there's an error. either way we need to break
        // the loop
        if (read(client_socket, buffer, 1024) <= 0) {
            std::cout << "read failed" << std::endl;
            break;
        }


        // todo: parsing: turn the buffer into std::string, get the arguments and the order.
        // the commands are: SET <key>  <value>, GET<key>, DEL <key>
        // we will use std::isstringstream, to separate the stream by spaces.
        std::string strbuffer = (std::string)buffer;
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
                response = "OK/n";
            }
            else {
                response = "NOT FOUND/n";
            }
        }

        write(client_socket, response.c_str(), response.length()); // write back to the client

        // cleaning the buffer before the next message
        memset(buffer, 0, sizeof(buffer));
    }
    close(client_socket);
}
