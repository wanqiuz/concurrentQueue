#ifndef FIFO_H
#define FIFO_H

#include <assert.h>
#include <mutex>
#include <iostream>
#include <queue>

template<class T>
class Fifo {
public:
    Fifo();

    T front();
    T back();
    void pop();
    bool tryPop();
    void push(const T &value);
    void push(T &&value);
    template<class... Args>
    void emplace(Args&&... args);

    size_t size() const;
    bool empty() const;

private:
    std::queue<T> queue;

    std::mutex mtx;
    std::condition_variable notEmpty;
    std::condition_variable notFull;
};

template<class T>
Fifo<T>::Fifo(): queue() {
}

template<class T>
T Fifo<T>::front() {
    std::unique_lock<std::mutex> lock(mtx);
    notEmpty.wait(lock, [this]() {return !this->queue.empty();});
    return queue.front();
}

template<class T>
T Fifo<T>::back() {
    std::unique_lock<std::mutex> lock(mtx);
    notEmpty.wait(lock, [this]() {return !this->queue.empty();});
    return queue.back();
}

template<class T>
void Fifo<T>::pop() {
    std::unique_lock<std::mutex> lock(mtx);
    notEmpty.wait(lock, [this]() {return !this->queue.empty();});
    queue.pop();
}

template<class T>
bool Fifo<T>::tryPop() {
    std::unique_lock<std::mutex> lock(mtx);
    if (this->queue.empty())
        return false;
    queue.pop();
    return true;
}

template<class T>
void Fifo<T>::push(const T &value) {
    std::unique_lock<std::mutex> lock(mtx);
    queue.push(value);
    notEmpty.notify_one();
}

template<class T>
void Fifo<T>::push(T &&value) {
    std::unique_lock<std::mutex> lock(mtx);
    queue.push(std::forward<T>(value));
    notEmpty.notify_one();
}

template<class T>
template<class... Args>
void Fifo<T>::emplace(Args&&... args) {
    std::unique_lock<std::mutex> lock(mtx);
    queue.emplace(std::forward<Args>(args) ...);
    notEmpty.notify_one();
}


template<class T>
size_t Fifo<T>::size() const {
    std::unique_lock<std::mutex> lock(mtx);
    return queue.size();
}

template<class T>
bool Fifo<T>::empty() const {
    std::unique_lock<std::mutex> lock(mtx);
    return queue.empty();
}




#endif