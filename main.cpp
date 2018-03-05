#include <iostream>
#include <csignal>
#include <sys/time.h>
#include "signal_change.h"
#include "line_track.h"
#include "serial.hpp"
#include "ball_yn.h"

#define ABUTMENT_MODE 0x1
#define DROP_MODE 0x2

using namespace std;
using namespace cv;

int state = 0x0;

ClipWatcher clipWatcher;

void printMes(int signo) {
    printf("Get a SIGALRM, signal NO:%d\n", signo);
    VideoCapture capture0(2);
    Mat frame;
    capture0 >> frame;
    bool ball = clipWatcher.watch(frame);
    char wdata[17];
    wdata[0] = 'a';
    wdata[1] = 'b';
    wdata[2] |= 0x02;
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

    LineTracker lineTracker;

    MySerial ms = MySerial();
    int fd = ms.open_port(1);
    ms.set_opt(fd, BAUDRATE, 8, 'O', 1);
    string data = "12345!";
    ms.nwrite(fd, data.c_str(), data.length());
    while (true) {
        //read message
        char rdata[13];
        int n = ms.nread(fd, rdata, 13);
        if (n != 13)continue;

        char wdata[17];
        wdata[0] = 'a';
        wdata[1] = 'b';
        wdata[2] = 0;
        if ((rdata[8] & (1)) != 0) {
            wdata[2] |= 0x01;
            Point2f point;
            lineTracker.watch(&point);
            short x = static_cast<short>(point.x);
            memcpy(wdata + 3, &x, sizeof(x));
            short y = static_cast<short>(point.y);
            memcpy(wdata + 4, &y, sizeof(y));
            if (wdata[2] != 0)
                ms.nwrite(fd, wdata, 17);
        }

        if ((rdata[8] & (1 << 1)) != 0) {
            if ((state & ABUTMENT_MODE) == 0) {
                state |= ABUTMENT_MODE;
                tick.it_value.tv_sec = 1;
                tick.it_value.tv_usec = 0;
                tick.it_interval.tv_sec = 1;
                tick.it_interval.tv_usec = 0;
            }
        } else if ((state & ABUTMENT_MODE) != 0) {
            state ^= ABUTMENT_MODE;
            tick.it_value.tv_sec = 0;
            tick.it_value.tv_usec = 0;
            tick.it_interval.tv_sec = 0;
            tick.it_interval.tv_usec = 0;
        }

        if ((rdata[8] & (1 << 2)) != 0) {
            wdata[2] |= 0x08;
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
            lineTracker.watch(&point);
            flag = false;
        }
        //test3
        if (waitKey(1) == 27)
            break;
    }
    waitKey(0);
}