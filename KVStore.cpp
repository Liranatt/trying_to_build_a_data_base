//
// Created by Ksenia on 10/03/2026.
//

#include "KVStore.h"

KVStore::KVStore(){}

KVStore::~KVStore() {}

bool KVStore::set(const std::string& key, const std::string& value) {
    std::unique_lock<std::shared_mutex> lock(rw_mutex);
    store[key] = value;
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
    return store.erase(key) > 0;
}

