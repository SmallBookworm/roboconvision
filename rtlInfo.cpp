//
// Created by peng on 18-5-9.
//

#include "rtlInfo.h"

using namespace std;

void RtlInfo::set(double xAngle, double xDis, double yAngle, double yDis, unsigned char flag) {
    lock_guard<mutex> l(info_mutex);
    this->xAngle = xAngle;
    this->xDis = xDis;
    this->yAngle = yAngle;
    this->yDis = yDis;
    this->flag = flag;
    this->used = false;
}

char RtlInfo::get(double *res) {
    lock_guard<mutex> l(info_mutex);
    if (used) {
        return -1;
    } else {
        res[0] = xAngle;
        res[1] = yAngle;
        res[2] = xDis;
        res[3] = yDis;
        used = true;
        return flag;
    }
}