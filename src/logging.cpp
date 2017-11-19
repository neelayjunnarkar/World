#include "logging.hpp"

void log(const char *msg) {
    EM_ASM({console.log(UTF8ToString($0))}, msg);
}

void log(const std::string &msg) {
    log(msg.c_str());
}