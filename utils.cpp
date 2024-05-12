#include "utils.h"
#include <string>

std::string utils::transform(const std::string &input_str, int offset) {
  std::string transformed_str;

  for (char curr_char : input_str) {
    if (std::isalpha(curr_char)) {
      char base = std::islower(curr_char) ? 'a' : 'A';
      int new_pos = (curr_char - base + offset) % 26;
      if (new_pos < 0)
        new_pos += 26; // Ensure wrapping for negative offsets
      curr_char = base + new_pos;
    }
    transformed_str += curr_char;
  }

  return transformed_str;
}

std::string utils::encrypt(const std::string &input_str, int offset) {
  return transform(input_str, offset);
}

std::string utils::decrypt(const std::string &inputStr, int offset) {
  return transform(inputStr, -offset);
}

