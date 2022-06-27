#ifndef _COMMON_H
#define _COMMON_H

#define DBG_MACRO_NO_WARNING
#include "dbg.h"
#include <cstdlib>
#include <string.h>

#ifndef DBG_MACRO_DISABLE
#define ERR_EXIT(...)                                                          \
  do {                                                                         \
    dbg(__VA_ARGS__);                                                          \
    exit(1);                                                                   \
  } while (false)
#else
// ATTENTION: ERR_EXIT here only support one parameter!!
#define ERR_EXIT(_str_)                                                        \
  do {                                                                         \
    perror(_str_);                                                             \
    exit(1);                                                                   \
  } while (false)
#endif

#define UNREACHABLE() ERR_EXIT("control flow should never reach here")
#define UNIMPLEMENTED() ERR_EXIT("unimplemented here")

template <typename T> inline void FREE_CONTAINER(T &p_container) {
  T empty;
  using std::swap;
  swap(p_container, empty);
}

template <class ForwardIt, class T>
ForwardIt remove(ForwardIt first, ForwardIt last, const T &value) {
  first = std::find(first, last, value);
  if (first != last)
    for (ForwardIt i = first; ++i != last;)
      if (!(*i == value))
        *first++ = std::move(*i);
  return first;
}

bool endWith(const std::string &str, const std::string &tail);

bool startWith(const std::string &str, const std::string &head);

void split(const std::string &str, const std::string &delim,
           std::vector<std::string> &res);
#endif
