#include "fifo.h"

#include <iostream>

void Fifo::reset() {
    mtx.lock();
    this->in = 0;
    this->out = 0;
    mtx.unlock();
}

unsigned int Fifo::length() {
    mtx.lock();
    unsigned int ret = this->in - this->out;
    mtx.unlock();
    return ret;
}

unsigned int Fifo::put(unsigned char *buffer, unsigned int length) {
    mtx.lock();
    unsigned int toPutTotal = std::min(length, this->size - this->in + this->out);
    unsigned int toPutRight = std::min(toPutTotal, this->size - (this->in & (this->size - 1)));
    memcpy(this->buffer + (this->in & (this->size - 1)), buffer, toPutRight);
    memcpy(this->buffer, buffer + toPutRight, toPutTotal - toPutRight);
    this->in += toPutTotal;
    mtx.unlock();

    return toPutTotal;
}

unsigned int Fifo::get(unsigned char *buffer, unsigned int length) {
    mtx.lock();
    unsigned int toGetTotal = std::min(length, this->in - this->out);
    unsigned int toGetRight = std::min(toGetTotal, this->size - (this->out & (this->size - 1)));
    memcpy(buffer, this->buffer + this->out, toGetRight);
    memcpy(buffer + toGetRight, this->buffer, toGetTotal - toGetRight);
    this->out += toGetTotal;
    if (this->in == this->out)
        this->in = this->out = 0;
    mtx.unlock();

    return toGetTotal;
}