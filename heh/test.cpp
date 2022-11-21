#include <iostream>
#include <string>
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <functional>
#include "thread.h"

void* fun1(){
    std::string str = Thread::GetThis()->getName();
    std::cout << Thread::GetThis()->getpid();
    for(int i = 0; i < 10; ++i){
        std::cout << str <<":"<< i <<std::endl;
    }
    
    return 0;
}
int main(){
    Thread a(std::bind(&fun1), "one");
    Thread b(std::bind(&fun1), "two");
    Thread c(std::bind(&fun1), "three");
    Thread d(std::bind(&fun1), "four");
    Thread e(std::bind(&fun1), "five");
    return 0;
}