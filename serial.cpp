#include "serial.hpp"

MySerial::MySerial() {
}

MySerial::~MySerial() {

}

int MySerial::set_opt(int fd, int nSpeed, int nBits, char nEvent, int nStop)  //串口设置
{
    struct termios newtio, oldtio;
    if (tcgetattr(fd, &oldtio) != 0) {
        perror("SetupSerial 1");
        return -1;
    }
    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag |= CLOCAL | CREAD;
    newtio.c_cflag &= ~CSIZE;
    // switch( nBits )
    // {
    // case 7:
    // 	newtio.c_cflag |= CS7;
    // 	break;
    // case 8:
    // 	newtio.c_cflag |= CS8;
    // 	break;
    // }

    newtio.c_cflag = BAUDRATE | /*CRTSCTS |*/ CS8 /*| CLOCAL | CREAD */;
    newtio.c_iflag = IGNPAR | ICRNL;
    newtio.c_oflag = 0;
    newtio.c_lflag = ICANON;

    // switch( nEvent )
    // {
    // case 'O':
    // 	newtio.c_cflag |= PARENB;
    // 	newtio.c_cflag |= PARODD;
    // 	newtio.c_iflag |= (INPCK | ISTRIP);
    // 	break;
    // case 'E':
    // 	newtio.c_iflag |= (INPCK | ISTRIP);
    // 	newtio.c_cflag |= PARENB;
    // 	newtio.c_cflag &= ~PARODD;
    // 	break;
    // case 'N':
    // 	newtio.c_cflag &= ~PARENB;
    // 	break;
    // }

    switch (nSpeed) {
        case 2400:
            cfsetispeed(&newtio, B2400);
            cfsetospeed(&newtio, B2400);
            break;
        case 4800:
            cfsetispeed(&newtio, B4800);
            cfsetospeed(&newtio, B4800);
            break;
        case 9600:
            cfsetispeed(&newtio, B9600);
            cfsetospeed(&newtio, B9600);
            break;
        case 115200:
            cfsetispeed(&newtio, B115200);
            cfsetospeed(&newtio, B115200);
            break;
        default:
            cfsetispeed(&newtio, B9600);
            cfsetospeed(&newtio, B9600);
            break;
    }
    if (nStop == 1)
        newtio.c_cflag &= ~CSTOPB;
    else if (nStop == 2)
        newtio.c_cflag |= CSTOPB;
    newtio.c_cc[VTIME] = 0;
    newtio.c_cc[VMIN] = 0;
    tcflush(fd, TCIFLUSH);
    if ((tcsetattr(fd, TCSANOW, &newtio)) != 0) {
        perror("com set error");
        return -1;
    }
    return 0;
}

int MySerial::open_port(int comport)                 //通过参数，打开相应的串口
{
    int fd = 0;
    if (comport == 1) {
        //O_RDWR读写模式 O_NOCTTY如果路径名指向终端设备,不要把这个设备用作控制终端 O_NDELAY表示不关心DCD信号所处的状态（端口的另一端是否激活或者停止）
        fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
        if (-1 == fd) {
            perror("Can't Open Serial /dev/ttyS0 Port");
            return (-1);
        }
    } else if (comport == 2) {
        fd = open("/dev/ttyS1", O_RDWR | O_NOCTTY | O_NDELAY);
        if (-1 == fd) {
            perror("Can't Open Serial /dev/ttyS1 Port");
            return (-1);
        }
    } else if (comport == 3) {
        fd = open("/dev/ttyS2", O_RDWR | O_NOCTTY | O_NDELAY);
        if (-1 == fd) {
            perror("Can't Open Serial /dev/ttyS2 Port");
            return (-1);
        }
    }
    if (fcntl(fd, F_SETFL, 0) < 0)
        perror("fcntl failed!");
    else
        printf("fcntl=%d\n", fcntl(fd, F_SETFL, 0));

    if (isatty(STDIN_FILENO) == 0)
        perror("standard input is not a terminal device");
    else
        printf("isartty success!\n");
    printf("fd-open=%d\n", fd);
    return fd;
}

int MySerial::nwrite(int serialfd, const char *data, int datalength)  //写串口信息
{
    int len = 0, total_len = 0;
    // for (total_len = 0 ; total_len < datalength;) 
    // {
    len = 0;
    //len = write(serialfd, &data[total_len], datalength - total_len);
    for (int i = 0; i < datalength; i++) {
        len += write(serialfd, &data[i], 1);
        usleep(1000);
    }
//         if (len > 0) 
//             total_len += len;          
//    }
    return (total_len);
}

void MySerial::nread(int fd, void *data, int datalength)   //读取串口信息
{
    int readlen = 0;
    if ((readlen = static_cast<int>(read(fd, data, datalength))) > 0) {
        printf("current condition is %s\n", data);
    }
}


