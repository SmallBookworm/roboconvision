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

    while (flag) {
        //test1
        int ff = watcher.watch();
        if (ff != -1) {
        cout<<ff<<endl;
        }
        //test2
        if (waitKey(1) == 27)
            break;
    }
    return 0;
}