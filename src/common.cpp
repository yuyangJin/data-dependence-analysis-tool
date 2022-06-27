#include "common.h"
bool endWith(const std::string &str, const std::string &tail) {
  return str.compare(str.size() - tail.size(), tail.size(), tail) == 0;
}

bool startWith(const std::string &str, const std::string &head) {
  return str.compare(0, head.size(), head) == 0;
}

void split(const std::string &str, const std::string &delim,
           std::vector<std::string> &res) {
  if ("" == str)
    return;
  // convert str from string to char*
  char *strs = new char[str.length() + 1];
  strcpy(strs, str.c_str());

  char *d = new char[delim.length() + 1];
  strcpy(d, delim.c_str());

  char *p = strtok(strs, d);
  while (p) {
    std::string s = p; // convert splited p from char* to string
    res.push_back(s);  // store in res(result)
    p = strtok(NULL, d);
  }

  return;
}

void split(const std::string &str, const char *delim,
           std::vector<std::string> &res) {
  split(str, std::string(delim), res);
}

// void removeChar(std::string &str, char* ) {

// }