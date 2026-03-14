
#include "KVStore.h"

KVStore::KVStore() {
    std::ifstream log_file("database.log");
    std::string line;
    while (std::getline(log_file, line)) {
        std::istringstream iss(line);
        std::string command, key, value;

        iss >> command;
        if (command == "SET") {
            iss >> key >> value;
            store[key] = value;
        }
        else if (command == "DEL") {
            iss >> key;
            store.erase(key);
        }
    }
    log_file.close();
}


KVStore::~KVStore() {}

bool KVStore::set(const std::string& key, const std::string& value) {
    std::unique_lock<std::shared_mutex> lock(rw_mutex);
    store[key] = value;

    std::ofstream log_file("database.log", std::ios::app);
    if (log_file.is_open()) {
        log_file << "SET " << key << " " << value << "\n";
        log_file.close();
    }

    return true;
}

std::optional<std::string> KVStore::get(const std::string& key) const {
    std::shared_lock<std::shared_mutex> lock(rw_mutex);

    auto it = store.find(key); // iterator of the store

    if (it != store.end()) { // store end take us to the value one after the last position.
        return it->second; // returns the value
    }
    return std::nullopt;
}

bool KVStore::del(const std::string& key) {
    std::unique_lock<std::shared_mutex> lock(rw_mutex);
    if (store.erase(key) > 0) {
        std::ofstream log_file("database.log", std::ios::app);
        if (log_file.is_open()) {
            log_file << "DEL " << key << "\n";
            log_file.close();
        }
        return true;
    }
    return false;
}


