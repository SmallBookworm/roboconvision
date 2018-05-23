#include <iostream>
#include <thread>
#include <csignal>
#include <sys/time.h>
#include "serial.hpp"
#include "Info.h"
#include "ball_tracker.h"
#include "rtlFinder.h"

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


    //union Out s{};
    //cout << s.data << " length:" << sizeof(s.data) << endl;

    RtlInfo rtlInfo;
    RtlFinder rtlFinder;
    bool test = false;
    if (!test) {
        thread thread11(rtlFinder, ref(rtlInfo));
        thread11.detach();
    }

    DeviationPosition position;
    position.await();
    Tracker tracker;
    thread thread1(tracker, ref(position));
    thread1.detach();

    Info info;
    while (true) {
        //read message
        unsigned char rdata;
        int n = ms.nread(fd, &rdata, 1);
        //sometime read nothing
        if (n <= 0)
            continue;
        //test data,startup when get full package
        cout << int(rdata) << endl;
        if (info.push(rdata) <= 0)continue;
        //wdata.meta.dataArea[0] = 0;
        cout << "Drop mode" << (info.result.meta.flag1[0] & (1 << 2)) << endl;
        //Drop mode
        if ((info.result.meta.flag1[0] & (1 << 2)) != 0) {
            if ((state & DROP_MODE) == 0) {
                state |= DROP_MODE;
                //ring coordinate
                short x, y, angle;
                memcpy(&x, &info.result.meta.positionX, sizeof(x));
                memcpy(&y, &info.result.meta.positionY, sizeof(x));
                memcpy(&angle, &info.result.meta.angle, sizeof(x));

                //the Global Positioning System's height is  340 mm
                float cHeight = 340;
                //make sure which ring
                float rHeight = -x > 4775 ? 3400 : 2400;
                Vec4f ring(y - 500, rHeight - cHeight, x + 3175, angle / 180.0 * M_PI);
                //change coordinate system
                float c1 = ring[0], c2 = ring[2];
                ring[0] = static_cast<float>(cos(ring[3]) * c1 - sin(ring[3]) * c2);
                ring[2] = static_cast<float>(cos(ring[3]) * c2 + sin(ring[3]) * c1);
                //camera relative position
                ring[2] -= 500;
                //mm -> m
                ring[0] /= 1000;
                ring[1] /= 1000;
                ring[2] /= 1000;

                position.init(ring);
            }
            Point2f ballPoint;
            int res = position.getPoint(ballPoint);
            if (res >= 0) {
                wdata.meta.dataArea[0] |= 0x02;
                wdata.meta.ringF1[0] = static_cast<unsigned char>(res);
                memcpy(wdata.meta.ballDX, &ballPoint.x, sizeof(ballPoint.x));
                memcpy(wdata.meta.ballDY, &ballPoint.y, sizeof(ballPoint.y));
            }

        } else if ((state & DROP_MODE) != 0) {
            state ^= DROP_MODE;
            cout << ">?" << endl;
            position.await();
        }

        //realtime find line
        double rtlCoordinate[4];
        char res = rtlInfo.get(rtlCoordinate);
        if (res > 0) {
            if ((res & 1) != 0) {
                wdata.meta.dataArea[0] |= 0x04;
                memcpy(wdata.meta.xDis, &rtlCoordinate[2], sizeof(rtlCoordinate[0]));
                memcpy(wdata.meta.xAngle, &rtlCoordinate[0], sizeof(rtlCoordinate[0]));
            }
            if ((res & 2) != 0) {
                wdata.meta.dataArea[0] |= 0x08;
                memcpy(wdata.meta.yDis, &rtlCoordinate[3], sizeof(rtlCoordinate[0]));
                memcpy(wdata.meta.yAngle, &rtlCoordinate[1], sizeof(rtlCoordinate[0]));
            }
        }
    }
    return 0;
}