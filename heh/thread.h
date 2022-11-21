#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory>
#include <functional>
#include <string>
#include <thread>

class Semaphore{
public:
    Semaphore(uint32_t count = 0);
    ~Semaphore();
    void notify();
    void wait();
private:
    Semaphore(const Semaphore&) = delete;
    Semaphore(const Semaphore&&) = delete;
    Semaphore& operator= (const Semaphore&) = delete;
private:
    sem_t m_semaphore;
};

class Thread{
public:
    typedef std::shared_ptr<Thread> ptr;
    Thread(std::function<void()> fun, const std::string& name);
    ~Thread();
    pid_t getpid()const {return m_pid;}
    const std::string& getName()const{return m_name;}
    void join();

    static const std::string& GetName();
    static void SetName(const std::string& name);
    static Thread* GetThis();
    
private:
    Thread(const Thread&) = delete;
    Thread(const Thread&&) = delete;
    Thread& operator=(const Thread&) = delete;

private:
    static void* run(void* arg);
    pid_t m_pid;
    pthread_t m_tid;
    std::string m_name;
    std::function<void()> m_cb;
    Semaphore m_sem;
};