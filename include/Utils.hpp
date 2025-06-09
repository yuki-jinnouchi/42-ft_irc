#pragma once
#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <string>
#include <vector>

class Utils {
 public:
  static std::vector<std::string> split(const std::string& str,
                                        const std::string& delimiter);
  static bool endsWith(const std::string& str, const std::string& suffix);
  static std::string toUpper(const std::string& str);

 private:
  Utils();
  ~Utils();
  Utils(const Utils& other);
  Utils& operator=(const Utils& other);
};

#endif  // __UTILS_HPP__
