# Simple In-Memory Key-Value Store

A basic, thread-safe In-Memory Key-Value store written in C++17. This project implements fundamental concurrency control and a raw TCP server for client-server communication. 

It is designed for learning and demonstrating core principles of concurrent programming and basic socket communication.

## Key Features
* **Basic Operations:** Supports `SET`, `GET`, and `DEL` commands.
* **Thread-Safety:** Manages concurrent access using a read-write lock (`std::shared_mutex`), allowing multiple simultaneous readers or a single exclusive writer.
* **Built-in TCP Server:** Listens for incoming connections (default port 8080) and spawns a detached thread to handle each client.
* **Basic Persistence:** Implements an append-only log (`database.log`) to record operations, loading the data back into memory upon server startup.
* **Integrated Stress Test:** The main executable automatically runs a multi-threaded stress test (100 concurrent reader and writer threads) to validate thread-safety and data integrity before initializing the server.

## Requirements
* C++17 compatible compiler.
* CMake 3.10 or higher.
* POSIX-compliant operating system (Linux, macOS) for Socket and Threading APIs.

## Build and Run

Use CMake to build the project:

```bash
mkdir build
cd build
cmake ..
make
```

Run the executable:

```bash
./KVStore
```
Upon execution, the program will run the internal stress test, print the validation results, and then boot up the TCP server on port 8080.

## Usage

You can interact with the server using standard tools like `nc` (Netcat) or `telnet`.

Connect to the server:
```bash
nc localhost 8080
```

Once connected, send plain text commands:

1. **Set a value:**
   ```text
   SET my_key my_value
   ```
   *(Returns `OK`)*

2. **Get a value:**
   ```text
   GET my_key
   ```
   *(Returns the value or `NOT FOUND`)*

3. **Delete a value:**
   ```text
   DEL my_key
   ```
   *(Returns `OK` or `NOT FOUND`)*
