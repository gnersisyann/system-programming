#include <iostream>

#include "shared_array.h"

int main()
{
    shared_array array("/test", 5);
    array.lock();
    for (int i = 0; i < 5; ++i)
    {
        array[i] = i + 1;
    }
    array.unlock();

    while (true)
    {
        array.lock();
        for (int i = 0; i < 5; ++i)
        {
            array[i] = array[i] * 2;
            std::cout << "First Process: Writing " << array[i] * 2 << " at index " << i << std::endl;
        }
        array.unlock();
        sleep(1);
    }
    return 0;
}
