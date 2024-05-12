#include "Server.h"
#include "Client.h"
#include <thread>
#include <iostream>

int main() {
  std::cout << "1) Start a new chat server.\n2) Connect to an existing chat server.\n";
  int choice;
  std::cin >> choice;
  switch (choice) {
    case 1: {
      Server server;
      server.CreateServer();

      auto read_thread = std::thread(&Server::ReceiveUpdate, &server);
      auto write_thread = std::thread(&Server::SendUpdate, &server);

      read_thread.join();
      write_thread.join();
      break;
    }
    case 2: {
      Client client;

      client.InitClient();
      auto read_thread = std::thread(&Client::ReceiveUpdate, &client);
      auto write_thread = std::thread(&Client::SendUpdate, &client);

      read_thread.join();
      write_thread.join();
      break;
    }
    default: {
      std::cout << "Incorrect value!";
      break;
    }
  }
  return 0;
}
