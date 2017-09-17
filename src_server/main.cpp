#include <qdatabase.h>

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


QString genPath(QString author_id){
    return QDir::homePath() + "/server/" + author_id + "/";
}
QString genName(){
    char s[10 + 26 + 26];
    memset(s, 0, strlen(s));
    for (int i = 48; i <= 57; i++)
        s[i - 48] = i;
    for (int i = 65; i <= 90; i++)
        s[10 + i - 65] = i;
    for (int i = 97; i <= 122; i++)
        s[10 + 26 + i - 97] = i;
    QString res;
    for (int i = 0; i < 25; i++){
        res.append(s[qrand() % 61]);
    }
    return res;
}
void insertPhoto(QByteArray data, QString author_id){
    QImage img;
    img.loadFromData(data);
    QString path = genPath(author_id) + "/" + genName();
    if (img.save(path, "PNG"))
        qDebug() << "true";
    db->addIntoTable("INSERT INTO `photo`.`photos` (`photo_id`, `author_id`, `like_counter`, `link`, `categories`, `text`, `complaint_counter`, `uploadDate`, `title`) VALUES ('0', " + author_id  + ", '0', '" + path + "', '', '', '0', current_time(), '')");
}

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
