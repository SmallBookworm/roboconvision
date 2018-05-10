//
// Created by peng on 18-5-9.
//

#include "rtlInfo.h"

using namespace std;

void RtlInfo::set(double xAngle, double xDis, double yAngle, double yDis) {
    lock_guard<mutex> l(rtl_mutex);
    this->xAngle = xAngle;
    this->xDis = xDis;
    this->yAngle = yAngle;
    this->yDis = yDis;
    this->used = false;
}

int RtlInfo::get(double *res) {
    lock_guard<mutex> l(rtl_mutex);
    if (used) {
        return -1;
    } else {
        res[0] = xAngle;
        res[1] = yAngle;
        res[2] = xDis;
        res[3] = yDis;
        used = true;
        return 1;
    }
}