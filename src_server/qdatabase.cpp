#include "qdatabase.h"

QDatabase::QDatabase(QObject *parent) : QObject(parent){

}
QDatabase::QDatabase(QDatabase &db){
    m_db = db.m_db;
    this->m_errorString = db.m_errorString;
}
QDatabase::~QDatabase(){}

void QDatabase::setPort(int port){
    m_db.setPort(port);
    emit portChanged();
}
void QDatabase::setDriver(QString driver){
    m_db = QSqlDatabase::addDatabase(driver);
    emit driverChanged();
}
void QDatabase::setDBName(QString dbname){
    m_db.setDatabaseName(dbname);
    emit dbnameChanged();
}
void QDatabase::setHostName(QString hostname){
    m_db.setHostName(hostname);
    emit hostnameChanged();
}
void QDatabase::setUsername(QString username){
    m_db.setUserName(username);
    emit usernameChanged();
}
void QDatabase::setPassword(QString password){
    m_db.setPassword(password);
    emit passwordChanged();
}

QString QDatabase::error(){
    return m_errorString;
}

bool QDatabase::Connect(){
    if (!m_db.open()){
        m_errorString = m_db.lastError().text();
        return false;
    }
    return true;
}
void QDatabase::Disconnect(){
    if (!m_db.isOpen()){
        qDebug() << "Connection is already closed.";
        return;
    }
    m_db.close();
    qDebug() << "Connection is closed.";
}

QDatabase QDatabase::operator=(QDatabase &db){
    m_db = db.m_db;
    this->m_errorString = db.m_errorString;
}

QStringList QDatabase::tables(){
    QSqlQuery query;
    query.exec("show tables;");
    QStringList res;
    while (query.next()) {
        res.append(query.value(0).toString());
    }
    return res;
}
QStringList QDatabase::fieldsName(QString tableName){
    QSqlRecord rec = m_db.record(tableName);
    QStringList list;
    for (int i = 0; i < rec.count(); i++){
        QString tmp = rec.fieldName(i);
        list.append(tmp);
    }
    return list;
}
QTableWidget* QDatabase::getTable(QString tableName){
    QStringList list = fieldsName(tableName);
    QTableWidget *table = new QTableWidget(0, list.size(), nullptr);
    table->setHorizontalHeaderLabels(list);
    QSqlQuery query;
    query.exec("select * from " + tableName + ";");
    if (query.isActive())
        while (query.next()){
            table->setRowCount(table->rowCount() + 1);
            for (int i = 0; i < list.count(); i++){
                QTableWidgetItem *w = new QTableWidgetItem();
                w->setText(query.value(i).toString());
                table->setItem(table->rowCount() - 1, i, w);
            }
        }
    return table;
}

//DML
bool QDatabase::addIntoTable(QString query_str){
    QSqlQuery query;
    query.exec(query_str);
    if (query.lastError().text() != ""){
        qDebug() << query.lastError().text();
        emit isError(query.lastError().text());
        return false;
    }
    else qDebug() << "Fine: Query is executed";
    query.clear();
    return true;
}
QTableWidget* QDatabase::getDescribe(QString tableName){
    QSqlQuery query;
    query.exec("describe " + tableName + ";");
    QTableWidget* res = new QTableWidget();
    res->setColumnCount(6);
    while (query.next()) {
        res->setRowCount(res->rowCount() + 1);
        for (int i = 0; i < 6; i++){
            QTableWidgetItem *w = new QTableWidgetItem();
            w->setText(query.value(i).toString());
            res->setItem(res->rowCount() - 1, i, w);
        }
    }
    return res;
}
