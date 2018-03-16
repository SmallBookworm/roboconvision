//
// Created by peng on 18-3-14.
//

#ifndef ROBOCONVISION_INFO_H
#define ROBOCONVISION_INFO_H

#include "vector"
#include <string.h>
#include <stdint.h>

#define OUTLENGTH 24
#define INLENGTH 16
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
    InMeta meta{head:{static_cast<unsigned char>(0xaa), static_cast<unsigned char>(0xbb)}};
};

struct OutMeta {
    unsigned char head[2];
    unsigned char dataArea[1];
     char positionX[2];
     char positionY[2];
     char angle[8];
    unsigned char conectF1[1];
    unsigned char conectF2[1];
    unsigned char ringF1[1];
    unsigned char ringF2[1];
    unsigned char sum[sumNum];
};
union Out {
    char data[19];
    OutMeta meta{head:{static_cast<unsigned char>(0xaa), static_cast<unsigned char>(0xbb)}};
};

void testSum(union Out* res);

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
