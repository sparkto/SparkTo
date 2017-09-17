#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <QByteArray>
#include <QDebug>
#include <QImage>
#include <QDir>

using namespace std;

int main(){
    qDebug() << "server";
    int sock, listener;
    sockaddr_in addr;

    char buff[1024];
    int readLenght;

    listener = socket(AF_INET, SOCK_STREAM, 0);

    if (listener < 0)
        perror("socket error");

    addr.sin_family = AF_INET;
    addr.sin_port = htons(5001);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listener, (sockaddr *)&addr, sizeof(addr)) < 0)
        perror("bind error:");

    listen(listener, 1);
    QByteArray s;

    while(true) {
        sock = accept(listener, NULL, NULL);
        if(sock < 0)
            perror("accept");

        switch(fork()){
            case -1:
            break;
            case 0: {
                while(true) {
                    readLenght = recv(sock, buff, 1024, 0);
                    if(readLenght <= 0)
                        break;
                    for (int i = 0; i < readLenght; i++)
                        s += buff[i];
                }
                QImage img;
                img.loadFromData(s);
                img.save(QDir::homePath() + "/client/2", "JPEG");
                qDebug() << "Inserting photo is complete.";
            }
            default:
            close(sock);
        }
    }
    close(sock);
    return 0;
}
