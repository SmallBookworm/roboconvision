//
// Created by peng on 18-3-14.
//

#ifndef ROBOCONVISION_INFO_H
#define ROBOCONVISION_INFO_H

#include "vector"
#include <string.h>

static const int sumNum = 4;
struct InMeta {
    char head[2];
    char positionX[2];
    char positionY[2];
    char angle[2];
    char flag1[1];
    char flag2[1];
    char flag3[1];
    char flag4[1];
    char sum[sumNum];
};
union In {
    char data[16];
    InMeta meta;
};

struct OutMeta {
    char head[2];
    char dataArea[1];
    char positionX[2];
    char positionY[2];
    char angle[8];
    char conectF1[1];
    char conectF2[1];
    char ringF1[1];
    char ringF2[1];
    char sum[sumNum];
};
union Out {
    char data[19];
    OutMeta meta;
};

class Info {
private:
    std::vector<char> data;
    const char dataHead[3] = "ab";
    int ableLength = 0;
    const int inLength = 16;
public:
    union In result{};

    //-1 sum test fail,0 continue,1 success
    int push(char od);

    int getSum();

    void getData();

};


#endif //ROBOCONVISION_INFO_H
