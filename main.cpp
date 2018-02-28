#include <iostream>
#include <csignal>
#include <sys/time.h>
#include "signal_change.h"
#include "line_track.h"
#include "serial.hpp"
#include "ball_yn.h"

using namespace std;
using namespace cv;

int state = 0;

ClipWatcher clipWatcher;

void printMes(int signo) {
    printf("Get a SIGALRM, signal NO:%d\n", signo);
    VideoCapture capture0(0);
    Mat frame;
    capture0 >> frame;
    bool ball = clipWatcher.watch(frame);
    char wdata[17];
    wdata[0] = 'a';
    wdata[1] = 'b';
    wdata[2] = 0;
    if (ball)
        wdata[10] = 1;
    else
        wdata[10] = 0;
    MySerial ms = MySerial();
    int fd = ms.open_port(1);
    fd = ms.set_opt(fd, BAUDRATE, 8, 'O', 0);
    ms.nwrite(fd, wdata, 17);
}

int main() {
    struct itimerval tick;
    signal(SIGALRM, printMes);
    memset(&tick, 0, sizeof(tick));
    tick.it_value.tv_sec = 0;
    tick.it_value.tv_usec = 0;
    tick.it_interval.tv_sec = 0;
    tick.it_interval.tv_usec = 0;
    if (setitimer(ITIMER_REAL, &tick, NULL) < 0)
        printf("Set time fail!");


    VideoCapture capture1(1);
    LineTracker lineTracker(capture1);

//    string as = "123";
    MySerial ms = MySerial();
    int fd = ms.open_port(1);
    fd = ms.set_opt(fd, BAUDRATE, 8, 'O', 0);
//    ms.nwrite(fd, as.c_str(), as.length());
    while (true) {
        char rdata[13];
        int n = ms.nread(fd, rdata, 13);
        if (n != 13)continue;
        char wdata[17];
        wdata[0] = 'a';
        wdata[1] = 'b';
        wdata[2] = 0;
        if ((rdata[8] & (1)) != 0) {
            wdata[2] |= 0x01;
            Point2f point = lineTracker.watch();
            short x = static_cast<short>(point.x);
            memcpy(wdata + 3, &x, sizeof(x));
            short y = static_cast<short>(point.y);
            memcpy(wdata + 4, &y, sizeof(y));
        }

        if ((rdata[8] & (1 << 1)) != 0) {
            wdata[2] |= 0x02;
            if (state != 1) {
                state = 1;
                tick.it_value.tv_sec = 1;
                tick.it_value.tv_usec = 0;
                tick.it_interval.tv_sec = 1;
                tick.it_interval.tv_usec = 0;
            }
        } else if ((rdata[8] & (1 << 2)) != 0) {
            wdata[2] |= 0x08;
            state = 2;
            tick.it_value.tv_sec = 0;
            tick.it_value.tv_usec = 0;
            tick.it_interval.tv_sec = 0;
            tick.it_interval.tv_usec = 0;
        } else {
            state = 0;
            tick.it_value.tv_sec = 0;
            tick.it_value.tv_usec = 0;
            tick.it_interval.tv_sec = 0;
            tick.it_interval.tv_usec = 0;
            if (wdata[2] != 0)
                ms.nwrite(fd, wdata, 17);
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

    VideoCapture capture1(0);
    LineTracker lineTracker(capture1);

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
            lineTracker.watch();
            flag = false;
        }
        //test3
        if (waitKey(1) == 27)
            break;
    }
    waitKey(0);
}