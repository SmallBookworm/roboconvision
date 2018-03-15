//
// Created by peng on 18-3-14.
//

#include "Info.h"

using namespace std;

int Info::push(char od) {
    this->data.push_back(od);
    //test data head
    if (this->ableLength < strlen(dataHead)) {
        if (od == dataHead[ableLength])
            ableLength++;
        else
            ableLength = 0;
        return 0;
    } else {
        ableLength++;
    }
    if (ableLength == this->inLength) {
        ableLength = 0;
        int sumTest = getSum();
        this->getData();
        int sum = 0;
        memcpy(&sum,result.meta.sum,sumNum);
        if (sumTest == sum)
            return 1;
        else
            return -1;
    }

    return 0;
}

void Info::getData() {
    vector<char>::iterator end = this->data.end();
    for (int i = 0; i < inLength; ++i) {
        result.data[i] = *(end - i);
    }
}

int Info::getSum() {
    int res = 0;
    vector<char>::iterator end = this->data.end();
    for (int i = sumNum; i < inLength; ++i) {
        res += *(end - i);
    }
    return res;
}