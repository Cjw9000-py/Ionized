
#include <errno.h>
#include "config.h"


int check_file_integrity() {
    const int max_size = 1024;
    char cwd[max_size];

    if (getcwd((char*)&cwd, 256)) {
        if (errno == ERANGE) {
            LOG_ERROR("could not check for file integrity because the cwd is longer than 256 bytes");
            return 1;
        }
    }

    // MACRO
#define CHECK_FILE(p, n) \
    char n[max_size]; \
    snprintf(n, max_size, "%s/%s", cwd, p); \
    if (access((const char*)&n, F_OK)) { \
        LOG_ERROR("INTEGRITY: could not find '" p "'"); \
    }
    // MACRO END

    CHECK_FILE(DATA_DIR, data_dir);
    CHECK_FILE(DATA_TOKEN_FILE, token_file);

    return 0;
}
