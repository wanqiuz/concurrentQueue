#ifndef FIFO_H
#define FIFO_H

// struct kfifo {
//     unsigned char *buffer;
//     unsigned int size;
//     unsigned int in;
//     unsigned int out;
// }
#include <assert.h>
#include <mutex>

class Fifo {
public:
    Fifo(unsigned int size ) {
        assert(isPowerOf2(size));

        buffer = new unsigned char[size];
        this->size = size;
        this->in = 0;
        this->out = 0;
    }

    void reset();
    unsigned int length();
    unsigned int put(unsigned char *buffer, unsigned int length);
    unsigned int get(unsigned char *buffer, unsigned int length);

private:
    static bool isPowerOf2(unsigned long n) {
        return (n != 0 && (n & (n - 1)) == 0);
    }
private:
    unsigned char *buffer;
    unsigned int size;
    unsigned int in;
    unsigned int out;

    std::mutex mtx;
};

#endif