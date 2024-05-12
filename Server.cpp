#include "Server.h"
#include "utils.h"
#include <iostream>

Server::Server() {
  socket_fd_ = 0;
  is_server_running_ = true;
  memset(&hints_, 0, sizeof hints_);
  hints_.ai_family = AF_INET; // Use IPv4
  hints_.ai_socktype = SOCK_DGRAM;
  hints_.ai_flags = AI_PASSIVE;
}

Server::~Server() {
  CloseServer();
}

void Server::CreateServer() {
  // Get address info
  if ((returned_value_ = getaddrinfo(nullptr, PORT, &hints_, &addrinfo_)) != 0) {
    std::cerr << gai_strerror(returned_value_) << '\n';
    exit(1);
  }

  // Iterate over the address structures
  for (auto info = addrinfo_; info != nullptr; info = info->ai_next) {
    // Create socket
    socket_fd_ = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
    if (socket_fd_ == -1) {
      perror("socket");
      continue;
    }

    // Bind socket
    if (bind(socket_fd_, info->ai_addr, info->ai_addrlen) == -1) {
      close(socket_fd_);
      perror("bind");
      continue;
    }

    // Print IP address
    char ip_str[16];
    inet_ntop(info->ai_family, &(reinterpret_cast<struct sockaddr_in *>(info->ai_addr)->sin_addr),
              ip_str, sizeof ip_str);
    std::cout << "Server is now running on IP address: " << ip_str << '\n';

    break; // Binding successful, exit loop
  }

  if (addrinfo_ == nullptr) {
    std::cerr << "server: failed to bind" << '\n';
    exit(2);
  }
}

void Server::ReceiveUpdate() {
  while (is_server_running_) {
    if ((bytes_ = recvfrom(socket_fd_,
                           string_,
                           max_data_length_ - 1,
                           0,
                           reinterpret_cast<sockaddr *>(&client_address_),
                           &client_address_len_)) == -1) {
      return;
    }
    if (bytes_ > 0) {
      HandleMessage();
    }
  }
}

void Server::SendUpdate() {
  char message[max_data_length_]{};
  while (is_server_running_) {
    std::cin.getline(message, max_data_length_);
    if (strcmp(message, "/stop") == 0) {

      //Send end message to all users
      BroadcastMessage(std::string("4:Server:Server has been shut down!"), sockaddr_in());

      //Shut down the server
      CloseServer();
      return;
    } else {
      BroadcastMessage(std::string("1:Server:") + message, client_address_);
    }
  }
}

void Server::HandleMessage() {
  // Handle data from a client
  std::string decrypted_str = utils::decrypt(std::string(string_), 16);
  char code = decrypted_str[0];
  if (code == '0') { // Client has logged in
    std::cout << "Client joined.\n";
    clients_vec_.push_back(client_address_);
    std::cout << "Clients pool size: " << clients_vec_.size() << "\n";
  } else if (code == '1') { // Client sent a message

    decrypted_str = decrypted_str.substr(0, bytes_);
    std::cout << "(" << inet_ntop(client_address_.sin_family,
                                              &client_address_.sin_addr,
                                              remote_ip_,
                                              sizeof remote_ip_) << "): " << decrypted_str << std::endl;

    BroadcastMessage(decrypted_str, client_address_);

  } else if (code == '2') {
    std::cout << "Client has left.\n";
    auto it = std::find_if(clients_vec_.begin(), clients_vec_.end(), [&](auto &item) {
      return CompareClients(&item, &client_address_);
    });

    if (it != clients_vec_.end()) {
      std::cout << "Deleting from the pool...\n";
      clients_vec_.erase(it);
    }

    std::cout << "Clients pool size: " << clients_vec_.size() << '\n';
  }
}

void Server::SendMessage(const std::string &message, struct sockaddr *dest) const {
  sendto(socket_fd_, message.c_str(), message.length(), 0, dest, client_address_len_);
}

void Server::BroadcastMessage(const std::string &message, sockaddr_in sender) {
  for (auto &client : clients_vec_)
    if (sender.sin_port != client.sin_port || sender.sin_addr.s_addr != client.sin_addr.s_addr) {
      SendMessage(message, reinterpret_cast<sockaddr *>(&client));
    }
}

bool Server::CompareClients(sockaddr_in *first_client, sockaddr_in *second_client) {
  return (inet_ntop(client_address_.sin_family, &reinterpret_cast<struct sockaddr_in *>(first_client)->sin_addr,
                    remote_ip_, sizeof remote_ip_)
      == inet_ntop(client_address_.sin_family, &reinterpret_cast<struct sockaddr_in *>(second_client)->sin_addr,
                   remote_ip_, sizeof remote_ip_));
}

void Server::CloseServer() {
  close(socket_fd_);
  is_server_running_ = false;
}
