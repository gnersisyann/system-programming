
#include <sys/mman.h>

#include "error_handler.h"

int main()
{
    if (shm_unlink("/shared_array") < 0) error_handler("shm_unlink error");
}