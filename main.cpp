#include <iostream>
#include "signal_change.h"

using namespace std;
using namespace cv;

void text1(bool* flag) {
    VideoCapture capture("001.avi");
    SignalWatcher watcher(capture);
    int ff=watcher.watch();
    if (ff ==1){
        *flag= false;
    }
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    bool flag=true;
    while (flag){
        text1(&flag);
    }
    return 0;
}