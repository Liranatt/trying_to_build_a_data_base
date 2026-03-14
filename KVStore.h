//
// Created by Ksenia on 10/03/2026.
//

#ifndef KVSTORE_H
#define KVSTORE_H
#include <iostream>
#include <unordered_map>
#include <string>
#include <shared_mutex>
#include <optional>
#include <mutex>
#include <fstream>
#include <sstream>

class KVStore {
private:
    std::unordered_map<std::string, std::string> store;
    mutable std::shared_mutex rw_mutex; // a lock that lets a lot of locks read together or only one to write

public:
    KVStore();
    ~KVStore();

    bool set(const std::string &key, const std::string &value); // will require lock write only

    std::optional<std::string> get(const std::string &key) const; // shared lock - optional in case the key doesnt exist

    bool del(const std::string &key);
};


#endif // KVSTORE_H