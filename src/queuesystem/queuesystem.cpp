/*
    basic implementation of queueing system using multiple thread as producer and consumer
    this is just basic implementation using shared resources among multiple thread (used container is queue, with basic log on overflow and underflow.. with queue size defined..)
    for avoiding race condition mutex (with guard_lock, i.e auto release/unlock mutex after object destruction) is used..
    few configurable variables are given such as no. of producer/consumers, sleep time (also generator), etc
*/ 


#include <iostream>

#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <chrono>

#define RAND_MAX_NUM 500
#define QUEUE_MAX_SIZE 5 // how much can the shared resources can acoomodate..
#define PRODUCER_COUNT 2 // no of thread count that will produce resources..
#define CONSUMER_COUNT 5 // no of thread count that will consume the resources

#define SLEEP_TIME_IN_MILI // comment if required in seconds,

static uint getRandNum()
{
    // can change to some sophesticated random number generator.., for now simple..
    return rand() % RAND_MAX_NUM;
}

#ifndef SLEEP_TIME_IN_MILI
// returns random sleep time in seconds
static uint getRandSleepTimeSec()
{
    return (rand() % 4 + 1); // ensures that it is not zero (1-4)
}
#else
// returns random sleep time in miliseconds
static uint getRandSleepTimeMiliSec()
{
    return (rand() % RAND_MAX_NUM + 1); // ensures that it is not zero (1 to RAND_MAX_NUM+1)
}
#endif
// #note: for fine grain control over sleep time, override with custom random number impl..


int main()
{
    std::vector<std::thread> threads(CONSUMER_COUNT+PRODUCER_COUNT);
    std::queue<uint> sharedRes; // it doesn't inheritly have max size so custom defining.., also it can be complex object, for now just basic uint number..
    
    std::mutex resourceLock;

    srand(time(0));

    // producer
    for (uint i=0;i<PRODUCER_COUNT;++i)
    {
        // emplace is prefered than push_back, due to direct in memory placement in container (here, that is thread) unlike push_back which copies later to move into container
        threads.emplace_back([&](int id) {
            while(true) 
            {
                uint randNum = getRandNum();
                
                // scope limited for lock/guard for mutex for shared resource..
                {
                    std::lock_guard<std::mutex> resGuard(resourceLock);
                    std::cout << "Producer: thread - " << id  << " || " << std::this_thread::get_id() << std::endl;
                    if (sharedRes.size() >= QUEUE_MAX_SIZE)
                    {
                        std::cout << "Queue max size hit.. overflowing.." << std::endl;
                        // continue; // continue or we can wait for few seconds before continuing.. 
                    }
                    else 
                    {
                        std::cout << "Resource pushed is: " << randNum << std::endl;
                        sharedRes.push(randNum);
                    }
                }
                
                #ifdef SLEEP_TIME_IN_MILI
                uint randSleep = getRandSleepTimeMiliSec();
                std::this_thread::sleep_for(std::chrono::milliseconds(randSleep));
                #else
                uint randSleep = getRandSleepTimeSec();
                std::this_thread::sleep_for(std::chrono::seconds(randSleep));
                #endif
            }
        }, i);
    }

    for (uint i=0;i<CONSUMER_COUNT;++i)
    {
        threads.emplace_back([&](int id) {
            while (true)
            {
                {
                    std::lock_guard<std::mutex> resGuard(resourceLock);
                    std::cout << "Consumer: thread - " << id << " || " << std::this_thread::get_id() << std::endl;
                    if (sharedRes.size() == 0) 
                    {
                        std::cout << "## No resource to consume " << std::endl; // so will sleep for random time before consuming
                    }
                    else
                    {
                        std::cout << "Resource consumed is: " << sharedRes.front() << std::endl;
                        sharedRes.pop();
                    }
                }
                    
                #ifdef SLEEP_TIME_IN_MILI
                uint randSleep = getRandSleepTimeMiliSec();
                std::this_thread::sleep_for(std::chrono::milliseconds(randSleep));
                #else
                uint randSleep = getRandSleepTimeSec();
                std::this_thread::sleep_for(std::chrono::seconds(randSleep));
                #endif
            }
        }, i);
    }


    for (auto& t: threads)
    {
        if (t.joinable())
        {
            t.join();
        }
    }

    return 0;
}