#include "unconfirm.h"

#include <mutex>
static std::mutex mut;

void Unconfirm::push(Command& buffer){
    mut.lock();
    this->unconfirm.push(buffer);
    mut.unlock();
}

void Unconfirm::pop(){
    mut.lock();
    this->unconfirm.pop();
    mut.unlock();
}

Command* Unconfirm::front(){
    mut.lock();
    Command* tmp = &this->unconfirm.front();
    mut.unlock();
    return tmp;
}

size_t Unconfirm::size(){
    mut.lock();
    size_t size = this->unconfirm.size();
    mut.unlock();
    return size;
}