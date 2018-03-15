#include <iostream>
#include <csignal>
#include <sys/time.h>
#include "signal_change.h"
#include "LineTracker.h"
#include "serial.hpp"
#include "Info.h"
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
    if (!capture0.isOpened()) {
        cerr << "capture is closed\n";
        return;
    }
    Mat frame;
    capture0 >> frame;
    if (frame.empty()) {
        cerr << "frame is empty\n";
        return;
    }
    bool ball = clipWatcher.watch(frame);
    union Out wdata{};
    fill_n(wdata.data, 17, 0);
    wdata.meta.head[0] = 'a';
    wdata.meta.head[1] = 'b';
    wdata.meta.dataArea[0] |= 0x02;
    if (ball)
        wdata.meta.conectF2[0] = 1;
    else
        wdata.meta.conectF2[0] = 0;
    MySerial ms = MySerial();
    int fd = ms.open_port(1);
    fd = ms.set_opt(fd, BAUDRATE, 8, 'O', 0);
    ms.nwrite(fd, wdata.data, 17);
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
    Info info;
    while (true) {
        //read message
        char rdata;
        int n = ms.nread(fd, &rdata, 1);

        if (info.push(rdata) > 0)continue;
        union Out wdata{};
        wdata.meta.head[0] = 'a';
        wdata.meta.head[1] = 'b';
        wdata.meta.dataArea[0] = 0;
        if ((info.result.meta.flag1[0] & (1)) != 0) {
            wdata.meta.dataArea[0] |= 0x01;
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
            if (wdata.meta.dataArea[0] != 0)
                ms.nwrite(fd, wdata.data, 17);
        }

        if ((info.result.meta.flag1[0] & (1 << 1)) != 0) {
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