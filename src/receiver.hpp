#ifndef RECEIVER_HPP
#define RECEIVER_HPP

#include <iostream>
#include <lcm/lcm-cpp.hpp>
#include <msg_lcm/exlcm/msgImagePath.hpp>
#include <msg_lcm/exlcm/msgImageMsg.hpp>
#include <pthread.h>

using namespace lcm;
using namespace std;
using namespace exlcm;

class Handler{
    public:
        Handler();
        ~Handler() {}
        void PYTHON_CPP_Callback(const lcm::ReceiveBuffer* rbuf,
                const std::string& chan,
                const msgImageMsg* msg);
        int receiverFlag;
        vector<int> ImageMsg;
        pthread_mutex_t flagLock;
        pthread_mutex_t msgLock;
};

class receiver{
    public:
        receiver();
        ~receiver() {}

        void receiverRun();
        void infoPub(msgImagePath msg);
        vector<int> infoGet();
        int flagStatus();
        Handler handler;
        lcm::LCM lcm;
    private:
        msgImagePath ImagePath;
};

#endif
