#include <iostream>
#include <csignal>
#include <sys/time.h>
#include "signal_change.h"
#include "LineTracker.h"
#include "serial.hpp"
#include "Info.h"
#include "ball_yn.h"

#define DOCKING_MODE 0x1
#define DROP_MODE 0x2

using namespace std;
using namespace cv;

int state = 0x0;

union Out wdata{};

MySerial ms = MySerial();
int fd;

void printMes(int signo) {
    //printf("Get a SIGALRM, signal NO:%d\n", signo);
    //sum flag
    assignSum(&wdata);
    ms.nwrite(fd, wdata.data, sizeof(wdata.data));
    //restore
    wdata = {};
}

int main() {
    fd = ms.open_port(1);
    ms.set_opt(fd, BAUDRATE, 8, 'N', 1);

    struct itimerval tick;
    signal(SIGALRM, printMes);
    memset(&tick, 0, sizeof(tick));
    tick.it_value.tv_sec = 1;
    tick.it_value.tv_usec = 0;
    tick.it_interval.tv_sec = 1;
    tick.it_interval.tv_usec = 0;
    if (setitimer(ITIMER_REAL, &tick, NULL) < 0)
        printf("Set time fail!");

    LineTracker lineTracker;

    //union Out s{};
    //cout << s.data << " length:" << sizeof(s.data) << endl;

    Info info;
    while (true) {
        //read message
        unsigned char rdata;
        int n = ms.nread(fd, &rdata, 1);
        //sometime read nothing
        if (n <= 0)
            continue;
        //cout << int(rdata) << endl;
        //cout << info.result.data << " length:" << sizeof(info.result.data) << endl;
        if (info.push(rdata) <= 0)continue;

        wdata.meta.dataArea[0] = 0;
        //position(coordinate)
        if ((info.result.meta.flag1[0] & (1)) != 0) {
            Point2f point;
            VideoCapture cap(1);
            if (!cap.isOpened()) {
                cerr << "capture is closed\n";
                continue;
            }
            Mat frame;
            cap >> frame;
            if (frame.empty()) {
                cerr << "frame is empty\n";
                continue;
            }
            lineTracker.watch(frame, &point);
            short x = static_cast<short>(point.x);
            memcpy(wdata.meta.positionX, &x, sizeof(x));
            short y = static_cast<short>(point.y);
            memcpy(wdata.meta.positionY, &y, sizeof(y));
            //valid data
            wdata.meta.dataArea[0] |= 0x01;
        }
        //Docking mode
        if ((info.result.meta.flag1[0] & (1 << 1)) != 0) {
            if ((state & DOCKING_MODE) == 0) {
                state |= DOCKING_MODE;
            }
        } else if ((state & DOCKING_MODE) != 0) {
            state ^= DOCKING_MODE;
        }
        //Drop mode
        if ((info.result.meta.flag1[0] & (1 << 2)) != 0) {
            wdata.meta.dataArea[0] |= 0x08;
            if ((state & DROP_MODE) == 0) {
                state |= DROP_MODE;
            }
        } else if ((state & DROP_MODE) != 0) {
            state ^= DROP_MODE;
        }
        if (waitKey(1) == 27)
            break;
    }
    return 0;
}

void text() {
    bool flag = true;
    VideoCapture capture0("/home/peng/文档/projectA/四色闪烁/002.avi");
    SignalWatcher watcher(capture0);

    LineTracker lineTracker;

    while (true) {
        //test1
        try {
            int ff = watcher.watch();
            if (ff != -1) {
                cout << ff << endl;
            }
        } catch (char const *error) {
            printf(error);
            break;
        }
        //test2
        if (flag) {
            Point2f point;
            VideoCapture cap(1);
            if (!cap.isOpened()) {
                cerr << "capture is closed\n";
                continue;
            }
            Mat frame;
            cap >> frame;
            if (frame.empty()) {
                cerr << "frame is empty\n";
                continue;
            }
            lineTracker.watch(frame, &point);
            flag = false;
        }
        //test3
        if (waitKey(1) == 27)
            break;
    }
    waitKey(0);
}