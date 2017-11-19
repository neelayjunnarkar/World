#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <emscripten.h>
#include <string>

void log(const char *msg);
void log(const std::string &msg);

#endif