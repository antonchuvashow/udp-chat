#ifndef CHAT_SERVER
#define CHAT_SERVER

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <vector>
#include <string>

class Server {
 public:
  Server();
  ~Server();

  void CreateServer();
  void ReceiveUpdate();
  void SendUpdate();

 private:
  const char *PORT{"2209"};
  int socket_fd_{};
  const static int max_data_length_ = 1024;
  struct sockaddr_in client_address_{}; //client address
  socklen_t client_address_len_ = sizeof(client_address_);
  char string_[max_data_length_]{}; //buffer for client data
  ssize_t bytes_{};
  char remote_ip_[46]{};
  int returned_value_{};
  struct addrinfo hints_{}, *addrinfo_{};
  std::vector<struct sockaddr_in> clients_vec_;
  bool is_server_running_;

  bool CompareClients(struct sockaddr_in *first_client, struct sockaddr_in *second_client);
  void HandleMessage();
  void SendMessage(const std::string &message, struct sockaddr *dest) const;
  void BroadcastMessage(const std::string &message, sockaddr_in sender);
  void CloseServer();
};

#endif //CHAT_SERVER
