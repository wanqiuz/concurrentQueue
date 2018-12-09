#include <iostream>
#include <sstream>
#include <thread>
#include <future>
#include "fifo.h"




#include <iostream>
#include <memory>
#include <string>
#ifdef GCC
#include <sys/time.h>
#else
#include <ctime>
#endif // GCC

class TimeInterval
{
public:
    TimeInterval(const std::string& d) : detail(d)
    {
        init();
    }

    TimeInterval()
    {
        init();
    }

    ~TimeInterval()
    {
#ifdef GCC
        gettimeofday(&end, NULL);
        std::cout << detail 
            << 1000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000 
            << " ms" << endl;
#else
        end = clock();
        std::cout << detail 
            << (double)(end - start) << " ms" << std::endl;
#endif // GCC
    }

protected:
    void init() {
#ifdef GCC
        gettimeofday(&start, NULL);
#else
        start = clock();
#endif // GCC
    }
private:
    std::string detail;
#ifdef GCC
    timeval start, end;
#else
    clock_t start, end;
#endif // GCC
};

#define TIME_INTERVAL_SCOPE(d)   std::shared_ptr<TimeInterval> time_interval_scope_begin = std::make_shared<TimeInterval>(d)






std::string getThreadId() {
    std::stringstream oss;
    oss << std::this_thread::get_id();
    return oss.str();
}

// template<class T>
// void put(Fifo<T> &fifo, int &counts) {
//     counts = 222;
//     T *buffer = new T[counts];
//     for (int i = 0; i < counts; ++i) {
//         buffer[i] = (T)i;
//     }
//     //fifo.put(buffer, counts);
//     counts = fifo.put(buffer, counts);
//     delete []buffer;
//     //std::cout << getThreadId() + " send: " + std::to_string(counts) + "\n";
// }

// template<class T>
// void get(Fifo<T> &fifo, int &counts) {
//     T *buffer = new T[1];
//     while (1) {
//         if (fifo.get(buffer, 1) == 0)
//             continue;

//         counts++;

//         // std::cout << getThreadId() + ": " + std::to_string((int)(buffer[0])) + "\n";
//         //std::cout << getThreadId() + " recieved: " + std::to_string(counts) + "\n";

//     }
//     delete []buffer;
// }


// bool testMessageCounts() {
//     Fifo<unsigned char> fifo(128);

//     int counts1 = 222;
//     std::thread thread1(put<unsigned char>, std::ref(fifo), std::ref(counts1));

//     int counts2 = 0;
//     std::thread thread2(get<unsigned char>, std::ref(fifo), std::ref(counts2));

//     int counts3 = 0;
//     std::thread thread3(get<unsigned char>, std::ref(fifo), std::ref(counts3));
//     std::this_thread::sleep_for(std::chrono::seconds(3));

//     std::cout << counts1 << std::endl;
//     std::cout << counts2 << std::endl;
//     std::cout << counts3 << std::endl;

//     return (counts1 == counts2 + counts3);

//     return true;

// }

class Message {
public:
    Message(std::string string) {
        data.push_back(string);
        data.push_back("b");
        data.push_back("c");
    }

    std::vector<std::string> data;
};

// void __testMessageCounts(Fifo<Message> &fifo) {
//     Message *message = new Message;
//     fifo.push(message);
//     message.data[0] = "d";
//     message.data.push_back("e");
//     std::cout << &message << std::endl;
    
// }
// // void __testMessageCounts(Fifo<Message> &fifo) {
// //     //Message message;
// //     fifo.push(Message());
// //     //message.data[0] = "d";
// //     //message.data.push_back("e");
    
// // }


// bool testMessageCounts() {
//     Fifo<Message> fifo;
//     __testMessageCounts(fifo);
//     Message message = fifo.front();
//     std::cout << &fifo << std::endl;
//     std::cout << message.data.size() << std::endl;
//     return true;

// }


// int main(int argc, char *argv[]) {
//     //testMessageCounts();
//     return 0;
// }



#include <vector>
#include <string>

int main() {

    Fifo<Message> fifo;
    fifo.emplace("ji");
    std::cout << fifo.front().data[0] << std::endl;

    std::vector<std::string> v;
    int count = 100000;
    v.reserve(count);       //预分配十万大小，排除掉分配内存的时间

    {
        TIME_INTERVAL_SCOPE("push_back string:");
        for (int i = 0; i < count; i++)
        {
            std::string temp("ceshi");
            v.push_back(temp);// push_back(const string&)，参数是左值引用
        }
    }

    v.clear();
    {
        TIME_INTERVAL_SCOPE("push_back move(string):");
        for (int i = 0; i < count; i++)
        {
            std::string temp("ceshi");
            v.push_back(std::move(temp));// push_back(string &&), 参数是右值引用
        }
    }

    v.clear();
    {
        TIME_INTERVAL_SCOPE("push_back(string):");
        for (int i = 0; i < count; i++)
        {
            v.push_back(std::string("ceshi"));// push_back(string &&), 参数是右值引用
        }
    }

    v.clear();
    {
        TIME_INTERVAL_SCOPE("push_back(c string):");
        for (int i = 0; i < count; i++)
        {
            v.push_back("ceshi");// push_back(string &&), 参数是右值引用
        }
    }

    v.clear();
    {
        TIME_INTERVAL_SCOPE("emplace_back(c string):");
        for (int i = 0; i < count; i++)
        {
            //std::string temp("ceshi");
            v.emplace_back("ceshi");// 只有一次构造函数，不调用拷贝构造函数，速度最快
        }
    }
}





