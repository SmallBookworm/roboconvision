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
    if (ableLength == this->inLength)
        return 1;
    return 0;
}

InMeta Info::getData() {
    union In result{};
    vector<char>::iterator end = this->data.end();
    for (int i = 0; i < inLength; ++i) {
    result.data[i]=*(end-i);
    }
    return result.meta;
}