#include <algorithm>
#include <iostream>
#include <vector>
#include <thread>
#include <string>
#include "KVStore.h"

void writer_task(KVStore& db, int thread_id) { // a function that would run inside about a thread of a writer
    std::string key = "user_" + std::to_string(thread_id);
    std::string value = "data_" + std::to_string(thread_id);
    db.set(key, value);
    // TODO - put the key and the value inside the db
}

void reader_task(KVStore& db, int thread_id) { // a function that would run inside about a thread of a writer
    std::string key = "user_" + std::to_string(thread_id);
    auto result = db.get(key);
    if (!result.has_value()) {
        std::cout << "Error: Key " << key << " not found." << std::endl;
    }

    // TODO - try to read the value from the database, its ok of it isnt in there yet because maybe
    // the writer hasnt finished writing
}

int main() {
    std::cout << "---- starting stress test ---" << std::endl;
    KVStore db;

    const int NUM_THREADS = 100;

    // a dynamic array (vector) - which will hold all the threads we created
    std::vector<std::thread> writers;
    std::vector<std::thread> readers;

    std::cout << "spawning " << NUM_THREADS << "writers and " << NUM_THREADS << "readers..." << std::endl;

    // TODO - a loop that runs twice on NUM_THREADS
    // in each iteration i need to create new std::thread that runs reader_task
    // and put it write into the writers vector
    // after that create std::thread that runs reader_task and put it into readers vector
    for (int i = 0; i < NUM_THREADS; i++) {
        writers.emplace_back(writer_task, std::ref(db), i);
        readers.emplace_back(reader_task, std::ref(db), i);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        writers[i].join();
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        readers[i].join();
    }
    // synchronization - a loop that goes over all the thread in vector writers
    // and activates join() function on them
    // another loop that does the same to readers

    std::cout << "All threads finished. validating data...." << std::endl;

    // validation - checking that we hadn't lossed any information

    int missing_keys = 0;
    for (int i = 0; i < NUM_THREADS; ++i) {
        std::string expected_key = "user_" + std::to_string(i);
        if (!db.get(expected_key).has_value()) {
            missing_keys++;
            std::cout << "Error: Key " << expected_key << " not found." << std::endl;
        }
    }

    if (missing_keys == 0) {
        std::cout << "Success: All " << NUM_THREADS << "Keys were safely stored!" << std::endl;
    }
    else {
        std::cout << "Failed: " << missing_keys << " keys were lost due to race conditions " << std::endl;
    }
    return 0;
}