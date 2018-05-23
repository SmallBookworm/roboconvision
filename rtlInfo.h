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
    //x:0x1,y:0x2
    unsigned char flag;
    bool used;
    std::mutex rtl_mutex;
public:
    RtlInfo() {
        used = true;
        flag = 0;
    }

    void set(double xAngle,
             double xDis,
             double yAngle,
             double yDis, unsigned char flag);

//-1 used,1 x,2 y,3 x y
    char get(double *res);
};


#endif //ROBOCONVISION_RTLINFO_H
