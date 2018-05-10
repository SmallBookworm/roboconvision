//
// Created by peng on 18-5-9.
//

#ifndef ROBOCONVISION_RTLINFO_H
#define ROBOCONVISION_RTLINFO_H

#include <mutex>

class RtlInfo {
private:
    double xAngle;
    double xDis;
    double yAngle;
    double yDis;
    bool used;
    std::mutex rtl_mutex;
public:
    RtlInfo() {
        used = true;
    }

    void set(double xAngle,
             double xDis,
             double yAngle,
             double yDis);

//-1 used,1 success
    int get(double *res);
};


#endif //ROBOCONVISION_RTLINFO_H
