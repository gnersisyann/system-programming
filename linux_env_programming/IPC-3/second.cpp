#include <iostream>

#include "shared_array.h"

int main()
{
    shared_array array("/test", 5);

    while (true)
    {
        array.lock();
        for (int i = 0; i < 5; ++i)
        {
            std::cout << "Second Process: Reading " << array[i] << " from index " << i << std::endl;
        }
        array.unlock();
        sleep(1);
    }

    return 0;
}
