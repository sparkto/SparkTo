#ifndef QDATABASE_H
#define QDATABASE_H

#include <QSqlDatabase>
#include <QTableWidget>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>
#include <QObject>
#include <QDebug>

class QDatabase : public QObject
{
    Q_OBJECT
public:
    explicit QDatabase(QObject *parent = nullptr);
    explicit QDatabase(QDatabase &db);
    ~QDatabase();

    void setPort(int post);
    void setDriver(QString driver);
    void setDBName(QString dbname);
    void setHostName(QString hostname);
    void setUsername(QString username);
    void setPassword(QString password);

    QString error();

    QDatabase operator=(QDatabase &db);

    QStringList tables();
    QStringList fieldsName(QString tableName);
    QTableWidget *getTable(QString tableName);

    //DML
    bool addIntoTable(QString query_str);
    QTableWidget *getDescribe(QString tableName);

signals:
    void portChanged();
    void isConnected();
    void dbnameChanged();
    void driverChanged();
    void queryExecuted();
    void isDisconnected();
    void isError(QString);
    void usernameChanged();
    void passwordChanged();
    void hostnameChanged();

public slots:
    bool Connect();
    void Disconnect();
private:
    QSqlDatabase m_db;
    QString m_errorString;
};

#endif // QDATABASE_H
