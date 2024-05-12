#ifndef CHAT_CLIENT
#define CHAT_CLIENT

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

class Client {
 public:
  ~Client();

  void InitClient();
  void ConnectToServer(const char *_hostname);
  void SendUpdate();
  void ReceiveUpdate();

 private:
  bool is_client_running_{};
  const char *PORT{"2209"};
  const static int max_data_length_ = 1024;
  char name_[1024]{};
  int socket_fd_{};
  struct addrinfo hints_{}, *serv_info_{}, *connection_{};

  ssize_t bytes_{};
  char string_[max_data_length_]{};
  int rv_{};

  void SendMessage(char cmd, const std::string &message);
  void ReceiveMessage(const std::string &message);
  void CloseConnection();
};

#endif //CHAT_CLIENT
