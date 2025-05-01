#pragma once
#ifndef __IRC_LOGGER_HPP__
#define __IRC_LOGGER_HPP__

#include <iostream>

#ifdef DEBUG
#define DEBUG_MSG(msg)                          \
  do {                                          \
    std::cerr << "DEBUG: " << msg << std::endl; \
  } while (0)
#else
#define DEBUG_MSG(msg) \
  do {                 \
  } while (0)
#endif

#endif  // __IRC_LOGGER_HPP__
