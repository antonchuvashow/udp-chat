#include "Client.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <fcntl.h>
#include "utils.h"

Client::~Client() {
  CloseConnection();
}

void Client::InitClient() {
  char hostname[max_data_length_];
  //Get hostname
  printf("Specify the IP you want to connect to: ");
  std::cin >> hostname;
  ConnectToServer(hostname);
}

void Client::ConnectToServer(const char *_hostname) {
  //Connect to hostname
  char s[46];

  memset(&hints_, 0, sizeof hints_);
  hints_.ai_family = AF_INET;
  hints_.ai_socktype = SOCK_DGRAM;

  if ((rv_ = getaddrinfo(_hostname, PORT, &hints_, &serv_info_)) != 0) {
    std::cerr << "getaddrinfo: " << gai_strerror(rv_) << "\n";
    return;
  }

  for (connection_ = serv_info_; connection_ != nullptr; connection_ = connection_->ai_next) {
    if ((socket_fd_ = socket(connection_->ai_family, connection_->ai_socktype, connection_->ai_protocol)) == -1) {
      perror("client: socket");
      continue;
    }
    fcntl(socket_fd_, F_SETFL, O_NONBLOCK);
    break;
  }

  if (connection_ == nullptr) {
    std::cerr << "client: failed to bind socket\n";
    return;
  }

  inet_ntop(connection_->ai_family,
            &reinterpret_cast<struct sockaddr_in *>(connection_->ai_addr)->sin_addr,
            s,
            sizeof s);
  std::cout << "client: connecting to " << s << "\n";

  std::cout << "Welcome to the Chat. What is your name?\n";
  std::cin >> name_;

  std::string joinMessage = std::string("0 ") + name_ + " joined.";
  if ((bytes_ =
           sendto(socket_fd_, utils::encrypt(joinMessage, 16).c_str(), joinMessage.length(), 0, connection_->ai_addr,
                  connection_->ai_addrlen)) == -1) {
    perror("talker: sendto");
    exit(1);
  }

  is_client_running_ = true;

  std::cout << "Nice to meet you " << name_ << "!\n";
}

void Client::SendUpdate() {
  char message[max_data_length_];
  while (is_client_running_) {
    std::cin.getline(message, max_data_length_);
    if (strcmp(message, "/leave") == 0) {
      std::string leaveMessage = std::string(name_) + " left.";
      SendMessage('2', leaveMessage);
      std::cout << "Left the server\n";

      is_client_running_ = false;
      CloseConnection();
      return;
    } else if (strlen(message) > 0) {
      SendMessage('1', std::string(message));
    }
  }
}

void Client::ReceiveUpdate() {
  while (is_client_running_) {
    bytes_ = recvfrom(socket_fd_, string_, max_data_length_ - 1, 0, connection_->ai_addr, &connection_->ai_addrlen);
    if (bytes_ != -1) {
      ReceiveMessage(std::string(string_, bytes_));
    }
  }
}

void Client::SendMessage(char cmd, const std::string &message) {
  //encrypt then send to server
  std::string fullMessage = utils::encrypt(std::string(1, cmd) + ":" + name_ + ":" + message, 16);
  if ((bytes_ = sendto(socket_fd_,
                       fullMessage.c_str(),
                       fullMessage.length(),
                       0,
                       connection_->ai_addr,
                       connection_->ai_addrlen)) == -1) {
    perror("talker: sendto");
    exit(1);
  }
}

void Client::ReceiveMessage(const std::string &message) {
// Split the incoming message by ':'
  std::istringstream iss(message);
  std::vector<std::string> linesSplit;
  std::string line;
  while (std::getline(iss, line, ':')) {
    linesSplit.push_back(line);
  }

// Process the split parts
  if (linesSplit.size() >= 3) {
    if (linesSplit[1] == "4") {
      // Client exit
      CloseConnection();
    }

    // Print message if it has enough parts
    std::cout << "(" << linesSplit[1] << "): " << linesSplit[2] << '\n';
  }
}

void Client::CloseConnection() {
  close(socket_fd_);
  is_client_running_ = false;
}
