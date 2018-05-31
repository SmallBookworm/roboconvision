//
// Created on 18-5-30.
//

#ifndef ROBOCONVISIONHM_THREADINFO_H
#define ROBOCONVISIONHM_THREADINFO_H

#include <mutex>

class ThreadInfo {
protected:
    //is info used?
    bool used = true;
    std::mutex info_mutex;
    //run:true finish:false
    bool threadState = true;
    std::mutex ts_mutex;
public:

    bool getThreadState();

    void setThreadState(bool state);

    void init();
};


#endif //ROBOCONVISIONHM_THREADINFO_H
