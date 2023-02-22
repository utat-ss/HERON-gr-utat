#ifndef DEBUG_LOGGER_H
#define DEBUG_LOGGER_H

#ifndef DEBUG_FILE
#define DEBUG_STREAM(x)
#else

#include <fstream>

extern std::ofstream debug_logger;

#define DEBUG_LOGGER
#define DEBUG_STREAM(x) debug_logger << x

#endif

#endif