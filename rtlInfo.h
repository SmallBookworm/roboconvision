//
// Created by peng on 18-5-9.
//

#ifndef ROBOCONVISION_RTLINFO_H
#define ROBOCONVISION_RTLINFO_H

#include <mutex>
#include "ThreadInfo.h"

class RtlInfo :public ThreadInfo{
private:
    double xAngle;
    double xDis;
    double yAngle;
    double yDis;
    //x:0x1,y:0x2
    unsigned char flag=0;
public:
    void set(double xAngle,
             double xDis,
             double yAngle,
             double yDis, unsigned char flag);

//-1 used,1 x,2 y,3 x y
    char get(double *res);
};


#endif //ROBOCONVISION_RTLINFO_H
