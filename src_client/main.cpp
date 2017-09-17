#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int sock;
    sockaddr_in addr;
    char buf[1024];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        perror("socket error");

    addr.sin_family = AF_INET;
    addr.sin_port = htons(5001);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        perror("connect error");

    QFile *f = new QFile();
    f->setFileName(QDir::homePath() + "/client/1");
    f->open(QIODevice::ReadOnly);
    while(!f->atEnd()){
        QByteArray data = f->read(1024);
        send(sock, data, 1024, 0);
    }
    close(sock);

    return a.exec();
}
