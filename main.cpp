#include <iostream>
#include <thread>
#include <csignal>
#include <sys/time.h>
#include "LineTracker.h"
#include "serial.hpp"
#include "Info.h"
#include "ball_tracker.h"
#include "lineTest.h"

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
    tick.it_value.tv_sec = 0;
    tick.it_value.tv_usec = 50000;
    tick.it_interval.tv_sec = 0;
    tick.it_interval.tv_usec = 50000;
    if (setitimer(ITIMER_REAL, &tick, NULL) < 0)
        printf("Set time fail!");

    LineTracker lineTracker;

    //union Out s{};
    //cout << s.data << " length:" << sizeof(s.data) << endl;

    DeviationPosition position;
    position.await();
    Tracker tracker;
    thread thread1(tracker, ref(position));
    thread1.detach();

    LineInfo lineInfo;
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
        //wdata.meta.dataArea[0] = 0;
        //position(coordinate)
        if ((info.result.meta.flag1[0] & (1)) != 0) {
            Point2f point;
            VideoCapture cap(2);
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
        cout << "Docking mode" << (info.result.meta.flag1[0] & (1 << 1)) << endl;
        //Docking mode
        if ((info.result.meta.flag1[0] & (1 << 1)) != 0) {
            if ((state & DOCKING_MODE) == 0) {
                state |= DOCKING_MODE;
                lineInfo.init();
                LineTest tracker;
                thread thread1(tracker, ref(lineInfo));
                thread1.detach();
            }
            float res[3];
            int resF = lineInfo.get(res);
            if (resF > 0) {
                cout << res << endl;
                wdata.meta.dataArea[0] |= 0x02;
                memcpy(wdata.meta.dockDModule, &res[0], sizeof(res[0]));
                memcpy(wdata.meta.dockArgument, &res[1], sizeof(res[0]));
                memcpy(wdata.meta.dockRAngle, &res[2], sizeof(res[0]));
            }
        } else if ((state & DOCKING_MODE) != 0) {
            state ^= DOCKING_MODE;
            lineInfo.setStop(true);
        }
        cout << "Drop mode" << (info.result.meta.flag1[0] & (1 << 2)) << endl;
        //Drop mode
        if ((info.result.meta.flag1[0] & (1 << 2)) != 0) {
            if ((state & DROP_MODE) == 0) {
                state |= DROP_MODE;
                position.init(Vec3f(0, 0, 0));
            }
            Point2f ballPoint;
            int res = position.getPoint(ballPoint);
            if (res >= 0) {
                wdata.meta.dataArea[0] |= 0x04;
                wdata.meta.ringF1[0] = static_cast<unsigned char>(res);
                memcpy(wdata.meta.ballDX, &ballPoint.x, sizeof(ballPoint.x));
                memcpy(wdata.meta.ballDY, &ballPoint.y, sizeof(ballPoint.y));
            }

        } else if ((state & DROP_MODE) != 0) {
            state ^= DROP_MODE;
            position.await();
        }
        if (waitKey(1) == 27)
            break;
    }
    return 0;
}