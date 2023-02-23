#ifndef INCLUDED_DEBUG_LOGGER_H
#define INCLUDED_DEBUG_LOGGER_H

#ifndef DEBUG_FILE
#define DEBUG_STREAM(x)
#else

#include <fstream>

#define DEBUG_LOGGER
extern std::ofstream debug_logger;
#define DEBUG_STREAM(x) debug_logger << x

#endif

#endif