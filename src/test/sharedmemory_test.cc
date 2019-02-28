/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---XXX---
*
****************************************
*
*/

#include <tiny_base/semaphore.h>
#include <tiny_base/sharedmemory.h>
#include <tiny_core/time.h>

#include <iostream>
#include <string.h>

using namespace std;

typedef struct
{
    SharedMemory* memory;
    Semaphore* sem;
} sync_t;

sync_t test1;

void consumer()
{
    int* data;
    std::string str;
    for (int i = 0; i < 1; i++) {
        test1.sem->lock();
        data = (int*)test1.memory->getSpace();
        std::cout << "consumer:" << *data << std::endl;
        test1.sem->unLock();
    }
}

void productor()
{
    int* data;
    std::string str;
    for (int i = 0; i < 200000; i++) {
        test1.sem->lock();
        data = (int*)test1.memory->getSpace();
        *data = *data + 1;
        std::cout << "[" << getpid() << "]write:" << *data << std::endl;
        test1.sem->unLock();
    }
}

#include <unistd.h>

int main()
{

    SharedMemory* sem_memory = new SharedMemory(sizeof(sem_t));
    test1.sem = new Semaphore(sem_memory, 1);
    test1.memory = new SharedMemory(sizeof(int));

    int* data = (int*)test1.memory->getSpace();
    *data = 0;

    pid_t pid;

    pid = fork();
    if (pid < 0) {
        std::cout << "fork error" << std::endl;
        return -1;
    } else if (pid == 0) {
        sleep(15);
        consumer();
    } else {

        pid = fork();
        if (pid < 0) {
            std::cout << "fork2 error" << std::endl;
            return -1;
        } else if (pid == 0) {
            productor();
        } else {
            productor();
        }
    }
    std::cout << "exit" << std::endl;

    return 0;
}
