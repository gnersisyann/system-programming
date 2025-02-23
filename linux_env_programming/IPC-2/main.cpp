#include <sys/wait.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <string>
#include <vector>

int findNthPrime(int n)
{
    std::vector<int> primes;
    int num = 2;

    while (primes.size() < n)
    {
        bool is_prime = true;

        for (int prime : primes)
        {
            if (num % prime == 0)
            {
                is_prime = false;
                break;
            }
        }

        if (is_prime)
        {
            primes.push_back(num);
        }

        ++num;
    }

    return primes[n - 1];
}

bool isNumber(const std::string &str)
{
    for (char const &ch : str)
    {
        if (!isdigit(ch))
        {
            return false;
        }
    }
    return true;
}

void PrimeCalculator()
{
    int pipefd1[2];
    int pipefd2[2];
    pid_t cpid;

    if (pipe(pipefd1) < 0 || pipe(pipefd2) < 0)
    {
        std::cout << "pipe() error" << std::endl;
        exit(errno);
    }

    cpid = fork();
    if (cpid < 0)
    {
        std::cout << "fork() error" << std::endl;
        close(pipefd1[0]);
        close(pipefd1[1]);
        close(pipefd2[0]);
        close(pipefd2[1]);
        exit(errno);
    }
    else if (cpid == 0)
    {
        close(pipefd1[1]);
        close(pipefd2[0]);

        char get[20];
        memset(get, 0, sizeof(get));

        read(pipefd1[0], get, sizeof(get));

        if (std::string(get) == "exit")
        {
            close(pipefd1[0]);
            close(pipefd2[1]);
            exit(0);
        }

        if (std::string(get) == "invalid")
        {
            close(pipefd1[0]);
            close(pipefd2[1]);
            exit(0);
        }

        int n = atoi(get);
        std::cout << "[Child] Calculating " << n << "-th prime number..." << std::endl;

        int prime = findNthPrime(n);
        std::cout << "[Child] Sending calculation result of prime(" << n << ")..." << std::endl;

        std::string primenum = std::to_string(prime);
        write(pipefd2[1], primenum.c_str(), primenum.size() + 1);

        close(pipefd1[0]);
        close(pipefd2[1]);
        exit(0);
    }
    else
    {
        close(pipefd1[0]);
        close(pipefd2[1]);

        std::cout << "[Parent] Please enter the number: ";
        std::string input;
        std::cin >> input;
        if (input == "exit")
        {
            write(pipefd1[1], input.c_str(), input.size() + 1);
            close(pipefd1[1]);
            close(pipefd2[0]);
            exit(0);
        }

        if (!isNumber(input))
        {
            std::cout << "invalid input" << std::endl;
            write(pipefd1[1], "invalid", 7);
            close(pipefd1[1]);
            close(pipefd2[0]);
            return;
        }

        std::cout << "[Parent] Sending " << input << " to the child process..." << std::endl;

        write(pipefd1[1], input.c_str(), input.size() + 1);

        std::cout << "[Parent] Waiting for the response from the child process..." << std::endl;

        wait(nullptr);

        char buf[20];
        memset(buf, 0, sizeof(buf));
        read(pipefd2[0], buf, sizeof(buf));
        std::string res(buf);
        std::cout << "[Parent] Received calculation result of prime(" << input << ") = " << res << std::endl;
        close(pipefd1[1]);
        close(pipefd2[0]);
    }
}

int main()
{
    while (true)
    {
        PrimeCalculator();
    }
}