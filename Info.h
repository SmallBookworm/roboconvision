//
// Created by peng on 18-3-14.
//

#ifndef ROBOCONVISION_INFO_H
#define ROBOCONVISION_INFO_H

#include "vector"
#include <string.h>

struct InMeta {
    char head[2];
    char positionX[2];
    char positionY[2];
    char angle[2];
    char flag1[1];
    char flag2[1];
    char flag3[1];
    char flag4[1];
    char sum[1];
};
union In {
    char data[13];
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
    char sum[4];
};
union Out {
    char data[17];
    OutMeta meta;
};

class Info {
private:
    std::vector<char> data;
    const char dataHead[3] = "ab";
    int ableLength = 0;
    const int inLength = 13;
public:
    //0 continue,1 success
    int push(char od);

    InMeta getData();

};


#endif //ROBOCONVISION_INFO_H
