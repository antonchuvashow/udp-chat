# UDP Chat Program

This is a simple UDP-based chat program implemented in C++. It allows users to either start a new chat server or connect to an existing chat server and communicate with each other via text messages.

## Features

- **Server Mode:** Start a new chat server and wait for client connections.
- **Client Mode:** Connect to an existing chat server to send and receive messages.
- **Multithreaded:** Utilizes multi-threading for simultaneous reading and writing operations.

## Prerequisites

- C++ compiler (Supporting C++17)
- CMake (2.8 or higher)
- POSIX-compliant operating system (Linux, macOS, etc.)

## Usage

1. Clone the repository to your local machine:

    ```bash
    git clone https://github.com/antonchuvashow/udp-chat.git
    ```

2. Navigate to the project directory:

    ```bash
    cd udp-chat
    ```

3. Create a build directory:

    ```bash
    mkdir build
    cd build
    ```

4. Generate build files using CMake:

    ```bash
    cmake ..
    ```

5. Build the project:

    ```bash
    make
    ```

6. Run the executable:

    ```bash
    ./chat
    ```

7. Follow the on-screen instructions and chat with other users.
