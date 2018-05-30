#include "receiver.hpp"

using namespace std;
using namespace exlcm;

receiver::receiver(){
    pthread_mutex_lock(&handler.flagLock);
    handler.receiverFlag = 0;
    pthread_mutex_unlock(&handler.flagLock);
    if(!lcm.good()){
        printf("LCM init error!\n");
        exit(1);
    }
    lcm.subscribe("PYTHON_CPP", &Handler::PYTHON_CPP_Callback, &handler);

}

void receiver::receiverRun(){
    lcm.handle();
}

void receiver::infoPub(msgImagePath msg){
    lcm.publish("CPP_PYTHON", &msg);
    handler.receiverFlag = 0;
}

vector<int> receiver::infoGet(){
    vector<int> msg;
    pthread_mutex_lock(&handler.msgLock);
    msg = handler.ImageMsg;
    pthread_mutex_unlock(&handler.msgLock);
    return msg;
}

int receiver::flagStatus(){
    pthread_mutex_lock(&handler.flagLock);
    int flag = handler.receiverFlag;
    pthread_mutex_unlock(&handler.flagLock);
    return flag;
}


Handler::Handler(){

}

void Handler::PYTHON_CPP_Callback(const lcm::ReceiveBuffer* rbuf,
    const std::string& chan,
    const msgImageMsg* msg){
    pthread_mutex_lock(&flagLock);
    receiverFlag = 1;
    pthread_mutex_unlock(&flagLock);
    pthread_mutex_lock(&msgLock);
    ImageMsg = (*msg).ImageMsg;
    pthread_mutex_unlock(&msgLock);
}
