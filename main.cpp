#include <iostream>
#include "signal_change.h"
#include "line_track.h"

using namespace std;
using namespace cv;

int main() {
    std::cout << "Hello, World!" << std::endl;
    bool flag = true;
    VideoCapture capture0("/home/peng/文档/projectA/四色闪烁/002.avi");
    SignalWatcher watcher(capture0);

    VideoCapture capture1(0);
    LineTracker lineTracker(capture1);

    while (1) {
        //test1
        try {
            int ff = watcher.watch();
            if (ff != -1) {
                cout<<ff<<endl;
            }
        }catch (char const* error){
            printf(error);
            break;
        }
        //test2
        if (flag){
        lineTracker.watch();
            flag=false;
        }
        //test3
        if (waitKey(1) == 27)
            break;
    }
    waitKey(0);
    return 0;
}