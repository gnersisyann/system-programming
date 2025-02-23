#include <unistd.h>

#include <iostream>

#include "parallel_scheduler.h"

int main()
{
    parallel_scheduler scheduler(3);

    scheduler.run([]() { std::cout << "Another task!" << std::endl; });

    for (int i = 0; i < 15; ++i)
    {
        scheduler.run(
            []()
            {
                sleep(1);
                std::cout << "Task in loop!" << std::endl;
            });
    }

    return 0;
}
