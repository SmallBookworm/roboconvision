//
// Created by peng on 18-3-14.
//

#ifndef ROBOCONVISION_INFO_H
#define ROBOCONVISION_INFO_H

#include "vector"
#include <string.h>
#include <stdint.h>

static const int sumNum = 4;

//typedef union {
//
//    struct PC_Buffer {
//        uint8_t Data_head1;                //¹Ì¶¨Êý¾ÝÍ·1(0xaa)
//        uint8_t Data_head2;                //¹Ì¶¨Êý¾ÝÍ·2(0xbb)
//
//        uint8_t flag;                    //
//        uint8_t save;                    //
//        int16_t pos_x;                    //ÊÀ½ç×ø±êX
//        int16_t pos_y;                    //ÊÀ½ç×ø±êy
//        int16_t angle;                    //¾ø¶Ô³¯Ïò
//        int16_t connect_x;                //Æ«²îx
//        int16_t connect_y;                //Æ«²îy
//        uint8_t save2[4];
//
//        uint32_t verify;                //Ð£Ñé£¨ÇóºÍ£
//
//    } buffer;
//
//    uint8_t Bytes[sizeof(struct PC_Buffer)];
//
//} PC_Rx_BufferType;

struct InMeta {
    unsigned char head[2];
    unsigned char positionX[2];
    unsigned char positionY[2];
    unsigned char angle[2];
    unsigned char flag1[1];
    unsigned char placeHold[3];
    unsigned char sum[sumNum];
};
union In {
    unsigned char data[16];
    InMeta meta{};
};

struct OutMeta {
    unsigned char head[2];
    unsigned char dataArea[1];
    unsigned char ringF1[1];
    unsigned char ballDX[4];
    unsigned char ballDY[4];
    unsigned char xAngle[4];
    unsigned char yAngle[4];
    unsigned char xDis[4];
    unsigned char yDis[4];
    unsigned char device[1];
    unsigned char placeHold[3];
    unsigned char sum[sumNum];
};
union Out {
    unsigned char data[36];
    OutMeta meta{head:{static_cast<unsigned char>(0xaa), static_cast<unsigned char>(0xbb)}};
};

void assignSum(union Out *res);

class Info {
private:
    std::vector<unsigned char> data;
    const unsigned char dataHead[2] = {static_cast<unsigned char>(0xaa), static_cast<unsigned char>(0xbb)};
    int ableLength = 0;
    const int inLength = 16;
public:
    union In result{};

    //-1 sum test fail,0 continue,1 success
    int push(unsigned char od);

    int getSum();

    void getData();

};


#endif //ROBOCONVISION_INFO_H
