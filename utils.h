#ifndef CIPHER
#define CIPHER

#include <string>
#include <netinet/in.h>

namespace utils {
std::string encrypt(const std::string &input_str, int _offset);
std::string decrypt(const std::string &_inputStr, int _offset);
char bound(char _charToCheck, char _left, char _right);
std::string transform(const std::string &input_str, int offset);
}

#endif // CIPHER