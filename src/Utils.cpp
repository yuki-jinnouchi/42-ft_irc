#include "Utils.hpp"
#include <iostream>

std::vector<std::string> Utils::split(const std::string& str,
                                      const std::string& delimiter) {
  std::vector<std::string> ret;
  size_t pos = 0;
  size_t delimiter_pos = str.find(delimiter, pos);
  while (delimiter_pos != std::string::npos) {
    if (delimiter_pos - pos > 0) {
      std::cout << str.substr(pos, delimiter_pos - pos) << std::endl;
      ret.push_back(str.substr(pos, delimiter_pos - pos));
    }
    pos = delimiter_pos + delimiter.length();
    delimiter_pos = str.find(delimiter, pos);
  }
  if (pos - str.length() > 0) {
    ret.push_back(str.substr(pos));
  }
  return ret;
}

bool Utils::endsWith(const std::string& str, const std::string& suffix) {
  if (str.length() < suffix.length()) {
    return false;
  }
  return str.compare(str.length() - suffix.length(), suffix.length(), suffix) ==
         0;
}
