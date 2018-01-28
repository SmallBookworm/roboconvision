#include <iostream>
#include "signal_change.h"

using namespace std;
using namespace cv;

int main() {
    std::cout << "Hello, World!" << std::endl;
    bool flag = true;
    VideoCapture capture("/home/peng/文档/projectA/四色闪烁/002.avi");
    SignalWatcher watcher(capture);

    while (flag) {
        //text1
        int ff = watcher.watch();
        if (ff != -1) {
        cout<<ff<<endl;
        }

        if (waitKey(1) == 27)
            break;
    }
    return 0;
}