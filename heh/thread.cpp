#include "thread.h"
#include <iostream>
static thread_local Thread* t_thread = nullptr;
static thread_local std::string t_thread_name = "UNKNOW";

Semaphore::Semaphore(uint32_t count){
    if(sem_init(&m_semaphore, 0, count)){
        throw std::logic_error("sem_init error");
    }
}
Semaphore::~Semaphore(){
    if(sem_destroy(&m_semaphore)){
        throw std::logic_error("sem_destroy error");
    }
}
void Semaphore::notify(){
    if(sem_post(&m_semaphore)){
        throw std::logic_error("sem_post error");
    }
}
void Semaphore::wait(){
    if(sem_wait(&m_semaphore)){
        throw std::logic_error("sem_wait error");
    }
}

const std::string& Thread::GetName(){
    return t_thread_name;
}
void Thread::SetName(const std::string& name){
    if(t_thread){
        t_thread->m_name = name;
    }
}
Thread* Thread::GetThis(){
    return t_thread;
}
Thread::Thread(std::function<void()> fun, const std::string& name)
    :m_cb(fun), m_name(name){
        if(name.empty()){
            m_name = "UNKNOW";
        }
        if(pthread_create(&m_tid, nullptr, &Thread::run, this)){
            std::cout << "error";
        }
        m_sem.wait();
}
Thread::~Thread(){
    if(m_tid){
        pthread_detach(m_tid);
    }
}
void* Thread::run(void* arg){
    Thread* thread = (Thread*)arg;
    t_thread = thread;
    t_thread_name = thread->m_name;
    thread->m_pid = pthread_self();
    pthread_setname_np(pthread_self(), thread->m_name.substr(0,15).c_str());
    std::function<void()> cb;
    cb.swap(thread->m_cb);
    thread->m_sem.notify();
    cb ();
    return 0;

}
void Thread::join(){
    if(m_tid){
        if(pthread_join(m_tid, nullptr)){
            std::cout <<"error";
        }
    }
    m_tid = 0;
}

