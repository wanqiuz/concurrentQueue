#include <iostream>
#include <sstream>
#include <thread>
#include <future>
#include "fifo.h"


std::string getThreadId() {
    std::stringstream oss;
    oss << std::this_thread::get_id();
    // unsigned long long tid = std::stoull(stid, 0, 16);
    return oss.str();
}

void put(Fifo &fifo, int &counts) {
    counts = 222;
    unsigned char *buffer = new unsigned char[counts];
    for (int i = 0; i < counts; ++i) {
        buffer[i] = (unsigned char)i;
    }
    counts = fifo.put(buffer, counts);
    delete []buffer;
    //std::cout << getThreadId() + " send: " + std::to_string(counts) + "\n";
}

void get(Fifo &fifo, int &counts) {
    unsigned char *buffer = new unsigned char[1];
    // while (fifo.length() != 0) {
    while (1) {
        if (fifo.get(buffer, 1) == 0)
            continue;

        counts++;

        // std::cout << getThreadId() + ": " + std::to_string((int)(buffer[0])) + "\n";
        //std::cout << getThreadId() + " recieved: " + std::to_string(counts) + "\n";

    }
    delete []buffer;
}


bool testMessageCounts() {
    Fifo fifo(128);

    // std::promise<unsigned int> promiseObj1;
    // std::future<unsigned int> futureObj1 = promiseObj1.get_future();
    // std::thread thread1(put, std::ref(fifo), std::ref(promiseObj1));

    // std::promise<unsigned int> promiseObj2;
    // std::future<unsigned int> futureObj2 = promiseObj2.get_future();
    // std::thread thread2(get, std::ref(fifo), std::ref(promiseObj2));

    // std::promise<unsigned int> promiseObj3;
    // std::future<unsigned int> futureObj3 = promiseObj3.get_future();
    // std::thread thread3(get, std::ref(fifo), std::ref(promiseObj3));

    int counts1 = 222;
    std::thread thread1(put, std::ref(fifo), std::ref(counts1));

    int counts2 = 0;
    std::thread thread2(get, std::ref(fifo), std::ref(counts2));

    int counts3 = 0;
    std::thread thread3(get, std::ref(fifo), std::ref(counts3));
    std::this_thread::sleep_for(std::chrono::seconds(3));

    // // std::cout << futureObj1.get() << std::endl;
    // // std::cout << futureObj2.get() << std::endl;
    // // std::cout << futureObj3.get() << std::endl;
    std::cout << counts1 << std::endl;
    std::cout << counts2 << std::endl;
    std::cout << counts3 << std::endl;
    // return true;
    return (counts1 == counts2 + counts3);

}

int main(int argc, char *argv[]) {
    testMessageCounts();
    return 0;
}







