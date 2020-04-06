#include <cstdlib>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <sstream>
#include "string.h"

template std::__1::basic_stringstream<char, std::__1::char_traits<char>, std::__1::allocator<char> >;

class Thread : public Object {
public:
    std::thread thread_;
    void start() {
        thread_ = std::thread([this](){this->run();});
    }

    void join() {thread_.join();}

    static void yield() {std::this_thread::yield();}

    static void sleep(size_t millis) {
        std::this_thread::sleep_for(std::chrono::milliseconds(millis));
    }

    virtual void run() = 0;

    static String* thread_id() {
        std::stringstream buf;
        buf<< std::this_thread::get_id();
        std::string buffer(buf.str());
        return new String(buffer.c_str(),buffer.size());
    }

};

class Lock : public Object {
public:
    std::recursive_mutex mtx_;
    std::condition_variable_any cv_;

    void lock() { mtx_.lock();}
    void unlock() {mtx_.unlock();}
    void notify_all() {cv_.notify_all();}
    void wait() {cv_.wait(mtx_);}
};