#include "chatbot.h"
#include "./ui_chatbot.h"
#include "QMessageBox"
#include "QString"
#include <QtCore>
#include <QtNetwork>
Chatbot::Chatbot(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Chatbot)
{
    ui->setupUi(this);
}

Chatbot::~Chatbot()
{
    delete ui;
}

void Chatbot::on_btn_pressed()
{
    QUrl url("https://api.openai.com/v1/engines/text-davinci-003/completions");
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader , "application/json");
    req.setRawHeader("Authorization" ,"Bearer XXXXXXXXXXXXXXXXXXXXXXXXXXXX");
    QJsonObject obj;
    QString messages = ui->msgbox->text();
    QString prompt = "User: " + messages;
    ui->textBrowser->setTextBackgroundColor(QColor("blue"));
    ui->textBrowser->append(prompt);
    obj["prompt"] = messages;

    QJsonDocument requestDoc(obj);
    QNetworkAccessManager manage;

    QNetworkReply *reply = manage.post(req , requestDoc.toJson());
    QEventLoop loop;
    QObject::connect(reply , &QNetworkReply::finished , &loop , &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(data);
        if (!jsonResponse.isNull() && jsonResponse.isObject()) {
            QJsonObject jsonObj = jsonResponse.object();
            if (jsonObj.contains("choices") && jsonObj["choices"].isArray()) {
                QJsonArray choices = jsonObj["choices"].toArray();
                if (!choices.isEmpty()) {
                    QString response = choices[0].toObject()["text"].toString().trimmed();
                    ui->textBrowser->setTextBackgroundColor(QColor("green"));
                    ui->textBrowser->append("\n Bot: " + response + "\n");
                }
            }
        }
    }
    else {
        QMessageBox::critical(this, "Error", reply->errorString());
    }
    reply->deleteLater();

    ui->msgbox->setText("");
}

