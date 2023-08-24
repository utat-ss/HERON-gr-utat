#include "debug_logger.h"

#ifdef DEBUG_LOGGER

std::ofstream debug_logger(DEBUG_FILE, std::ios_base::out);

#endif