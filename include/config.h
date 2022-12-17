#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DEBUG
#define VERBOSE

#ifdef DEBUG
// always be verbose when in debug mode
#define VERBOSE
#endif

#define __LOG(s, l, m) fprintf(s, l ": %s\n", m)
#define LOG_ERROR(m) __LOG(stderr, "ERROR", m)

#ifdef VERBOSE
#define LOG_INFO(m) __LOG(stdout, "INFO", m)
#else
#define LOG_INFO(m)
#endif

#ifdef DEBUG
#define LOG_DEBUG(m) __LOG(stdout, "DEBUG", m)
#else
#define LOG_DEBUG(m)
#endif



// file config
#define DATA_DIR "data/"
#define DATA_TOKEN_FILE DATA_DIR "tokens.txt"

int check_file_integrity();


