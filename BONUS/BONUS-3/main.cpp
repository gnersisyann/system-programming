#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>

#include <iostream>

pthread_mutex_t mutex;

class BankAccount
{
   private:
    int balance;

   public:
    BankAccount() : balance(0) {}

    void debit(int amount) { balance -= amount; }

    void credit(int amount) { balance += amount; }

    int getBalance() { return balance; }
};

void *do_job(void *arg)
{
    BankAccount *acc = (BankAccount *)arg;
    int amount;
    char operation;
    pthread_mutex_lock(&mutex);
    std::cin >> operation >> amount;
    switch (operation)
    {
        case 'c':
            acc->credit(amount);
            break;
        case 'd':
            if (amount > acc->getBalance())
            {
                std::cout << "Not enough money" << std::endl;
                break;
            }
            acc->debit(amount);
            break;
    }
    pthread_mutex_unlock(&mutex);
    return nullptr;
}

int main()
{
    pthread_mutex_init(&mutex, nullptr);
    BankAccount account;
    int N;
    std::cin >> N;
    pthread_t *tids = new pthread_t[N];
    for (int i = 0; i < N; ++i)
    {
        pthread_create(&tids[i], nullptr, do_job, &account);
    }
    for (int i = 0; i < N; ++i)
    {
        pthread_join(tids[i], nullptr);
    }
    pthread_mutex_destroy(&mutex);
    std::cout << "Final balance is " << account.getBalance() << std::endl;
}