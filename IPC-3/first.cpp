#include <iostream>

#include "shared_array.h"

int main()
{
    shared_array array("/test", 10);
    array.lock();
    for (int i = 0; i < 10; ++i)
    {
        array[i] = i;
    }
    array.unlock();

    while (true)
    {
        array.lock();
        for (int i = 0; i < 10; ++i)
        {
            array[i] = i * 2;
            std::cout << "First Process: Writing " << i * 2 << " at index " << i << std::endl;
        }
        array.unlock();
        sleep(1);
    }
    return 0;
}
